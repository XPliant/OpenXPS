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
 *   FILE : openXpsTypes.h
 * 
 *   Abstract: This file defines the XP data types in OpenXPS.
 ************************************************************************/
/**
 * \file openXpsTypes.h
 * \brief This file contains openXps type definitions
 * \copyright (c) 2016 Cavium Inc
 */
 
#ifndef _openXpsTypes_h
#define _openXpsTypes_h

#include "errno.h"
#include "stddef.h"
#include "stdint.h"

#include "openXpsEnums.h"

#define XP_MAX_PACKET_SIZE                      10000
#define XP_MAX_DEVICES                          (64)

#define CPU_PORT       				176

#define XP_MAX_10G_PORTS                        128
#define XP_MAX_TOTAL_PORTS                      XP_MAX_10G_PORTS
#define XP_MAX_PTGS                             32
#define XP_PORTS_PER_PORT_GROUP                 4
#define XP_MAX_VLAN_NUM (4 * 1024)
#define XP_MAC_ADDR_LEN                         6

/**
 * \brief  Maximum number of OF tables
 */
#define XP_OF_MAX_TABLE_PROFILES                24

typedef uint32_t xpDevice_t;
typedef uint16_t xpVlan_t;
typedef uint32_t xpPort_t;
typedef uint32_t xpVif_t;
typedef uint8_t macAddr_t[6];        ///< User defined type for Mac Address
typedef uint8_t ipv4Addr_t[4];       ///< User defined type for Ipv4 Address
typedef uint8_t ipv6Addr_t[16];      ///< User defined type for Ipv6 Address
typedef uint8_t macAddrHigh_t[5];    ///< User defined type for MAC Address for Higher 32 Bits
typedef uint8_t macAddrLow_t;
typedef uint32_t xpDatapath_t;

/**
 * \brief This type (enum) defines the global function calls return statuses 
 */
