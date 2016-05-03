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
 *   FILE : openXpsOpenflow.h
 * 
 *   Abstract: This file defines the apis for OpenFlow Management in OpenXPS.
 ************************************************************************/
/**
 * \file openXpsOpenflow.h
 * \brief This file contains API prototypes and type definitions
 *        for the openXps OpenFlow Management
 * \copyright (c) 2016 Cavium Inc
 */

#ifndef _openXpsOpenflow_h_
#define _openXpsOpenflow_h_

#include "openXpsTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief This defines the type of function pointer that the user can register for
 *        OpenFlow packet-in messages.
 */
typedef XP_STATUS (*xpOfPacketInHandler) (xpsDevice_t, xpDatapath_t, xpOfPacketIn_t *, void *);

/**
 * \brief This defines the type of function pointer that the user can register for
 *        OpenFlow statistics flush event.
 */
typedef XP_STATUS (*xpOfFlowStatsFlushHandler) (xpsDevice_t, xpDatapath_t, uint32_t, xpOfFlowId_t *, xpOfStats_t *, void *);

/**
 * \brief This method maintains operation of XP_OFPP_FLOOD and XP_OFPP_ALL
 * OpenFlow service ports. It enables/disables sending packet out of port
 * interface in case action output is set to XP_OFPP_FLOOD or XP_OFPP_ALL
 * service port.
 *
 * \param [in] devId  Physical device identifier
 * \param [in] ofppPort  One of service OpenFlow ports: XP_OFPP_FLOOD, XP_OFPP_ALL
 * \param [in] portIntf  OpenFlow port identifier
 * \param [in] enable  Enable/disable sending packets out of port interface.
 *
 * \return XP_STATUS
 */
XP_STATUS xpsOfSetPortEnable(xpsDevice_t devId, xpOfPortNo_e ofppPort, xpsInterfaceId_t portIntf, uint8_t enable);

/**
 * \brief This method reads the sending enable state of port interface from
 * XP_OFPP_FLOOD or XP_OFPP_ALL OpenFlow service port. If 'enable' holds non
 * zero value then the packets will be sent out of port interface in case
 * action output is set to XP_OFPP_FLOOD or OFPP_ALL service port.
 *
 * \param [in] devId  Physical device identifier
 * \param [in] ofppPort  One of service OpenFlow ports: XP_OFPP_FLOOD, XP_OFPP_ALL
 * \param [in] portIntf  OpenFlow port identifier
 * \param [out] enable  Enable/disable sending status for the port interface.
 *
 * \return XP_STATUS
 */
XP_STATUS xpsOfGetPortEnable(xpsDevice_t devId, xpOfPortNo_e ofppPort, xpsInterfaceId_t portIntf, uint8_t *enable);

/**
 * \brief  This method will take the table profile from the user
 *         and create the tables based on the key size and
 *         numDbs required.
 *
 * \param [in] devId  Physical device identifier
 * \param [in] tblInfo  Table profile which describes particular HW tables properties
 *
 * \return XP_STATUS
 */
XP_STATUS xpsOfCreateTable(xpsDevice_t devId, xpOpenFlowTableProfile_t tblInfo);

/**
 * \brief  This method will delete particular OpenFlow HW tables
 *         base on provided table profile information.
 *
 * \param [in] devId  Physical device identifier
 * \param [in] tblInfo  Table profile that indicates which HW tables must be removed
 *
 * \return XP_STATUS
 */
XP_STATUS xpsOfDeleteTable(xpDevice_t devId, const xpOpenFlowTableProfile_t *tblInfo);

/**
 * \brief This method will enable lookup for a particular tableId and tableType
 *
 * \param [in] devId  Physical device identifier
 * \param [in] tableId  OpenFlow table identifier
 * \param [in] tableType  HW table type
 *
 * \return XP_STATUS
 */
XP_STATUS xpsOfEnableTableLookup(xpsDevice_t devId, uint32_t tableId, xpOfTableType_e tableType);

/**
 * \brief This method will disable lookup for a particular tableId and tableType
 *        Disabling TCAM lookup results in OpenFlow table-miss behavior that
 *        can only drop packets if no match is found in a flow table.
 *
 * \param [in] devId  Physical device identifier
 * \param [in] tableId  OpenFlow table identifier
 * \param [in] tableType  HW table type
 *
 * \return XP_STATUS
 */
