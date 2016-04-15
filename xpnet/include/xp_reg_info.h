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


#ifndef _XP_REG_INFO_H
#define _XP_REG_INFO_H

#define XP_HP_INT_REG_SIZE_IN_DWRD   4
#define XP_LP_INT_REG_SIZE_IN_DWRD   3

#define NUM_HP_INT_REG_BLOCKS        35
#define NUM_LP_INT_REG_BLOCKS        33

#define DMA0_INTR_START_BIT_POS      8
#define DMA0_TXDONE(reg_addr, qnum)  (reg_addr + 16 * qnum)
#define DMA0_RXDONE(reg_addr, qnum)  (reg_addr + 16 * qnum)

#define XP_GET_PCI_BASE_OFFSET_FROM_REG_NAME(reg_name, type) \
    (xp_regs_list[reg_name][type])

#define XP_GET_DMA_Q_REG_OFFSET(reg_offset, size, qnum) \
    reg_offset + (qnum * size * sizeof(u32));

typedef enum xp_mode {
    XP_A0_UNCOMPRESSED,
    XP_B0_UNCOMPRESSED,
    XP_B0_COMPRESSED,
    XP80_SUPPORTED_DEVICE_MODES
} xp_mode_t;

typedef enum xp_register {
    XP_HIGH_PRIO_REG_ADDR,
    XP_LOW_PRIO_REG_ADDR,
    XP_HIGH_INTR_SOURCE_REG_ADDR,
    XP_DMA0_TXDONE_STATUS_REG,
    XP_DMA0_RXDONE_STATUS_REG,
    XP_HIGH_PRIO_INT_ENABLE_REG_ADDR,
    XP_LOW_PRIO_INT_ENABLE_REG_ADDR,

    XP_MGMT_RX_DMA0_CFG_REG_DMA0_RX_CDP_REG_E,
    XP_MGMT_TX_DMA0_CFG_REG_DMA0_TX_CDP_REG_E,
    XP_MGMT_RX_DMA0_CFG_REG_DMA0_RX_CMD_REG_E,
    XP_MGMT_TX_DMA0_CFG_REG_DMA0_TX_CMD_REG_E,
    XP_MGMT_LOCAL_REG_CORE_CTRL_REG__1_E,
    XP_MGMT_LOCAL_REG_TX_DMA0_RETRY_CNT_REG_E,
    XP_MGMT_LOCAL_REG_MGMT_CTRL_REG_E,
    XP_MGMT_TX_DMA0_CFG_REG_DMA0_CLR_ERR_CNT_REG_E,
    XP_MGMT_TX_DMA0_CFG_REG_DMA0_TX_CHAIN_LEN_ERR_REG_E,
    XP_MGMT_TX_DMA0_CFG_REG_DMA0_TX_CPU_OWN_DESC_ERR_REG_E,
    XP_MGMT_TX_DMA0_CFG_REG_DMA0_TX_ZERO_BUF_LEN_ERR_REG_E,
    XP_MGMT_TX_DMA0_CFG_REG_DMA0_TX_PCIE_ERR_REG_E,
    XP_MGMT_TX_DMA0_CFG_REG_DMA0_TX_DMA_INTF_ERR_REG_E,
    XP_MGMT_TX_DMA0_CFG_REG_DMA0_TX_PKT_DROP_E,
    XP_MGMT_TX_DMA0_CFG_REG_XP_MGMT_TX_DMA0_CFG_REGLOCKREG_E,
    XP_MGMT_TX_DMA0_CFG_REG_SCRATCHPAD_E,
    XP_MGMT_RX_DMA0_CFG_REG_DMA0_RX_CHAIN_LEN_ERR_REG_E,
    XP_MGMT_RX_DMA0_CFG_REG_DMA0_RX_CPU_OWN_DESC_ERR_REG_E,
    XP_MGMT_RX_DMA0_CFG_REG_DMA0_RX_ZERO_BUF_LEN_ERR_REG_E,
    XP_MGMT_RX_DMA0_CFG_REG_DMA0_RX_PCIE_ERR_REG_E,
    XP_MGMT_RX_DMA0_CFG_REG_XP_MGMT_RX_DMA0_CFG_REGLOCKREG_E,
    XP_MGMT_RX_DMA0_CFG_REG_SCRATCHPAD_E,

    XP_MAX_REG_ACCESS_LIST
} xp_register_t;

extern u32 xp_regs_list[XP_MAX_REG_ACCESS_LIST][XP80_SUPPORTED_DEVICE_MODES];

#endif /* _XP_REG_INFO_H */

