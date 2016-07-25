/************************************************************************
 * Copyright (C) 2016, Cavium, Inc.
 * All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 2
 * of the License.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * A copy of the GNU General Public License, version 2 is available in the file 
 * named LICENSE-GPLv2.md either in this directory or its root. 
 * Alernatively to obtain a copy, write to the Free Software Foundation, Inc., 
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * File: xp_netdev_intf.c
 * 
 * Abstract: This file contains the netdev interface implementation required
 * for xpliant. 
 ************************************************************************/
#include "xp_common.h"
#include "xp_export.h"

#include <linux/list.h>
#include <linux/hash.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/inet.h>
#include <linux/rwsem.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/seq_file.h>
#include <linux/netdevice.h>
#include <net/netns/generic.h>

#include <linux/hashtable.h>

#define XP_VIF_TO_HASH(x) (x)

#define XP_NO_VIF          -1
#define XP_NO_RIF          -1

#define XP_CB_TX_PORT       5555

#define XP_HASH_BITS        8
#define XP_NETLINK_USER     31

#define XP_INVALID_TRAP_ID -1
#define XP_MAX_REASON_CODE  803
#define XP_MAX_TRAP_ID      0x5000

#define XP_MIN_FP_INTF      0
#define XP_MAX_FP_INTF      128

#define XP_MIN_LAG_INTF     4096
#define XP_MAX_LAG_INTF     5019

#define XP_OFFSET_BD_INTF   65536
#define XP_NUM_RP_INTF      16384
#define XP_MAX_BD_INTF      (XP_OFFSET_BD_INTF + XP_NUM_RP_INTF)

typedef struct xp_trap_config
{
    unsigned int fd;                 /* File descriptor        */
    unsigned int rc;                 /* Reason code            */
    unsigned int trap_id;            /* Trap ID                */
    unsigned int rx_count;           /* Rx packets counter     */
    struct socket *sock_ptr;         /* User space socket      */
    xp_nl_hostif_trap_channel_t ch;  /* Host interface channel */
} xp_trap_config_t;

typedef struct xp_trap_table {
    spinlock_t lock;
    struct xp_trap_config *trap[XP_MAX_TRAP_ID + 1];
    unsigned int rc_map[XP_MAX_REASON_CODE + 1];
} xp_trap_table_t;

struct xp_pernet {
    struct sock *nlsk;
};

typedef enum xp_netdev_type {
    XP_FP_NETDEV=0,
    XP_ROUTER_NETDEV,
    XP_LAG_NETDEV
} xp_netdev_type;

struct xp_netdev_priv {
    unsigned int xpnet_intf_id;

    union {
        unsigned int vif;
        unsigned int rif;
    };
    
    xphTxHdr tx_header;
    struct list_head list;
    struct hlist_node hlist;
    struct net_device *netdev;
    unsigned int is_tx_header_set;
    xp_netdev_type netdev_type;
    rwlock_t netdev_priv_lock;
};

struct xp_skb_info {
    unsigned int rc;        /* Reason code                       */
    unsigned int rif_id;    /* Ingress router interaface         */
    unsigned int intf_id;   /* Ingress interface                 */
    unsigned int port_num;  /* Ingress port number               */
    struct sk_buff *skb;    /* Socket buffer the info belongs to */
};

extern netdev_tx_t xpnet_start_xmit(struct sk_buff *skb, 
                                    xpnet_private_t *net_priv);

static LIST_HEAD(xp_netdev_list);
static DEFINE_RWLOCK(xp_netdev_list_lock);

/* Active netdev table. */
static DEFINE_HASHTABLE(xp_active_netdev_htable, XP_HASH_BITS);

/* Serialize requests from userspace. */
static DEFINE_MUTEX(xp_netlink_mutex);

/* Per-network key. */
static int xp_pernet_id;

/* Debug flag. */
static int xp_debug;

/* Sign up on network namespace notifications. */
static int xp_netlink_sock_init(void);
/* Deregister network namespace notifications. */
static void xp_netlink_sock_deinit(void);

static u8 XP_ETH_ADDR_HIGH[3] = {0x08, 0x23, 0x56};

static xp_trap_table_t *trap_table;
static struct socket *cb_sock_ptr;
static struct socket *client_socket;

/* Packet mirroring to socket. */
static int packet_mirroring_enable;

static void xp_rx_skb_dump(struct sk_buff *skb)
{
    if (xp_debug) {
#if 0
        print_hex_dump_debug("RX: ", DUMP_PREFIX_NONE, 
                             16, 1, skb->data, skb->len, true);
#endif
    }
}

static int xp_trap_init(void)
{
    unsigned int i = 0;
    DBG("Enter: %s\n", __FUNCTION__);

    trap_table = kmalloc(sizeof(xp_trap_table_t), GFP_ATOMIC);
    if (!trap_table) {
        return -ENOMEM;
    }

    for (i = 0; i < ARRAY_SIZE(trap_table->rc_map); i++) {
        trap_table->rc_map[i] = XP_INVALID_TRAP_ID;
    }

    for (i = 0; i < ARRAY_SIZE(trap_table->trap); i++) {
        trap_table->trap[i] = NULL;
    }

    spin_lock_init(&trap_table->lock);
    return 0;
}

static void xp_trap_deinit(void)
{
    unsigned int i = 0;
    unsigned long flags = 0;

    DBG("Enter: %s\n", __FUNCTION__);
    spin_lock_irqsave(&trap_table->lock, flags);

    for (i = 0; i < ARRAY_SIZE(trap_table->trap); i++) {
        if (trap_table->trap[i]) {
            trap_table->rc_map[trap_table->trap[i]->rc] = XP_INVALID_TRAP_ID;
            kfree(trap_table->trap[i]);
            trap_table->trap[i] = NULL;
        }
    }

    spin_unlock_irqrestore(&trap_table->lock, flags);

    kfree(trap_table);
    trap_table = NULL;
}