XP_STATUS xpsOfDisableTableLookup(xpsDevice_t devId, uint32_t tableId, xpOfTableType_e tableType);

/**
 * \brief This method will take a key field list and a table id from the user
 *        and configure the key mask for the table according to the field list.
 *
 * \param [in] devId  Physical device identifier
 * \param [in] tableId  OpenFlow table identifier
 * \param [in] tableType  HW table type
 * \param [in] keyList  List of Table's key fields
 * \param [in] keyListLen  Number of keys in keyList
 *
 * \return XP_STATUS
 */
XP_STATUS xpsOfSetTableKeyFormat(xpsDevice_t devId, uint32_t tableId, xpOfTableType_e tableType,
                                 const xpOxmType_e *keyList, uint32_t keyListLen);

/**
 * \brief This method applies actions to specified packet.
 *
 *        A list of actions will be applied in the order
 *        they are specified; an empty action list
 *        drops the packet.
 *
 * \param [in] devId  Physical device identifier
 * \param [in] dpId  Logical OpenFlow datapath identifier
 * \param [in] inPort  Ingress port that must be associated with the packet for OpenFlow processing
 * \param [in] packet  Packet to be processed
 * \param [in] packetLen  Packet's length
 * \param [in] actionList  List of actions defining how the packet should be processed by the switch
 * \param [in] actionListLen  Number of actions in actionList
 *
 * \return XP_STATUS
 */
XP_STATUS xpsOfApplyActions(xpsDevice_t devId, xpDatapath_t dpId, uint32_t inPort, uint8_t* packet,
                            uint32_t packetLen, xpOfAction_t* actionList, uint32_t actionListLen);

/**
 * \brief This method returns one table statistics.
 *
 * \param [in] devId  Physical device identifier
 * \param [in] pdId  Logical OpenFlow datapath identifier
 * \param [in] tableId  OpenFlow table identifier
 * \param [out] tableStats  OpenFlow table statistics
 *
 * \return XP_STATUS
 */
XP_STATUS xpsOfGetTableStats(xpsDevice_t devId, xpDatapath_t dpId,
                             uint8_t tableId, xpOfTableStats_t *tableStats);

/**
 * \brief This method adds flow entry to the specified table.
 *
 * \param [in] devId  Physical device identifier
 * \param [in] dpId  Logical OpenFlow datapath identifier
 * \param [in] flowEntry  OpenFlow flow entry to be created
 * \param [out] flowId  OpenFlow entry identifier
 *
 * \return XP_STATUS
 */
XP_STATUS xpsOfAddFlow(xpsDevice_t devId, xpDatapath_t dpId,
                       xpFlowEntry_t *flowEntry, xpOfFlowId_t *flowId);

/**
 * \brief This method modifies one flow entry in the specified table
 *        based on provided flow entry identifier.
 *
 *        If a matching entry exists in the table, the instructions
 *        field of this entry is updated with the value from
 *        the request.
 *
 * \param [in] devId  Physical device identifier
 * \param [in] dpId  Logical OpenFlow datapath identifier
 * \param [in] tableId  OpenFlow table identifier
 * \param [in] flowId  OpenFlow entry to be modified
 * \param [in] instructions  New OpenFlow entry's instructions
 *
 * \return XP_STATUS
 */
XP_STATUS xpsOfModifyFlowById(xpsDevice_t devId, xpDatapath_t dpId,
                              uint8_t tableId, const xpOfFlowId_t *flowId,
                              const xpOfInstructions_t *instructions);

/**
 * \brief This method modifies one flow entry in the specified table.
 *        It causes OpenFlow table lookup based on provided flow entry's
 *        match fields. If a matching entry exists in the table,
 *        the instructions field of this entry is updated with the value from
 *        the request.
 *
 * \param [in] devId  Physical device identifier
 * \param [in] dpId  Logical OpenFlow datapath identifier
 * \param [in] flowEntry  OpenFlow flow entry to be modified with new OpenFlow instructions
 *
 * \return XP_STATUS
 */