typedef enum XP_STATUS_E
{
    XP_NO_ERR,                      ///< 0 - Normal return code (XP_OK)
    XP_ERR_OUT_OF_MEM,              ///< 1 - Out of Memory
    XP_ERR_NULL_POINTER,            ///< 2 - Dereferencing Null Pointer
    XP_ERR_ARRAY_OUT_OF_BOUNDS,     ///< 3 - Array out of Bounds
    XP_ERR_OUT_OF_KPUS,             ///< 4 - Exceeded all KPUs, Parsing is incomplete
    XP_ERR_UNPACK_FAILURE,          ///< 5 - Packer Unpack Failure
    XP_ERR_PACK_FAILURE,            ///< 6 - Packer Pack Failure
    XP_ERR_INVALID_KPU,             ///< 7 - Invalid KPU
    XP_ERR_RESOURCE_DNE,            ///< 8 - Does Not Exist
    XP_ERR_RESOURCE_NOT_AVAILABLE,  ///< 9 - Trying to access NULL object
    XP_ERR_RESOURCE_IS_PRE_ALLOCATED,   ///< 10 - Trying to allocate pre-allocated resource
    XP_ERR_INVALID_OFFSET,          ///< 11 - Trying to access an invalid offset in memory
    XP_ERR_INVALID_KEY_SIZE,        ///< 12 - Unsupported key size
    XP_ERR_DATA_IS_NOT_ALLIGNED,    ///< 13 - Data is not aligned
    XP_ERR_INVALID_ID,              ///< 14 - Invalid ID
    XP_ERR_INVALID_DATA_SIZE,       ///< 15 - Invalid Data Size
    XP_ERR_INVALID_DATA,            ///< 16 - Invalid Data
    XP_ERR_SEARCH_MISS,             ///< 17 - TxQ Search Miss
    XP_ERR_REHASH_FAIL,             ///< 18 - Hash Table Rehash Failure
    XP_ERR_INVALID_SHARE_GROUP,     ///< 19 - Hash Table Invalid Share Group
    XP_ERR_UNRESOLVABLE_COL,        ///< 20 -
    XP_ERR_CALL_BASE_CLASS_OBJ,     ///< 21 - Inheritence Error
    XP_ERR_KEY_EXISTS,              ///< 22 - Failed to insert key because key already present
    XP_ERR_KEY_NOT_FOUND,           ///< 23 - Failed to remove/update key because key does not exist
    XP_ERR_MAILBOX_FULL,            ///< 24 - Mailbox Full
    XP_ERR_DATA_OUT_OF_RANGE,       ///< 25 - Data out of Range
    XP_ERR_INVALID_ROW_BLK_NUM,     ///< 26 -
    XP_ERR_INVALID_TABLE_TYPE,      ///< 27 - Invalid Table Type
    XP_ERR_INVALID_TABLE_PROPERTIES,///< 28 - Invalid Table Properties
    XP_ERR_INVALID_INPUT,           ///< 29 - Invalid Input
    XP_ERR_INVALID_TABLE_DEPTH,     ///< 30 - Invalid Table Depth
    XP_ERR_INVALID_TABLE_PTR,       ///< 31 - Invalid Table Pointer
    XP_ERR_INVALID_TBL_CTX,         ///< 32 - Invalid Table Context
    XP_ERR_PM_NOT_INIT,             ///< 33 -
    XP_ERR_PM_SWPOOL_EMPTY,         ///< 34 -
    XP_ERR_INVALID_TABLE_KEY,       ///< 35 - Invalid Table Key
    XP_ERR_INVALID_TABLE_HANDLE,    ///< 36 - Invalid Table Handle
    XP_ERR_INVALID_TABLE_ENTRY,     ///< 37 - Invalid Table Entry
    XP_ERR_INVALID_VALUE_SIZE,      ///< 38 - Invalid Size of Value
    XP_ERR_GET_TABLE_OBJ,           ///< 39 - Unable to get Table Object
    XP_ERR_PM_INIT_DNE,             ///< 40 -
    XP_ERR_PM_DEV_ADD_DNE,          ///< 41 - Device Primitives already added
    XP_ERR_PM_DEV_NOT_ADD,          ///< 42 - Device Primitives not been added
    XP_ERR_PM_HWLOOKUP_FAIL,        ///< 43 - Device Primitives HW Lookup Failure
    XP_ERR_INVALID_ENTRY_HANDLE,    ///< 44 - Invalid Entry Handle
    XP_ERR_INVALID_FIELD_REQ,       ///< 45 - Invalid Field
    XP_ERR_INVALID_INSERTION_ID,    ///< 46 - Invalid Insertion ID
    XP_ERR_INVALID_VIF_ID,          ///< 47 - Invalid Vif ID
    XP_ERR_INVALID_DEV_ID,          ///< 48 - Invalid Device ID
    XP_ERR_OUT_OF_IDS,              ///< 49 - Out of IDs
    XP_ERR_ID_IS_PRE_ALLOCATED,     ///< 50 - ID is already allocated
    XP_ERR_FREEING_UNALLOCATED_ID,  ///< 51 - Freeing an unallocated ID
    XP_ERR_INVALID_ID_RANGE,        ///< 52 - Invalid ID Range
    XP_ERR_INVALID_PORT_MODE,       ///< 53 - Invalid Port mode
    XP_ERR_MDIO_READ_OP_NOT_SUPPORTED,      ///< 54 - MDIO Read operation not supported
    XP_ERR_MDIO_WRITE_OP_NOT_SUPPORTED,     ///< 55 - MDIO Write operation not supported
    XP_ERR_STATISTICS_STATUS_NOT_CLEAR,     ///< 56 - Statistics Engine Busy
    XP_ERR_INVALID_PREAMBLE_LEN_VALUE,   ///< 57 - Invalid ingress preamble length value
    XP_ERR_MODE_NOT_SUPPORTED,      ///< 58 - Mode not supported
    XP_ERR_MAC_NOT_CONFIGURED,      ///< 59 - Mac not configured
    XP_ERR_MEM_ALLOC_ERROR,         ///< 60 - Memory Allocation Error
    XP_ERR_MEM_DEALLOC_ERROR,       ///< 61 - Memory De-allocation Error
    XP_ERR_MUNMAP,                  ///< 62 -
    XP_ERR_SHMEM,                   ///< 63 -
    XP_ERR_NOT_INITED,              ///< 64 - Not Inited
    XP_ERR_INIT,                    ///< 65 - Initialization Error
    XP_ERR_BUS_NOT_INITED,          ///< 66 - Bus not initialized
    XP_ERR_INVALID_BUS,             ///< 67 - Invalid Bus
    XP_ERR_BUS_INIT,                ///< 68 - Bus Initialization error
    XP_ERR_INVALID_INTERFACE,       ///< 69 - Invalid Interface
    XP_ERR_INVALID_CONFIG_MODE,     ///< 70 - Invalid Configuration Mode
    XP_ERR_INVALID_POLICY,          ///< 71 - Invalid Policy Requested
    XP_ERR_INVALID_VALUE,           ///< 72 - Invalid Value
    XP_ERR_INVALID_ARG,             ///< 73 - Invalid Argument
    XP_ERR_INVALID_SEND_MODE,       ///< 74 -
    XP_ERR_INVALID_QUEUE_NUM,       ///< 75 - Invalid Queue Number
    XP_ERR_PACKET_DRV_INTERFACE_INITED, ///< 76 - Packet Drive Interface already Initialized
    XP_ERR_DESCRIPTORS_NOT_AVAILABLE,   ///< 77 - Descriptors not available
    XP_ERR_DMA0_TIMEOUT,            ///< 78 - DMA 0 Timeout
    XP_ERR_INVALID_QUEUE,           ///< 79 - Invalid Queue
    XP_ERR_NOT_FOUND,               ///< 80 - Interface Not Found
    XP_ERR_PKT_NOT_AVAILABLE,       ///< 81 - Packet not Available
    XP_ERR_BUFFERS_NOT_AVAILABLE,   ///< 82 - Buffers not Available
    XP_ERR_QUEUE_NOT_ENABLED,       ///< 83 - Queue not Enabled
    XP_ERR_DMA_TRANSMIT,            ///< 84 - DMA Transmit Error
    XP_ERR_REGACCESS,               ///< 85 - Register Access Error
    XP_ERR_DEVICE_OPEN,             ///< 86 - Unable to open Device
    XP_ERR_FILE_OPEN,               ///< 87 - Unable to open File
    XP_ERR_IOCTL,                   ///< 88 - IO Control Error
    XP_ERR_DEVICE_CLOSE,            ///< 89 - Unable to close Device
    XP_ERR_SOCKET_SELECT,           ///< 90 -
    XP_ERR_SOCKET_RECV,             ///< 91 - Socket Receive Error
    XP_ERR_SOCKET_SEND,             ///< 92 - Socket Send Error
    XP_ERR_SOCKET_INIT,             ///< 93 - Socket Init Error
    XP_ERR_SOCKET_CLOSE,            ///< 94 - Socket Close Error
    XP_ERR_INVALID_MAGIC_NUM,       ///< 95 -
    XP_ERR_EVENT_REGISTRATION,      ///< 96 - Event Registration Error
    XP_ERR_REG_READ,                ///< 97 - Register Read Failure
    XP_ERR_REG_WRITE,               ///< 98 - Register Write Failure
    XP_ERR_SRAM_READ,               ///< 99 - SRAM Read Failure
    XP_ERR_SRAM_WRITE,              ///< 100 - SRAM Write Failure
    XP_ERR_SRAM_BUSY,               ///< 101 - SRAM Busy
    XP_ERR_IPC_BUSY,                ///< 102 - IPC Busy
    XP_ERR_AGE_INVALID_PROPERTIES,  ///< 103 - Invalid Properties for Aging
    XP_ERR_AGE_CYCLE_BUSY,          ///< 104 - Aging Cycle Busy
    XP_PORT_NOT_VALID,              ///< 105 - Port not Valid
    XP_PORT_INSTANCE_NOT_CREATED,   ///< 106 - Port Instance not Created
    XP_MDIO_MASTER_LIMIT_EXCEED,    ///< 107 - MDIO Master Limit Exceeded
    XP_MDIO_MASTER_BUSY,            ///< 108 - MDIO Master Busy
    XP_PORT_NOT_ENABLED,            ///< 109 - Port not Enabled
    XP_INVALID_MAC_MODE,            ///< 110 - Invalid Mac Mode
    XP_INVALID_PCS_MODE,            ///< 111 - invalid PCS Mode
    XP_ERR_PCS_SYNC,                ///< 112 - PCS Synchronization Error
    XP_ERR_MAC_NOT_INITED,          ///< 113 - Mac not Initialized
    XP_PORT_NOT_INITED,             ///< 114 - Port not Initialized
    XP_ERR_OUT_OF_RANGE,            ///< 115 - Out of Range
    XP_ERR_SERDES_NOT_INITED,       ///< 116 - SERDES Not Initialized
    XP_ERR_AAPL_NULL_POINTER,       ///< 117 - AAPL Dereferencing Null Pointer
    XP_ERR_INVALID_PCS_CONFIG_MODE, ///< 118 - Invalid PCS Configuration Mode
    XP_ERR_MDIO_MASTER_NOT_CONFIGURED,  ///< 119 - MDIO Master Not Configured
    XP_ERR_INVALID_PORTNUM,         ///< 120 - Invalid Port Number
    XP_ERR_INVALID_CHAN_NUM,        ///< 121 - Invalid Channel Number
    XP_ERR_INVALID_MODE_SPEED,      ///< 122 - Invalid Speed Mode
    XP_ERR_INVALID_VLAN_FRM_LEN_CONFIG, ///< 123 - Invalid VLAN Frame Length Configuration
    XP_ERR_VALUE_OUT_OF_RANGE,      ///< 124 - Value out of Range
    XP_ERR_INVALID_SPEED,           ///< 125 - Invalid Speed
    XP_ERR_INVALID_SERDES_CHANNEL_NUM,              ///< 126 -Invalid SERDES Channel Number
    XP_ERR_INTERRUPT_HANDLER_NOT_REGISTERED,          ///< 127 - Link Up Handler not Registered
    XP_ERR_AN_SET_NOT_ALLOWED,      ///< 128 - AutoNegotiation Set Not Allowed
    XP_ERR_AN_GET_NOT_ALLOWED,      ///< 129 - AutoNegotiation Get Not Allowed
    XP_ERR_AN_RESTART_NOT_ALLOWED,  ///< 130 - AutoNegotiation Restart Not Allowed
    XP_ERR_MDIO_STATUS_NOT_CLEAR,   ///< 131 - MDIO Status Not Clear
    XP_ERR_INVALID_PCSMODE,         ///< 132 - Invalid PCS mode
    XP_ERR_OP_NOT_SUPPORTED,        ///< 133 - Operation not Supported
    XP_ERR_UNKNOWN_EVENT,           ///< 134 - Unknown Event Occured
    XP_ERR_INIT_FAILED,             ///< 135 - Initialization Failure
    XP_ERR_SERDES_INIT,             ///< 136 - SERDES Initialization Error
    XP_ERR_SERDES_FW_NOT_INITED,    ///< 137 -
    XP_ERR_INVALID_MAX_ENTRIES,     ///< 138 -
    XP_ERR_INVALID_LANE_NUM,        ///< 139 - Invalid Lane Number
    XP_ERR_PORT_CONFIGURATION,      ///< 140 - Port Configuration Error
    XP_ERR_THREAD_CREATION,         ///< 142 - Thread Creation Failure
    XP_ERR_LOCK_INIT,               ///< 142 - Lock Initialization error
    XP_ERR_ACQUIRING_LOCK,          ///< 143 -
    XP_ERR_RELEASING_LOCK,          ///< 144 -
    XP_ERR_LOCK_DESTROY,            ///< 145 -
    XP_ERR_SOCKET_TX,               ///< 146 - Transmit Packet Socket Failure
    XP_ERR_SOCKET_POLLING,          ///< 147 - Socket Polling Failure
    XP_ERR_TIMEOUT,                 ///< 148 - Socket Timeout
    XP_ERR_CONFIG_OPEN,             ///< 149 -
    XP_ERR_OFFSET_OPEN,             ///< 150 -
    XP_ERR_INVALID_VALE,            ///< 151 - XXX Typo?
    XP_SERDES_LOOPBACK_ERROR,       ///< 152 - SERDES Loopback Failure

    /* Management WM specific error codes */
    XP_ERR_INVALID_PARAMS,          ///< 153 - Invalid Parameters
    XP_ERR_TESTBENCH_FAIL,          ///< 154 - Testbench Failure
    XP_ERR_UNREGISTERED_OFFSET,     ///< 155 - Unregistered Offset
    XP_ERR_SIGACTION_FAIL,          ///< 156 - Signal Action Failure
    XP_ERR_DMA_DESC_NULL_ADDR,      ///< 157 - DMA Descriptor NULL
    XP_ERR_DMA_DESC_OWN,            ///< 158 - DMA Descriptor CPU Owned
    XP_ERR_DMA_TX,                  ///< 159 - DMA Transmit Failure
    XP_ERR_DMA_RX,                  ///< 160 - DMA Receive Failure
    XP_ERR_DMA_RX_EXIT,             ///< 161 - DMA Receive Exit error
    XP_ERR_PORT_NOT_AVAILABLE,      ///< 162 - Port Not Available
    XP_UMAC_RX_DISABLE,             ///< 163 -
    XP_UMAC_TX_DISABLE,             ///< 164 -
    XP_UMAC_NO_ACTION_REQD,         ///< 165 -
    XP_ERR_INVALID_UMAC_MODE,       ///< 166 -
    XP_ERR_UMAC_NOT_IN_SGMII_MODE,  ///< 167 -
    XP_ERR_UMAC_NOT_IN_MIX_MODE,    ///< 168 -
    XP_ERR_UMAC_NOT_IN_QSGMII_MODE, ///< 169 -
    XP_ERR_INVALID_SBUS_CMD,        ///< 170 - Invalid SBUS Command
    XP_ERR_FILE_CLOSE,              ///< 171 - File Closed
    XP_ERR_INVALID_BITFIELD,        ///< 172 - Invalid Bitfield
    XP_ERR_SHADOW_MEM_MGR_FAIL,     ///< 173 - Shadow Memory Manager Failure
    XP_ERR_SHADOW_MEM_IOCTL_FAIL,   ///< 174 - Shadow Memory IOCTL Failure
    XP_ERR_MEM_FAIL,                ///< 175 - Memory Failure
    XP_ERR_MEM_RDWR_FAIL,           ///< 176 - Memory Read/Write Failure
    XP_ERR_DMAC_RAW_SOCKET_FAIL,    ///< 177 - Destination MAC Raw Socket Failure
    XP_ERR_DMAC_IOCTL_FAIL,         ///< 178 - Destination MAC IOCTL Failure
    XP_ERR_DMAC_RAW_SOCKET_RD_FAIL, ///< 179 - Destination MAC Raw Socket Read Failure
    XP_ERR_DMAC_RAW_SOCKET_WR_FAIL, ///< 180 - Destination MAC Raw Socket Write Failure
    XP_ERR_FW_FILE_NOT_FOUND,       ///< 181 - Unable to open Service CPU Firmware File
    XP_ERR_FILE_READ,               ///< 182 - Service CPU Firmware File Read Failure
    XP_ERR_SCPU_EXCEPTION,          ///< 183 - Service CPU Exception
    XP_ERR_UMAC_READ_ONLY,          ///< 184 -
    XP_ERR_UMAC_WRITE_ONLY,         ///< 185 -
    XP_ERR_SERDES_ACCESS,           ///< 186 - SERDES Access Failure
    XP_ERR_SBUS_THERMAL_SENSOR_FAILED,  ///< 187 - SBUS Thermal Sensor Failure
    XP_ERR_I2C_DEV_OPEN,            ///< 188 -
    XP_ERR_I2C_DEV_CLOSE,           ///< 189 -
    XP_ERR_I2C_XFER,                ///< 190 - I2C Transfer Error
    XP_ERR_DONGLE_I2C_XFER,         ///< 191 - I2C Dongle Transfer Error
    XP_ERR_MDIO_XFER,               ///< 192 - MDIO Transfer Error
    XP_ERR_MDIO_BUSY,               ///< 193 - MDIO Busy
    XP_PORT_INITED,             ///< 194 - port is already inited
    XP_ERR_CALL_DERIVE_CLASS_OBJ,   ///< 195
    XP_ERR_INVALID_MIN_ETH_FRAME_SIZE,  ///< 196 - Invalid min ethernet frame size received by UMAC. It should be min 64 byte

    XP_ERR_OF_BAD_MATCH_FIELD = 300,///< 300 - Unsupported field type in OpenFlow match
    XP_ERR_OF_BAD_MATCH_VALUE,      ///< 301 - Unsupported value in a match field
    XP_ERR_OF_BAD_MATCH_WILDCARDS,  ///< 302 - Unsupported combination of fields masked or omitted in OpenFlow match.
    XP_ERR_OF_BAD_ACTION,           ///< 303 - Unknown or unsupported action type
    XP_ERR_OF_BAD_SET_ACTION,       ///< 304 - Unsupported type in SET_FIELD action
    XP_ERR_OF_UNSUP_ACTION_ORDER,   ///< 305 - Unsupported actions order
    XP_ERR_OF_BAD_OUT_PORT,         ///< 306 - Problem validating output port
    XP_ERR_OF_UNSUP_METADATA,       ///< 307 - Metadata value unsupported by datapath
    XP_ERR_OF_BAD_TABLE_ID,         ///< 308 - Table does not exist
    XP_ERR_OF_TABLE_FULL,           ///< 309 - Flow not added because table was full
    XP_ERR_OF_UNSUP_INST,           ///< 310 - Switch or table does not support the instruction
    XP_ERR_OF_BAD_GOTO_TABLE_ID,    ///< 311 - Invalid Table-ID specified for GOTO instruction
    XP_ERR_OF_INVALID_FLOW_ID,      ///< 312 - Invalid flow id of flow key specified
    XP_ERR_OF_INVALID_GROUP,        ///< 313 - Invalid group id or attempt to add more than one bucket to "indirect" group
    XP_ERR_OF_GROUP_EXISTS,         ///< 314 - Group with such ID exists
    XP_ERR_OF_OUT_OF_GROUP,         ///< 315 - Group add failed because luck of space
    XP_ERR_OF_OUT_OF_BUCKET,        ///< 316 - Group add failed because of bucket limitation
    XP_ERR_OF_WEIGHT_UNSUPPORTED,   ///< 317 - Group load sharing unsupported
    XP_ERR_ACM_STATIC_RANGE,        ///< 318 - ACM client corresponds to a static id range. Allocation is not allowed/required for this client+type.
    XP_ERR_INVALID_VLAN_ID,         ///< 319 - Group load sharing unsupported
    XP_ERR_SERDES_NO_SIGNAL,        ///< 320 - no signal detected on serdes
    XP_ERR_MAX_LIMIT,               ///< 321 - Max Limit Exceeded
    XP_INVALID_FEC_MODE,            ///< 322 - FEC mode is invalid
    XP_ERR_LINK_UP,                 ///< 323
    XP_ERR_INVALID_DEVICE_TYPE,     ///< 324 - Invalid device type provided
    XP_STATUS_NUM_OF_ENTRIES
} XP_STATUS;

