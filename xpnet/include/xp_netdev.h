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
* File: xp_netdev.h
* 
* Abstract: This file contains the enums, macros, defs required for netdev 
* support in this kernel module.
************************************************************************/
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

#define XPNET_NUM_QUEUES             	(64)
#define XPNET_RX_NUM_QUEUES             (64)
#define XPNET_RX_NUM_DESCS              (10)
#define XPNET_TX_NUM_QUEUES             (64)
#define XPNET_TX_NUM_DESCS              (10)
#define XPNET_MAX_DMA_SEGMENT_SIZE      (4 << 10)
#define XPNET_DESC_SIZE                 (sizeof(xpnet_descriptor_t))
#define XPNET_MAX_ATTEMPTS              (50)

#define XPNET_PROC_FILE_NAME            "xpnet"
#define XPNET_PROC_FILE_PATH            "/proc/" XPNET_PROC_FILE_NAME

#define XPNET_PROC_STATS                "stats"
#define XPNET_PROC_TRAP_TABLE           "trap-table"
#define XPNET_PROC_DEBUG                "debug"
#define XPNET_PROC_NETDEV               "net-dev"
#define XPNET_PROC_TXHDR                "tx-hdr"

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

#define NAME_STR_HELP    "help"     /*help string name macro*/
#define NAME_STR_REG     "reg"      /*Reg string name macro*/
#define NAME_STR_TRAP    "trap"     /*Trap string name macro*/
#define NAME_STR_STAT    "stat"     /*Stat string name macro*/
#define NAME_STR_DESC    "desc"     /*Desc string name macro*/
#define NAME_STR_NETDEV  "net-dev"  /*net-dev string name macro*/
#define NAME_STR_TXHDR   "tx-hdr"   /*tx-hdr string name macro*/

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
    struct delayed_work dwork_tx_trig;

    /* RX descritpr ring for queues */
    xpnet_queue_struct_t rx_queue[XPNET_RX_NUM_QUEUES];
    xpnet_queue_struct_t tx_queue[XPNET_RX_NUM_QUEUES];
    int num_rxqueues, num_txqueues; /* Number of queues enabled              */

    int txqno, rxqno;               /* CPU RX and TX queue numbers.          */
    spinlock_t priv_lock;           /* Lock for this private data structure  */
    int dma_trigger;                /* Lock for this private data structure  */

    struct net_device_stats stats;  /* Netdev structure for DMA statastic    */
    int instance;                                             /* Instance number */
    struct proc_dir_entry *proc_root;     		      /* PDE for root xpnet node  */
    struct proc_dir_entry *proc_que[XPNET_RX_NUM_QUEUES];     /* PDE for all 64 queues */
    struct proc_dir_entry *proc_stats;                        /* PDE for stats */
    struct proc_dir_entry *proc_ttable;                       /* PDE for trap table */
    struct proc_dir_entry *proc_debug;                             /* PDE for debug mode */
    struct proc_dir_entry *proc_netdev;                             /* PDE for net dev */
    struct proc_dir_entry *proc_txhdr;                             /* PDE for tx header*/
} xpnet_private_t;

typedef struct xpnet_que_info {
    xpnet_private_t *priv;
    int que_no;
}xpnet_que_info_t;

extern xpnet_private_t *g_net_priv;


int32_t xp_dev_reg_write_q(xpnet_private_t *net_priv, u32 reg_id, 
                           u8 data_size, u32 *value, u32 qnum);
int32_t xp_dev_reg_read_q(xpnet_private_t *net_priv, u32 reg_id, 
                          u8 data_size, u32 *value, u32 qnum);

#endif /* _XP_NETDEV_H */

