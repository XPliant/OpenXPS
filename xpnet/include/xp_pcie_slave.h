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


#ifndef _XP_PCIE_SLAVE_H
#define _XP_PCIE_SLAVE_H

#include "xp_reg_info.h"
#include <linux/cdev.h>

/* Register Read/Write */
typedef struct xp_reg_rw {
    u32 reg_address;  /* Address to read/write */

    /* If count > 1, contiguous access of registers
       starting from regAddress up-till count */
    u16 count;

    /* 0->Write, 1->Read, Note: Direction will be
       same for all contiguous access */
    u8  direction;

    /* Size to be read/write in bytes,
       valid values are: 1,2,4 */
    u8  size;

    /* Value(s) to be written/read */
    u8 *value;
} xp_reg_rw_t;

typedef struct xp_private {
    /* OS specific structure. */
    struct pci_dev *pdev;
    /* Mem-mapped pointer to base of chip regs. */
    u64 __iomem *vma;
    /* End of mem-mapped chip space excluding sendbuf and user regs. */
    u64 __iomem *mmio_end;      
    /* Physical address of chip for io_remap, etc. */
    resource_size_t mmio_start;
    /* User specific structure. */
    xp_reg_rw_t *pci_rw;   
    s64 irq;
    struct workqueue_struct *w_queue;
    struct device *dev;
    struct cdev cdev;
    s32 msi_count;
    s32 msi_fail;
    /* For storing xpnet_private structure. */
    struct xpnet_private *xpnet;
    /* Lock to make tx DMA and reg read mutual exclusive. */ 
    spinlock_t tx_dma_read_lock;
    pid_t app_pid;
    struct siginfo sig_info;

    /* Device type with mode(compress or uncompress). */
    xp_mode_t mode;
} xp_private_t;

#endif /* _XP_PCIE_SLAVE_H */