/**
 * \struct xpPacketInfo
 * \brief Packet Information to be used while Packet Tx/Rx
 *
 */
typedef struct xpPacketInfo
{
    void *buf;       /**< Packet data */
    uint16_t bufSize;    /**< Size of Packet */
    uint8_t priority;    /**< Priority of the Packet, applicable during Packet Tx */
}xpPacketInfo;

/**
 * \struct xpPortConfigData
 * \brief This structure defines the per-port configurations data format
 */
typedef struct xpPortConfig_t
{
    uint32_t portDefaultEXP : 3;         ///< MPLS <EXP> assignment, if enabled
    uint32_t portDefaultDSCP : 6;        ///< IP <DSCP> assignment, if enabled 
    uint32_t portDefaultDEI : 1;         ///< Ethernet<DEI> assignment, if enabled
    uint32_t portDefaultPCP : 3;         ///< Ethernet<PCP> assignment, if enabled
    uint32_t portDefaultDP : 2;          ///< Packet <Drop Precedence> assignment, if enabled
    uint32_t portDefaultTC : 4;          ///< Packet <Traffic Class> assignment, if enabled 
    uint32_t mplsQosEn : 1;              ///< Control to enable MPLS QoS assignment    
    uint32_t IPQosEn : 1;                ///< Control to enable IP QoS assignment
    uint32_t layer2QosEn : 1;            ///< Control to enable Ethernet L2 QoS assignment
    uint32_t egressFilterId : 8;         ///< Egress Filtering ID assignment, if enabled   
    uint32_t setIngressVif : 1;          ///< Control to enable Ingress VIF assignment 
    uint32_t setBridgeDomain : 1;        ///< Control to enable Bridge Domain ID assignment  
    uint32_t acceptedFrameType : 2;      ///< Control to enable accepted frames types
    uint32_t bypassTunnelVif : 1;        ///< Control to enable the VIF assignment for tunnels 
    uint32_t bypassACLsPBR : 1;          ///< Control to enable the Policy-Based-Routing
    uint32_t samplerEn : 1;              ///< Control to enable the Sampling
    uint32_t policerEn : 1;              ///< Control to enable the Policing  
    uint32_t portState : 2;              ///< Port Spanning State configuration
    uint32_t setEgressPortFilter : 1;    ///< Control ro enable Egress Filtering ID assignment
    uint32_t macSAmissCmd : 2;           ///< Packet Command assignment for MAC SA miss       
    uint32_t bumPolicerEn : 1;           ///< TODO: Control to enable
    uint32_t portACLEn : 1;              ///< Control to enable Ingress ACL Policy
    uint32_t portDebugEn : 1;            ///< Control to enable Debug Information Output  
    uint32_t portAclId : 8;              ///< Ingress ACL-ID assignment, if enabled
    uint32_t privateWireEn : 1;          ///< Control to enable Virtual Private Wire Service (VPWS)  
    uint32_t addPortBasedTag : 1;        ///< COntrol to enable adding a tag for Virtual Private Wire Service (VPWS)
    uint32_t Reserved2 : 6;              ///< Reserved
    uint32_t bridgeDomain : 16;          ///< Bridge Domain ID  
    uint32_t evif : 16;                  ///< Egress VIF
    uint32_t mirrorBitMask : 16;         ///< Mirroring Mask vector 
    uint32_t ingressVif : 20;            ///< Ingress VIF assignment, if enabled 
    uint32_t pvidModeAllPkt : 1;         ///< Control to enable Port VLAN-ID assignment to All Rx packets on a port
    uint32_t pvid : 12;                  ///< Port VLAN-ID assignment, if enabled  
    uint32_t mplsQosProfileIdx : 3;      ///< MPLS QoS Profile Index assignment, if enabled
    uint32_t ipQosProfileIdx : 3;        ///< IP QoS Profile Index assignment, if enabled
    uint32_t l2QosProfileIdx : 3;        ///< L2 Ethernet QoS Index assignment, if enabled
    uint32_t etagExists : 1;             ///< Control to enable E-Tag on Port Rx  
} xpPortConfig_t;

