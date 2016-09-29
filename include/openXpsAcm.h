//
//  openXpsAcm.h
//
/************************************************************************/
/*  Copyright (c) [2011-2015] Cavium, Inc. All rights reserved.                                      */
/*  Unpublished - rights reserved under the Copyright Laws of the       */
/*  United States.  Use, duplication, or disclosure by the              */
/*  Government is subject to restrictions as set forth in               */
/*  subparagraph (c)(1)(ii) of the Rights in Technical Data and         */
/*  Computer Software clause at 252.227-7013.                           */
/************************************************************************/
/**
 * \file openXpsAcm.h
 * \brief This file contains API prototypes and type definitions
 *        for the XPS ACM Manager
 */


#ifndef _openXpsAcm_h_
#define _openXpsAcm_h_

#include "openXpsTypes.h"
#include "openXpsEnums.h"

#ifdef __cplusplus
extern "C" {
#endif

 /**
 * \public
 * \brief Updates a field in the entry.
 *
 * This gets the counter value from the specified index in the
 * device
 *
 * \param [in] devId 
 * \param [in] countIndex
 * \param [out] * nPkts 
 * \param [out] * nBytes 
 *
 * \return XP_STATUS
 */
 XP_STATUS xpsAcmGetCounterValue(xpsDevice_t devId, xpAcmClient_e client, uint32_t countIndex, uint64_t *nPkts, uint64_t *nBytes);

#ifdef __cplusplus
}
#endif

#endif  //_xpsAcm_h_
                        