XP_STATUS xpsOfModifyFlow(xpsDevice_t devId, xpDatapath_t dpId,
                          const xpFlowEntry_t *flowEntry);

/**
 * \brief This method removes one flow entry from the specified table
 *        by flow entry identifier.
 *
 * \param [in] devId  Physical device identifier
 * \param [in] dpId  Logical OpenFlow datapath identifier
 * \param [in] tableId  OpenFlow table identifier
 * \param [in] flowId  OpenFlow entry to be removed
 *
 * \return XP_STATUS
 */
XP_STATUS xpsOfRemoveFlowById(xpsDevice_t devId, xpDatapath_t dpId,
                              uint8_t tableId, const xpOfFlowId_t *flowId);

/**
 * \brief This method removes one flow entry from the specified table.
 *        It causes OpenFlow table lookup based on provided flow entry's
 *        match fields. If a matching entry exists in the table,
 *        it is removed.
 *
 * \param [in] devId  Physical device identifier
 * \param [in] dpId  Logical OpenFlow datapath identifier
 * \param [in] flowEntry  OpenFlow entry to be removed
 *
 * \return XP_STATUS
 */
XP_STATUS xpsOfRemoveFlow(xpsDevice_t devId, xpDatapath_t dpId,
                          const xpFlowEntry_t *flowEntry);

/**
 * \brief This method returns one flow entry statistic.
 *
 * The function gets the value of per-flow statistic registers.
 * The registers are "clear on read" so caller is responsible
 * for accumulation of data if one is needed.
 *
 * \param [in] devId  Physical device identifier
 * \param [in] dpId  Logical OpenFlow datapath identifier
 * \param [in] tableId  OpenFlow table identifier
 * \param [in] flowId  OpenFlow entry identifier
 * \param [out] flowStats  OpenFlow entry statistics
 *
 * \return XP_STATUS
 */
XP_STATUS xpsOfGetFlowStats(xpsDevice_t devId, xpDatapath_t dpId,
                            uint8_t tableId, const xpOfFlowId_t *flowId,
                            xpOfStats_t *flowStats);

/**
 * \brief This method allows the user to register OF Packet-In handler
 *
 * \param [in] devId  Physical device identifier
 * \param [in] dpId  Logical OpenFlow datapath identifier
 * \param [in] func  User provided callback function to be called on Packet-In event
 * \param [in] userData  User provided data that will be passed as callback function parameter
 *
 * \return XP_STATUS
 */
XP_STATUS xpsOfRegisterPacketInHandler(xpsDevice_t devId, xpDatapath_t dpId,
                                       xpOfPacketInHandler func, void *userData);

/**
 * \brief This method allows the user to unregister OF Packet-In handler
 *
 * \param [in] devId  Physical device identifier
 * \param [in] dpId  Logical OpenFlow datapath identifier
 *
 * \return XP_STATUS
 */
XP_STATUS xpsOfUnregisterPacketInHandler(xpsDevice_t devId, xpDatapath_t dpId);

/**
 * \brief This method allows the user to register handler
 *        that will be executed once flow entry has changed
 *        physical location due to SRAM entry rehashing or
 *        TCAM entries shuffling. This handler will receive
 *        current flow's statistic as input parameter.
 *
 * \param [in] func  User provided callback function to be called
 * \param [in] userData  User provided data that will be passed as callback function parameter
 *
 * \return XP_STATUS
 */
XP_STATUS xpsOfRegisterFlowStatsFlushHandler(xpOfFlowStatsFlushHandler func, 
                                             void *userData);

/**
 * \brief This method allows the user to unregister handler
 *        that should be executed once flow entry has changed
 *        physical location due to SRAM entry rehashing or
 *        TCAM entries shuffling.
 *
 * \return XP_STATUS
 */
XP_STATUS xpsOfUnregisterFlowStatsFlushHandler(void);

/**
 * \brief Get the interface id corresponding to the XP_OFPP_LOCAL port
 *        for OpenFlow data path
 *
 * \param [in] devId  Physical device identifier
 * \param [in] dpId  Logical OpenFlow datapath identifier
 * \param [out] intfId  XP_OFPP_LOCAL interface identifier
 *
 * \return XP_STATUS
 */
