/************************************************************************
 * Copyright (C) 2016, Cavium, Inc.
 * All Rights Reserved.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License"); you may
 *   not use this file except in compliance with the License. You may obtain
 *   a copy of the License at http://www.apache.org/licenses/LICENSE-2.0.
 *   
 *   THIS CODE IS PROVIDED ON AN *AS IS* BASIS, WITHOUT WARRANTIES OR 
 *   CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
 *   LIMITATIONS ANY IMPLIED WARRANTIES OR CONDITIONS OF TITLE, FITNESS
 *   FOR PARTICULAR PURPOSE, MERCHANTABILITY OR NON-INFRINGEMENT.
 *   
 *   See the Apache Version 2.0 License for specific language governing 
 *   permissions and limitations under the License.
 *   
 *   FILE : openXpsEnums.h
 * 
 *   Abstract: This file defines the XP enumerations in OpenXPS.
 ************************************************************************/
/**
 * \file openXpsEnums.h
 * \brief This file contains openXps enum definitions
 * \copyright (c) 2016 Cavium Inc
 */

#ifndef _openXpsEnums_h_
#define _openXpsEnums_h_

/**
 * \brief This type (enum) defines the hashable fields
 */
typedef enum xpHashField
{
    XP_ETHERNET_MAC_DA,                       ///< Ethernet MAC DA
    XP_ETHERNET_MAC_SA,                       ///< Ethernet MAC SA
    XP_ETHERNET_E_TAG_TPID,                   ///< Ethernet 802.1Q Tag Protocol Identifier (TPID)
    XP_ETHERNET_E_TAG_E_PCP,                  ///< Ethernet 802.1Q Tag Priority Code Point (PCP)
    XP_ETHERNET_E_TAG_E_DEI,                  ///< Ethernet 802.1Q Tag Drop Eligible Indicator (DEI)
    XP_ETHERNET_E_TAG_INGRESS_E_CID_BASE,
    XP_ETHERNET_E_TAG_RESERVED,               ///< Reserved
    XP_ETHERNET_E_TAG_GRP,
    XP_ETHERNET_E_TAG_E_CID_BASE,
    XP_ETHERNET_E_TAG_INGRESS_E_CID_EXT,
    XP_ETHERNET_E_TAG_E_CID_EXT,
    XP_ETHERNET_O_TAG_TPID,
    XP_ETHERNET_O_TAG_PCP,
    XP_ETHERNET_O_TAG_DEI,
    XP_ETHERNET_O_TAG_VID,
    XP_ETHERNET_S_TAG_TPID,
    XP_ETHERNET_S_TAG_PCP,
    XP_ETHERNET_S_TAG_DEI,
    XP_ETHERNET_S_TAG_VID,
    XP_ETHERNET_C_TAG_TPID,
    XP_ETHERNET_C_TAG_PCP,
    XP_ETHERNET_C_TAG_DEI,
    XP_ETHERNET_C_TAG_VID,
    XP_ETHERNET_ETHERTYPE,
    XP_TU_ETHER_MAC_DA,
    XP_TU_ETHER_MAC_SA,
    XP_TU_ETHER_S_TAG_TPID,
    XP_TU_ETHER_S_TAG_PCP,
    XP_TU_ETHER_S_TAG_DEI,
    XP_TU_ETHER_S_TAG_VID,
    XP_TU_ETHER_C_TAG_TPID,
    XP_TU_ETHER_C_TAG_PCP,
    XP_TU_ETHER_C_TAG_DEI,
    XP_TU_ETHER_C_TAG_VID,
    XP_TU_ETHER_ETHERTYPE,
    XP_RESERVED1,
    XP_RESERVED2,
    XP_RESERVED3,
    XP_RESERVED4,
    XP_RESERVED5,
    XP_RESERVED6,
    XP_RESERVED7,
    XP_PBB_MAC_DA,
    XP_PBB_MAC_SA,
    XP_PBB_E_TAG_TPID,
    XP_PBB_E_TAG_E_PCP,
    XP_PBB_E_TAG_E_DEI,
    XP_PBB_E_TAG_INGRESS_E_CID_BASE,
    XP_PBB_E_TAG_RESERVED,
    XP_PBB_E_TAG_GRP,
    XP_PBB_E_TAG_E_CID_BASE,
    XP_PBB_E_TAG_INGRESS_E_CID_EXT,
    XP_PBB_E_TAG_E_CID_EXT,
    XP_PBB_O_TAG_TPID,
    XP_PBB_O_TAG_PCP,
    XP_PBB_O_TAG_DEI,
    XP_PBB_O_TAG_VID,
    XP_PBB_B_TAG_TPID,
    XP_PBB_B_TAG_PCP,
    XP_PBB_B_TAG_DEI,
    XP_PBB_B_TAG_VID,
    XP_PBB_I_SID_TPID,
    XP_PBB_I_SID_I_PCP,
    XP_PBB_I_SID_I_DEI,
    XP_PBB_I_SID_UCA,
    XP_PBB_I_SID_RESERVED,
    XP_PBB_I_SID,
    XP_ARP_HTYPE,
    XP_ARP_PTYPE,
    XP_ARP_HLEN,
    XP_ARP_PLEN,
    XP_ARP_OPER,
    XP_ARP_SHA,
    XP_ARP_SPA,
    XP_ARP_SHA_2,
    XP_ARP_SPA_2,
    XP_IPV4_VERSION,
    XP_IPV4_IHL,
    XP_IPV4_DSCP,
    XP_IPV4_ECN,
    XP_IPV4_TOTAL_LENGTH,
    XP_IPV4_IDENTIFICATION,
    XP_IPV4_RESERVED_1,
    XP_IPV4_DF,
    XP_IPV4_MF,
    XP_IPV4_FRAGMENT_OFFSET,
    XP_IPV4_TIME_TO_LIVE,
    XP_IPV4_PROTOCOL,
    XP_IPV4_HEADER_CHECKSUM,
    XP_IPV4_SOURCE_IP_ADDRESS,
    XP_IPV4_DESTINATION_IP_ADDRESS,
    XP_IPV4_OPTIONS,
    XP_IPV6_VERSION,
    XP_IPV6_ECN,
    XP_IPV6_FLOW_LABEL,
    XP_IPV6_PAYLOAD_LENGTH,
    XP_IPV6_NEXT_HEADER,
    XP_IPV6_TIME_TO_LIVE,
    XP_IPV6_SOURCE_IP_ADDRESS,
    XP_IPV6_SPLIT_DESTINATION_IP_ADDRESS,
    XP_MPLS_LABEL0,
    XP_MPLS_EXP0,
    XP_MPLS_BS0,
    XP_MPLS_TTL0,
    XP_MPLS_LABEL1,
    XP_MPLS_EXP1,
    XP_MPLS_BS1,
    XP_MPLS_TTL1,
    XP_MPLS_LABEL2,
    XP_MPLS_EXP2,
    XP_MPLS_BS2,
    XP_MPLS_TTL2,
    XP_MPLS_LABEL3,
    XP_MPLS_EXP3,
    XP_MPLS_BS3,
    XP_MPLS_TTL3,
    XP_MPLS_CONTROL_WORD,
    XP_GRE_C,
    XP_GRE_R,
    XP_GRE_K,
    XP_GRE_S,
    XP_GRE_VERSION,
    XP_GRE_PROTOCOL,
    XP_GRE_CHECKSUM,
    XP_GRE_RESERVED_2,
    XP_GRE_KEY,
    XP_GRE_SEQ_NUM,
    XP_ICMP_TYPE,
    XP_ICMP_CODE,
    XP_ICMP_CHECKSUM,
    XP_ICMP_MESSAGE_SPECIFIC_INFO,
    XP_IGMP_VERSIONORTYPE,
    XP_IGMP_MAX_RESPONSE_TIME,
    XP_IGMP_CHECKSUM,
    XP_IGMP_GROUP_ADDRESS,
    XP_ICMPV6_TYPE,
    XP_ICMPV6_CODE,
    XP_ICMPV6_CHECKSUM,
    XP_ICMPV6_MESSAGE_BODY,
    XP_UDP_SOURCE_PORT,
    XP_UDP_DESTINATION_PORT,
    XP_UDP_LENGTH,
    XP_UDP_CHECKSUM,
    XP_TCP_SOURCE_PORT,
    XP_TCP_DESTINATION_PORT,
    XP_TCP_SEQUENCE_NUMBER,
    XP_TCP_ACK_NUMBER,
    XP_TCP_OFFSET,
    XP_TCP_RESERVED,
    XP_TCP_FLAGS,
    XP_TCP_WINDOW,
    XP_TCP_CHECKSUM,
    XP_TCP_URGENT_POINTER,
    XP_TCP_TCP_OPTIONS,
    XP_FCOE_VERSION,
    XP_FCOE_RESERVED,
    XP_FCOE_SOF,
    XP_FCOE_R_CTL,
    XP_FCOE_D_ID,
    XP_FCOE_CS_CTL_PRIORITY,
    XP_FCOE_S_ID,
    XP_FCOE_TYPE,
    XP_FCOE_F_CTL,
    XP_FCOE_SEQ_ID,
    XP_FCOE_DF_CTL,
    XP_FCOE_SEQ_CNT,
    XP_FCOE_SPLIT_OX_ID,
    XP_FCOE_SPLIT_RX_ID,
    XP_FCOE_SPLIT_PARAMETER,
    XP_CNM_CNM_VER,
    XP_CNM_QFEEDBACK,
    XP_CNM_CPID,
    XP_CNM_CNMQOFFSET,
    XP_CNM_CNMQDELTA,
    XP_CNM_ENCAP_PRIORITY,
    XP_CNM_ENCAP_DA,
    XP_CNM_ENCAP_LEN,
    XP_GENEVE_VERSION,
    XP_GENEVE_OPTIONSLEN,
    XP_GENEVE_OAMFRAME,
    XP_GENEVE_CRITICALFLAG,
    XP_GENEVE_RESERVED_1,
    XP_GENEVE_PROTOCOL,
    XP_GENEVE_VNI,
    XP_GENEVE_RESERVED_2,
    XP_GENEVE_OPTIONCLASS,
    XP_GENEVE_TYPE,
    XP_GENEVE_RESERVED_3,
    XP_GENEVE_OPTLEN,
    XP_GENEVE_RESERVED_4,
    XP_GENEVE_FLOWID,
    XP_GENEVE_OPTIONCLASS_2,
    XP_GENEVE_TYPE_2,
    XP_GENEVE_RESERVED_5,
    XP_GENEVE_OPTLEN_2,
    XP_GENEVE_RESERVED_6,
    XP_GENEVE_TEMPLATEID,
    XP_GRE_NOLAYER_C,
    XP_GRE_NOLAYER_R,
    XP_GRE_NOLAYER_K,
    XP_GRE_NOLAYER_S,
    XP_GRE_NOLAYER_VERSION,
    XP_GRE_NOLAYER_PROTOCOL,
    XP_GRE_NOLAYER_CHECKSUM,
    XP_GRE_NOLAYER_RESERVED_2,
    XP_GRE_NOLAYER_KEY,
    XP_GRE_NOLAYER_SEQ_NUM,
    XP_UDPVXLANNSH_SOURCE_PORT,
    XP_UDPVXLANNSH_DESTINATION_PORT,
    XP_UDPVXLANNSH_LENGTH,
    XP_UDPVXLANNSH_CHECKSUM,
    XP_UDPVXLANNSH_FLAGS,
    XP_UDPVXLANNSH_RESERVED_1,
    XP_UDPVXLANNSH_NEXTPROTOCOL,
    XP_UDPVXLANNSH_VNI,
    XP_UDPVXLANNSH_RESEVED_2,
    XP_UDPVXLANNSH_FLAGS_1,
    XP_UDPVXLANNSH_NSH_LENGTH,
    XP_UDPVXLANNSH_MD_TYPE,
    XP_UDPVXLANNSH_NSHNEXTPROTOCOL,
    XP_UDPVXLANNSH_SERVICEPATHID,
    XP_UDPVXLANNSH_SERVICEINDEX,
    XP_UDPVXLANNSH_SOURCEISLANDID,
    XP_UDPVXLANNSH_DESTISLANDID,
    XP_UDPVXLANNSH_RESERVED_3,
    XP_NSH_FLAGS_1,
    XP_NSH_NSH_LENGTH,
    XP_NSH_MD_TYPE,
    XP_NSH_NSHNEXTPROTOCOL,
    XP_NSH_SERVICEPATHID,
    XP_NSH_SERVICEINDEX,
    XP_NSH_SOURCEISLANDID,
    XP_NSH_DESTISLANDID,
    XP_NSH_RESERVED_3,
    XP_NUM_HASH_FIELDS
}xpHashField;

