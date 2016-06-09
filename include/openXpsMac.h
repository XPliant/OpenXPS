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
 *   FILE : openXpsMac.h
 * 
 *   Abstract: This file defines the apis for Port MAC Management in OpenXPS.
 ************************************************************************/
/**
 * \file openXpsMac.h
 * \brief This file contains API prototypes and type definitions
 *        for the openXps MAC Management
 * \copyright (c) 2016 Cavium Inc
 */

#ifndef _openXpsMac_h_
#define _openXpsMac_h_

#include "openXpsTypes.h"
#include "openXpsEnums.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief  Initialise the port
 *
 * \param [in] devId device Id. Valid values are 0-63
 * \param [in] macNum       mac number. Valid values are 0 to 32.
 * \param [in] macConfigMode  mac mode of the port
 * \param [in] speed speed of the 10/100/1000Mbps modes. used when these modes are used
 * \param [in] initSerDes set to 1 to reinit SerDes
 * \param [in] prbsTestMode 
 * \param [in] firmwareUpload
 * \param [in] fecMode 
 * \param [in] enableFEC set to 1 to enable FEC
 *
 * \return XP_STATUS
*/
XP_STATUS xpsMacPortGroupInit(xpsDevice_t devId, uint8_t macNum, xpMacConfigMode macConfig, xpSpeed speed, uint8_t initSerdes, uint8_t prbsTestMode, uint8_t firmwareUpload, xpFecMode fecMode, uint8_t enableFEC);

/**
 * \brief  Deinitialise the port group
 *
 * \param [in] devId device Id. Valid values are 0-63
 * \param [in] macNum       mac number. Valid values are 0 to 32.
 *
 * \return XP_STATUS
*/
XP_STATUS xpsMacPortGroupDeInit(xpsDevice_t devId, uint8_t macNum);

/**
 * \brief Change the MAC mode 
 *
 * This method assumes that all existing ports are deleted for given MAC, 
 * and performs the mode change sequence
 *
 * \param [in] devId device Id. Valid values are 0-63
 * \param [in] macNum Mac number. Valid values are 0 to 32.
 * \param [in] newMode enum specifying mac configuration mode
 * \param [in] fecMode enum specifying the FEC mode, if enabled
 * \param [in] enableFEC enable FEC mode
 *
 * \return XP_STATUS
 */
XP_STATUS xpsMacSwitchMacConfigMode(xpsDevice_t devId, uint8_t macNum, xpMacConfigMode newMode, xpFecMode fecMode, uint8_t enableFEC);

/**
 * \brief To get mac mode of port
 *
 * \param [in] devId device id. Valid values are 0-63
 * \param [in] portNum Port number. Valid values are 0 to 127 and 176.
 * \param [out] macConfigMode - mac mode
 *
 * \return XP_STATUS
 */
XP_STATUS xpsMacConfigModeGet(xpDevice_t devId, uint8_t portNum, xpMacConfigMode *macConfigMode);


/**
 * \brief Get MAC number mapping for a specific port number 
 *
 * \param [in] devId device Id. Valid values are 0-63
 * \param [in] portNum Port number. Valid values are 0 to 127.
 * \param [out] macNum Mac number. Valid values are 0 to 32.
 *
 * \return XP_STATUS
 */
XP_STATUS xpsMacGetMacNumForPortNum(xpsDevice_t devId, uint8_t portNum, uint8_t *macNum);

/**
 * \brief Serdes Tuning  
 * \param [in] devId device Id. Valid values are 0-63
 * \param [in] portList List of ports associated with the serdes to tune
 * \param [in] numOfPort Number of ports to tune
 * \param [in] tuneMode DFE serdes tuning type
 * \param [in] force Force serdes tuning
 *
 * \return XP_STATUS
 */
XP_STATUS xpsMacPortSerdesTune(xpsDevice_t devId, xpPort_t *portList, uint32_t numOfPort, xpSerdesDfeTuneMode_t tuneMode, uint8_t force);

/**
 * \brief Check whether a specific port is initialized  
 * \param [in] devId device Id. Valid values are 0-63
 * \param [in] portNum Port number. Valid values are 0 to 127.
 *
 * \return XP_STATUS
 */
XP_STATUS xpsIsPortInited(xpsDevice_t devId, uint8_t portNum);

/**
 * \brief Check whether a specific port is valid  
 * \param [in] devId device Id. Valid values are 0-63
 * \param [in] portNum Port number. Valid values are 0 to 127.
 * \param [out] *valid - Port valid or not
 *
 * \return XP_STATUS
 */
