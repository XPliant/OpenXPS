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
 *   FILE : openXpsPolicer.h
 * 
 *   Abstract: This file defines the apis for Policer Management in OpenXPS.
 ************************************************************************/
/**
 * \file openXpsPolicer.h
 * \brief This file contains API prototypes and type definitions
 *        for the openXps Policer Management
 * \copyright (c) 2016 Cavium Inc
 */

#ifndef _openXpsPolicer_h_
#define _openXpsPolicer_h_

#include "openXpsTypes.h"
#include "openXpsEnums.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 *\struct xpsPolicerEntry_t
 *\brief Policer entry format structure
 *       
 */
typedef struct xpsPolicerEntry_t
{
    uint32_t cir;
    uint32_t pir;
    uint32_t cbs;
    uint32_t pbs;
    uint32_t colorAware;
    uint32_t dropRed;
    uint32_t dropYellow;
    uint32_t updateResultRed;
    uint32_t updateResultYellow;
    uint32_t updateResultGreen;
    xpPolicingResultType polResult;

} xpsPolicerEntry_t;

/**
* \brief Enable policing for a specific port.
*
* \param [in] portIntfId Port Interface ID
* \param [in] enable Policing enable
*
* \return XP_STATUS
*/
XP_STATUS xpsPolicerEnablePortPolicing (xpsInterfaceId_t portIntfId, uint16_t enable);

/**
* \brief Add a policing entry for specific port
*
* \param [in] portIntfId Port Interface ID
* \param [in] client Advanced Counting Module (ACM) client enum
* \param [in] *pEntry Policer Entry added to hardware
*
* \return XP_STATUS
*/
XP_STATUS xpsPolicerAddPortPolicingEntry (xpsInterfaceId_t portIntfId, xpAcmClient_e client, xpsPolicerEntry_t *pEntry);

#ifdef __cplusplus
}
#endif

#endif  //_openXpsPolicer_h_