static struct xp_trap_config* xp_trap_config_get(unsigned int id)
{
    unsigned long flags = 0;
    xp_trap_config_t *trap_cfg = NULL;

    spin_lock_irqsave(&trap_table->lock, flags);
    trap_cfg = (id >= ARRAY_SIZE(trap_table->trap)) ? 
        NULL : trap_table->trap[id];
    spin_unlock_irqrestore(&trap_table->lock, flags);

    return trap_cfg;
}

static unsigned int xp_trap_id_get(unsigned int rc)
{
    unsigned int trap_id = 0;
    unsigned long flags = 0;
    
    spin_lock_irqsave(&trap_table->lock, flags);
    trap_id = (rc >= ARRAY_SIZE(trap_table->rc_map)) ? 
        XP_INVALID_TRAP_ID : trap_table->rc_map[rc];
    spin_unlock_irqrestore(&trap_table->lock, flags);

    return trap_id;
}

static int xp_trap_id_entry_add(xp_trap_config_t *cfg)
{
    unsigned long flags = 0;
    xp_trap_config_t *trap_cfg = NULL;

    DBG("Enter: %s\n", __FUNCTION__);
    if (cfg->trap_id > XP_MAX_TRAP_ID) {
        return -ESPIPE;
    }

    trap_cfg = kmalloc(sizeof(xp_trap_config_t), GFP_ATOMIC);
    if (!trap_cfg) {
        return -ENOMEM;
    }

    memcpy(trap_cfg, cfg, sizeof(xp_trap_config_t));

    spin_lock_irqsave(&trap_table->lock, flags);
    trap_table->rc_map[trap_cfg->rc] = trap_cfg->trap_id;

    if (trap_table->trap[trap_cfg->trap_id]){
        kfree(trap_table->trap[trap_cfg->trap_id]);
    }

    trap_table->trap[trap_cfg->trap_id] = trap_cfg;
    spin_unlock_irqrestore(&trap_table->lock, flags);

    return 0;
}

static int xp_trap_id_entry_remove(unsigned int trap_id)
{
    unsigned long flags = 0;

    DBG("Enter: %s\n", __FUNCTION__);
    if (trap_id > XP_MAX_TRAP_ID) {
        return -ESPIPE;
    }

    if (trap_table->trap[trap_id]) {
        spin_lock_irqsave(&trap_table->lock, flags);
        kfree(trap_table->trap[trap_id]);
        trap_table->trap[trap_id] = NULL;
        spin_unlock_irqrestore(&trap_table->lock, flags);
    }

    return 0;
}

static int xp_ndo_open(struct net_device *dev) 
{
    DBG("Enter: %s\n", __FUNCTION__);

    netif_start_queue(dev);
    return 0;
}

static int xp_ndo_stop(struct net_device *dev)
{
    DBG("Enter: %s\n", __FUNCTION__);

    netif_stop_queue(dev);
    return 0;
} 

static netdev_tx_t xp_ndo_start_xmit(struct sk_buff *skb, 
                                     struct net_device *dev) 
{
    netdev_tx_t rc = NETDEV_TX_OK;
    xphTxHdr *tx_header = NULL;
    unsigned int new_skb_used = 0;
    unsigned int tx_header_len = sizeof(xphTxHdr);
    unsigned int headroom = skb_headroom(skb);
    struct sk_buff *new_skb = NULL;
    struct xp_netdev_priv *priv = NULL;
    unsigned long flags = 0;

    if (!(dev->flags & IFF_UP)) {
        dev_kfree_skb_any(skb);
        return NETDEV_TX_OK;
    }

    priv = netdev_priv(dev);

    /* Check if we have headroom to add xp tx header else allocate. */
    if (headroom < tx_header_len) {
        /* If called in interrupt shd be changed to GFP_ATOMIC. */
        new_skb = skb_copy_expand(skb, tx_header_len, 0, GFP_ATOMIC);
        new_skb_used = 1;
    } 

    /* skb->data will point to xp tx header */
    if (new_skb_used) {
        tx_header = (xphTxHdr *)skb_push(new_skb, tx_header_len);

        /* copy xp txheader. Read lock on entry */
        read_lock_irqsave(&priv->netdev_priv_lock, flags);
        memcpy(tx_header, &priv->tx_header, sizeof(xphTxHdr));
        read_unlock_irqrestore(&priv->netdev_priv_lock, flags);

        rc = xpnet_start_xmit(new_skb, g_net_priv);
    } else {
        tx_header = (xphTxHdr *)skb_push(skb, tx_header_len);

        /* copy xp txheader. Read lock on entry */
        read_lock_irqsave(&priv->netdev_priv_lock, flags);
        memcpy(tx_header, &priv->tx_header, sizeof(xphTxHdr));
        read_unlock_irqrestore(&priv->netdev_priv_lock, flags);
        rc = xpnet_start_xmit(skb, g_net_priv);
    }

