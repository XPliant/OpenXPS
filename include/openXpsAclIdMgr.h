// openXpsAclIdMgr.h

/************************************************************************/
/*  Copyright (c) [2011-2015] Cavium, Inc. All rights reserved.                                      */
/*  Unpublished - rights reserved under the Copyright Laws of the       */
/*  United States.  Use, duplication, or disclosure by the              */
/*  Government is subject to restrictions as set forth in               */
/*  subparagraph (c)(1)(ii) of the Rights in Technical Data and         */
/*  Computer Software clause at 252.227-7013.                           */
/************************************************************************/

/**
 * \file openXpsAclIdMgr.h
 * \brief This file contains API prototypes and type definitions
 *        for the ACL ID allocator
 *
 */

#ifndef _openXpsAclIdMgr_h_
#define _openXpsAclIdMgr_h_

#include "openXpsTypes.h"
#include "openXpsEnums.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
* Some variables explanation:
*     AclId - Access Control List Id
*/

/**
 * \brief API to allocate AclId
 *
 * \param [in] devId
 * \param [out] aclId
 *
 * \return XP_STATUS
 */
XP_STATUS xpsAclIdAllocEntry(xpsDevice_t devId, uint8_t* aclId);

/**
 * \brief API to free AclId
 *
 * \param [in] devId
 * \param [in] aclId
 *
 * \return XP_STATUS
 */
XP_STATUS xpsAclIdFreeEntry(xpsDevice_t devId, uint8_t aclId);


#ifdef __cplusplus
}
#endif

#endif // _openXpsAclIdMgr_h_
