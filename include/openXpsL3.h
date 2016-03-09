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
 *   FILE : openXpsL3.h
 * 
 *   Abstract: This file defines the apis for L3 Management in OpenXPS.
 ************************************************************************/
/**
 * \file openXpsL3.h
 * \brief This file contains API prototypes and type definitions
 *        for the openXps L3 Management
 * \copyright (c) 2016 Cavium Inc
 */

#ifndef _openXpsL3_h_
#define _openXpsL3_h_

#include "openXpsTypes.h"
#include "openXpsEnums.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \struct xpsL3NextHopData_t
 * \brief This structure defines the L3  
 *        Next-Hop data
 */
typedef struct xpsL3NextHopData_t
{
    xpsInterfaceId_t  l3InterfaceId;
    macAddr_t macDa;
    xpsInterfaceId_t  egressIntfId;
}xpsL3NextHopData_t;

/**
 * \struct xpsL3NextHopEntry_t
 * \brief This structure defines the L3  
 *        Next-Hop entry
 */
typedef struct xpsL3NextHopEntry_t
{
    xpPktCmd_e      pktCmd;      ///< Packet forwarding processing command   
    uint32_t   serviceInstId;    ///< A Service Instance ID to represent VNI/TNI/ISID for tunnelled packets
    uint32_t    vpnLabel;        ///< VPN Label 
    uint32_t    propTTL;         ///< TTL
    xpsL3NextHopData_t  nextHop; ///< Next-Hop Data
    uint32_t  reasonCode;        ///< Reason Code 
}xpsL3NextHopEntry_t;

/**
 * \struct xpsL3HostEntry_t
 * \brief This structure defines the L3
 *        host entry
 */
typedef struct xpsL3HostEntry_t
{
    uint32_t vrfId;              ///< VRF   
    xpIpPrefixType_t type;       ///< IP Prefix type
    ipv4Addr_t ipv4Addr;         ///< IPv4 
    ipv6Addr_t ipv6Addr;         ///< IPv6 
    xpsL3NextHopEntry_t nhEntry; ///< Next-Hop Entry
}xpsL3HostEntry_t;

/**
 * \struct xpsL3RouteEntry_t
 * \brief This structure defines the L3
 *        routing entry
 */
typedef struct xpsL3RouteEntry_t
{
    uint32_t vrfId;              ///< VRF
    xpIpPrefixType_t type;       ///< IP Prefix type
    ipv4Addr_t ipv4Addr;         ///< IPv4
    ipv6Addr_t ipv6Addr;         ///< IPv6
    uint32_t   ipMaskLen;        ///< IP Mask
    uint32_t   nhEcmpSize;       ///< Next-Hop size
    uint32_t   nhId;             ///< Next-Hop ID
}xpsL3RouteEntry_t;

/**
 * \brief Add a Router MAC for a specific L3 Interface
 *
 * \param [in] devId Device ID
 * \param [in] l3IntfId Layer 3 Interface ID
 * \param [in] mac Mac Address
 * \return XP_STATUS
*/
XP_STATUS xpsL3AddIntfIngressRouterMac(xpsDevice_t devId, xpsInterfaceId_t l3IntfId, macAddr_t mac);

/**
 * \brief Set constant 40 MSB for a device Router Egress MAC.
 *
 * \param [in] devId Device ID
 * \param [in] macHi 40 MSB bits of the Egress Router MAC 
 * \return XP_STATUS
*/
XP_STATUS xpsL3SetEgressRouterMacMSbs(xpsDevice_t devId,  macAddrHigh_t macHi);

/**
 * \brief Set constant 8 LSB for a device Router Egress MAC per L3 Interface.
 *
 * \param [in] devId Device ID
 * \param [in] l3IntfId Layer 3 Interface ID
 * \param [in] macSa Mac Source Address
 *
 * \return XP_STATUS
 */
XP_STATUS xpsL3SetIntfEgressRouterMacLSB(xpsDevice_t devId, xpsInterfaceId_t l3IntfId, macAddrLow_t macSa);

/**
 * \brief Configures the VRF index for routing
 *        interface created on a specific L3 Interface.
 *
 * \param [in] devId
 * \param [in] l3IntfId
 * \param [in] vrfId
 *
 * \return XP_STATUS
*/
XP_STATUS xpsL3SetIntfVrf(xpsDevice_t devId, xpsInterfaceId_t l3IntfId, uint32_t vrfId);

/**
 * \brief Enable IPv4 unicast routing lookups
 *        for packets ingressing on a specific L3 Interface.
 *
 * \param [in] devId
 * \param [in] l3IntfId
 * \param [in] enable
 *
 * \return XP_STATUS
*/
XP_STATUS xpsL3SetIntfIpv4UcRoutingEn(xpsDevice_t devId, xpsInterfaceId_t l3IntfId, uint32_t enable);

