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
 *   FILE : openXpsVlan.h
 * 
 *   Abstract: This file defines the apis for VLAN Management in OpenXPS.
 ************************************************************************/
/**
 * \file openXpsVlan.h
 * \brief This file contains API prototypes and type definitions
 *        for the openXps Vlan Management
 * \copyright (c) 2016 Cavium Inc
 */

#ifndef _openXpsVlan_h_
#define _openXpsVlan_h_

#include "openXpsTypes.h"
#include "openXpsEnums.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 *\struct xpsVlanConfig_t
 *\brief VLAN entry format structure
 *       
 */
typedef struct
{
    uint32_t              stpId;                   ///< Spanning Tree Protocol ID
    xpsCountMode_e        countMode;               ///< ACM Count mode
    uint32_t              enableMirror;            ///< Enable Mirroring
    uint32_t              mirrorAnalyzerId;        ///< Mirror Analyzer ID
    xpsPktCmd_e           saMissCmd;               ///< FDB MAC SA miss command
    xpsPktCmd_e           bcCmd;                   ///< Broadcast packet type command  
    xpsPktCmd_e           unknownUcCmd;            ///< Unknown Unicast packet type command 
    xpsPktCmd_e           arpBcCmd;                ///< ARP Broadcast packet type command
    xpsVlanBridgeMcMode_e ipv4McbridgeMode;        ///< IPv4 Bridge Multicast Multicast configuration
    xpsVlanBridgeMcMode_e ipv6McbridgeMode;        ///< IPv6 Bridge Multicast Multicast configuration
    xpsPktCmd_e           unRegMcCmd;              ///< Unregistered Multicast packet type command
} xpsVlanConfig_t;

#define XPS_VLANID_MIN  (0u)
#define XPS_VLANID_MAX  (XP_MAX_VLAN_NUM - 1u)

/**
 * \brief This method creates a VLAN instance on a device, and
 *        writes device specific table entries and fields 
 *        with their default values.
 *
 * \param [in] devId Device ID
 * \param [in] vlanId VLAN ID
 *
 * \return XP_STATUS
 */
XP_STATUS xpsVlanCreate (xpsDevice_t devId, xpsVlan_t vlanId);

/**
 * \brief This method destroys a VLAN instance on a device, and
 *        releases and/or writes device specific table entries.
 *
 * \param [in] devId Device ID
 * \param [in] vlanId VLAN ID
 *
 * \return XP_STATUS
 */
XP_STATUS xpsVlanDestroy (xpsDevice_t devId, xpsVlan_t vlanId);

/**
 * \brief This method adds an End point to a specific VLAN instance.
 *
 * \param [in] devId Device ID
 * \param [in] vlanId VLAN ID
 * \param [in] intfId
 * \param [in] tagType
 * \param [in] data
 *
 * \return XP_STATUS
 */
XP_STATUS xpsVlanAddEndpoint(xpsDevice_t devId, xpsVlan_t vlanId, xpsInterfaceId_t intfId, xpsL2EncapType_e tagType, uint32_t data);

/**
 * \brief This method adds an interface to a specific VLAN instance.
 *
 * \param [in] devId Device ID
 * \param [in] vlanId VLAN ID
 * \param [in] intfId
 * \param [in] tagType
 *
 * \return XP_STATUS
 */
 XP_STATUS xpsVlanAddInterface(xpsDevice_t devId, xpsVlan_t vlanId, xpsInterfaceId_t intfId, xpsL2EncapType_e tagType);

/**
 * \brief This method removes an End point from a specific VLAN instance.
 *
 * \param [in] devId Device ID
 * \param [in] vlanId VLAN ID
 * \param [in] intfId
 *
 * \return XP_STATUS
 */
XP_STATUS xpsVlanRemoveEndpoint(xpsDevice_t devId, xpsVlan_t vlanId, xpsInterfaceId_t intfId);

/**
 * \brief This method removes an interface from a specific VLAN instance.
 *
 * \param [in] devId Device ID
 * \param [in] vlanId VLAN ID
 * \param [in] intfId
 *
 * \return XP_STATUS
 */
XP_STATUS xpsVlanRemoveInterface(xpsDevice_t devId, xpsVlan_t vlanId, xpsInterfaceId_t intfId);

/**
 * \brief This method configures the action (packet command)
 *        for ARP broadcast packets, for a specific VLAN.
 *
 * \param [in] devId Device ID
 * \param [in] vlanId VLAN ID
 * \param [in] arpBcCmd
 *
 * \return XP_STATUS
 */
XP_STATUS xpsVlanSetArpBcCmd (xpsDevice_t devId, xpsVlan_t vlanId, xpsPktCmd_e arpBcCmd);

/**
 * \brief This method configures the Unknown SA command/action
 *        for a specific VLAN.
 *
 * \param [in] devId Device ID
 * \param [in] vlanId VLAN ID
 * \param [in] saMissCmd
 *
 * \return XP_STATUS
 */
XP_STATUS xpsVlanSetUnknownSaCmd (xpsDevice_t devId, xpsVlan_t vlanId,  xpsPktCmd_e saMissCmd);

/**
 * \brief This method returns configuration of Unknown SA command/action
 *        for a specific VLAN.
 *
 * \param [in] devId Device ID
 * \param [in] vlanId VLAN ID
 * \param [out] *saMissCmd
 *
 * \return XP_STATUS
 */
XP_STATUS xpsVlanGetUnknownSaCmd (xpsDevice_t devId, xpsVlan_t vlanId,  xpsPktCmd_e *saMissCmd);

/**
 * \brief This method configures multiple VLAN properties
 *          for a specific VLAN.
 *
 * \param [in] devId Device ID
 * \param [in] vlanId VLAN ID
 * \param [in] *vlanConfig
 *
 * \return XP_STATUS
 */
XP_STATUS xpsVlanSetConfig (xpsDevice_t devId, xpsVlan_t vlanId, xpsVlanConfig_t *vlanConfig);

/**
 * \brief This method returns multiple VLAN properties
 *          for a specific VLAN.
 *
 * \param [in] devId Device ID
 * \param [in] vlanId VLAN ID
 * \param [out] *vlanConfig
 *
 * \return XP_STATUS
 */
XP_STATUS xpsVlanGetConfig (xpsDevice_t devId, xpsVlan_t vlanId, xpsVlanConfig_t *vlanConfig);

/**
 * \brief This method sets the Global Control MAC. 
 *        Applicable across all VLANs on a device
 *
 * \param [in] devId Device ID
 * \param [in] mac
 *
 * \return XP_STATUS
 */
XP_STATUS xpsVlanSetGlobalControlMac(xpsDevice_t devId, macAddr_t mac);

/**
 * \brief This method clears the Global Control MAC. 
 *        Applicable across all VLAN'S
 *
 * \param [in] devId Device ID
 * \param [in] mac
 *
 * \return XP_STATUS
 */
XP_STATUS xpsVlanClearGlobalControlMac(xpsDevice_t devId, macAddr_t mac);

/**
 * \brief This method enables OpenFlow processing on a specific {VLAN, Interface}.
 *
 * \param [in] devId Device ID
 * \param [in] vlanId VLAN ID
 * \param [in] intfId
 * \param [in] enable
 *
 * \return XP_STATUS
 */
XP_STATUS xpsVlanSetOpenFlowEnable(xpsDevice_t devId, xpsVlan_t vlanId, xpsInterfaceId_t intfId, uint32_t enable);

#ifdef __cplusplus
}
#endif

#endif  //_openXpsVlan_h_