/**
 * \struct xpStatistics
 * \brief This structure defines the per-port statistics MIB counters
 */

typedef struct __attribute__((packed)) xp_Statistics
{
    uint64_t frameRxOk;
    uint64_t frameRxAll;
    uint64_t frameRxFcsErr;
    uint64_t frameRxAnyErr;
    uint64_t octetsRxGoodFrame;
    uint64_t octetsRx;
    uint64_t frameRxUnicastAddr;
    uint64_t frameRxMulticastAddr;
    uint64_t frameRxBroadcastAddr;
    uint64_t frameRxTypePause;
    uint64_t frameRxLengthErr;
    uint64_t frameRxUndersize;
    uint64_t frameRxOversize;
    uint64_t fragmentsRx;
    uint64_t jabberFrameRx;
    uint64_t frameRxLength64;
    uint64_t frameRxLength65To127;
    uint64_t frameRxLength128To255;
    uint64_t frameRxLength256To511;
    uint64_t frameRxLength512To1023;
    uint64_t frameRxLength1024To1518;
    uint64_t frameRxLength1519AndUp;
    uint64_t frameDroppedFromRxFIFOFullCondition;
    uint64_t frameTransmittedOk;
    uint64_t frameTransmittedAll;
    uint64_t frameTransmittedWithErr;
    uint64_t octetsTransmittedWithoutErr;
    uint64_t octetsTransmittedTotal;
    uint64_t framesTransmittedUnicast;
    uint64_t framesTransmittedMulticast;
    uint64_t framesTransmittedBroadcast0;
    uint64_t framesTransmittedPause;
}xp_Statistics;

