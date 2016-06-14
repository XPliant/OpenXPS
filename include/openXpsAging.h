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
 *   FILE : openXpsAging.h
 *
 *   Abstract: This file defines the apis for Aging Management in OpenXPS.
 ************************************************************************/
/**
 * \file openXpsAging.h
 * \brief This file contains API prototypes and type definitions
 *        for the openXps Aging Management
 * \copyright (c) 2016 Cavium Inc
 */

#ifndef _openXpsAging_h_
#define _openXpsAging_h_

#include "openXpsTypes.h"
#include "openXpsEnums.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief This API calls the read FIFO message API to identify aged
 *        out entry. If it will find such entry then API will call
 *        process FIFO message API to call the registered handler
 *        and perform necessary steps on that entry
 *
 * \param [in] devId Device Id of device.
 *
 * \return XP_STATUS
 *
 */
XP_STATUS xpsAgeFifoHandler(xpDevice_t devId);

/**
 * \brief This API sets the Aging Mode to the global age Configuration.
 *
 * \param [in] devId Device Id of device.
 * \param [in] ageMode Global table aging mode
 *
 * \return XP_STATUS
 *
 */
XP_STATUS xpsSetAgingMode(xpDevice_t devId, XP_AGE_MODE_T ageMode);

/**
 * \brief This API sets the Aging Cycle Unit Time into  the
 *        global age Configuration.
 *
 * \param [in] devId Device Id of device.
 * \param [in] unitTime Number of clock cycles
 *
 * \return XP_STATUS
 *
 */
XP_STATUS xpsSetAgingCycleUnitTime(xpDevice_t devId, uint32_t unitTime);

#ifdef __cplusplus
}
#endif

#endif  //_openXpsAging_h_