/**
 * \brief This type (enum) lists the packet forwarding processing commands  
 */
typedef enum xpPktCmd_e
{
    XP_PKTCMD_DROP = 0,     ///< DROP the packet
    XP_PKTCMD_FWD,          ///< FORWARD the packet
    XP_PKTCMD_TRAP,         ///< TRAP the packet   
    XP_PKTCMD_FWD_MIRROR,   ///< FORWARD and Mirror the packet
    XP_PKTCMD_MAX
} xpPktCmd_e;

/**
 * \brief This type (enum) defines the accepted frame types at ingress port
 */
typedef enum
{
    FRAMETYPE_ALL = 0,
    FRAMETYPE_UN_PRI,
    FRAMETYPE_TAG,
    FRAMETYPE_MAX
}xpsPortFrameType_e;

/**
 * \brief This type (enum) defines the port configuration parameters
 */
typedef enum xpsPortConfigFieldList_t
{
     XPS_PORT_FIELD_NUM_MIN,
     XPS_PORT_DEFAULT_EXP,
     XPS_PORT_DEFAULT_DSCP,
     XPS_PORT_DEFAULT_DEI,
     XPS_PORT_DEFAULT_PCP,
     XPS_PORT_DEFAULT_DP,
     XPS_PORT_DEFAULT_TC,
     XPS_PORT_MPLS_QOS_EN,
     XPS_POST_IP_QOS_EN,
     XPS_PORT_LAYER2_QOS_EN,
     XPS_PORT_EGRESS_FILTER_ID,
     XPS_PORT_SET_INGRESS_VIF,
     XPS_PORT_SET_BRIDGE_DOMAIN,
     XPS_PORT_ACCEPTED_FRAME_TYPE,
     XPS_PORT_BYPASS_TUNNEL_VIF,
     XPS_PORT_BYPASS_ACLSPBR,
     XPS_PORT_SAMPLER_EN,
     XPS_PORT_POLICER_EN,
     XPS_PORT_STATE,
     XPS_PORT_SET_EGRESS_PORT_FILTER,
     XPS_PORT_MAC_SAMISSCMD,
     XPS_PORT_BUM_POLICER_EN,
     XPS_PORT_ACL_EN,
     XPS_PORT_DEBUG_EN,
     XPS_PORT_ACL_ID,
     XPS_PORT_PRIVATE_WIRE_EN,
     XPS_PORT_ADD_PORT_BASED_TAG,
     XPS_PORT_RESERVED2,
     XPS_PORT_BRIDGE_DOMAIN,
     XPS_PORT_EVIF,
     XPS_PORT_MIRROR_BITMASK,
     XPS_PORT_INGRESS_VIF,
     XPS_PORT_PVID_MODE_ALLPKT,
     XPS_PORT_PVID,
     XPS_PORT_MPLS_QOS_PROFILE_IDX,
     XPS_PORT_IP_QOS_PROFILE_IDX,
     XPS_PORT_L2QOS_PROFILE_IDX,
     XPS_PORT_ETAG_EXISTS,
     XPS_PORT_FIELD_NUM_MAX
} xpsPortConfigFieldList_t;