XP_STATUS xpsOfGetOfppLocalIntfId(xpsDevice_t devId, xpDatapath_t dpId, 
                                  xpsInterfaceId_t *intfId);

/**
 * \brief Get the interface id corresponding to the XP_OFPP_CONTROLLER port
 *        for OpenFlow data path
 *
 * \param [in] devId  Physical device identifier
 * \param [in] dpId  Logical OpenFlow datapath identifier
 * \param [out] intfId  XP_OFPP_CONTROLLER interface identifier
 *
 * \return XP_STATUS
 */
XP_STATUS xpsOfGetOfppControllerIntfId(xpsDevice_t devId, xpDatapath_t dpId, 
                                       xpsInterfaceId_t *intfId);

/**
 * \brief This method adds new group entry to the group table.
 *
 *        If a group entry with the specified group identifier
 *        already resides in the group table, then the switch
 *        will refuse to add the group entry and return an error.
 *
 * \param [in] devId  Physical device identifier
 * \param [in] dpId  Logical OpenFlow datapath identifier
 * \param [in] groupId  OpenFlow group identifier to be created
 * \param [in] groupType  OpenFlow group type to be created
 * \param [in] buckets  OpenFlow group's action buckets array
 * \param [in] bucketsNum  Number of OpenFlow group's action buckets
 *
 * \return XP_STATUS
 */
XP_STATUS xpsOfAddGroup(xpsDevice_t devId, xpDatapath_t dpId,
                        uint32_t groupId, xpOfGroupType_e groupType, 
                        xpOfBucket_t* buckets, uint32_t bucketsNum);

/**
 * \brief This method modifies group entry in the group table.
 *
 *        If a group entry with the specified group identifier already
 *        resides in the group table, then that entry will be removed,
 *        and the new group entry added.
 *
 *        If a group entry with the specified group identifier
 *        does not already exist then the switch will refuse
 *        the group modification and return an error status.
 *
 * \param [in] devId  Physical device identifier
 * \param [in] dpId  Logical OpenFlow datapath identifier
 * \param [in] groupId  OpenFlow group identifier to be modified
 * \param [in] groupType  OpenFlow group type to be modified
 * \param [in] buckets  OpenFlow group's action buckets array
 * \param [in] bucketsNum  Number of OpenFlow group's action buckets
 *
 * \return XP_STATUS
 */
XP_STATUS xpsOfModifyGroup(xpsDevice_t devId, xpDatapath_t dpId,
                           uint32_t groupId, xpOfGroupType_e groupType, 
                           xpOfBucket_t* buckets, uint32_t bucketsNum);

/**
 * \brief This method removes group entry from the specified table.
 *
 * \param [in] devId  Physical device identifier
 * \param [in] dpId  Logical OpenFlow datapath identifier
 * \param [in] groupId  OpenFlow group identifier to be removed
 *
 * \return XP_STATUS
 */
XP_STATUS xpsOfRemoveGroup(xpsDevice_t devId, xpDatapath_t dpId,
                           uint32_t groupId);

/**
 * \brief This method returns group entry statistic.
 *
 * \param [in] devId  Physical device identifier
 * \param [in] dpId  Logical OpenFlow datapath identifier
 * \param [in] groupId  OpenFlow group identifier
 * \param [out] groupStats  OpenFlow group statistics
 *
 * \return XP_STATUS
 */
XP_STATUS xpsOfGetGroupStats(xpsDevice_t devId, xpDatapath_t dpId,
                             uint32_t groupId, xpOfStats_t *groupStats);

/**
 * \brief This method returns per-bucket statistic of one group entry.
 *
 * \param [in] devId  Physical device identifier
 * \param [in] dpId  Logical OpenFlow datapath identifier
 * \param [in] groupId  OpenFlow group identifier
 * \param [in] bucketsNum  Size of user-allocated array for per-bucket statistics
 * \param [out] bucketStats  User-allocated array for per-bucket statistics
 *
 * \return XP_STATUS
 */
XP_STATUS xpsOfGetBucketStats(xpsDevice_t devId, xpDatapath_t dpId,
                              uint32_t groupId, uint32_t bucketsNum, 
                              xpOfStats_t* bucketStats);

#ifdef __cplusplus
}
#endif

#endif  //_openXpsOpenflow_h_