/**
 * \struct xpOfFlowId_t
 * \brief This structure defines OpenFlow entry identifier
 */
typedef struct xpOfFlowId_t {
    bool exactMatch;        ///< Exact match flow entry.
    uint32_t index;         ///< Flow logical ID.
    uint32_t rehashIndex;   ///< New logical ID of old exact match flow entry
                            ///< that has been relocated due to insertion of new
                            ///< exact match flow entry.
} xpOfFlowId_t;

/**
 * \struct xpOfTableStats_t
 * \brief This structure contains OpenFlow table statistic
 */
typedef struct xpOfTableStats_t {
    uint64_t missedCount;   ///< Number of packets that missed table.
    uint64_t matchedCount;  ///< Number of packets that hit table.
} xpOfTableStats_t;

/**
 * \struct xpOfIpv4Match_t
 * \brief This structure contains IPv4 match fields
 */
typedef struct xpOfIpv4Match_t {
    /* IPv4 */
    ipv4Addr_t srcAddr;     ///< IPv4 source address.
    ipv4Addr_t dstAddr;     ///< IPv4 destination address.
    uint8_t tos;            ///< IP ToS (including DSCP and ECN).
    uint8_t proto;          ///< IP Protocol.
} xpOfIpv4Match_t;

/**
 * \struct xpOfMatch_t
 * \brief This structure contains OpenFlow match fields
 */
