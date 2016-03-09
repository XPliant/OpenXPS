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
 *   FILE : openXpsInterface.h
 * 
 *   Abstract: This file defines the apis for Interface Management in OpenXPS.
 ************************************************************************/
/**
 * \file openXpsInterface.h
 * \brief This file contains API prototypes and type definitions
 *        for the openXps Interface Management
 * \copyright (c) 2016 Cavium Inc
 */

#ifndef _openXpsInterface_h_
#define _openXpsInterface_h_

#include "openXpsTypes.h"
#include "openXpsEnums.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \def XPS_INTF_INVALID_ID
 *
 * Sentinel ID used to represent an invalid value
 */
#define XPS_INTF_INVALID_ID 0xFFFFFFFFu

/**
 * \def XPS_INTF_MAP_INTFID_TO_VIF(intfId)
 *
 * A Macro that returns VIF for a specific intfId
 */
#define XPS_INTF_MAP_INTFID_TO_VIF(intfId) ((xpVif_t)(intfId))

/**
 * \def XPS_INTF_MAP_VIF_TO_INTFID(vif)
 *
 * A Macro to return intfId for a specific VID
 */
#define XPS_INTF_MAP_VIF_TO_INTFID(vif)  ((xpsInterfaceId_t)(vif))

/**
 * \brief Retrieves an interface's type information
 *
 * There is no set interface type API. In order to change an
 * interface's type, the interface must be destroyed and
 * recreated
 *
 * \param [in] intfId Interface ID
 * \param [out] *type
 *
 * \return XP_STATUS
 */
XP_STATUS xpsInterfaceGetType(xpsInterfaceId_t intfId, xpsInterfaceType_e *type);

#ifdef __cplusplus
}
#endif

#endif // _openXpsInterface_h_
