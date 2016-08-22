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
 *   FILE : openXpsPacketDrv.h
 * 
 *   Abstract: This file defines the apis for Packet Driver Management in OpenXPS.
 ************************************************************************/
/**
 * \file openXpsPacketDrv.h
 * \brief This file contains API prototypes and type definitions
 *        for the openXps Packet Driver Management
 * \copyright (c) 2016 Cavium Inc
 */
#ifndef _OPENXPSPACKETDRV_H_
#define _OPENXPSPACKETDRV_H_

#include "openXpsTypes.h"
#include "openXpsEnums.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Function pointer for cpu packet rx processing
 * \param [in] devId Device Id. Valid values are 0-63
 * \param [in] portNum Port number
 * \param [out] buf Buffer pointer where packet data is available.
 * \param [in] bufSize Size of the packet
 * \param [in] userData User data passed to Rx handler callback
 *
 * \return XP_STATUS
 */
typedef XP_STATUS (*xpsPacketRxHdlr)(xpDevice_t devId, xpsPort_t portNum, void *buf, uint16_t bufSize, void* userData);

/**
 * \brief Get the Packet driver current TX/RX interface
 *
 * \param [in] devId device Id. Valid values are 0-63
 * \param [out] packetInterface Current packet driver interface. Enum value of DMA or ETHER
 * \return [XP_STATUS] On success XP_NO_ERR
*/
XP_STATUS xpsPacketDriverInterfaceGet(xpsDevice_t devId, xpPacketInterface* const packetInterface);

/**
 * \brief Set the configuration mode (Interrupt/Polling) for Packet driver\n
 *      Valid only for receive mode
 *
 * \param [in] devId device Id. Valid values are 0-63
 * \param [in] configMode enum value of INTR or POLL
 * \return [XP_STATUS] On success XP_NO_ERR
*/
XP_STATUS xpsPacketDriverRxConfigModeSet(xpsDevice_t devId, xpRxConfigMode configMode);

/**
 * \brief Get the current configuration mode (Interrupt/Polling) for Packet driver
 *
 * \param [in] devId device Id. Valid values are 0-63
 * \param [out] *configMode enum value to be return. Value is INTR or POLL
 * \return [XP_STATUS] On success XP_NO_ERR
*/
XP_STATUS xpsPacketDriverRxConfigModeGet(xpsDevice_t devId, xpRxConfigMode* const configMode);

/**
 * \brief Send packet on the current interface
 *
 * In DMA mode, this function will setup the descriptor from information provided in
 * buffer and will configure the DMA and trigger it to start transfer of packets
 * In Ethernet mode, this function will send buffer over Ethernet Interface.
 * There are two tx modes: SYNC and ASYNC. In the SYNC mode the system waits till
 * tx is completed. In the ASYNC mode the system moves ahead after the completion
 * of packet transmission. The customer OS is notified about Tx completion via
 * interrupt.
 *
 * \param [in] devId device Id. Valid values are 0-63
 * \param [in] *pktInfo Packet information
 * \param [in] sendMode Mode specifying Tx. Has value SYNC_TX/ASYNC_TX
 *
 * \return [XP_STATUS] On success XP_NO_ERR
*/
XP_STATUS xpsPacketDriverSend(xpsDevice_t devId, const struct xpPacketInfo* const pktInfo, txMode sendMode);

/*
 * \brief Encapsulate the packet
 *
 * \param [in] devId device Id. Valid values are 0-63
 * \param [out] *packet Packet pointer
 * \param [in] srcVif source vif
 * \param [in] dstVif destination vif
 * \param [in] sendToEgress packet send egress or not
 */
XP_STATUS xpsPacketDriverCreateHeader(xpsDevice_t devId, xpPacketInfo* pktInfo, xpVif_t srcVif, xpVif_t dstVif, bool sendToEgress);

/**
 * \brief Receive packet on default or previously set interface.
 *
 * In DMA mode receive maxPkt number of packets on the DMA interface. If requested
 * number of packets are not available return the packets that are available.
 * In Ethernet/netdev mode, this function will receive single buffer over Ethernet/netdev Interface.
 *
 * \param [in] devId device Id. Valid values are 0-63
 * \param [in,out] **pktInfo pointer to array of xpPacketInfo
 * \param [in,out] *maxPkt Maximum number of packets to be received. Will be updated to indicate number of packets actually received
 * \return [XP_STATUS] On success XP_NO_ERR
*/
XP_STATUS xpsPacketDriverReceive(xpsDevice_t devId, struct xpPacketInfo **pktInfo, uint16_t *maxPkt);

/**
 * \brief Register the packet send/receive completion handler
 *
 * \param [in] devId device Id. Valid values are 0-63
 * \param [in] onSend Pointer to the handler to be called on packet send completion
 * \param [in] onRecv Pointer to the handler to be called on packet receive completion
 * \param [in] onError Pointer to handler to be called on error
 * \return [XP_STATUS] On success XP_NO_ERR. On failure XP_ERR_NOT_INITED
*/
XP_STATUS xpsPacketDriverCompletionHndlrRegister(xpsDevice_t devId, xpPacketTxCompletion onSend, xpPacketAvailable onRecv, xpDmaError onError);

/**
 * \brief De-register the packet send completion function,\n
 *    packet receive completion function or both type of function
 *
 * \param [in] devId device Id. Valid values are 0-63
 * \param [in] what enum value to de-register packet send completion function/packet receive\n
 *      completion function or both type of function
 * \return [XP_STATUS] On success XP_NO_ERR
*/
XP_STATUS xpsPacketDriverCompletionHndlrDeRegister(xpsDevice_t devId, xpCallbackMethodType what);