    if (rc == NETDEV_TX_OK) {
        /*
         * Case A:
         * ->dma error - upper proctocol retries.
         * Case 1(newSkb used):  newSkb is freed by xpnet_start_xmit.
         * We need to free skb here.
         * Case 2(newSkb not used):
         * If we used fpNet skb, then free is handled xpnet_start_xmit.
         * No action reqd.
         * Case B:
         * ->trasmit success.
         * Case 1(newSkb used): Packet free(newSkb) is handled in
         * xpnet_start_xmit..
         * No need of our fp skb buffer. So we free it right here in
         * case we allocated a new one.
         * Case 2(newSkb not used): If we used fnet skb, free is
         * already handled. No action reqd.
         */
        if (new_skb_used) {
            dev_kfree_skb_any(skb);
        }

        dev->stats.tx_packets++;
    } else if (rc == NETDEV_TX_BUSY) {
        /* DMA is busy. IT cant handle more packets. We
         * also inform to kernel. 
         * Kernel retries packet after wake queue, so no freeing of fp
         * skb.xpnet_start_xmit also doesnot free skb.  Case 1(newSkb used):
         * But we free the expanded skb as it gets allocated again once
         * kernel retries.  Mapping this skb(fpnetdev) with expanded
         * skb is not handled now(optimised soln).  Case 2:(newSkb not
         * used): fp skb alloced by kernel. we dont free it(kernel
         * retry). */
        if (new_skb_used) {
            dev_kfree_skb_any(new_skb);
        }

        netif_stop_queue(dev);
    }

    return rc;
}

static int xp_ndo_set_mac_address(struct net_device *dev, void *p)
{
    struct sockaddr *addr = p;
    DBG("Enter: %s\n", __FUNCTION__);

    memcpy(dev->dev_addr, addr->sa_data, dev->addr_len);

    DBG("%s: changed MAC to %pM for interface %u\n", 
        dev->name, dev->dev_addr, 
        ((struct xp_netdev_priv *)netdev_priv(dev))->xpnet_intf_id);
    return 0;
}

static struct net_device_ops xp_ndo = { 
    .ndo_open = xp_ndo_open, 
    .ndo_stop = xp_ndo_stop, 
    .ndo_change_mtu = eth_change_mtu, 
    .ndo_start_xmit = xp_ndo_start_xmit, 
    .ndo_set_mac_address = xp_ndo_set_mac_address,
}; 

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,15,0)
static void xp_packet_send(struct sock *sk)
#else
static void xp_packet_send(struct sock *sk, int bytes)
#endif
{
    struct sk_buff *skb = skb_dequeue(&sk->sk_receive_queue);

    if (skb) {
        skb_pull(skb, 8); /* Strip UDP header */

        if (NETDEV_TX_BUSY == xpnet_start_xmit(skb, g_net_priv)) {
            ERR("DMA is busy...\n");
        }
    }
}

static void xp_netdev_all_remove(void)
{
    struct list_head *iter = NULL;
    struct list_head *iter_safe = NULL;
    unsigned long flags = 0;
    DBG("Enter: %s\n", __FUNCTION__);

    write_lock_irqsave(&xp_netdev_list_lock, flags);
    list_for_each_safe(iter, iter_safe, &xp_netdev_list) {
        struct xp_netdev_priv *entry = 
            list_entry(iter, struct xp_netdev_priv, list);

        LOG("Removed netdev interface, xpnet_id: %u\n", 
            entry->xpnet_intf_id);

        hash_del(&entry->hlist);
        list_del(&entry->list);
        unregister_netdev(entry->netdev);
        free_netdev(entry->netdev);
    }
    write_unlock_irqrestore(&xp_netdev_list_lock, flags);
}

static void xp_netdev_setup(struct net_device *dev)
{
    DBG("Enter: %s\n", __FUNCTION__); 

    ether_setup(dev);
    memset(netdev_priv(dev), 0, sizeof(struct xp_netdev_priv)); 
    dev->netdev_ops = &xp_ndo;

    /* Set XP MAC prefix */
    memcpy(dev->dev_addr, XP_ETH_ADDR_HIGH, sizeof(XP_ETH_ADDR_HIGH));
}

static int xp_nl_msg_if_create(struct net *netns, xp_nl_msg_intf_t *intf_msg)
{
    struct list_head *iter = NULL;
    struct net_device *netdev = NULL;
    struct xp_netdev_priv *priv = NULL;
    unsigned long flags = 0;
    DBG("Enter: %s\n", __FUNCTION__);

    write_lock_irqsave(&xp_netdev_list_lock, flags);
    list_for_each(iter, &xp_netdev_list) {
        struct xp_netdev_priv *entry = 
            list_entry(iter, struct xp_netdev_priv, list);

        if (entry->xpnet_intf_id == intf_msg->xpnet_intf_id) {
            ERR("Specifed interface already exists, "
                "intf name: %s, xpnet_id: %u\n", 
                intf_msg->intf_name, intf_msg->xpnet_intf_id);
            write_unlock_irqrestore(&xp_netdev_list_lock, flags);
            return 0;
        }
    }

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,19,0)
    netdev = alloc_netdev(sizeof(struct xp_netdev_priv), 
                          intf_msg->intf_name, xp_netdev_setup);
#else
    netdev = alloc_netdev(sizeof(struct xp_netdev_priv), intf_msg->intf_name, 
                          NET_NAME_UNKNOWN, xp_netdev_setup);
#endif

    /* Move the interface to the specified network namespace. */
    dev_net_set(netdev, netns);

    priv = netdev_priv(netdev);
    memset(priv, 0x00, sizeof(struct xp_netdev_priv));

    priv->xpnet_intf_id = intf_msg->xpnet_intf_id;
    rwlock_init(&priv->netdev_priv_lock);
    priv->vif = XP_NO_VIF;
    priv->rif = XP_NO_RIF;
    priv->netdev = netdev;
    
    if (0 > register_netdev(netdev)) {
        ERR("Could not register netdev, intf name: %s, xpnet_id: %u\n", 
            intf_msg->intf_name, intf_msg->xpnet_intf_id);
        free_netdev(netdev);
        write_unlock_irqrestore(&xp_netdev_list_lock, flags);
        return -EPERM;
    }

    list_add(&priv->list, &xp_netdev_list);

    LOG("Registered a new netdev, intf name: %s, xpnet_id: %u\n", 
        intf_msg->intf_name, intf_msg->xpnet_intf_id);

    write_unlock_irqrestore(&xp_netdev_list_lock, flags);
    return 0;
}