/**
 * \typedef xpCountMode_e
 * \brief This type (enum) defines the counting modes
 *        The configuration is a bitmask, which can enable
 *        counting (bit 0), policing (bit 1) and sampling (bit2).
 *        
 */
typedef enum xpCountMode_e
{
    COUNT_MODE_NONE = 0x0,  ///< None of Counting, Policing and Sampling
    COUNT_MODE_CNT,         ///< Counting only
    COUNT_MODE_POL,         ///< Policing only
    COUNT_MODE_CNT_POL,     ///< Counting and Policing
    COUNT_MODE_SMP,         ///< Sampling only
    COUNT_MODE_CNT_SMP,     ///< Counting and Sampling
    COUNT_MODE_POL_SMP,     ///< Policing and Sampling
    COUNT_MODE_CNT_POL_SMP, ///< Counting, Policing and Sampling
    COUNT_MODE_MAX
} xpCountMode_e;

/**
 * \brief This type (enum) defines an the egress encapsulation
 *        types for an L2 Domain.
 */
typedef enum xpL2EncapType_e
{
    XP_L2_ENCAP_DOT1Q_UNTAGGED,   ///< Ethernet 802.1Q UnTagged
    XP_L2_ENCAP_DOT1Q_TAGGED,     ///< Ethernet 802.1Q Tagged
    XP_L2_ENCAP_DOT1Q_PRIOTAGGED, ///< Ethernet Priority Tagged
    XP_L2_ENCAP_QINQ_UNTAGGED,    ///< Strip both Ctag and Stag
    XP_L2_ENCAP_QINQ_CTAGGED,     ///< Strip only Stag
    XP_L2_ENCAP_QINQ_STAGGED,     ///< Add Stag
    XP_L2_ENCAP_VXLAN,            ///< VXLAN Tunnel
    XP_L2_ENCAP_NVGRE,            ///< NVGRE Tunnel             
    XP_L2_ENCAP_PBB,              ///< Provider Backbone Bridges (also known as "Mac-In-Mac") Tunnel
    XP_L2_ENCAP_GENEVE,           ///< GENEVE Tunnel
    XP_L2_ENCAP_MPLS,             ///< MPLS
    XP_L2_ENCAP_INVALID = -1
} xpL2EncapType_e;

/**
 * \brief This type (enum) defines the Multicast
 *        Bridging modes configurable for a VLAN.
 */
typedef enum xpVlanBridgeMcMode_e
{
    MC_BRIDGE_MODE_DISABLED = 0,  ///< Disabled
    MC_BRIDGE_MODE_FDB,           ///< FDB Mode  
    MC_BRIDGE_MODE_S_G,           ///< {S, G} IP MC Bridging Mode
    MC_BRIDGE_MODE_0_G,           ///< {0, G} IP MC Bridging Mode
    MC_BRIDGE_MODE_MAX
} xpVlanBridgeMcMode_e;

/**
 * \brief This type (enum) defines the Spanning Tree States
 *        for a port.
 */
typedef enum xpVlanStgState_e
{
    SPAN_STATE_DISABLE = 0,     ///< Disable Spanning Tree State
    SPAN_STATE_LEARN,           ///< Learning Spanning Tree State 
    SPAN_STATE_FORWARD,         ///< Forwarding Spanning Tree State
    SPAN_STATE_BLOCK,           ///< Blocking Spanning Tree State
    SPAN_STATE_MAX
} xpVlanStgState_e;

/**
 * \brief This type (enum) defines Count Mode.
 */
typedef enum xpBdCountMode_e
{
    BD_COUNT_MODE_NONE,                 ///< Counting is disabled
    BD_COUNT_MODE_ROUTED_AND_SWITCHED,  ///< Count both switched and routed traffic
    BD_COUNT_MODE_ONLY_ROUTED,          ///< Count only routed traffic
    BD_COUNT_MODE_MAX
} xpBdCountMode_e;

/**
 * \brief This type (enum) defines IP type  
 *        used as IP Route Table Prefix lookup.
 *
 **/
typedef enum xpIpPrefixType_t
{
    XP_PREFIX_TYPE_IPV4,         ///< IPv4   
    XP_PREFIX_TYPE_IPV6,         ///< IPv6  
    XP_PREFIX_TYPE_MAX
} xpIpPrefixType_t;

/**
 *\brief This type (enum) defines the device initialization modes
 *       INIT_COLD:
                    Performs addDevice and all respective managers
                    will perform "software init", "hardware init" and
                    load default values. This is used when the system
                    is initialized the first time.
         INIT_WARM: (shadow is not present in the system)
                    Performs addDevice and all respective managers
                    will perform "software init" only. Most cases the
                    managers should recreate config and state from hardware
                    but typical case there is no shadow maintained, hence
                    table/reg data is not modified on the live system.
         INIT_WARM_SYNC_TO_HW:
                    (shadow is available and is persistent across warm restart)
                    Performs addDevice, and software initialization
                    for all blocks. "software init" only. For
                    managers should load data from shadow to hardware
                    and push the configration to hardware.
         INIT_WARM_SYNC_TO_SHADOW:
                    (shadow is available and is synced by reading hardware)
                    Performs addDevice, and software initialization
                    for all blocks. "software init" only. For
                    managers should load data from hardware to shadow. For
                    other configurations and data is recreated/read from hw.
 */
typedef enum xpInitType_t
{
    INIT_UNKNOWN = 0,
    INIT_COLD,                      ///< Software and Hardware Initialization with Default Values
    INIT_WARM,                      ///< Software Initialization only, no Shadow present
    INIT_WARM_SYNC_TO_HW,           ///< Software Initialization only, Then load data from Shadow to Hardware
    INIT_WARM_SYNC_TO_SHADOW,       ///< Software Initialization only, Then load data from Hardware to Shadow
    INIT_MAX
} xpInitType_t;

/**
 * \brief This type (enum) defines the MAC configuration modes
 */
