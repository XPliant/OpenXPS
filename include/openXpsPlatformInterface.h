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
 *   FILE : openXpsPlatformInterface.h
 * 
 *   Abstract: This file defines the platform API in OpenXPS.
 ************************************************************************/
/**
 * \file openXpsPlatformInterface.h
 * \brief This file contains API prototypes and type definitions
 *        for the openXps Platform Interface Management
 * \copyright (c) 2016 Cavium Inc
 */

#ifndef _openXpsPlatformInterface_h_
#define _openXpsPlatformInterface_h_


#include <sys/types.h>
#include "openXpsTypes.h"
#include "openXpsEnums.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Generic method for platform init.
 *
 * @param [in] target - Target name (svb, crb1, crb2, ...)
 * @param [in] initType - possible init modes (if mode is not available use INIT_UNKNOWN)
 * @param [in] diagMode
 * @param [in] targetPlatformPath - Directory containing platform file
 *             The platform configuration file shall be placed under “targetPlatformPath/target/target.cfg”
 * @return [XP_STATUS] On success XP_NO_ERR
 */
XP_STATUS xpPlatformInit(const char *target, xpInitType_t initType, bool diagMode, char *targetPlatformPath);

#ifdef __cplusplus
}
#endif

#endif // _openXpsPlatformInterface_h_