static int xp_nl_msg_if_delete(struct net *netns, xp_nl_msg_intf_t *intf_msg)
{
    struct list_head *iter = NULL;
    unsigned long flags = 0;
    DBG("Enter: %s\n", __FUNCTION__);

    write_lock_irqsave(&xp_netdev_list_lock, flags);
    list_for_each(iter, &xp_netdev_list) {
        struct xp_netdev_priv *entry = 
            list_entry(iter, struct xp_netdev_priv, list);

        if (entry->xpnet_intf_id == intf_msg->xpnet_intf_id) {
            LOG("Removed netdev interface, xpnet_id: %u\n", 
                intf_msg->xpnet_intf_id);

            hash_del(&entry->hlist);
            list_del(&entry->list);
            unregister_netdev(entry->netdev);
            free_netdev(entry->netdev);
            goto out;
        }
    }

    ERR("Tried to remove interface but it wasn't found, xpnet_id: %u\n", 
        intf_msg->xpnet_intf_id);

out:
    write_unlock_irqrestore(&xp_netdev_list_lock, flags);
    return 0;
}

static int xp_nl_msg_if_link(struct net *netns, xp_nl_msg_link_t *link_msg)
{
    struct list_head *iter = NULL;
    unsigned long flags = 0;
    DBG("Enter: %s\n", __FUNCTION__);

    write_lock_irqsave(&xp_netdev_list_lock, flags);
    list_for_each(iter, &xp_netdev_list) {
        struct xp_netdev_priv *entry = 
            list_entry(iter, struct xp_netdev_priv, list);

        if (entry->xpnet_intf_id == link_msg->xpnet_intf_id) {

            if ((XP_NO_VIF != entry->vif) || (XP_NO_RIF != entry->rif)) {
                ERR("Tried to link interface which has "
                    "been already linked, xpnet_id: %u, vif: %u, rif: %u\n",
                    entry->xpnet_intf_id, entry->vif, entry->rif);
                break;
            }

            if ((XP_MIN_FP_INTF <= link_msg->vif) && 
                (link_msg->vif < XP_MAX_FP_INTF)) {
                LOG("Added netdev interface link, xpnet_id: %u, vif: %u\n", 
                    link_msg->xpnet_intf_id, link_msg->vif);
                entry->netdev_type = XP_FP_NETDEV;
                entry->vif = link_msg->vif;
                hash_add(xp_active_netdev_htable, 
                         &entry->hlist, XP_VIF_TO_HASH(entry->vif));
            } else if ((XP_OFFSET_BD_INTF <= link_msg->rif) && 
                       (link_msg->rif < XP_MAX_BD_INTF)) {
                LOG("Added netdev interface link, xpnet_id: %u, rif: %u\n", 
                    link_msg->xpnet_intf_id, link_msg->rif);

                entry->rif = link_msg->rif;
                entry->netdev_type = XP_ROUTER_NETDEV;
                hash_add(xp_active_netdev_htable, 
                         &entry->hlist, XP_VIF_TO_HASH(entry->rif));
            } else if ((XP_MIN_LAG_INTF <= link_msg->vif) && 
                       (link_msg->vif < XP_MAX_LAG_INTF)) {
                LOG("Added netdev interface link, xpnet_id: %u, lag vif: %u\n", 
                    link_msg->xpnet_intf_id, link_msg->vif);

                entry->vif = link_msg->vif;
                entry->netdev_type = XP_LAG_NETDEV;
                hash_add(xp_active_netdev_htable, 
                         &entry->hlist, XP_VIF_TO_HASH(entry->vif));
            } else {
                ERR("Tried to link unknown type of interfaces, "
                    "xpnet_id: %u, vif: %u, rif: %u\n", 
                    link_msg->xpnet_intf_id, link_msg->vif, link_msg->rif);
            }

            break;
        }
    }

    write_unlock_irqrestore(&xp_netdev_list_lock, flags);
    return 0;
}

static int xp_nl_msg_if_unlink(struct net *netns, xp_nl_msg_link_t *link_msg)
{
    struct list_head *iter = NULL;
    unsigned long flags = 0;
    DBG("Enter: %s\n", __FUNCTION__);

    write_lock_irqsave(&xp_netdev_list_lock, flags);
    list_for_each(iter, &xp_netdev_list) {
        struct xp_netdev_priv *entry = 
            list_entry(iter, struct xp_netdev_priv, list);

        if (entry->xpnet_intf_id == link_msg->xpnet_intf_id) {
            LOG("Removed netdev interface link, xpnet_id: %u\n", 
                link_msg->xpnet_intf_id);

            hash_del(&entry->hlist);
            entry->vif = XP_NO_VIF;
            entry->rif = XP_NO_RIF;
            break;
        }
    }

    write_unlock_irqrestore(&xp_netdev_list_lock, flags);
    return 0;
}