/**
 * \brief Enable IPv6 unicast routing lookups
 *        for packets ingressing on a specific L3 Interface.
 *
 * \param [in] devId
 * \param [in] l3IntfId
 * \param [in] enable
 *
 * \return XP_STATUS
*/
XP_STATUS xpsL3SetIntfIpv6UcRoutingEn(xpsDevice_t devId, xpsInterfaceId_t l3IntfId, uint32_t enable);

/**
 * \brief Add a L3 Host Entry
 *
 * \param [in] devId Device Id of device.
 * \param [in] *pL3HostEntry Pointer to L3 Host entry structure.
 * \param [out] *entryHashIndex Index at which the FDB entry is written to hardware. 
 * \param [out] *entryRehashIndex Index of entry which has been rehashed.
 *
 * \return XP_STATUS
 */
XP_STATUS xpsL3AddIpHostEntry(xpsDevice_t devId, xpsL3HostEntry_t *pL3HostEntry, uint32_t *entryHashIndex, uint32_t *entryRehashIndex);

/**
 * \brief Update a L3 Route Entry
 *
 * \param [in] devId Device Id of device.
 * \param [in] *pL3RouteEntry Pointer to L3 Route entry structure.
 *
 * \return XP_STATUS
 */
XP_STATUS xpsL3UpdateIpRouteEntry(xpsDevice_t devId, xpsL3RouteEntry_t *pL3RouteEntry);

/**
 * \brief Remove the L3 Host Entry at the specific index
 *
 * \param [in] devId Device Id of device.
 * \param [in] index Index of the Host entry to be removed from hardware.
 * \param [in] *pL3HostEntry Pointer to L3 Host entry structure.
 *
 * \return XP_STATUS
 */
XP_STATUS xpsL3RemoveIpHostEntryByIndex(xpsDevice_t devId, uint32_t index, xpsL3HostEntry_t *pL3HostEntry);

/**
 * \brief Add L3 Route Entry
 *
 * \param [in] devId Device Id of device.
 * \param [in] *pL3RouteEntry Pointer to L3 Route entry structure.
 * \param [out] *prfxBucketIdx Index at which the L3 Route entry is written to hardware. 
 *
 * \return XP_STATUS
 */
XP_STATUS xpsL3AddIpRouteEntry(xpsDevice_t devId, xpsL3RouteEntry_t *pL3RouteEntry, uint32_t *prfxBucketIdx);

/**
 * \brief Remove the L3 Route Entry at the specific index
 *
 * \param [in] devId Device Id of device.
 * \param [in] *pL3RouteEntry Pointer to L3 Route entry to be removed from hardware.
 *
 * \return XP_STATUS
 */
XP_STATUS xpsL3RemoveIpRouteEntry(xpsDevice_t devId, xpsL3RouteEntry_t *pL3RouteEntry);

/**
 * \brief Allocate Next-Hop for given ECMP Size
 *
 * \param [out] nhId Next-Hop ID
 * \param [in] nhEcmpSize ECMP Group size
 *
 * \return XP_STATUS
 */
XP_STATUS xpsL3CreateRouteNextHop(uint32_t nhEcmpSize, uint32_t *nhId);

/**
 * \brief Set the NextHop Entry for a specific Next-Hop ID
 *
 * \param [in] devId Device Id of device.
 * \param [in] nhId Next-Hop Id of device.
 * \param [in] *pL3NextHopEntry Pointer to L3 Next-Hop entry structure.
 *
 * \return XP_STATUS
 */
XP_STATUS xpsL3SetRouteNextHop(xpsDevice_t devId, uint32_t nhId, xpsL3NextHopEntry_t *pL3NextHopEntry);

/**
 * \brief Free the allocated Next-Hop
 *
 * \param [in] nhId Next-Hop Id of device
 *
 * \return XP_STATUS
 */
XP_STATUS xpsL3DestroyRouteNextHop(uint32_t nhEcmpSize, uint32_t nhId);

/**
 * \brief Create L3 Interface over a specific VLAN.
 *
 * \param [in] vlanId VLAN-ID
 * \param [out] *l3IntfId Pointer to the allocated L3 Interface ID
 *
 * \return XP_STATUS
*/
XP_STATUS xpsL3CreateVlanIntf(xpsVlan_t vlanId, xpsInterfaceId_t *l3IntfId);

/**
 * \brief Removes L3 Interface from a specific VLAN.
 *
 * \param [in] xpsVlan_t vlanId
 * \param [in] l3IntfId L3 Interface ID
 *
 * \return XP_STATUS
*/
XP_STATUS xpsL3DestroyVlanIntf(xpsVlan_t vlanId, xpsInterfaceId_t l3IntfId);

/**
 * \brief Set hashable fields for a device
 *
 * \param [in] deviceId   Device id
 * \param [in] xpHashField[]  array of fields
 *
 * \return XP_STATUS
 */
XP_STATUS xpsL3SetHashFields(xpsDevice_t deviceId, xpHashField* fields, size_t size);

#ifdef __cplusplus
}
#endif

#endif  //_openXpsL3_h_