typedef struct xpOfMatch_t {
    /* L1 */
    uint64_t metadata;          ///< Table Metadata.
    uint32_t inPort;            ///< Ingress vIf.
    uint32_t ipPhyPort;         ///< IN_PORT: Physical port.

    /* L2 */
    macAddr_t ethDst;           ///< Ethernet destination address.
    macAddr_t ethSrc;           ///< Ethernet source address.
    uint16_t ethType;           ///< Ethernet frame type.

    /* VLAN */
    xpOfMatchPktType_e tagging; ///< Match packets with specific tagging type.
    xpVlan_t vlanVID;           ///< VLAN-ID from outermost 802.1Q header.
    uint8_t vlanPCP;            ///< VLAN_VID !=NONE:
                                ///< VLAN-PCP from outermost 802.1Q header.

    /* L3 */
    union {
        xpOfIpv4Match_t ipv4;   ///< IPv4 match fields (ETH_TYPE=0x0800)
    };

    /* L4 */
    uint16_t tpSrc;    ///< TCP/UDP/SCTP source port (IP_PROTO=6/17/132).
    uint16_t tpDst;    ///< TCP/UDP/SCTP destination port (IP_PROTO=6/17/132).

} xpOfMatch_t;

/**
 * \union xpOfPacketField_t
 * \brief This union contains packet fields that can be modified by XP_OFPAT_SET_FIELD action.
 */
typedef union {
    /* Ethernet */
    macAddr_t ethAddr;      ///< Ethernet address.
                            ///< Valid for: ETH_DST | ETH_SRC |
                            ///< ARP_SHA | ARP_THA |
                            ///< IPV6_ND_SLL | IPV6_ND_TLL

    uint16_t ethType;       ///< Ethernet packet type.
                            ///< Valid for ETH_TYPE.

    /* VLAN */
    xpVlan_t vlanVID;       ///< VLAN_VID
                            ///<
    uint8_t vlanPCP;        ///< VLAN_PCP
                            ///<
    /* IPv4 */
    ipv4Addr_t ipv4Addr;    ///< IPv4 address. Valid for: IPV4_SRC | IPV4_DST |
                            ///< ARP_SPA | ARP_TPA

    uint8_t ipDSCP;         ///< IP_DSCP
                            ///<
    uint8_t ipECN;          ///< IP_ECN
                            ///<
    uint8_t ipProto;        ///< IP_PROTO
                            ///<
    /* L4 */
    uint16_t l4Port;        ///< L4 port number. Valid for: TCP_SRC | TCP_DST |
                            ///< UDP_SRC | UDP_DST | SCTP_SRC | SCTP_DST.
} xpOfPacketField_t;

/**
 * \struct xpOfActionSetField_t
 * \brief This structure contains OpenFlow action set fields
 */
typedef struct xpOfActionSetField_t {
    xpOfActionType_e type;  ///< XP_OFPAT_SET_FIELD
    xpOxmType_e subType;    ///< Packet field to be modified. See xpOxmType_t.

    xpOfPacketField_t data; ///< Data Packet’s Field.
    xpOfPacketField_t mask; ///< Mask Packet’s Field.
} xpOfActionSetField_t;

/**
 * \struct xpOfActionOutput_t
 * \brief This structure contains OpenFlow action output fields
 */
typedef struct xpOfActionOutput_t {
    xpOfActionType_e type;  ///< XP_OFPAT_OUTPUT
    uint32_t port;          ///< Egress vIf or one of xpOfPortNo_e.
    uint16_t pktMaxLen;     ///< Max length to send to controller when
                            ///< the 'port' is the XP_OFPP_CONTROLLER or
                            ///< 0xffff to send complete packet. */
} xpOfActionOutput_t;

/**
 * \struct xpOfActionSetTTL_t
 * \brief This structure contains OpenFlow action set TTL fields
 */
typedef struct xpOfActionSetTTL_t {
    xpOfActionType_e type;  ///< XP_OFPAT_SET_NW_TTL
    uint8_t ttl;            ///< IP TTL
} xpOfActionSetTTL_t;

/**
 * \struct xpOfActionGroup_t
 * \brief This structure contains OpenFlow action group fields
 */
typedef struct xpOfActionGroup_t {
    xpOfActionType_e type;  ///< XP_OFPAT_GROUP
    uint32_t groupId;       ///< Group identifier.
} xpOfActionGroup_t;

/**
 * \union xpOfAction_t
 * \brief This union defines all supported OpenFlow actions
 */
typedef union xpOfAction_t {
    xpOfActionType_e type;          ///< Action type. The following actions
                                    ///< do not have specific data fields:
                                    ///<     XP_OFPAT_DEC_NW_TTL,
                                    ///<     XP_OFPAT_POP_VLAN

    xpOfActionSetField_t setField;  ///< Data for XP_OFPAT_SET_FIELD action

    xpOfActionOutput_t output;      ///< Data for XP_OFPAT_OUTPUT action

    xpOfActionSetTTL_t setTTL;      ///< Data for XP_OFPAT_SET_NW_TTL action

    xpOfActionGroup_t group;        ///< Data for XP_OFPAT_GROUP action

} xpOfAction_t;

/**
 * \struct xpOfBucket_t
 * \brief This structure defines a bucket for use in OF groups
 */