static int xp_nl_msg_tx_header(struct net *netns, 
                               xp_nl_msg_tx_hdr_t *tx_header_msg)
{
    struct list_head *iter = NULL;
    unsigned long flags = 0;
    DBG("Enter: %s\n", __FUNCTION__);

    /* Read lock only */
    read_lock_irqsave(&xp_netdev_list_lock, flags);
    list_for_each(iter, &xp_netdev_list) {
        struct xp_netdev_priv *entry = 
            list_entry(iter, struct xp_netdev_priv, list);

        if (entry->xpnet_intf_id == tx_header_msg->xpnet_intf_id) {
            write_lock_irqsave(&entry->netdev_priv_lock, flags);
            if (XP_NL_OPERATION_ADD == tx_header_msg->operation) {
                LOG("Set TX header for netdev interface, xpnet_id: %u\n", 
                    tx_header_msg->xpnet_intf_id);
                /* write lock on entry only */
                memcpy(&entry->tx_header, 
                       &tx_header_msg->tx_header, sizeof(xphTxHdr));
                entry->is_tx_header_set = 1;
            } else if (XP_NL_OPERATION_REMOVE == tx_header_msg->operation) {
                LOG("Remove TX header for netdev interface, xpnet_id: %u\n", 
                    tx_header_msg->xpnet_intf_id);
                /* write lock on entry only */
                memset(&entry->tx_header, 0x00, sizeof(xphTxHdr));
                entry->is_tx_header_set = 0;
            }
 
            write_unlock_irqrestore(&entry->netdev_priv_lock, flags);
            break;
        }
    }

    read_unlock_irqrestore(&xp_netdev_list_lock, flags);
    return 0;
}

static int xp_nl_msg_trap(struct net *netns, xp_nl_msg_trap_t *trap_msg)
{
    unsigned int rc = 0;
    xp_trap_config_t trap_cfg = { 0 };
    DBG("Enter: %s\n", __FUNCTION__);

    if (XP_NL_OPERATION_ADD == trap_msg->operation) {
        if (XP_MAX_REASON_CODE < trap_msg->rc) {
            ERR("Reason code %u is out of range.\n", trap_msg->rc);
            return -ESPIPE;
        }

        trap_cfg.sock_ptr = NULL;
        trap_cfg.rx_count = 0;
        trap_cfg.trap_id = trap_msg->trap_id;
        trap_cfg.rc = trap_msg->rc;
        trap_cfg.fd = trap_msg->fd;
        trap_cfg.ch = trap_msg->ch;

        if (XP_NL_HOSTIF_TRAP_FD == trap_cfg.ch) {
            trap_cfg.sock_ptr = sockfd_lookup(trap_cfg.fd, &rc);
        }

        xp_trap_id_entry_add(&trap_cfg);
    } else if (XP_NL_OPERATION_REMOVE == trap_msg->operation) {
        xp_trap_id_entry_remove(trap_msg->trap_id);        
    }

    return 0;
}

static int xp_nl_msg_cb_fd(struct net *netns, xp_nl_msg_cb_fd_t *cb_fd_msg)
{
    int rc = 0;
    DBG("Enter: %s\n", __FUNCTION__);

    if (!(cb_sock_ptr = sockfd_lookup(cb_fd_msg->fd, &rc))) {
        ERR("Unable to get a callback for FD: %u\n", cb_fd_msg->fd);
    }

    return rc;
}

static int xp_nl_msg_mirror(struct net *netns, xp_nl_msg_mirror_t *mirror_msg)
{
    DBG("Enter: %s\n", __FUNCTION__);

    if ((XP_NL_OPERATION_ENABLE == mirror_msg->operation) ||
        (XP_NL_OPERATION_DISABLE == mirror_msg->operation)) {
        packet_mirroring_enable = (XP_NL_OPERATION_ENABLE == 
                                   mirror_msg->operation) ? 1 : 0;
        LOG("Packet mirroring: %u\n", packet_mirroring_enable);
    }

    return 0;
}

static int xp_nl_msg_process(struct net *netns, void *nl_msg_payload)
{
    unsigned int offset = 0;
    xp_nl_tlv_msg_t main_tlv_header;

    DBG("Enter: %s\n", __FUNCTION__);
    memcpy(&main_tlv_header, nl_msg_payload, sizeof(xp_nl_tlv_msg_t));

    offset += sizeof(xp_nl_tlv_msg_t);
    main_tlv_header.payload_len += sizeof(xp_nl_tlv_msg_t);

    while (main_tlv_header.payload_len) {
        xp_nl_msg_t msg_type;

        memcpy(&msg_type, 
               (uint8_t *)nl_msg_payload + offset, sizeof(xp_nl_msg_t));
        offset += sizeof(xp_nl_msg_t);

        switch (msg_type) {
            case XP_NL_MSG_INTF_ADD:
            {
                xp_nl_msg_intf_t msg;
                memcpy(&msg, nl_msg_payload + offset, sizeof(msg));
                xp_nl_msg_if_create(netns, &msg);
                offset += sizeof(msg);
            }
            break;

            case XP_NL_MSG_INTF_DEL:
            {
                xp_nl_msg_intf_t msg;
                memcpy(&msg, nl_msg_payload + offset, sizeof(msg));
                xp_nl_msg_if_delete(netns, &msg);
                offset += sizeof(msg);
            }
            break;

            case XP_NL_MSG_LINK_ADD:
            {
                xp_nl_msg_link_t msg;
                memcpy(&msg, nl_msg_payload + offset, sizeof(msg));
                xp_nl_msg_if_link(netns, &msg);
                offset += sizeof(msg);
            }
            break;

            case XP_NL_MSG_LINK_DEL:
            {
                xp_nl_msg_link_t msg;
                memcpy(&msg, nl_msg_payload + offset, sizeof(msg));
                xp_nl_msg_if_unlink(netns, &msg);
                offset += sizeof(msg);
            }
            break;

            case XP_NL_MSG_TX_HDR_SET:
            {
                xp_nl_msg_tx_hdr_t msg;
                memcpy(&msg, nl_msg_payload + offset, sizeof(msg));
                xp_nl_msg_tx_header(netns, &msg);
                offset += sizeof(msg);
            }
            break;
    
            case XP_NL_MSG_TRAP_SET:
            {
                xp_nl_msg_trap_t msg;
                memcpy(&msg, nl_msg_payload + offset, sizeof(msg));
                xp_nl_msg_trap(netns, &msg);
                offset += sizeof(msg);
            }
            break;

            case XP_NL_MSG_CB_FD_SET:
            {
                xp_nl_msg_cb_fd_t msg;
                memcpy(&msg, nl_msg_payload + offset, sizeof(msg));
                xp_nl_msg_cb_fd(netns, &msg);
                offset += sizeof(msg);
            }
            break;

            case XP_NL_MSG_MIRROR_SET:
            {
                xp_nl_msg_mirror_t msg;
                memcpy(&msg, nl_msg_payload + offset, sizeof(msg));
                xp_nl_msg_mirror(netns, &msg);
                offset += sizeof(msg);
            }
            break;

            default:
                return -EINVAL;
        }

        main_tlv_header.payload_len -= offset;
    }

    return 0;
}