typedef enum xpMacConfigMode
{
    MAC_MODE_4X10GB            = 3,   ///< 4x10Gbps
    MAC_MODE_1X10GB            = 9,   ///< 1x10Gbps
    MAC_MODE_1X40GB            = 5,   ///< 1x40Gbps
    MAC_MODE_1X50GB            = 14,  ///< 50Gbps
    MAC_MODE_1X100GB           = 6,   ///< 1x100Gbps
    MAC_MODE_MAX_VAL           = 15,  ///< TOTAL NUMBER OF MAC_MODES
}xpMacConfigMode;

/**
 * \brief This type (enum) defines the Packet Driver 
 *        Rx modes
 */
typedef enum xpRxConfigMode
{
    INTR = 0,
    POLL
}xpRxConfigMode;


/**
 * \brief This type (enum) defines SAL object types 
 */

typedef enum xpSalType_e
{
   XP_SAL_HW_TYPE,    ///< Hardware
   XP_SAL_WM_TYPE,    ///< Simulation 
   XP_SAL_MAX_OS_TYPE,

   XP_SAL_OTHER_OS_TYPE,
}xpSalType_t;

/**
 * \brief This type (enum) defines the Packet Driver 
 *        callback handler types
 */
typedef enum xpCallbackMethodType
{
    PD_SEND = 0,
    PD_RECV,
    PD_ERR,
    PD_ALL
} xpCallbackMethodType;

/**
 * \brief This type (enum) defines the Packet Driver 
 *        Tx modes
 */
typedef enum txMode
{
    SYNC_TX,
    ASYNC_TX
} txMode;

/**
 * \brief This type (enum) defines the table aging modes
 */

typedef enum XP_AGE_MODE_T
{
    XP_AGE_MODE_AUTO,
    XP_AGE_MODE_TRIGGER,
    XP_AGE_MODE_INVALID
}XP_AGE_MODE_T;

/**
 * \brief This type (enum) defines the Packet Driver 
 *        Rx handler types
 */

typedef enum xpCpuRxPktCbType
{
    XP_L2_LEARN         = 0,
    XP_OPENFLOW         = 3,
    XP_MAX_CPU_RX_HDLR  = 6
} xpCpuRxPktCbType_e;

/**
 * \brief This type (enum) defines the Forward-Error-Correction (FEC) types
 */
typedef enum xpFecMode
{
    RS_FEC_MODE,  ///< Reed-Solomon FEC
    FC_FEC_MODE,  ///< Fire Code FEC
    MAX_FEC_MODE
}xpFecMode;

/**
 * \brief This type (enum) defines the serdes tuning types
 */
typedef enum xpSerdesDfeTuneMode_t
{
    DFE_ICAL,           ///< Initial calibration, coarse tuning. (Default)
    DFE_PCAL,           ///< Periodic calibration, fine tuning, no LF,HF adjustments
    DFE_START_ADAPTIVE, ///< Launch continous pCAL
    DFE_STOP_ADAPTIVE,  ///< Stop continous pCAL
    DFE_ENABLE_RR,      ///< Enable SerDes to participate in Round-Robin pCal
    DFE_DISABLE_RR      ///< Disable SerDes from participation in Round-Robin pCal
} xpSerdesDfeTuneMode_t;

/**
 * \brief This type (enum) defines the port speeds
 */
typedef enum xpSpeed
{
    SPEED_10MB = 0,   ///< 10Mbps
    SPEED_100MB,      ///< 100Mbps
    SPEED_1GB,        ///< 1Gbps
    SPEED_1GB_PCS,    ///< 1Gbps
    SPEED_10GB,       ///< 10Gbps
    SPEED_40GB,       ///< 40Gbps
    SPEED_100GB,      ///< 100Gbps
    SPEED_25GB,       ///< 25Gbps
    SPEED_50GB,       ///< 50Gbps
    SPEED_MAX_VAL
}xpSpeed;

/**
 * \brief This type (enum) defines the event source identifiers
 */
typedef enum xpEventType
{
    /* Common interrupts in A0 and B0 */
    LINK_UP = 0,                     ///< MAC Link Up Event     
    LINK_DOWN,                       ///< MAC Link Down Event     
    FAULT_DETECTED,                  ///< MAC Link Up Event     
    FAULT_NOT_DETECTED,
    SERDES_SIG_DETECTED,             ///< MAC Serdes Signal Detection Event 
    SERDES_SIG_NOT_DETECTED,         ///< MAC Serdes Signal No Detection Event 
    TX_UNDER_RUN,                    ///< MAC Tx Packet Underrun Event: While transmitting a packet, MAC does not have data to read from the Transmit DMA Fifo  
    TX_JABBER,                       ///< MAC Tx Packet Jabber Event
    TX_TIMESTAMPS_FIFO_OVER_FLOW,    ///< MAC Tx Timestamp Overlow error Event: Last Timestamp value is dropped
    TX_TIMESTAMPS_FIFO_AVAILABLE,    ///< MAC Tx Timestamp Fifo available Event: An indication that the Transmit Timestamp FIFO has data and is not empty
    /* RS/FC FEC interrupts */
    FEC_ALIGNMENT_MAKER_LOST_LANE0,  ///< FEC Alignment Marker error on lane 0 Event
    FEC_ALIGNMENT_MAKER_LOST_LANE1,  ///< FEC Alignment Marker error on lane 1 Event
    FEC_ALIGNMENT_MAKER_LOST_LANE2,  ///< FEC Alignment Marker error on lane 2 Event
    FEC_ALIGNMENT_MAKER_LOST_LANE3,  ///< FEC Alignment Marker error on lane 3 Event
    FEC_UNCORRECTABLE_FRM,           ///< FEC Uncorrectable Frame error Event
    FEC_DESKEW_LOST,                 ///< FEC Deskew error Event
    FEC_BER_OVER_THRESHOLD,          ///< FEC High BER Event
    /* pcs interrupts event */
    /* High speed multi-channel pcs */
    HSMCPCS_BLOCK_LOCK,
    HSMCPCS_FAULT,
    HSMCPCS_TX_GEARBOX_FIFO_ERROR,
    HSMCPCS_DEBUG_DESKEW_OVERFLOW,
    UMAC_COMMON_INT_END,

    /* A0 interrupts */
    UMAC_A0_INT_START,
    TX_BASE_PAGE_STARTED,
    TX_NEXT_PAGE_STARTED,
    RX_BASE_PAGE_STARTED,
    RX_NEXT_PAGE_STARTED,
    BPAN_COMPLETED,
    UMAC_A0_INT_END,

    /* B0 interrupts */
    UMAC_B0_INT_START,
    HSMCPCS_DECODER_TRAP,
    HSMCPCS_LOSS_OF_SYNC,
    HSMCPCS_LOSS_OF_BLOCK_LOCK,
    HSMCPCS_HIGH_BER_EVENT,
    HSMCPCS_ERROR_BLOCK,
    LSMCPCS_AN_DONE,
    FC_FEC_BLOCK_LOCK_GAINED,
    FC_FEC_BLOCK_LOCK_LOST,
    FC_FEC_UNCORRECTED_CODEWORD,
    FC_FEC_BAD_CODEWORD,
    FC_FEC_TOO_MANY_REQUESTED,
    BPAN_INT,
    TX_IDLE,                      ///< TX idle event
    RX_FIFO_ERR,                  ///< Rx FIFO error interrupt
    TX_FIFO_OVERFLOW_ERR,         ///< Rx FIFO overflow interrupt
    TX_DCNT_VIOLATION_ERR,        ///< Tx DCNT violation error
    TX_FIFO_SOF_EOF_VIOLATION_ERR, ///< Tx FIFO SOF/EOF protocol violation error
    RX_LOCAL_FAULT,
    RX_REMOTE_FAULT,
    RX_CRC_ERROR,
    UMAC_B0_INT_END

}xpEventType;