/**
 * \brief register a callback
 *
 * \param [in] type feature type
 * \param [in] cbPtr callback pointer
 * \param [in] userData user argument
 * \return [XP_STATUS] On success XP_NO_ERR
 */
XP_STATUS xpsPacketDriverFeatureRxHndlr(xpsCpuRxPktCbType_e type, xpsPacketRxHdlr cbPtr, void* userData);

/**
 * \brief de-register a callback
 *
 * \param [in] type feature type
 * \return [XP_STATUS] On success XP_NO_ERR
 */
XP_STATUS xpsPacketDriverFeatureRxHndlrDeRegister(xpsCpuRxPktCbType_e type);

XP_STATUS xpsPacketDriverGetTxHdrSize(size_t* sizeOfTxHd);

XP_STATUS xpsNlTxHeaderMsgAssign(xpsDevice_t devId, xpsInterfaceId_t xpsIfId, uint8_t *msg);

XP_STATUS xpsNlSockGetBuf(uint8_t** nlData, uint8_t** nlBuf);

XP_STATUS xpsNlSockFreeBuf(uint8_t* nlBuf);

XP_STATUS xpsNlSockSendMsg(uint8_t* nlBuf);

/**
 * \brief xpsNetdevXpnetIdAllocate returns xpnetId
 *  
 * \param [in] devId: device Id. Valid values are 0-63 
 * \param [out] xpnetId: xpnet identificator
 *
 * \return [XP_STATUS]
 */
XP_STATUS xpsNetdevXpnetIdAllocate(xpsDevice_t devId, uint32_t *xpnetId);

/**
 * \brief xpsNetdevXpnetIdAllocateWith returns specified xpnetId
 *  
 * \param [in] devId: device Id. Valid values are 0-63 
 * \param [in] xpnetId: xpnet identificator
 *
 * \return [XP_STATUS]
 */
XP_STATUS xpsNetdevXpnetIdAllocateWith(xpsDevice_t devId, uint32_t xpnetId);

/**
 * \brief xpsNetdevXpnetIdFree frees xpnetId
 *  
 * \param [in] devId: device Id. Valid values are 0-63 
 * \param [in] xpnetId: xpnet identificator
 *
 * \return [XP_STATUS]
 */
XP_STATUS xpsNetdevXpnetIdFree(xpsDevice_t devId, uint32_t xpnetId);

/**
 * \brief xpsNetdevIfCreate creates a netdev interface
 *  
 * \param [in] devId: device Id. Valid values are 0-63 
 * \param [in] xpnetId: xpnet identificator 
 * \param [in] ifName: interface name
 *
 * \return [XP_STATUS]
 */
XP_STATUS xpsNetdevIfCreate(xpsDevice_t devId, uint32_t xpnetId, char *ifName);

/**
 * \brief xpsNetdevIfDelete deletes a netdev interface
 *  
 * \param [in] devId: device Id. Valid values are 0-63 
 * \param [in] xpnetId: xpnet identificator 
 *
 * \return [XP_STATUS]
 */
XP_STATUS xpsNetdevIfDelete(xpsDevice_t devId, uint32_t xpnetId);

/**
 * \brief xpsNetdevIfLinkSet links/unlinks a netdev interface 
 *                           with xp interface
 *  
 * \param [in] devId: device Id. Valid values are 0-63
 * \param [in] xpnetId: xpnet identificator 
 * \param [in] xpsIf: xps interface
 * \param [in] flag: link/unlink
 *
 * \return [XP_STATUS]
 */
XP_STATUS xpsNetdevIfLinkSet(xpsDevice_t devId, uint32_t xpnetId, 
                             xpsInterfaceId_t xpsIf, bool flag);

/**
 * \brief xpsNetdevIfTxHeaderSet sets XP TX header
 *  
 * \param [in] devId: device Id. Valid values are 0-63 
 * \param [in] xpnetId: xpnet identificator 
 * \param [in] xpsIf: xps interface
 * \param [in] flag: set/unset
 *
 * \return [XP_STATUS]
 */
XP_STATUS xpsNetdevIfTxHeaderSet(xpsDevice_t devId, uint32_t xpnetId, 
                                 xpsInterfaceId_t xpsIf, bool flag);

/**
 * \brief xpsNetdevTrapSet assign/unassign trap
 *  
 * \param [in] devId: device Id. Valid values are 0-63 
 * \param [in] trapId: trap indentificator
 * \param [in] rCode: reason code
 * \param [in] ch: channel
 * \param [in] fd: socket descriptor
 * \param [in] flag: add/remove
 *
 * \return [XP_STATUS]
 */
XP_STATUS xpsNetdevTrapSet(xpsDevice_t devId, uint32_t trapId, uint32_t rCode, 
                           uint32_t ch, uint32_t fd, bool flag);

/**
 * \brief xpsNetdevCbFdSet registers file descriptor in kernel 
 *        module to recived packets
 *  
 * \param [in] devId: device Id. Valid values are 0-63
 * \param [in] fd: call back channel FD 
 * \param [in] flag: set/unset 
 *
 * \return [XP_STATUS]
 */
XP_STATUS xpsNetdevCbFdSet(xpsDevice_t devId, uint32_t fd, bool flag);

/**
 * \brief xpsNetdevMirrorSet enables mirroring to XDK
 *  
 * \param [in] devId: device Id. Valid values are 0-63
 * \param [in] flag: enable/disable
 *
 * \return [XP_STATUS]
 */
XP_STATUS xpsNetdevMirrorSet(xpsDevice_t devId, bool flag);

#ifdef __cplusplus
}
#endif

#endif //_OPENXPSPACKETDRV_H_
