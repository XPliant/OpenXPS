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
 *   FILE : openXpsPort.h
 * 
 *   Abstract: This file defines the apis for Port Management in OpenXPS.
 ************************************************************************/
/**
 * \file openXpsPort.h
 * \brief This file contains API prototypes and type definitions
 *        for the openXps Port Management
 * \copyright (c) 2016 Cavium Inc
 */

#ifndef _openXpsPort_h_
#define _openXpsPort_h_

#include "openXpsTypes.h"
#include "openXpsEnums.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Get Ihe interface ID corresponding to the CPU port
 *
 * \param [in] devId Device Id of device
 * \param [out] *intfId Interface ID
 *
 * \return XP_STATUS
 */
XP_STATUS xpsPortGetCPUPortIntfId(xpsDevice_t devId, xpsInterfaceId_t *intfId);

/**
 * \brief Get the interface ID corresponding to a specific port
 *
 * \param [in] devId Device Id of device
 * \param [in] portNum Port
 * \param [out] xpsInterfaceId_t *intfId
 *
 * \return XP_STATUS
 */
XP_STATUS xpsPortGetPortIntfId(xpsDevice_t devId, xpsPort_t portNum, xpsInterfaceId_t *intfId);

/**
 * \brief set fieldValue in port config
 *
 * \param [in] intfId
 * \param [in] portIfId
 * \param [in] fNum
 * \param [in] fData
 *
 * \return int
 */
XP_STATUS xpsPortSetField(xpsDevice_t devId, xpsInterfaceId_t portIfId,xpsPortConfigFieldList_t fNum,uint32_t fData);

/**
 * \brief get fieldValue in port config
 *
 * \param [in] intfId
 * \param [in] portIfId
 * \param [in] fNum
 * \param [in] fData
 *
 * \return int
 */
XP_STATUS xpsPortGetField(xpsDevice_t devId, xpsInterfaceId_t portIfId,xpsPortConfigFieldList_t fNum,uint32_t *fData);

/**
 * \brief Set configurations for a specific port 
 *
 * \param [in] devId Device Id of device
 * \param [in] portNum Port
 * \param [in] *pConfig Port Configuration Entry added to hardware
 *
 * \return XP_STATUS
 */
XP_STATUS xpsPortSetConfig(xpsDevice_t devId, xpsInterfaceId_t portIfId, xpsPortConfig_t *pConfig);

/**
 * \brief Get configurations for a specific port 
 *
 * \param [in] devId Device Id of device
 * \param [in] portNum Port
 * \param [in] *pConfig Port Configuration Entry read
 *
 * \return XP_STATUS
 */

XP_STATUS xpsPortGetConfig(xpsDevice_t devId, xpsInterfaceId_t portIfId, xpsPortConfig_t *pConfig);

/**
 * \brief Get the Device Id and Port Number from the interface
 *        Id
 *
 * \param [in] intfId
 * \param [out] * devId
 * \param [out] * portNum
 *
 * \return int
 */
XP_STATUS xpsPortGetDevAndPortNumFromIntf(xpsInterfaceId_t intfId, xpsDevice_t *devId, xpsPort_t *portNum);

#ifdef __cplusplus
}
#endif

#endif //_openXpsPort_h_