/**
 * \brief This type (enum) defines the Advanced Counting Module (ACM) modes
 */
typedef enum XP_ACM_CONFIG_MODE
{
    XP_ACM_NONE,            ///< None
    XP_ACM_COUNTING,        ///< Counting
    XP_ACM_SAMPLING,        ///< Sampling
    XP_ACM_POLICING,        ///< Policing
    XP_ACM_MAX_NUM
}XP_ACM_CONFIG_MODE;

/**
 * \brief This type (enum) defines the Advanced Counting Module (ACM) clients
 */
typedef enum xpAcmClient_e
{
    XP_ACM_UNKNOWN,
    /* Openflow ACM clients */
    XP_ACM_OPENFLOW_0,
    XP_ACM_OPENFLOW_1,
    XP_ACM_OPENFLOW_2,
    XP_ACM_OPENFLOW_3,
    XP_ACM_OPENFLOW_4,
    XP_ACM_OPENFLOW_5,
    XP_ACM_OPENFLOW_6,
    XP_ACM_OPENFLOW_7,
    XP_ACM_OPENFLOW_8,
    XP_ACM_OPENFLOW_9,
    XP_ACM_OPENFLOW_10,
    XP_ACM_OPENFLOW_11,
    XP_ACM_OPENFLOW_12,
    XP_ACM_OPENFLOW_13,
    XP_ACM_OPENFLOW_14,
    XP_ACM_OPENFLOW_15,
    XP_ACM_OPENFLOW_16,
    XP_ACM_OPENFLOW_17,
    XP_ACM_OPENFLOW_18,
    XP_ACM_OPENFLOW_19,
    XP_ACM_OPENFLOW_20,
    XP_ACM_OPENFLOW_21,
    XP_ACM_OPENFLOW_22,
    XP_ACM_OPENFLOW_23,
    XP_ACM_OPENFLOW_MAX     = 31,
    /* Default profile ACM clients */
    XP_ACM_ING_PORT_POLICER = 0x20,
    XP_ACM_ING_PORT_SAMPLER = 0x40,
    XP_ACM_VNI_COUNTER      = 0x80,
    XP_ACM_ING_BD_COUNTER   = 0x100,
    XP_ACM_VLAN_BUM_POLICER = 0x200,
    XP_ACM_IACL_POLICER     = 0x400,
    XP_ACM_IPACL_COUNTER    = 0x800,
    XP_ACM_IBACL_COUNTER    = 0x1000,
    XP_ACM_IRACL_COUNTER    = 0x2000,
    XP_ACM_EACL_POLICER     = 0x4000,
    XP_ACM_EACL_COUNTER     = 0x8000,
    XP_ACM_EGR_BD_COUNTER   = 0x10000,
    XP_ACM_DROP_RSN_COUNTER = 0x20000,
    XP_ACM_PORT_RANGE       = 0x40000,
    XP_ACM_PORT_VLAN        = 0x80000,
    XP_ACM_TUNNEL_COUNTER   = 0x100000,
    XP_ACM_COPP_POLICER     = 0x200000,
}xpAcmClient_e;

/**
 * \brief This type (enum) defines the interface upon which Packet Driver operation is performed
 */
typedef enum xpPacketInterface
{
    XP_DMA = 0,     ///< Perform DMA in user space
    XP_NETDEV_DMA = 2   ///< Perform DMA in kernel space
} xpPacketInterface;

/**
 * \brief This type (enum) defines the Policing results types
 */
typedef enum xpPolicingResultType
{
    XP_POL_RESULT_EXP,
    XP_POL_RESULT_DSCP,
    XP_POL_RESULT_DEI,
    XP_POL_RESULT_PCP,
    XP_POL_RESULT_TC,
    XP_POL_RESULT_DP

} xpPolicingResultType;

typedef enum XP_PROFILE_TYPE_E
{
    XP_RESERVED_PROFILE                     = 0,
    XP_DEFAULT_TWO_PIPE_PROFILE             = 1,
    XP_ROUTE_CENTRIC_SINGLE_PIPE_PROFILE    = 2,
    XP_OPENFLOW_HYBRID_SINGLE_PIPE_PROFILE  = 3,
    XP_MAX_PROFILE                          = 8
}XP_PROFILE_TYPE;

/**
 * \typedef xpSkuMode_e
 * \brief This type (enum) defines the typical SKUs modes the device may be configured to
 *
 */
typedef enum xpSkuMode_e
{
    SKU_128X10          = 0,
    SKU_32X40           = 1,
    SKU_64X40           = 2,
    SKU_32X100          = 3,
    SKU_128X25          = 4,
    SKU_64X50           = 5,
    SKU_MODE_MAX        = 18
} xpSkuMode_e;

/**
 * \typedef xpNlMsgType
 * \brief This type (enum) defines the New Address Message types
 * User defined data willbe in Type(int), Length(int), Value(Length bytes) format.
 *
 */
typedef enum xpNlMsgType
{
    XPNL_CREATE_NETDEV_INTF_MSG = 0x0001,
    XPNL_DELETE_NETDEV_INTF_MSG,
    XPNL_TXHEADER_MSG,
    XPNL_TRAP_CONFIG_MSG,
    XPNL_CB_FD_REG_MSG,
    XPNL_INTF_ID = 0x7FFF,
    XPNL_INTF_NAME,
    XPNL_TXHEADER,
    XPNL_TRAP_ID,
    XPNL_REASONCODE,
    XPNL_CHANNEL,
    XPNL_FD,
}xpNlMsgType_e;

/**
 * \brief  OpenFlow Ports
 */
typedef enum xpOfPortNo_e 
{

    XP_OFPP_MAX         = 0xffffff00u,  ///<  Maximum number of physical and logical switch ports.

    /* Reserved OpenFlow Port (fake output "ports"). */
    XP_OFPP_IN_PORT     = 0xfffffff8u,  ///< Send the packet out the input port.
                                        ///< This reserved port must be explicitly
                                        ///< used in order to send back out of
                                        ///< the input port.
    XP_OFPP_TABLE       = 0xfffffff9u,  ///< Submit the packet to the first
                                        ///< flow table. This destination port can
                                        ///< only be used in packet-out messages.
    XP_OFPP_NORMAL      = 0xfffffffau,  ///< Process with normal L2/L3 switching.
    XP_OFPP_FLOOD       = 0xfffffffbu,  ///< All physical ports in VLAN, except
                                        ///< input port and those blocked or
                                        ///< link down.
    XP_OFPP_ALL         = 0xfffffffcu,  ///< All physical ports except input port.
    XP_OFPP_CONTROLLER  = 0xfffffffdu,  ///< Send to controller.
    XP_OFPP_LOCAL       = 0xfffffffeu,  ///< Local openflow "port".
    XP_OFPP_ANY         = 0xffffffffu,  ///< Not associated with any openflow "port".
} xpOfPortNo_e;

/**
 * \brief  OpenFlow Action Type
 */
