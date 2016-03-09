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
 *   FILE : openXpsQos.h
 * 
 *   Abstract: This file defines the apis for QoS Management in OpenXPS.
 ************************************************************************/
/**
 * \file openXpsQos.h
 * \brief This file contains API prototypes and type definitions
 *        for the openXps QoS Management
 * \copyright (c) 2016 Cavium Inc
 */

#ifndef _openXpsQos_h_
#define _openXpsQos_h_

#include "openXpsTypes.h"
#include "openXpsEnums.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * \brief This method sets the Priority Flow Control priority for a specific port and queue.
 *
 * \param [in] devId
 * \param [in] portNum
 * \param [in] queueNum
 * \param [in] prio
 *
 * \return XP_STATUS
 */
XP_STATUS xpsQosFcSetPfcPriority(xpsDevice_t devId, uint32_t portNum, uint32_t queueNum, uint8_t prio);

/**
 * \brief This method configures shaping for a specific port on a device
 *
 * This API is used only to configure a shape and does not
 * explicitly enable shaping.
 *
 * \see xpsQosShaperSetPortShaperEnable()
 *
 *
 * \param [in] devId
 * \param [in] devPort
 * \param [in] rateKbps
 * \param [in] maxBurstByteSize
 *
 * \return XP_STATUS
 */
XP_STATUS xpsQosShaperConfigurePortShaper(xpsDevice_t devId, xpPort_t devPort, uint64_t rateKbps, uint32_t maxBurstByteSize);

/**
 * \brief This method enables or disables shaping for a specific port on a device
 *
 * \param [in] devId
 * \param [in] devPort
 * \param [in] enableShaper
 *
 * \return XP_STATUS
 */
XP_STATUS xpsQosShaperSetPortShaperEnable(xpsDevice_t devId, xpPort_t devPort, uint32_t enableShaper);

#ifdef __cplusplus
}
#endif

#endif //_openXpsQos_h_

