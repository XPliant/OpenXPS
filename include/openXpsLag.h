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
 *   FILE : openXpsLag.h
 * 
 *   Abstract: This file defines the apis for LAG Management in OpenXPS.
 ************************************************************************/
/**
 * \file openXpsLag.h
 * \brief This file contains API prototypes and type definitions
 *        for the openXps LAG Management
 * \copyright (c) 2016 Cavium Inc
 */

#ifndef _openXpsLag_h_
#define _openXpsLag_h_

#include "openXpsTypes.h"
#include "openXpsEnums.h"

/**
 * \def XPS_LAG_MAX_PORTS
 * \brief Maximum number of ports in a lag
 */
#define XPS_LAG_MAX_PORTS 128

#ifdef __cplusplus
extern "C" {
#endif

typedef struct xpsLagPortIntfList_s
{
    uint32_t size;
    xpsInterfaceId_t portIntf[XPS_LAG_MAX_PORTS];
} xpsLagPortIntfList_t;

/**
 * \brief Enum used to indicate whether SDK should
 *        automatically distribute the ports in a LAG
 */
typedef enum xpsLagAutoDistributeControl_e
{
    AUTO_DIST_DISABLE = 0,
    AUTO_DIST_ENABLE,
} xpsLagAutoDistributeControl_e;

/**
 * \brief Create a new LAG in the system
 *
 * Create a new LAG Interface and associate it with an ID. 
 * This ID to be used to perform Lag operations and deployment
 *
 * This API only modifies software states. Deploy must be called
 * on each device in order to commit changes to LAG
 * configuration to Hardware
 *
 * \param [out] lagIntf LAG Interface ID
 *
 * \return XP_STATUS
 */
XP_STATUS xpsLagCreate(xpsInterfaceId_t *lagIntf);

/**
 * \brief Destroy an existing LAG in the system
 *
 * Destroy an existing LAG Interface and free the associated ID.
 *
 * Prior to calling this API, all ports must be removed from the LAG
 * Upon removing all of the LAG ports, this API must be called for
 * all devices in the system
 *
 * Calling xpsLagDestroy without following the steps above may
 * lead to an inconsistent hardware state
 *
 * \see xpsLagRemovePort
 * \see xpsLagDeploy
 *
 * \param [in] lagIntf
 *
 * \return XP_STATUS
 */
XP_STATUS xpsLagDestroy(xpsInterfaceId_t lagIntf);

/**
 * \brief Add a port to a LAG
 *
 * This API will add a port to a LAG. This API will only change
 * the software state of the LAG Interface. xpsLagDeploy must
 * be called to commit these changes
 *
 * \param [in] lagIntf LAG Interface ID
 * \param [in] portIntf Port Interface ID
 *
 * \return XP_STATUS
 */
XP_STATUS xpsLagAddPort(xpsInterfaceId_t lagIntf, xpsInterfaceId_t portIntf);

/**
 * \brief Remove a port from a LAG
 *
 * Remove a port from a LAG. This API will only change
 * the software state of the Lag Interface. xpsLagDeploy
 * must be called to commit these changes
 *
 * \param [in] lagIntf LAG Interface ID
 * \param [in] portIntf Port Interface ID
 *
 * \return XP_STATUS
 */
XP_STATUS xpsLagRemovePort(xpsInterfaceId_t lagIntf, xpsInterfaceId_t portIntf);

/**
 * \brief Commit changes made to a Lag configuration to Hardware
 *
 * This API is like a "repository commit". In essence, it's job
 * is to perform all of the hardware writes on a per device
 * basis based off of changes made to a LAG Configuration
 *
 * These changes include the following:
 *  - Creating a new Lag
 *  - Adding Ports to a Lag
 *  - Removing Ports from a Lag
 *
 * Lag destruction is a multi step process, and xpsLagDeploy
 * plays a role by virtue of performing the writes to the
 * hardware post removal of ports of a Lag
 *
 * \param [in] devId Device Id of device.
 * \param [in] lagIntf LAG Interface ID
 * \param [in] autoEnable Automatically distribute in SDK the ports in a LAG
 *
 * \return XP_STATUS
 */
XP_STATUS xpsLagDeploy(xpsDevice_t devId, xpsInterfaceId_t lagIntf, xpsLagAutoDistributeControl_e autoEnable);

/**
 * \brief Get list of port interfaces in the LAG
 *
 * \param [in] lagIntf LAG Interface ID
 * \param [out] portIntfList List of ports that are members in a specific LAG 
 *
 * \return XP_STATUS
 */
XP_STATUS xpsLagGetPortIntfList(xpsInterfaceId_t lagIntf, xpsLagPortIntfList_t *portIntfList);

/**
 * \brief Check if a port interface is a part of a specific 
 *        LAG interface
 *
 * This API will check specific port membership in a specific LAG
 *
 * \param [in] portIntf Port Interface ID
 * \param [in] lagIntf LAG Interface ID
 * \param [out] isMember a port is a member in a specific LAG or not
 *
 * \return XP_STATUS
 */
XP_STATUS xpsLagIsPortIntfMember(xpsInterfaceId_t portIntf, xpsInterfaceId_t lagIntf, uint32_t *isMember);

/**
 * \brief Set the hashable fields for a specific device
 * 
 * \param [in] deviceId   Device Id of device.
 * \param [in] xpHashField[]  array of hashable fields 
 * \param [in] size array fields sizes 
 *
 * \return XP_STATUS
 */
XP_STATUS xpsLagSetHashFields(xpsDevice_t deviceId, xpHashField* fields, size_t size);

/**
 * \brief Pick the first available LAG interface ID available in the system
 *
 * \param [out] *lagIntf LAG Interface ID
 *
 * \return XP_STATUS
 */
XP_STATUS xpsLagGetFirst(xpsInterfaceId_t *lagIntf);

/**
 * \brief Get the next available LAG interface ID available in the system
 *
 * \param [in] lagIntf LAG Interface ID
 * \param [out] *lagIntfNext Next available LAG Interface ID
 *
 * \return XP_STATUS
 */
XP_STATUS xpsLagGetNext(xpsInterfaceId_t lagIntf, xpsInterfaceId_t *lagIntfNext);

#ifdef __cplusplus
}
#endif

#endif //_openXpsLag_h_
