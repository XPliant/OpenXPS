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
* File: xp_reg_info.h
* 
* Abstract: This file contains the register definitions.
************************************************************************/
#ifndef _XP_REG_INFO_H
#define _XP_REG_INFO_H

#define XP_HP_INT_REG_SIZE_IN_DWRD   4
#define XP_LP_INT_REG_SIZE_IN_DWRD   3

#define NUM_HP_INT_REG_BLOCKS        35
#define NUM_LP_INT_REG_BLOCKS        33

#define DMA0_INTR_START_BIT_POS(addr_mode)  ((addr_mode) ? 9 : 8)

#define DMA0_TXDONE(reg_addr, qnum)  (reg_addr + 16 * qnum)
#define DMA0_RXDONE(reg_addr, qnum)  (reg_addr + 16 * qnum)

#define XP_GET_PCI_BASE_OFFSET_FROM_REG_NAME(reg_name, type) \
    (xp_regs_list[reg_name][type])

#define XP_GET_DMA_Q_REG_OFFSET(reg_offset, size, qnum) \
    reg_offset + (qnum * size * sizeof(u32));

typedef enum xp_address_mode {
    XP_A0_UNCOMPRESSED,
    XP_B0_UNCOMPRESSED,
    XP_B0_COMPRESSED,
    XP80_SUPPORTED_DEVICE_MODES
} xp_address_mode_t;

typedef enum xp_register {
    HIGH_PRIO_REG_ADDR,
    LOW_PRIO_REG_ADDR,
    HIGH_INTR_SOURCE_REG_ADDR,
    DMA0_TXDONE_STATUS_REG,
    DMA0_RXDONE_STATUS_REG,
    HIGH_PRIO_INT_ENABLE_REG_ADDR,
    LOW_PRIO_INT_ENABLE_REG_ADDR,

    DMA0_RX_CDP_REG_E,
    DMA0_TX_CDP_REG_E,
    DMA0_RX_CMD_REG_E,
    DMA0_TX_CMD_REG_E,
    CORE_CTRL_REG__1_E,
    TX_DMA0_RETRY_CNT_REG_E,
    MGMT_CTRL_REG_E,
    DMA0_CLR_ERR_CNT_REG_E,
    DMA0_TX_CHAIN_LEN_ERR_REG_E,
    DMA0_TX_CPU_OWN_DESC_ERR_REG_E,
    DMA0_TX_ZERO_BUF_LEN_ERR_REG_E,
    DMA0_TX_PCIE_ERR_REG_E,
    DMA0_TX_DMA_INTF_ERR_REG_E,
    DMA0_TX_PKT_DROP_E,
    TX_DMA0_CFG_REGLOCKREG_E,
    TX_DMA0_SCRATCHPAD_E,
    DMA0_RX_CHAIN_LEN_ERR_REG_E,
    DMA0_RX_CPU_OWN_DESC_ERR_REG_E,
    DMA0_RX_ZERO_BUF_LEN_ERR_REG_E,
    DMA0_RX_PCIE_ERR_REG_E,
    RX_DMA0_CFG_REGLOCKREG_E,
    RX_DMA0_SCRATCHPAD_E,

    XP_MAX_REG_ACCESS_LIST
} xp_register_t;

extern u32 xp_regs_list[XP_MAX_REG_ACCESS_LIST][XP80_SUPPORTED_DEVICE_MODES];

#endif /* _XP_REG_INFO_H */

