// openXpsTcamMgr.h

/************************************************************************/
/*  Copyright (c) [2011-2015] Cavium, Inc. All rights reserved.                                      */
/*  Unpublished - rights reserved under the Copyright Laws of the       */
/*  United States.  Use, duplication, or disclosure by the              */
/*  Government is subject to restrictions as set forth in               */
/*  subparagraph (c)(1)(ii) of the Rights in Technical Data and         */
/*  Computer Software clause at 252.227-7013.                           */
/************************************************************************/

/**
 * \file openXpsTcamMgr.h
 * \brief This file contains API prototypes and type definitions
 *        for the XPS TCAM Manager
 *
 */

#ifndef _openXpsTcamMgr_h_
#define _openXpsTcamMgr_h_

#include "openXpsTypes.h"
#include "openXpsEnums.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
* Some variables explanation:
*     tableId - HW table ID
*     entryId - ID that corresponds to TCAM hit index, but never changes. Application has to remember it.
*     tcamId  - HW hit index. Should be used for HW access.
*/

/**
* \brief Rule move function moves table entry from src to dst possition.
*        Function is provided by TCAM Mgr user application.
*        Source and destination indexes are HW indexes.
*/
typedef XP_STATUS (*xpsTcamMgrRuleMove_fn)(xpsDevice_t devId, uint32_t tableId, uint32_t dst, uint32_t src);

/**
 * \typedef _xpsTcamMgrShufflingList_e
 * \brief List of available shuffling algorithms.
 */
typedef enum _xpsTcamMgrShufflingList_e {
    XPS_TCAM_ACL_ALGORITHM = 0,
    XPS_TCAM_LIST_ALGORITHM,
    XPS_SHUFFLING_ALGORITHM_NUM
} xpsTcamMgrShufflingList_e;

/*
 * State Management APIs
 */

/**
 * \brief API to initialize the TCAM Manager
 *
 * This will allocate memory for global and per device data
 *
 * \param [in] scopeId
 *
 * \return XP_STATUS
 */
XP_STATUS xpsTcamMgrInit(xpsScope_t scopeId);

/**
 * \brief API to de-initialize the TCAM Manager
 *
 * This will de-allocate memory for global and per device data
 *
 * \param [in] scopeId
 *
 * \return XP_STATUS
 */
XP_STATUS xpsTcamMgrDeInit(xpsScope_t scopeId);

/**
 * \brief API to Add Device Information to the TCAM Manager
 *
 * This API is a NOP. We need a more explicit Add Device for Per
 * Device Databases
 *
 * \param [in] devId
 *
 * \return XP_STATUS
 */
XP_STATUS xpsTcamMgrAddDevice(xpsDevice_t devId);

/**
 * \brief API to Remove Device Information from the TCAM
 *        Manager
 *
 * This API is a NOP. We need a more explicit Remove Device for
 * Per Device Databases
 *
 * \param [in] devId
 *
 * \return XP_STATUS
 */
XP_STATUS xpsTcamMgrRemoveDevice(xpsDevice_t devId);

/**
 * \brief API to start TCAM table management
 *
 * \param [in] devId
 * \param [in] tableId
 * \param [in] shufflingAlg
 *
 * \return XP_STATUS
 */
XP_STATUS xpsTcamMgrAddTable(xpsDevice_t devId, uint32_t tableId, xpsTcamMgrShufflingList_e shufflingAlg);

/**
 * \brief API to configure TCAM table management
 *
 * \param [in] devId
 * \param [in] tableId
 * \param [in] ruleMove
 * \param [in] tableEntryNum
 * \param [in] entryPrioNum
 *
 * \return XP_STATUS
 */
XP_STATUS xpsTcamMgrConfigTable(xpsDevice_t devId, uint32_t tableId, 
        xpsTcamMgrRuleMove_fn ruleMove, uint32_t tableEntryNum,
        uint32_t entryPrioNum);

/**
 * \brief API to stop TCAM table management
 *
 * \param [in] devId
 * \param [in] tableId
 *
 * \return XP_STATUS
 */
XP_STATUS xpsTcamMgrRemoveTable(xpsDevice_t devId, uint32_t tableId);

/**
 * \brief API to add TCAM entry into TCAM Manager
 *
 * \param [in] devId
 * \param [in] tableId
 * \param [in] priority
 * \param [out] entryId
 *
 * \return XP_STATUS
 */
XP_STATUS xpsTcamMgrAllocEntry(xpsDevice_t devId, uint32_t tableId, uint32_t priority, uint32_t* entryId);

/**
 * \brief API to delete TCAM entry from TCAM Manager
 *
 * \param [in] devId
 * \param [in] tableId
 * \param [in] entryId
 *
 * \return XP_STATUS
 */
XP_STATUS xpsTcamMgrFreeEntry(xpsDevice_t devId, uint32_t tableId, uint32_t entryId);

/**
 * \brief API to deliver entry ID from TCAM ID
 *
 * \param [in] devId
 * \param [in] tableId
 * \param [in] tcamId
 * \param [out] entryId
 *
 * \return XP_STATUS
 */
XP_STATUS xpsTcamMgrEntryFromTcamIdGet(xpsDevice_t devId, uint32_t tableId, uint32_t tcamId, uint32_t* entryId);

/**
 * \brief API to deliver TCAM ID from entry ID
 *
 * \param [in] devId
 * \param [in] tableId
 * \param [in] entryId
 * \param [out] tcamId
 *
 * \return XP_STATUS
 */
XP_STATUS xpsTcamMgrTcamIdFromEntryGet(xpsDevice_t devId, uint32_t tableId, uint32_t entryId, uint32_t* tcamId);

/**
 * \brief Debug API to simplify programmers life
 *
 * \param [in] devId
 * \param [in] tableId
 *
 * \return XP_STATUS
 */
XP_STATUS xpsTcamMgrDebug(xpsDevice_t devId, uint32_t tableId);

/**
 * \brief API to move the HW tcam values from one entry(src) to other entry(dst)
 *
 * \param [in] devId
 * \param [in] tableId
 * \param [in] dst
 * \param [in] src
 *
 * \return XP_STATUS
 */
XP_STATUS xpsTcamMgrRuleMoveAcl(xpsDevice_t devId, uint32_t tableId, uint32_t dst, uint32_t src);

/**
 * \brief API to allocate TCAM ID
 *
 * \param [in] devId
 * \param [in] tableId
 * \param [out] tcamId
 *
 * \return XP_STATUS
 */
XP_STATUS xpsTcamIdAllocEntry(xpsDevice_t devId, uint32_t tableId, uint32_t* tcamId);

/**
 * \brief API to free TCAM ID
 *
 * \param [in] devId
 * \param [in] tableId
 * \param [in] tcamId
 *
 * \return XP_STATUS
 */
XP_STATUS xpsTcamIdFreeEntry(xpsDevice_t devId, uint32_t tableId, uint32_t tcamId);
#ifdef __cplusplus
}
#endif

#endif // _openXpsTcamMgr_h_