static void xp_netlink_rcv(struct sk_buff *skb) 
{ /* Receive messages from netlink socket. */
    DBG("Enter: %s\n", __FUNCTION__);

    mutex_lock(&xp_netlink_mutex);
    xp_nl_msg_process(sock_net(skb->sk), 
                      nlmsg_data((struct nlmsghdr *)skb->data));
    mutex_unlock(&xp_netlink_mutex);
}

static struct xp_pernet* xp_pernet(struct net *net) 
{ /* Get private per network parameters. */
    DBG("Enter: %s\n", __FUNCTION__);
    return net_generic(net, xp_pernet_id);
}

static int __net_init xp_net_init(struct net *net) 
{ /* Per-network init notificator. */
    struct xp_pernet *pernet = xp_pernet(net);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,5,0)
    struct netlink_kernel_cfg cfg = {
        .input = xp_netlink_rcv,
    };

    pernet->nlsk = netlink_kernel_create(net, XP_NETLINK_USER, &cfg);
#else
    pernet->nlsk = netlink_kernel_create(net, XP_NETLINK_USER, 
                                         0, xp_netlink_rcv, NULL, NULL);
#endif

    DBG("Enter: %s\n", __FUNCTION__);

    if (NULL == pernet->nlsk) {
        ERR("Could not create netlink socket in namespace.\n");
        return -ENOMEM;
    }

    return 0;
}

static void __net_exit xp_net_exit(struct net *net) 
{ /* Per-network exit notificator. */
    struct xp_pernet *pernet = xp_pernet(net);

    DBG("Enter: %s\n", __FUNCTION__);
    netlink_kernel_release(pernet->nlsk);
    pernet->nlsk = NULL;
}

static struct pernet_operations xp_net_ops = {
    .init = xp_net_init,
    .exit = xp_net_exit,
    .id = &xp_pernet_id,
    .size = sizeof(struct xp_pernet),
};

static int xp_netlink_sock_init(void) 
{ /* Sign up on network namespace notifications. */
    DBG("Enter: %s\n", __FUNCTION__);
    return register_pernet_subsys(&xp_net_ops);
}

static void xp_netlink_sock_deinit(void) 
{ /* Deregister network namespace notifications. */
    DBG("Enter: %s\n", __FUNCTION__);
    unregister_pernet_subsys(&xp_net_ops);
}

