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
 *   FILE : openXpsSalInterface.h
 * 
 *   Abstract: This file defines the System Abstraction Layer (SAL) 
 *             functions in OpenXPS to provide abstraction 
 *             and infrastructure to specify SAL object types
 ************************************************************************/
/**
 * \file openXpsSalInterface.h
 * \brief This file contains API prototypes and type definitions
 *        for the openXp Software Abstraction Layer Interface
 * \copyright (c) 2016 Cavium Inc
 */
#ifndef _openXpSalInterface_h_
#define _openXpSalInterface_h_

#include "openXpsTypes.h"
#include "openXpsEnums.h"
 
#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief This method sets the SAL object type.
 *
 * \param [in] salType SAL Object Type
 *
 */

 void xpSetSalType(xpsSalType_e salType);

/**
 * \brief This method initializes the SAL object.
 *
 */
 int xpSalInit();

#ifdef __cplusplus
}
#endif

#endif //_openXpSalInterface_h_