typedef enum xpOfActionType_e 
{
    XP_OFPAT_OUTPUT       = 0,      ///< Output to switch port
    XP_OFPAT_POP_VLAN     = 6,      ///< Pop the outer VLAN tag
    XP_OFPAT_GROUP        = 10,     ///< Apply group
    XP_OFPAT_SET_NW_TTL   = 11,     ///< Set IP TTL
    XP_OFPAT_DEC_NW_TTL   = 12,     ///< Decrement IP TTL
    XP_OFPAT_SET_FIELD    = 13,     ///< Set a header field using OXM TLV format
} xpOfActionType_e;

/**
 * \brief  OpenFlow Match Field Type
 */
typedef enum xpOxmType_e 
{
    XP_OFPXMT_OFB_IN_PORT      = 0,     ///< Switch input port
    XP_OFPXMT_OFB_IN_PHY_PORT  = 1,     ///< Switch physical input port
    XP_OFPXMT_OFB_METADATA     = 2,     ///< Metadata passed between tables

    XP_OFPXMT_OFB_ETH_DST      = 3,     ///< Ethernet destination address
    XP_OFPXMT_OFB_ETH_SRC      = 4,     ///< Ethernet source address
    XP_OFPXMT_OFB_ETH_TYPE     = 5,     ///< Ethernet frame type
    XP_OFPXMT_OFB_VLAN_VID     = 6,     ///< VLAN id
    XP_OFPXMT_OFB_VLAN_PCP     = 7,     ///< VLAN priority
    XP_OFPXMT_OFB_IP_PROTO     = 10,    ///< IP protocol
    XP_OFPXMT_OFB_IPV4_SRC     = 11,    ///< IPv4 source address
    XP_OFPXMT_OFB_IPV4_DST     = 12,    ///< IPv4 destination address
    XP_OFPXMT_OFB_TCP_SRC      = 13,    ///< TCP source port
    XP_OFPXMT_OFB_TCP_DST      = 14,    ///< TCP destination port
    XP_OFPXMT_OFB_UDP_SRC      = 15,    ///< UDP source port
    XP_OFPXMT_OFB_UDP_DST      = 16,    ///< UDP destination port
    XP_OFPXMT_OFB_SCTP_SRC     = 17,    ///< SCTP source port
    XP_OFPXMT_OFB_SCTP_DST     = 18,    ///< SCTP destination port
    XP_OFPXMT_OFB_MAX
} xpOxmType_e;

/**
 * \brief  OpenFlow Match Packet Type
 */
typedef enum xpOfMatchPktType_e 
{
    XP_MATCH_PKT_ANY         = 0, ///< Match packets with and without a VLAN tag.
                                  ///< The VLAN VID and PCP fields value
                                  ///< must be ignored.
    XP_MATCH_PKT_UNTAGGED    = 1, ///< Match only packets without a VLAN tag.
                                  ///<
    XP_MATCH_PKT_TAGGED      = 2, ///< Match only packets with a VLAN tag.
                                  ///< In case VLAN VID and PCP fields values
                                  ///< are not defined (wildcarded) match packets
                                  ///< with a VLAN tag regardless of its value.
    XP_MATCH_PKT_MASK        = 3, ///< Match packet mask
                                  ///<
} xpOfMatchPktType_e;

/**
 * \brief  OpenFlow Group Type
 */
typedef enum xpOfGroupType_e 
{
    XP_OFPGT_ALL       = 0, ///< Executes all buckets in the group.
    XP_OFPGT_SELECT    = 1, ///< Executes one bucket in the group based on
                            ///< a switch-computed selection algorithm.
    XP_OFPGT_INDIRECT  = 2, ///< Executes a single bucket defined in
                            ///< this group.
    XP_OFPGT_FF        = 3, ///< Executes the first live bucket.
} xpOfGroupType_e;

/**
 * \brief  OpenFlow Flow Type
 */
typedef enum xpOfFlowType_e 
{
    XP_OFPFT_STANDARD_FLOW      = 0,  ///< Standard OpenFlow flow visible for
                                      ///< SDN controller.
    XP_OFPFT_HIDDEN_HIGH_FLOW   = 1,  ///< Flow with priority higher than
                                      ///< standard OpenFlow flow. This flow
                                      ///< is invisible for SDN controller.
                                      ///< Can be used to implement in-band management.
    XP_OFPFT_HIDDEN_LOW_FLOW    = 2,  ///< Flow with priority lower than
                                      ///< standard OpenFlow flow. This flow
                                      ///< is invisible for SDN controller.
                                      ///< Can be used to implement OF Table
                                      ///< default behavior through a hidden rule.
} xpOfFlowType_e;

/**
 * \brief  OpenFlow Packet-In reason codes
 */
typedef enum xpOfPacketInReason_e
{
    XP_OFPR_TABLE_MISS   = 0, ///< No matching flow (table-miss flow entry).
    XP_OFPR_APPLY_ACTION = 1, ///< Output to controller in apply-actions.
    XP_OFPR_INVALID_TTL  = 2, ///< Packet has invalid IP or MPLS TTL.
    XP_OFPR_ACTION_SET   = 3, ///< Output to controller in action set.
    XP_OFPR_GROUP        = 4, ///< Output to controller in group bucket.
    XP_OFPR_PACKET_OUT   = 5, ///< Output to controller in packet-out.
    XP_OFPR_PORT_LOCAL   = 7, ///< Output to XP_OFPP_LOCAL.
} xpOfPacketInReason_e;

/**
 * \brief  OpenFlow Table type
 */
typedef enum xpOfTableType_e
{
    XP_OF_HASH_TABLE = 0,
    XP_OF_TCAM_TABLE,
    XP_OF_TABLE_NUM
} xpOfTableType_e;

// XPS ENUMS

/**
 * \brief This type (enum) defines the interfaces 
 */
typedef enum xpsInterfaceType_e
{
    XPS_PORT = 0,
    XPS_EXTENDED_PORT,         ///< 802.1BR port extention
    XPS_PORT_CONTROL_VIF,      ///< Control Vif with ignore TRT bit set
    XPS_LAG,                   ///< LAG  
    XPS_TUNNEL_MPLS,           ///< MPLS
    XPS_TUNNEL_VXLAN,          ///< VXLAN Tunnel
    XPS_TUNNEL_NVGRE,          ///< NVGRE Tunnel  
    XPS_TUNNEL_GENEVE,         ///< GENEVE Tunnel
    XPS_TUNNEL_PBB,            ///< Provider Backbone Bridges (also known as "Mac-In-Mac") Tunnel 
    XPS_TUNNEL_GRE,            ///< GRE Tunnel
    XPS_TUNNEL_VPN_GRE,        ///< VPN GRE Tunnel 
    XPS_TUNNEL_VPN_GRE_LOOSE,  ///< VPN GRE Loose Mode Tunnel
    XPS_TUNNEL_GRE_ERSPAN2,    ///< GRE ERSPAN Tunnel 
    XPS_TUNNEL_IP_IN_IP,       ///< IP-in-IP Tunnel   
    XPS_L2_MCAST_GROUP,        ///< L2 Multicast Group
    XPS_L3_MCAST_GROUP,        ///< L3 Multicast Group 
    XPS_PORT_ROUTER,           ///< Port Based Router
    XPS_SUBINTERFACE_ROUTER,   ///< Sub interface Router
    XPS_VLAN_ROUTER,           ///< VLAN Based Router
    XPS_TUNNEL_ROUTER,         ///< Tunnel Based Router
    XPS_VPN_ROUTER             ///< L3VPN Based Router
} xpsInterfaceType_e;