typedef struct xpOfBucket_t {
    uint16_t weight;          ///< Relative weight. Valid for XP_OFPGT_SELECT only!
    uint32_t actionSetLen;    ///< Number of actions in the action set.
    xpOfAction_t *actionSet;  ///< Action set associated with the bucket.
} xpOfBucket_t;

/**
 * \struct xpOfInstructions_t
 * \brief This structure defines OpenFlow instructions.
 *        The instructions of the set execute in the order
 *        specified by this structure.
 */
typedef struct xpOfInstructions_t {

    //@{
    uint32_t actionListLen;   ///< Implements OFPIT_APPLY_ACTIONS instruction.
                              ///< Specifies the number of actions in action list.
    xpOfAction_t *actionList; ///< Implements OFPIT_APPLY_ACTIONS instruction.
                              ///< Applies the specific action(s) immediately, without
                              ///< any change to the Action Set. This instruction
                              ///< may be used to modify the packet between two tables or
                              ///< to execute multiple actions of the same type.
    //@}

    //@{
    bool clearActions;        ///< Implements OFPIT_CLEAR_ACTIONS instruction.
                              ///< Clears all the actions in the action set immediately.
    //@}

    //@{
    uint32_t actionSetLen;    ///< Implements OFPIT_WRITE_ACTIONS instruction.
                              ///< Specifies the number of actions in action set.
    xpOfAction_t *actionSet;  ///< Implements OFPIT_WRITE_ACTIONS instruction.
                              ///< Merges the specified action(s) into the current
                              ///< action set. If an action of the given type exists
                              ///< in the current set, overwrite it, otherwise add it.
    //@}

    //@{
    bool writeMetadata;       ///< Implements OFPIT_WRITE_METADATA instruction.
    uint64_t metadata;        ///< Metadata value
    uint64_t metadataMask;    ///< Metadata bitmask
    //@}

    //@{
    uint8_t gotoTable;        ///< Implements OFPIT_GOTO_TABLE instruction.
                              ///< Specifies a next table in the lookup pipeline or
                              ///< zero if not specified.
    //@}

} xpOfInstructions_t;

/**
 * \struct xpFlowEntry_t
 * \brief This structure specifies OpenFlow entry parameters.
 */
typedef struct xpFlowEntry_t {
    xpOfFlowType_e type;              ///< Flow type.
    uint8_t tableId;                  ///< ID of OpenFlow Table.
    uint16_t priority;                ///< Priority level of flow entry.
    xpOfMatch_t match;                ///< Fields to match with packets.
    xpOfMatch_t matchMask;            ///< Match mask.
    xpOfInstructions_t instructions;  ///< Instructions that will be executed on packet match

} xpFlowEntry_t;

/**
 * \struct xpOfStats_t
 * \brief This structure specifies OpenFlow statistics.
 *        A counter will be set to the maximum field value
 *        if not supported.
 */
typedef struct xpOfStats_t
{
    uint64_t packetCount;     ///< Number of packets processed.
    uint64_t byteCount;       ///< Number of bytes processed.
} xpOfStats_t;

/**
 * \struct xpOfPacketIn_t
 * \brief Packet-in context.
 */
typedef struct xpOfPacketIn_t {
    xpOfPacketInReason_e reason;
    uint8_t tableId;       ///< ID of the table that was looked up.
    uint32_t flowId;       ///< The ID of the flow entry that was looked up
                           ///< or -1 if undefined (e.g., group bucket or
                           ///< action set).
    uint64_t tunnelId;     ///< Metadata associated with a logical port.
    uint64_t metadata;     ///< Table Metadata.
    uint32_t inPort;       ///< Ingress vIf.
    uint32_t inPhyPort;    ///< Physical port.

    uint8_t *packet;       ///< Trapped packet.
    uint16_t packetLen;    ///< Trapped packet's full length.
    uint16_t maxLen;       ///< Number of bytes from 'packet' that
                           ///< must be sent to the controller.
                           ///< See pktMaxLen in xpOfActionOutput_t.
} xpOfPacketIn_t;

/**
 * \struct xpOfTcam_t
 * \brief OpenFlow TCAM table profile
 */
typedef struct xpOfTcam_t
{
    uint32_t keySize;
    uint32_t dataSize;
    uint32_t numDb;
} xpOfTcam_t;

/**
 * \struct xpOfHash_t
 * \brief OpenFlow SRAM table profile
 */
typedef struct xpOfHash_t
{
    uint32_t keySize;
    uint32_t dataSize;
    uint32_t numEntries;
    uint32_t numOfWays;
} xpOfHash_t;

/**
 * \union xpOfTableData_t
 * \brief OpenFlow table profile
 */
typedef union xpOfTableData_t
{
    xpOfTcam_t ofTcam;
    xpOfHash_t ofHash;
} xpOfTableData_t;

/**
 * \union xpOfTableInfo_t
 * \brief OpenFlow table information
 */
typedef struct xpOfTableInfo_t
{
    uint32_t        tableId;
    xpOfTableType_e tblType;
    xpOfTableData_t tblData;
} xpOfTableInfo_t;

/**
 * \union xpOpenFlowTableProfile_t
 * \brief OpenFlow table profile
 */
typedef struct xpOpenFlowTableProfile_t
{
    uint32_t numTables;
    xpOfTableInfo_t ofTblProfile[XP_OF_MAX_TABLE_PROFILES];
} xpOpenFlowTableProfile_t;

/**
 * \brief Function pointer to be registered for the port event handler
 * \param [in] devId Device ID
 * \param [in] portNum Port Number
 *
 * \return XP_STATUS
 */
typedef void (*xpEventHandler)(xpDevice_t devId, uint8_t portNum);

/**
 * \brief Function pointer to be registered for completion of packet transmission
 * \param [in] intrSrcDev Device Id. Valid values are 0-63
 *
 * \return XP_STATUS
 */
typedef void (*xpPacketTxCompletion)(xpDevice_t intrSrcDev);

