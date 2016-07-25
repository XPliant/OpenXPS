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
* File: xp_pcie_slave.h
* 
* Abstract: This file contains the enums, macros, msg defs required for pcie 
* slave component/module of this driver.
************************************************************************/
#ifndef _XP_PCIE_SLAVE_H
#define _XP_PCIE_SLAVE_H

#include "xp_reg_info.h"
#include <linux/cdev.h>

#define PROC_FS_NAME_SIZE 50
#define REG_RW_STATUS_SIZE 200
#define HIGH_INTR_SRC_REG_SIZE 5
 
/* Register Read/Write */
typedef struct __attribute__((__packed__)) xp_reg_rw {
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
    u64 value;
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
    xp_address_mode_t mode;

    /* For storing PDE entry */
    struct proc_dir_entry *reg_proc;
    /* For storing PDE name */
    char proc_fs_name[PROC_FS_NAME_SIZE];
    /* For storing reg read/write status */
    char reg_rw_status[REG_RW_STATUS_SIZE];
} xp_private_t;

#endif /* _XP_PCIE_SLAVE_H */

