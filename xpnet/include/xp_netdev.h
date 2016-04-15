/************************************************************************/
/*  Copyright (c) [2016] Cavium, Inc. All rights reserved.              */
/*  Unpublished - rights reserved under the Copyright Laws of the       */
/*  United States.  Use, duplication, or disclosure by the              */
/*  Government is subject to restrictions as set forth in               */
/*  subparagraph (c)(1)(ii) of the Rights in Technical Data and         */
/*  Computer Software clause at 252.227-7013.                           */
/************************************************************************/
/*
 * This software is licensed to you under the terms of the GNU General Public
 * License version 2 (the "GPL"). 
 * TBD: need to update the GPL banner from Cavium Legal .
 */


#ifndef _XP_NETDEV_H
#define _XP_NETDEV_H

#include <linux/pci.h>
#include <linux/version.h>
#include <linux/etherdevice.h>
#include <linux/proc_fs.h>
#include <net/inet_sock.h>

#include "xp_header.h"
#include "xp_pcie_slave.h"
#include "xp_reg_info.h"

#define XPNET_RX_NUM_QUEUES             (1)
#define XPNET_RX_NUM_DESCS              (10)
#define XPNET_TX_NUM_QUEUES             (1)
#define XPNET_TX_NUM_DESCS              (10)
#define XPNET_MAX_DMA_SEGMENT_SIZE      (4 << 10)
#define XPNET_DESC_SIZE                 (sizeof(xpnet_descriptor_t))
#define XPNET_MAX_ATTEMPTS              (50)

#define XPNET_CEIL_LEN(len, align)\
({\
    ((len + align - 1) & ~(align - 1));\
})

#define xpnet_decr_and_wrap(idx, limit)\
({\
    (((idx + limit) - 1) % (limit));\
})

#define xpnet_add_and_wrap(idx, incr, limit)\
({\
    (((idx + limit) + (incr)) % (limit));\
})

#define xpnet_get_busy_desc(q)\
({\
    ((q->xpq_num_desc + q->tail - (q->head + 1)) % q->xpq_num_desc);\
})

#define xpnet_get_free_desc(q)\
({\
    (q->xpq_num_desc - 1 - (xpnet_get_busy_desc(q)));\
})

typedef enum xpnet_enum {
    XPNET_OK = 0,
    XPNET_QUEUE_TYPE_RX = 0x100,
    XPNET_QUEUE_TYPE_TX,
    /* Not enough room for 4byte aligned length */
    XPNET_REASON_TAILROOM,
    XPNET_REASON_4KCROSS,
    XPNET_REASON_SHORTPACKET, /* < 84 bytes */
    XPNET_QUEUE_STOPPED,
    XPNET_QUEUE_ACTIVE,
    XPNET_QUEUE_RESET_PENDING,
    XPNET_QUEUE_BUSY,
    XPNET_QUEUE_ERROR,
    XPNET_TYPE_MAX
} xpnet_enum_t;

typedef struct xpnet_descriptor {
    /* Memory overlay of the descriptor in DMA memory */
    u64 qword[4];
} xpnet_descriptor_t;

typedef struct xpnet_shards_struct {
    void *va;       /* Virtual address of the buffer                      */
    dma_addr_t dma; /* Mapped dma address of the buffer                   */
    int len;        /* Length of buffer filled, allocated len = PAGE_SIZE */
} xpnet_shards_struct_t;

typedef struct xpnet_desc_struct {
    /* Metadata required for book-keeping of the descriptors in a queue   */
    int num_shards; /* set only for the first one, how many shards        */
    xpnet_descriptor_t *va;
    dma_addr_t cdp; /* to update the cdp in h/w                           */

    xpnet_shards_struct_t buf_sta;
} xpnet_desc_struct_t;

typedef struct xpnet_queue_struct {
/* Metadata required for book-keeping of the queue                          */
    spinlock_t xpq_lock;   /* Lock for this queue access                    */
    u32 xpq_id;            /* ID of the queue                               */
    xpnet_enum_t xpq_type; /* Enum RX/TX                                    */
    u32 xpq_num_desc;      /* Elements on xpq_desc                          */
    int status;            /* Stopped, active, etc                          */
    int tail;              /* Index where producer function operates on     */
    int head;              /* Index where consumer function operates on     */
    void *va;              /* VA of the start of descriptor chain           */
    dma_addr_t dma;        /* DMA address of the start of descriptor chain  */

    /* Metadata of the descriptors */
    xpnet_desc_struct_t xpq_desc_meta[XPNET_RX_NUM_DESCS];
} xpnet_queue_struct_t;

typedef struct xpnet_private {
    u32 hw_flags;            /* 32 bits h/w specific  usage, e.g., revision */
    struct pci_dev *pdev;    /* OS specific structure                       */
    xp_private_t *pci_priv;  /* Only for pci char drv dep                   */

    struct napi_struct napi; /* Currently unused                            */

    /* Host RX and host TX complete */
    struct workqueue_struct *wqueue;
    struct delayed_work dwork;

    /* RX descritpr ring for queues */
    xpnet_queue_struct_t rx_queue[XPNET_RX_NUM_QUEUES];
    xpnet_queue_struct_t tx_queue[XPNET_RX_NUM_QUEUES];
    int num_rxqueues, num_txqueues; /* Number of queues enabled              */

    struct proc_dir_entry *proc;    /* Debug and other info                  */
    int txqno, rxqno;               /* CPU RX and TX queue numbers. XXX TODO */
    int state;                      /* Being reset, up, down, etc            */
    spinlock_t priv_lock;           /* Lock for this private data structure  */
    int dma_trigger;                /* Lock for this private data structure  */

    struct net_device_stats stats;  /* Netdev structure for DMA statastic    */
} xpnet_private_t;

extern xpnet_private_t *g_net_priv;


int32_t xp_dev_reg_write_q(xpnet_private_t *net_priv, u32 reg_id, 
                           u8 data_size, u32 *value, u32 qnum);
int32_t xp_dev_reg_read_q(xpnet_private_t *net_priv, u32 reg_id, 
                          u8 data_size, u32 *value, u32 qnum);

#endif /* _XP_NETDEV_H */