XP_STATUS xpsMacIsPortNumValid(xpsDevice_t devId, uint8_t portNum, uint8_t *valid);

/**
 * \brief Enable or Disable a given range of ports  
 * \param [in] devId device Id. Valid values are 0-63
 * \param [in] portNum Port number. Valid values are 0 to 127.
 * \param [in] enable true to enable the port, false to disable the port
 *
 * \return XP_STATUS
 */
XP_STATUS xpsMacPortEnable(xpsDevice_t devId, uint8_t portNum, uint8_t enable);

/**
 * \brief Controls the MRU acceptance frame length for a specifoc port  
 * \param [in] devId device Id. Valid values are 0-63
 * \param [in] portNum Port number. Valid values are 0 to 127.
 * \param [in] frameSize MRU allowance frame length 
 *
 * \return XP_STATUS
 */
XP_STATUS xpsMacSetRxMaxFrmLen(xpsDevice_t devId, uint8_t portNum, uint16_t frameSize);

/**
 * \brief Get the speed for a specific port 
 *
 *        This API gets the port speed from Hardware or from 
 *        the stored value xpSerdes / xpExtPhyPort class 
 * \param [in] devId device Id. Valid values are 0-63
 * \param [in] portNum Port number. Valid values are 0 to 127.
 * \param [out] speed - Enum specifies the port speed 
 *
 * \return XP_STATUS
 */
XP_STATUS xpsMacGetPortSpeed(xpsDevice_t devId, uint8_t portNum, xpSpeed *speed);

/**
 * \brief Get the Auto-negotiation status for a specific port 
 *
 * \param [in] devId device Id. Valid values are 0-63
 * \param [in] portNum Port number. Valid values are 0 to 127.
 * \param [out] status - Enum specifies the the status of auto-negotiation setting 
 *
 * \return XP_STATUS
 */
XP_STATUS xpsMacGetPortAutoNegEnable(xpsDevice_t devId, uint8_t portNum, uint8_t *status);

/**
 * \brief DeRegister event handler for ports 
 * previously registered with eventHandlerDeRegister
 *
 * \param [in] devId device Id. Valid values are 0-63
 * \param [in] portNum Port number. Valid values are 0 to 127.
 * \param [in] eventType event type to be de-registered
 *
 * \return XP_STATUS
 */
XP_STATUS xpsMacEventHandlerDeRegister(xpsDevice_t devId, uint8_t portNum, xpEventType eventType);
/**
 * \brief Register event handler for ports
 *
 * \param [in] devId device Id. Valid values are 0-63
 * \param [in] portNum Port number. Valid values are 0 to 127.
 * \param [in] eventType event type to be registered
 * \param [in] eventHandler event type to be registered
 *
 * \return XP_STATUS
 */
XP_STATUS xpsMacEventHandlerRegister(xpsDevice_t devId, uint8_t portNum, xpEventType eventType, xpEventHandler eventHandler);

/**
 * \brief Get MAC MIB counters statistics in xpStatistics structure for a specific port
 *
 * \param [in] devId device Id. Valid values are 0-63
 * \param [in] portNum Port number. Valid values are 0 to 127.
 * \param [in] fromStatNum Start collecting statistics from this number. Valid values are 0-31
 * \param [in] toStatNum Collect statistics till this number. Valid values are 0-31
 * \param [out] stat a structure which holds MAC MIB statistics of a port. Only statistics of counters which falls in range of fromStatNum to toStatNum will be updated.
 *
 * \return XP_STATUS
 */
XP_STATUS xpsMacGetCounterStats(xpsDevice_t devId, uint8_t portNum, uint8_t fromStatNum, uint8_t toStatNum, xp_Statistics *stat);

/**
 * \brief Get the link status for a specific port
 *
 * \param [in] devId device Id. Valid values are 0-63
 * \param [in] portNum Port number. Valid values are 0 to 127.
 * \param [out] status Link status
 *
 * \return XP_STATUS
 */
XP_STATUS xpsMacGetLinkStatus(xpsDevice_t devId, uint8_t portNum, uint8_t *status);

/**
 * \brief Get the MRU acceptance frame length configuration for a specific port
 *
 * \param [in] devId device Id. Valid values are 0-63
 * \param [in] portNum Port number. Valid values are 0 to 127.
 * \param [out] frameSize MRU acceptance frame length configuration for a specific port
 *
 * \return XP_STATUS
 */

XP_STATUS xpsMacGetRxMaxFrmLen(xpsDevice_t devId, uint8_t portNum, uint16_t *frameSize);

#ifdef __cplusplus
}
#endif

#endif // _openXpsMac_h_