static int xp_client_sock_init(void) 
{
    int rc = 0;
    struct sockaddr_in server = { 0 };

    DBG("Enter: %s\n", __FUNCTION__);

    rc = sock_create(PF_INET, SOCK_DGRAM, IPPROTO_UDP, &client_socket);
    if (rc < 0) {
        goto err_sock_create;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons((unsigned short) XP_CB_TX_PORT);

    rc = client_socket->ops->bind(client_socket, 
                                  (struct sockaddr *)&server, sizeof(server));
    if (rc < 0) {
        goto err_sock_bind;
    }

    client_socket->sk->sk_data_ready = xp_packet_send;
    return 0;


err_sock_bind:
    sock_release(client_socket);

err_sock_create:
    return rc;
}

static void xp_client_sock_deinit(void) 
{
    DBG("Enter: %s\n", __FUNCTION__);
    sock_release(client_socket);
}

static void xp_rx_skb_info_get(struct sk_buff *skb, struct xp_skb_info *info) 
{
    unsigned int bd_id = 0;
    unsigned int intf_id = 0;
    xphRxHdr *rx_header = (xphRxHdr *)skb->data;

    /* Ingress port number */
    info->port_num = rx_header->ingressPortNum;

    /* Extract ivif from packet */
    intf_id = rx_header->ingressVifLsbByte2;
    intf_id = intf_id << 8 | rx_header->ingressVifLsbByte1;
    intf_id = intf_id << 4 | rx_header->ingressVifLsbByte0;
    info->intf_id = intf_id;

    /* bdId */
    bd_id = rx_header->metadata.bdId[0];
    bd_id = ((bd_id << 8) | rx_header->metadata.bdId[1]);

    /* Router interface */
    info->rif_id = (bd_id < XP_OFFSET_BD_INTF) ? 
        bd_id + XP_OFFSET_BD_INTF : bd_id;

    /* Get skb reason code */
    info->rc = (rx_header->reasonCodeMSB << 2) | rx_header->reasonCodeLsbs;
    info->skb = skb;
}

static void xp_rx_netdev_skb_netif_rx(struct sk_buff *skb, 
                                      struct net_device *netdev) 
{
    struct sk_buff *new_skb = netdev_alloc_skb_ip_align(netdev, skb->len);

    if (new_skb) {
        memcpy(new_skb->data, skb->data, skb->len);
        skb_put(new_skb, skb->len);

        new_skb->protocol = eth_type_trans(new_skb, netdev);
        new_skb->pkt_type = PACKET_HOST;
        new_skb->ip_summed = CHECKSUM_UNNECESSARY;

        netdev->stats.rx_bytes += skb->len;
        netdev->stats.rx_packets++;

        /* Send packet to kernel stack. */
        netif_rx(new_skb);
    }
}

static void xp_rx_skb_fd_process(struct socket *sock, 
                                 struct xp_skb_info *skb_info) 
{
    if (sock) {
        struct iovec iov;
        struct msghdr msg;
        struct sockaddr_in to;
        struct inet_sock *inetp = inet_sk(sock->sk);
        mm_segment_t old_fs = get_fs();

        /* generate answer message */
        memset(&to, 0, sizeof(to));
        to.sin_family = AF_INET;
        to.sin_addr.s_addr = in_aton("127.0.0.1");
        to.sin_port = inetp->inet_sport;

        iov.iov_base = skb_info->skb->data;
        iov.iov_len  = skb_info->skb->len;

        memset(&msg, 0, sizeof(msg));
        msg.msg_name = &to;
        msg.msg_namelen = sizeof(to);
        msg.msg_control = NULL;
        msg.msg_controllen = 0;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 19, 0)
        iov_iter_init(&msg.msg_iter, WRITE, &iov, 1, skb_info->skb->len);
#else
        msg.msg_iov = &iov;
        msg.msg_iovlen = 1;
#endif
        /* adjust memory boundaries */
        set_fs(KERNEL_DS);
#if LINUX_VERSION_CODE < KERNEL_VERSION(4,1,0)
        sock_sendmsg(sock, &msg, skb_info->skb->len);
#else
        sock_sendmsg(sock, &msg);
#endif
        set_fs(old_fs);

        DBG("Queueing packet on inet->inet_sport fd: %d\n", inetp->inet_sport);
    }
}

static void xp_rx_skb_netdev_process(struct xp_skb_info *skb_info) 
{
    unsigned int vif = XP_NO_VIF;
    unsigned int rif = skb_info->rif_id;
    struct xp_netdev_priv *entry = NULL;
    unsigned long flags = 0;

    if (packet_mirroring_enable) {
        xp_rx_skb_fd_process(cb_sock_ptr, skb_info);
    }

    /* Netdev interface doesn't need RX metainfo header.
       So, let's remove it. */
    skb_pull(skb_info->skb, sizeof(xphRxHdr));

    if (((XP_MIN_FP_INTF <= skb_info->intf_id) && 
        (skb_info->intf_id < XP_MAX_FP_INTF)) ||

        ((XP_MIN_LAG_INTF <= skb_info->intf_id) && 
        (skb_info->intf_id < XP_MAX_LAG_INTF))) {

        vif = skb_info->port_num;
    }

    if (XP_NO_VIF != vif) {
        read_lock_irqsave(&xp_netdev_list_lock, flags);
        hash_for_each_possible(xp_active_netdev_htable, 
                               entry, hlist, XP_VIF_TO_HASH(vif)) {
            if (entry->vif == vif) {
                xp_rx_netdev_skb_netif_rx(skb_info->skb, entry->netdev);
                break;
            }
        }
        read_unlock_irqrestore(&xp_netdev_list_lock, flags);
    }

    read_lock_irqsave(&xp_netdev_list_lock, flags);
    hash_for_each_possible(xp_active_netdev_htable, 
                           entry, hlist, XP_VIF_TO_HASH(rif)) {
        if (entry->rif == rif) {
            xp_rx_netdev_skb_netif_rx(skb_info->skb, entry->netdev);
            break;
        }
    }

    read_unlock_irqrestore(&xp_netdev_list_lock, flags);
}

void xp_rx_skb_process(xpnet_private_t *priv, struct sk_buff *skb) 
{
    unsigned int trap_id = 0;
    struct xp_skb_info skb_info = { 0 };
    struct xp_trap_config *trap_cfg = NULL;

    xp_rx_skb_dump(skb);
    xp_rx_skb_info_get(skb, &skb_info);

    priv->stats.rx_packets++;
    priv->stats.rx_bytes += skb->len;

    DBG("Trapped packet, RC: %u\n", skb_info.rc);

    trap_id = xp_trap_id_get(skb_info.rc);
    trap_cfg = xp_trap_config_get(trap_id);

    /* No trap is registered then send it traffic to CB channel. */
    if (trap_id == XP_INVALID_TRAP_ID || !trap_cfg) {
        xp_rx_skb_fd_process(cb_sock_ptr, &skb_info);
        goto out;
    }

    switch (trap_cfg->ch) {
        case XP_NL_HOSTIF_TRAP_CB:
            xp_rx_skb_fd_process(cb_sock_ptr, &skb_info);
            break;

        case XP_NL_HOSTIF_TRAP_FD:
            xp_rx_skb_fd_process(trap_cfg->sock_ptr, &skb_info);
            break;

        case XP_NL_HOSTIF_TRAP_NETDEV:
            xp_rx_skb_netdev_process(&skb_info);
            break;

        default:
            goto out;
    }

    trap_cfg->rx_count++;

out:
    dev_kfree_skb_any(skb);
}