/**
 * \typedef xpQosMapPfl_t
 * \public
 * \brief This type (enum) defines the number of Qos Profiles
 *        supported in the QosMap Table
 */
typedef enum
{
    QOSMAP_PROFILE_TRUST,
    QOSMAP_PROFILE_1,
    QOSMAP_PROFILE_2,
    QOSMAP_PROFILE_3,
    QOSMAP_PROFILE_4,
    QOSMAP_PROFILE_5,
    QOSMAP_PROFILE_6,
    QOSMAP_PROFILE_7,
    QOSMAP_PROFILE_MAX,
} xpQosMapPfl_t;

/**
 *IACL related enums
 */
/**
*\type xpAclType_e
*\public
*\brief This type (enum) defines types of ACL tables
*
**/
typedef enum
{
    XP_ACL_IACL0,               ///< Port IACL
    XP_ACL_IACL1,               ///< Bridge IACL
    XP_ACL_IACL2,               ///< Router IACL              
    XP_ACL_EACL,                ///< EACL
    XP_ACML_TOTAL_TYPE,
} xpAclType_e;

/**
 *\type xpIaclType_e
 *\public
 *\brief This type (enum) defines type of IACL Table.
 *
 **/
typedef enum
{
    XP_IACL0,
    XP_IACL1,
    XP_IACL2,
    XP_IACL_TOTAL_TYPE,
} xpIaclType_e;

/**
 * \brief IPv4 IACL Key Fields
 */
typedef enum
{
    XP_IACL_KEY_TYPE_V4,        ///< 8b 
    XP_IACL_ID,                 ///< 8b ACL ID
    XP_IACL_MAC_DA,             ///< 48b Mac DA
    XP_IACL_MAC_SA,             ///< 48b Mac SA
    XP_IACL_V4_ETHER_TYPE,      ///< 16b IPv4 Ethernet type
    XP_IACL_CTAG_VID_DEI_PCP,   ///< 16b C-Tag Virtual ID + DEI + PCP       
    XP_IACL_STAG_VID_DEI_PCP,   ///< 16b S-Tag Virtual ID + DEI + PCP      
    XP_IACL_DIP_V4,             ///< 32b DIP
    XP_IACL_SIP_V4,             ///< 32b SIP
    XP_IACL_L4_DEST_PORT,       ///< 16b L4 Destination Port
    XP_IACL_L4_SRC_PORT,        ///< 16b L4 Source Port
    XP_IACL_IVIF,               ///< 16b Ingress Virtual Interface
    XP_IACL_ICMP_CODE,          ///< 8b ICMP Code
    XP_IACL_ICMP_MSG_TYPE,      ///< 8b ICMP Message Type
    XP_IACL_PROTOCOL,           ///< 8b IP Protocol
    XP_IACL_DSCP_ECN,           ///< 8b QoS DSCP and ECN Value
    XP_IACL_BD,                 ///< 16b Bridge Domain ID
    XP_IACL_TTL,                ///< 8b IP Time to Live
    XP_IACL_PKTLEN,             ///< 8b IP Packet Length
    XP_IACL_TCP_FLAGS,          ///< 8b TCP Flags
    XP_IACL_VRF_ID,             ///< 16b Virtual Router Interface ID
    XP_IACL_TAG_FRAGMENT_INFO,  ///< 8b IP Fragment Bits
    XP_IACL_IPV4_MAX_FLDS
} xpIaclV4KeyFlds;

/**
 * \brief IPv6 IACL Key Fields
 */
typedef enum
{
    XP_IACL_KEY_TYPE_V6,        ///< 8b    
    XP_IACL_V6_ID,              ///< 8b ACL ID
    XP_IACL_DIP_V6,             ///< 128b V6 DIP
    XP_IACL_SIP_V6,             ///< 128b V6 SIP
    XP_IACL_NXT_HDR,            ///< 8b Next Header
    XP_IACL_L4_V6_DEST_PORT,    ///< 16b Destination Port 
    XP_IACL_L4_V6_SRC_PORT,     ///< 16b Source Port
    XP_IACL_ICMP_V6_CODE,       ///< 8b ICMP Code
    XP_IACL_ICMP_V6_MSG_TYPE,   ///< 8b ICMP Message Type    
    XP_IACL_HOP_LIMIT,          ///< 8b Hop Limit
    XP_IACL_V6_PROTOCOL,        ///< 8b IPv6 Protocol
    XP_IACL_V6_TCP_FLAGS,       ///< 8b TCP Flags
    XP_IACL_V6_PKTLEN,          ///< 8b IP Packet Length
    XP_IACL_TC_ROUTERMAC,       ///< 8b Router MAC & Traffic Class
    XP_IACL_IPV6_MAX_FLDS
}xpIaclV6KeyFlds;

/**
 * \brief MPLS IACL Key Fields
 */
typedef enum
{
    XP_IACL_KEY_TYPE_MPLS_V4,         ///< 8b
    XP_IACL_MPLS_V4_ID,               ///< 8b ACL ID
    XP_IACL_MPLS_V4_DIP,              ///< 32b DIP
    XP_IACL_MPLS_V4_SIP,              ///< 32b SIP
    XP_IACL_MPLS_V4_L4_DEST_PORT,     ///< 16b L4 Destination Port
    XP_IACL_MPLS_V4_L4_SRC_PORT,      ///< 16b L4 Source Port
    XP_IACL_MPLS_V4_IVIF,             ///< 8b IVIF
    XP_IACL_MPLS_V4_ICMP_CODE,        ///< 8b ICMP Code
    XP_IACL_MPLS_V4_ICMP_MSG_TYPE,    ///< 8b ICMP Message Type
    XP_IACL_MPLS_V4_PROTOCOL,         ///< 8b IPv4 Protocol
    XP_IACL_MPLS_V4_DSCP_ECN,         ///< 8b QoS DSCP and ECN
    XP_IACL_MPLS_V4_BD,               ///< 16b Bridge Domain ID
    XP_IACL_MPLS_V4_TCP_FLAGS,        ///< 8b  TCP Flags
    XP_IACL_MPLS_V4_TTL,              ///< 8b Time to Live
    XP_IACL_MPLS_V4_PKTLEN_LABEL_EXP, ///< 40b Packet Len & MPLS Label and EXP
    XP_IACL_IPV4_MPLS_MAX_FLDS
}xpIaclV4MplsKeyFlds;

/**
 * \brief IPv4 EACL Key Fields
 */
typedef enum
{
    XP_EACL_KEY_TYPE_V4,        ///< 8b
    XP_EACL_MAC_DA,
    XP_EACL_MAC_SA,
    XP_EACL_DIP_V4,
    XP_EACL_SIP_V4,
    XP_EACL_L4_DEST_PORT,
    XP_EACL_L4_SRC_PORT,
    XP_EACL_V4_ETHER_TYPE,
    XP_EACL_ICMP_MSG_TYPE,
    XP_EACL_PROTOCOL,
    XP_EACL_EGRESS_BD,
    XP_EACL_EGRESS_VIF,
    XP_EACL_VI,
    XP_EACL_DSCP_PCP_EXP,
    XP_EACL_TCP_FLAGS_RSNCODE,
    XP_EACL_TC_DP,
    XP_EACL_IPV4_MAX_FLDS,
} xpEaclV4KeyFlds;