/**
 * \brief Function pointer to indicate whether the packet is available
 * \param [in] xpDevice_t intrSrcDev Device Id. Valid values are 0-63
 * \param [out] const void * buf Buffer pointer where packet data is available.
 * \param [in] uint16_t bufSize Size of packet available
 *
 * \return XP_STATUS
 */
typedef void (*xpPacketAvailable)(xpDevice_t intrSrcDev, const void* buf, uint16_t bufSize);

/**
 * \brief Function pointer to indicate a DMA error
 * \param [in] xpDevice_t intrSrcDev Device Id. Valid values are 0-63
 *
 * \return XP_STATUS
 */
typedef void (*xpDmaError)(xpDevice_t intrSrcDev);

// XPS TYPES
typedef xpDevice_t      xpsDevice_t;
typedef xpVlan_t        xpsVlan_t;
typedef xpPort_t        xpsPort_t;
typedef xpVif_t         xpsVif_t;
typedef macAddrLow_t    xpsMacAddrLow_t;
typedef xpDatapath_t    xpsDatapath_t;

typedef uint32_t xpsInterfaceId_t;
typedef uint32_t xpsStp_t;

/**
 * \struct xpsPacketInfo_t
 * \brief This structure defines the packet Tx/Rx information for Packet Driver 
 *
 */
typedef xpPacketInfo xpsPacketInfo_t;

/**
 * \struct xpsPortConfig_t
 * \brief This structure defines the per-port configurations data format
 *
 */
typedef xpPortConfig_t xpsPortConfig_t;

/**
 * \struct xpsStatistics_t
 * \brief This structure defines the per-port statistics MIB counters
 *
 */
typedef struct xp_Statistics xpsStatistics_t;

/**
 * \struct xpsOfFlowId_t
 * \brief This structure defines OpenFlow entry identifier
 *
 */
typedef xpOfFlowId_t xpsOfFlowId_t;

/**
 * \struct xpsOfTableStats_t
 * \brief This structure contains OpenFlow table statistic
 *
 */
typedef xpOfTableStats_t xpsOfTableStats_t;

/**
 * \struct xpsOfIpv4Match_t
 * \brief This structure contains IPv4 match fields
 *
 */
typedef xpOfIpv4Match_t xpsOfIpv4Match_t;

/**
 * \struct xpsOfMatch_t
 * \brief This structure contains OpenFlow match fields
 *
 */
typedef xpOfMatch_t xpsOfMatch_t;

/**
 * \union xpsOfPacketField_t
 * \brief This union contains packet fields that can be modified by XP_OFPAT_SET_FIELD action.
 *
 */
typedef xpOfPacketField_t xpsOfPacketField_t;

/**
 * \struct xpsOfActionSetField_t
 * \brief This structure contains OpenFlow action set fields
 *
 */
typedef xpOfActionSetField_t xpsOfActionSetField_t;

/**
 * \struct xpsOfActionOutput_t
 * \brief This structure contains OpenFlow action output fields
 *
 */
typedef xpOfActionOutput_t xpsOfActionOutput_t;

/**
 * \struct xpsOfActionSetTTL_t
 * \brief This structure contains OpenFlow action set TTL fields
 *
 */
typedef xpOfActionSetTTL_t xpsOfActionSetTTL_t;

/**
 * \struct xpsOfActionGroup_t
 * \brief This structure contains OpenFlow action group fields
 *
 */
typedef xpOfActionGroup_t xpsOfActionGroup_t;

/**
 * \union xpsOfAction_t
 * \brief This union defines all supported OpenFlow actions
 *
 */
typedef xpOfAction_t xpsOfAction_t;

/**
 * \struct xpsOfBucket_t
 * \brief This structure defines a bucket for use in OF groups
 *
 */
typedef xpOfBucket_t xpsOfBucket_t;

/**
 * \struct xpsOfInstructions_t
 * \brief This structure defines OpenFlow instructions.
 *        The instructions of the set execute in the order
 *        specified by this structure.
 *
 */
typedef xpOfInstructions_t xpsOfInstructions_t;

/**
 * \struct xpsFlowEntry_t
 * \brief This structure specifies OpenFlow entry parameters.
 *
 */
typedef xpFlowEntry_t xpsFlowEntry_t;

/**
 * \struct xpsOfStats_t
 * \brief This structure specifies OpenFlow statistics.
 *        A counter will be set to the maximum field value
 *        if not supported.
 *
 */
typedef xpOfStats_t xpsOfStats_t;

/**
 * \struct xpsOfPacketIn_t
 * \brief Packet-in context.
 *
 */
typedef xpOfPacketIn_t xpsOfPacketIn_t;

/**
 * \struct xpsOfTcam_t
 * \brief OpenFlow TCAM table profile
 *
 */
typedef xpOfTcam_t xpsOfTcam_t;

/**
 * \struct xpsOfHash_t
 * \brief OpenFlow SRAM table profile
 *
 */
typedef xpOfHash_t xpsOfHash_t;

/**
 * \union xpsOfTableData_t
 * \brief OpenFlow table profile
 *
 */
typedef xpOfTableData_t xpsOfTableData_t;

/**
 * \union xpsOfTableInfo_t
 * \brief OpenFlow table information
 *
 */
typedef xpOfTableInfo_t xpsOfTableInfo_t;

/**
 * \union xpsOpenFlowTableProfile_t
 * \brief OpenFlow table profile
 *
 */
typedef xpOpenFlowTableProfile_t xpsOpenFlowTableProfile_t;

/**
 * \brief Function pointer to be registered for the port event handler
 *
 */
typedef xpEventHandler xpsEventHandler_f;

/**
 * \brief Function pointer to be registered for completion of packet transmission
 *
 */
typedef xpPacketTxCompletion xpsPacketTxCompletion_f;

/**
 * \brief Function pointer to indicate whether the packet is available
 *
 */
typedef xpPacketAvailable xpsPacketAvailable_f;

/**
 * \brief Function pointer to indicate a DMA error
 * \param [in] xpDevice_t intrSrcDev
 *
 * \return XP_STATUS
 */
typedef xpDmaError xpsDmaError_f;

#endif 
