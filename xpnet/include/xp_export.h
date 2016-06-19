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
* File: xp_export.h
* 
* Abstract: This file contains the enums, macros, definitions to be exported.
************************************************************************/
#ifndef _XP_EXPORT_H
#define _XP_EXPORT_H

#include <linux/if.h>
#include <linux/types.h>
#include "xp_header.h"

typedef enum xp_nl_hostif_trap_channel {
    XP_NL_HOSTIF_TRAP_FD,     /* Receive packets via file desriptor */
    XP_NL_HOSTIF_TRAP_CB,     /* Receive packets via callback       */
    XP_NL_HOSTIF_TRAP_NETDEV, /* Receive packets via OS net device  */
    XP_NL_HOSTIF_TRAP_CUSTOM_RANGE_BASE = 0x10000000
} xp_nl_hostif_trap_channel_t;

typedef enum xp_nl_operation {
    XP_NL_OPERATION_ADD,
    XP_NL_OPERATION_REMOVE,
    XP_NL_OPERATION_ENABLE,
    XP_NL_OPERATION_DISABLE,
} xp_nl_operation_t;

typedef enum xp_nl_msg {
    XP_NL_MSG_INTF_ADD,   /* Create a netdev interface     */
    XP_NL_MSG_INTF_DEL,   /* Remove a netdev interface     */

    XP_NL_MSG_LINK_ADD,   /* Link a netdev with a VIF/RIF  */
    XP_NL_MSG_LINK_DEL,   /* Remove a netdev VIF/RIF link  */

    XP_NL_MSG_TX_HDR_SET, /* Add / remove a TX meta header */
    XP_NL_MSG_TRAP_SET,   /* Add / remove TRAP table entry */
    XP_NL_MSG_CB_FD_SET,  /* Add / remove FD callback      */

    XP_NL_MSG_MIRROR_SET, /* Enable / disable mirroring    */
} xp_nl_msg_t;

typedef struct xp_nl_msg_intf {
    __u32 xpnet_intf_id;
    __u8  intf_name[IFNAMSIZ];
} xp_nl_msg_intf_t;

typedef struct xp_nl_msg_link {
    __u32 xpnet_intf_id;

    union {
        __u32 vif;
        __u32 rif;
    };
} xp_nl_msg_link_t;

typedef struct xp_nl_msg_tx_hdr {
    __u32 xpnet_intf_id;
    __u8  operation;
    __u8  tx_header[sizeof(xphTxHdr)];
} xp_nl_msg_tx_hdr_t;

typedef struct xp_nl_msg_trap {
    __u32 rc;
    __u32 fd;
    __u32 ch;
    __u32 trap_id;
    __u8  operation;
} xp_nl_msg_trap_t;

typedef struct xp_nl_msg_cb_fd {
    __u32 fd;
    __u8  operation;
} xp_nl_msg_cb_fd_t;

typedef struct xp_nl_msg_mirror {
    __u8 operation;
} xp_nl_msg_mirror_t;

typedef struct xp_nl_tlv_msg {
    __u32 msg_type;    /* xp_nl_msg_t      */
    __u32 payload_len; /* Len of payload[] */
    __u8  payload[];   /* Payload          */
} xp_nl_tlv_msg_t;

#endif /* _XP_EXPORT_H */