/**
 * \brief IPv6 EACL Key Fields
 */
typedef enum
{
    XP_EACL_KEY_TYPE_V6,
    XP_EACL_DIP_V6,
    XP_EACL_SIP_V6,
    XP_EACL_L4_V6_DEST_PORT,
    XP_EACL_L4_V6_SRC_PORT,
    XP_EACL_V6_ETHER_TYPE,
    XP_EACL_ICMP_V6_MSG_TYPE,
    XP_EACL_V6_EGRESS_BD,
    XP_EACL_V6_EGRESS_VIF,
    XP_EACL_V6_RSNCODE_TC_DP,
    XP_EACL_IPV6_MAX_FLDS,
} xpEaclV6KeyFlds;

typedef enum
{
    XP_IACL_V4_TYPE,
    XP_IACL_V6_TYPE,
    XP_IACL_V4_MPLS_TYPE,
} xpIaclKeyType;

/**
 * \brief This type (enum) defines the hashable fields        
 */
typedef xpHashField xpsHashField_e;

/**
 * \brief This type (enum) lists the packet forwarding processing commands  
 */
typedef xpPktCmd_e xpsPktCmd_e;


/**
 * \brief This type (enum) defines the counting modes
 *        The configuration is a bitmask, which can enable
 *        counting (bit 0), policing (bit 1) and sampling (bit2).
 */
typedef xpCountMode_e xpsCountMode_e;

/**
 * \brief This type (enum) defines an the egress encapsulation
 *        types for an L2 Domain.
 */
typedef xpL2EncapType_e xpsL2EncapType_e;

/**
 * \brief This type (enum) defines the Multicast
 *        Bridging modes configurable for a VLAN.
 */
typedef xpVlanBridgeMcMode_e xpsVlanBridgeMcMode_e;

/**
 * \brief This type (enum) defines the Count Mode
 *        for a VLAN.
 */
typedef xpBdCountMode_e xpsVlanCountMode_e;

/**
 * \brief This type (enum) defines the Spanning Tree States
 *        for a port.
 */
typedef xpVlanStgState_e xpsStpState_e;

/**
 * \brief This type (enum) defines IP type  
 *        used as IP Route Table Prefix lookup.
 **/
typedef xpIpPrefixType_t xpsIpPrefixType_e;

/**
 *\brief This type (enum) defines the device initialization modes
 *       INIT_COLD:
                    Performs addDevice and all respective managers
                    will perform "software init", "hardware init" and
                    load default values. This is used when the system
                    is initialized the first time.
         INIT_WARM: (shadow is not present in the system)
                    Performs addDevice and all respective managers
                    will perform "software init" only. Most cases the
                    managers should recreate config and state from hardware
                    but typical case there is no shadow maintained, hence
                    table/reg data is not modified on the live system.
         INIT_WARM_SYNC_TO_HW:
                    (shadow is available and is persistent across warm restart)
                    Performs addDevice, and software initialization
                    for all blocks. "software init" only. For
                    managers should load data from shadow to hardware
                    and push the configration to hardware.
         INIT_WARM_SYNC_TO_SHADOW:
                    (shadow is available and is synced by reading hardware)
                    Performs addDevice, and software initialization
                    for all blocks. "software init" only. For
                    managers should load data from hardware to shadow. For
                    other configurations and data is recreated/read from hw.
 */
typedef xpInitType_t xpsInitType_e;
typedef xpInitType_t xpsInitType_t;

/**
 * \brief This type (enum) defines the MAC configuration modes
 *
 */
typedef xpMacConfigMode xpsMacConfigMode_e;

/**
 * \brief This type (enum) defines the Packet Driver 
 *        Rx modes
 */
typedef xpRxConfigMode xpsRxConfigMode_e;


/**
 * \brief This type (enum) defines SAL object types 
 */
typedef xpSalType_t xpsSalType_e;

/**
 * \brief This type (enum) defines the Packet Driver 
 *        callback handler types
 */
typedef xpCallbackMethodType xpsCallbackMethodType_e;

/**
 * \brief This type (enum) defines the Packet Driver 
 *        Tx modes
 */
typedef txMode xpsTxMode_e;

/**
 * \brief This type (enum) defines the Packet Driver 
 *        Rx handler types
 */
typedef xpCpuRxPktCbType_e xpsCpuRxPktCbType_e;

/**
 * \brief This type (enum) defines the Forward-Error-Correction (FEC) types
 */
typedef xpFecMode xpsFecMode_e;

/**
 * \brief This type (enum) defines the serdes tuning types
 */
typedef xpSerdesDfeTuneMode_t xpsSerdesDfeTuneMode_e;

/**
 * \brief This type (enum) defines the port speeds
 */
typedef enum xpSpeed xpsSpeed_e;

/**
 * \brief This type (enum) defines the event source identifiers
 */
typedef enum xpEventType xpsEventType_e;

/**
 * \brief This type (enum) defines the Advanced Counting Module (ACM) modes
 */
typedef  XP_ACM_CONFIG_MODE xpsAcmConfigMode_e;

/**
 * \brief This type (enum) defines the Advanced Counting Module (ACM) clients
 */
typedef  xpAcmClient_e xpsAcmClient_e;

/**
 * \brief This type (enum) defines the Driver Access Layer (DAL) types
 */
typedef enum xpsDalType_e
{
   XPS_DAL_HARDWARE,      ///< Hardware 
   XPS_DAL_WHITEMODEL     ///< Simulation
}xpsDalType_e;

/**
 * \brief This type (enum) defines the interface upon which Packet Driver operation is performed
 */
typedef  xpPacketInterface xpsPacketInterface_e;

/**
 * \brief This type (enum) defines the Policing results types
 */
typedef  xpPolicingResultType xpsPolicingResultType_e;

typedef  XP_PROFILE_TYPE xpsProfileType_e;

/**
 * \brief This type (enum) defines the typical SKUs modes the device may be configured to
 */
typedef  xpSkuMode_e xpsSkuMode_e;

/**
 * \brief This type (enum) defines the New Address Message types
 * User defined data willbe in Type(int), Length(int), Value(Length bytes) format.
 */
typedef  xpNlMsgType_e xpsNlMsgType_e;

/**
 * \brief  OpenFlow Ports
 */
typedef  xpOfPortNo_e xpsOfPortNo_e;

/**
 * \brief  OpenFlow Action Type
 */
typedef  xpOfActionType_e xpsOfActionType_e;

/**
 * \brief  OpenFlow Match Field Type
 */
typedef  xpOxmType_e xpsOxmType_e;

/**
 * \brief  OpenFlow Match Packet Type
 */
typedef  xpOfMatchPktType_e xpsOfMatchPktType_e;

/**
 * \brief  OpenFlow Group Type
 */
typedef  xpOfGroupType_e xpsOfGroupType_e;

/**
 * \brief  OpenFlow Flow Type
 */
typedef  xpOfFlowType_e xpsOfFlowType_e;

/**
 * \brief  OpenFlow Packet-In reason codes
 */
typedef  xpOfPacketInReason_e xpsOfPacketInReason_e;

/**
 * \brief  OpenFlow Table type
 */
typedef  xpOfTableType_e xpsOfTableType_e;

#endif  //_openXpsEnums_h_
