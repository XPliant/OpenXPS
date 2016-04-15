/*****************************************************************************/
/* Copyright (C) 2016 Xpliant Inc Confidential                               */
/* Unpublished - rights reserved under the Copyright Laws of the             */
/* United States.  Use, duplication, or disclosure by the                    */
/* Government is subject to restrictions as set forth in                     */
/* sub paragraph (c)(1)(ii) of the Rights in Technical Data and              */
/* Computer Software clause at 252.227-7013.                                 */
/*****************************************************************************/
/*
 * This software is licensed to you under the terms of the GNU General Public
 * License version 2 (the "GPL"). 
 * TBD: need to update the GPL banner from Cavium Legal .
 */


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
    __u32 knet_intf_id;
    __u8  intf_name[IFNAMSIZ];
} xp_nl_msg_intf_t;

typedef struct xp_nl_msg_link {
    __u32 knet_intf_id;

    union {
        __u32 vif;
        __u32 rif;
    };
} xp_nl_msg_link_t;

typedef struct xp_nl_msg_tx_hdr {
    __u32 knet_intf_id;
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