int xp_netdev_mode_init(void)
{
    int rc = 0;
    DBG("Enter: %s\n", __FUNCTION__);

    rc = xp_trap_init();
    if (rc < 0) {
        ERR("Unable to init a trap table.\n");
        goto err_xp_trap_init;
    }

    rc = xp_netlink_sock_init();
    if (rc < 0) {
        ERR("Unable to create a netlink socket.\n");
        goto err_xp_netlink_sock_init;
    }

    rc = xp_client_sock_init();
    if (rc < 0) {
        ERR("Unable to create a client socket.\n");
        goto err_xp_sock_create;
    }

    return 0;


err_xp_sock_create:
    xp_netlink_sock_deinit();

err_xp_netlink_sock_init:
    xp_trap_deinit();

err_xp_trap_init:
    return rc;
}

int xp_netdev_mode_deinit(void)
{
    DBG("Enter: %s\n", __FUNCTION__);

    xp_trap_deinit();
    xp_netlink_sock_deinit();
    xp_netdev_all_remove();
    xp_client_sock_deinit();

    return 0;
}

void xp_debug_set(int mode)
{
    DBG("Enter: %s\n", __FUNCTION__);
    xp_debug = mode;
}

void xp_netdev_print(struct seq_file *sf)
{
    unsigned int backet = 0;
    struct list_head *iter = NULL;
    struct xp_netdev_priv *entry = NULL;
    char *ch_name[] = { "FP", "ROUTER", "LAG" };
    unsigned long flags = 0;

    DBG("Enter: %s\n", __FUNCTION__);
    seq_printf(sf,"Netdev interfaces:\n");
    seq_printf(sf, "----------------------------------------------------\n");

    read_lock_irqsave(&xp_netdev_list_lock, flags);
    list_for_each(iter, &xp_netdev_list) {
        entry = list_entry(iter, struct xp_netdev_priv, list);
        seq_printf(sf," - Netdev: %8s, xpnet_id: %4u, vif/rif: %5d,"
                        " tx header: %u\n", entry->netdev->name,
                         entry->xpnet_intf_id, entry->vif,
                         entry->is_tx_header_set);
    }

   seq_printf(sf,"\nActive netdev interfaces:\n");
   seq_printf(sf,"----------------------------------------------------\n");
    hash_for_each(xp_active_netdev_htable, backet, entry, hlist) {
        seq_printf(sf," - Netdev: %8s, type: %s, xpnet_id: %4u, vif: %5d,"
        " tx header: %u\n", entry->netdev->name, ch_name[entry->netdev_type],
        entry->xpnet_intf_id, entry->vif, entry->is_tx_header_set);
    }
    read_unlock_irqrestore(&xp_netdev_list_lock, flags);
}

void xp_netdev_tx_header_print(unsigned int xpnet_intf_id, struct seq_file *sf)
{
    struct list_head *iter = NULL;
    unsigned long flags = 0;

    DBG("Enter: %s\n", __FUNCTION__);
    seq_printf(sf,"Netdev interfaces:\n");
    seq_printf(sf,"----------------------------------------------------\n");

    read_lock_irqsave(&xp_netdev_list_lock, flags);
    list_for_each(iter, &xp_netdev_list) {
        struct xp_netdev_priv *entry = 
            list_entry(iter, struct xp_netdev_priv, list);

        if (entry->is_tx_header_set) {
            seq_printf(sf," - Netdev: %8s, xpnet_id: %4u, vif: %5d\n", 
                entry->netdev->name, entry->xpnet_intf_id, entry->vif);

            seq_printf(sf,"     EVIF: B0(0x%2x) B1(0x%2x) B2(0x%2x); "
                "IVIF: B0(0x%2x) B1(0x%2x) B2(0x%2x); NextEngine: 0x%2x\n\n", 
                entry->tx_header.egressVifLsbByte0, 
                entry->tx_header.egressVifLsbByte1, 
                entry->tx_header.egressVifLsbByte2, 
                entry->tx_header.ingressVifLsbByte0,
                entry->tx_header.ingressVifLsbByte1, 
                entry->tx_header.ingressVifLsbByte2,
                entry->tx_header.nextEngine);
        }
    }

    read_unlock_irqrestore(&xp_netdev_list_lock, flags);
}

int xp_trap_table_print(struct seq_file *sf)
{
    unsigned int i = 0;
    unsigned long flags = 0;
    xp_trap_config_t *trap_cfg = NULL;
    char *ch_name[] = { "FD", "CB", "NETDEV" };

    DBG("Enter: %s\n", __FUNCTION__);

    seq_printf(sf,"|----------------------------------------------------|\n");
    seq_printf(sf,"|TrapId| RC | Fd | Channel | SockPtr      | rxPktCnt |\n");
    seq_printf(sf,"|----------------------------------------------------|\n");

    if (!trap_table) {
        return -EPERM;
    }

    spin_lock_irqsave(&trap_table->lock, flags);

    for (i = 0; i < ARRAY_SIZE(trap_table->trap); i++) {
        trap_cfg = trap_table->trap[i];

        if (trap_cfg) {
            seq_printf(sf, "|%6x|%4d|%4d|%9s|%14p|%10d|\n", trap_cfg->trap_id,
                       trap_cfg->rc, trap_cfg->fd, ch_name[trap_cfg->ch], 
                       trap_cfg->sock_ptr, trap_cfg->rx_count);
        }
    }

    seq_printf(sf,"|----------------------------------------------------|\n");
    seq_printf(sf,"|SockPtr for CB channel is :%17p\n", cb_sock_ptr);
    seq_printf(sf,"|----------------------------------------------------|\n");

    spin_unlock_irqrestore(&trap_table->lock, flags);
    return 0;
}

