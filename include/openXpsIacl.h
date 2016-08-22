//
// openXpsIacl.h
//
/************************************************************************/
/*  Copyright (c) [2011-2015] Cavium, Inc. All rights reservered. Confidential.                         */
/*  Unpublished - rights reserved under the Copyright Laws of the       */
/*  United States.  Use, duplication, or disclosure by the              */
/*  Government is subject to restrictions as set forth in               */
/*  subparagraph (c)(1)(ii) of the Rights in Technical Data and         */
/*  Computer Software clause at 252.227-7013.                           */
/************************************************************************/
/**
 * \file openXpsIacl.h
 * \brief This file contains API prototypes and type definitions
 *        for the XPS Ingress ACL Manager
 */


#ifndef _OPENXPSIACL_H_
#define _OPENXPSIACL_H_

#include "openXpsEnums.h"
#include "openXpsTypes.h"
#ifdef __cplusplus
extern "C" {
#endif


typedef xpIaclData_t xpsIaclData_t;
typedef xpIaclKeyType xpsIaclKeyType_e;
typedef xpIaclkeyFieldList_t xpsIaclkeyFieldList_t;
typedef xpIaclKeyFieldId_t xpsIaclKeyFieldId_t;
#define XP_MAX_NUM_FLDS 18



/**
 * \brief This will allow user to create tables based on user defined table 
 *        profile. Profile will have info about acl type, keySize and number 
 *        of Db's required corresponding to a acl type (XP_IACL0, XP_IACL1 and XP_IACL2)
 *        In table profile user can mention number of tables he wants to create. Number 
 *        of ACL entries for a Specific ACL type in the TCAM will be determined by 
 *        number of Dbs  and the keySize assigned for that ACL type. 
 *        
 * \param [in] devId
 * \param [in] tableProfile Its structure with info about number of tcam dbs and keysize
 *                          for each type of ingress ACL tables(XP_IACL0, XP_IACL1 and 
 *                          XP_IACL2).Key Size possible values are(WIDTH_64,WIDTH_128,
 *                          WIDTH_192,WIDTH_384)
 *
 * \return XP_STATUS
*/
XP_STATUS xpsIaclCreateTable(xpsDevice_t devId, xpIaclTableProfile_t tableProfile);

/**
 * \brief This will allow the user to delete tables created previously based on the table
 *        profile specific to a acl type.It will free the db's allocated for the aclType.
 *
 *
 * \param [in] devId
 * \param [in] iaclType  It can be of port,bridge or router aclType.
 *                       Possible Values(XP_IACL0, XP_IACL1 and XP_IACL2).
 *
 * \return XP_STATUS 
*/
XP_STATUS xpsIaclDeleteTable(xpsDevice_t devId, xpIaclType_e iaclType);

/**
 * @brief this method provides user a flexibility to define port Iacl keyformat, wherein a definitive set of fields
 *        can be selected by user to be part of the key .This also enables the SW to determine the keySize based on 
 *        user defined format.Based on the keySize number of possible ACL entries in the TCAM dB can also be determined.
 *        The user defined key format size should always be equal or less than the keySize provided for port aclType
 *        during xpsIaclCreateTable API call.Note:-There can be only one key format defined for each type
 *        of ACL type.
 *
 * \param [in] devId
 * \param [in] keyType Possible values are XP_IACL_V4_TYPE or XP_IACL_V4_TYPE.
 * \param [in] *fldData Its pointer to a ACL key structure which consists information about key 
 *             Fields list,keyType,validity bit(0-valid,1-invalid) and number of fields in the key.The key field List 
 *             consists of various flds which is to be filled based on the keyType.Possible values for flds are values
 *             in the following enums xpIaclV4KeyFlds and xpIaclV6KeyFlds.
 *
 * \return XP_STATUS
*/

XP_STATUS xpsIaclDefinePaclKey(xpsDevice_t devId, xpsIaclKeyType_e keyType, xpsIaclkeyFieldList_t *fldData);

/**
 * \brief this method provides user a flexibility to define bridge Iacl keyformat, wherein a definitive set of fields
 *        can be selected by user to be part of the key .This also enables the SW to determine the keySize based on
 *        user defined format.Based on the keySize number of possible ACL entries in the TCAM dB can also be determined.
 *        The user defined key format size should always be equal or less than the keySize provided for bridge aclType
 *        during xpsIaclCreateTable API call.Note:-There can be only one key format defined for each type
 *        of ACL type.
 *
 * \param [in] xpsDevice_t devId
 * \param [in] keyType Possible values are XP_IACL_V4_TYPE or XP_IACL_V4_TYPE.
 * \param [in] *fldData Its pointer to a IACL key structure which consists information about key
 *             Fields list,keyType,validity bit(0-valid,1-invalid) and number of fields in the key.The key field List
 *             consists of various flds which is to be filled based on the keyType.Possible values for flds are values
 *             in the following enums xpIaclV4KeyFlds and xpIaclV6KeyFlds.
 *
 * \return XP_STATUS
*/

XP_STATUS xpsIaclDefineBaclKey(xpsDevice_t devId, xpsIaclKeyType_e keyType, xpsIaclkeyFieldList_t *fldData);

/**
 * \brief this method provides user a flexibility to define router Iacl keyformat, wherein a definitive set of fields
 *        can be selected by user to be part of the key .This also enables the SW to determine the keySize based on
 *        user defined format.Based on the keySize number of possible ACL entries in the TCAM dB can also be determined.
 *        The user defined key format size should always be equal or less than the keySize provided for router aclType
 *        during xpsIaclCreateTable API call.Note:-There can be only one key format defined for each type
 *        of ACL type.
 *
 * \param [in] devId
 * \param [in] keyType Possible values are XP_IACL_V4_TYPE or XP_IACL_V4_TYPE.
 * \param [in] *fldData Its pointer to a IACL key structure which consists information about key
 *             Fields list,keyType,validity bit(0-valid,1-invalid) and number of fields in the key.The key field List
 *             consists of various flds which is to be filled based on the keyType.Possible values for flds are values
 *             in the following enums xpIaclV4KeyFlds and xpIaclV6KeyFlds.
 *
 * \return XP_STATUS
*/
XP_STATUS xpsIaclDefineRaclKey(xpsDevice_t devId, xpsIaclKeyType_e keyType, xpsIaclkeyFieldList_t *fldData);

//Writing the IACL Key:
/**
 * \brief write Port IACL key(rule data)in the TCAM at user provided camIndex.User must have defined the key format once
 *        prior, using API xpsIaclDefinePaclKey.
 *
 *
 * \param [in] devId
 * \param [in] camIndex Its index at which the IACL key is written on the HW.User should manage cam index.
 * \param [in] *fldData Its pointer to a IACL key structure which consists information about key
 *             Fields list,keyType,validity bit(0-valid,1-invalid) and number of fields in the key.The key field List
 *             consists of various flds which is to be filled based on the keyType.Possible values for flds are values
 *             in the following enums xpIaclV4KeyFlds and xpIaclV6KeyFlds.Along with each key fields the value and mask
 *             should be provided which is to be programed on the TCAM.

 *
 * \return XP_STATUS
*/
XP_STATUS xpsIaclWritePaclKey(xpsDevice_t devId, uint32_t camIndex, xpsIaclkeyFieldList_t *fldData);

/**
 * \brief write Bridge IACL key(rule data)in the TCAM at user provided camIndex.User must have defined key format once
 *        prior, using API xpsIaclDefineBaclKey.
 *
 *
 * \param [in] devId
 * \param [in] camIndex Its index at which the IACL key is written on the HW.User should manage cam index.
 * \param [in] *fldData Its pointer to a IACL key structure which consists information about key
 *             Fields list,keyType,validity bit(0-valid,1-invalid) and number of fields in the key.The key field List
 *             consists of various flds which is to be filled based on the keyType.Possible values for flds are values
 *             in the following enums xpIaclV4KeyFlds and xpIaclV6KeyFlds.Along with each key fields the value and mask
 *             should be provided which is to be programed on the TCAM.
 *
 * \return XP_STATUS
*/
XP_STATUS xpsIaclWriteBaclKey(xpsDevice_t devId, uint32_t camIndex, xpsIaclkeyFieldList_t *fldData);

/**
 * \brief write RouterInf IACL key(rule data)inthe TCAM at user provided camIndex.User must have defined key format once
 *        prior, using API xpsIaclDefineRaclKey.
 *
 *
 * \param [in] devId
 * \param [in] camIndex Its index at which the IACL key is written on the HW.User should manage cam index.
 * \param [in] *fldData Its pointer to a IACL key structure which consists information about key
 *             Fields list,keyType,validity bit(0-valid,1-invalid) and number of fields in the key.The key field List
 *             consists of various flds which is to be filled based on the keyType.Possible values for flds are values
 *             in the following enums xpIaclV4KeyFlds and xpIaclV6KeyFlds.Along with each key fields the value and mask
 *             should be provided which is to be programed on the TCAM.
 *
 * \return XP_STATUS
*/
XP_STATUS xpsIaclWriteRaclKey(xpsDevice_t devId, uint32_t camIndex, xpsIaclkeyFieldList_t *fldData);


//Writing the IACL Data:
/**
 * \brief Write the IACL action data for port
 *
 *
 * \param [in] devId
 * \param [in] camIndex Its index at which the IACL action data is written on the HW.User should manage cam index.
 * \param [in] *iaclData Its pointer to the action data specific to the IACL entry.
 *
 * \return XP_STATUS
*/
XP_STATUS xpsIaclWritePaclData(xpsDevice_t devId, uint32_t camIndex, xpsIaclData_t *iaclData);

/**
 * \brief Write the IACL action Data for bridge Interface
 *
 *
 * \param [in] devId
 * \param [in] camIndex Its index at which the IACL action data is written on the HW.User should manage cam index.
 * \param [in] *iaclData Its pointer to the action data specific to the IACL entry.
 *
 * \return XP_STATUS
*/
XP_STATUS xpsIaclWriteBaclData(xpsDevice_t devId, uint32_t camIndex, xpsIaclData_t *iaclData);

/**
 * \brief Write the IACL action Data for router interface
 *
 *
 * \param [in] devId
 * \param [in] camIndex Its index at which the IACL action data is written on the HW.User should manage cam index.
 * \param [in] *iaclData Its pointer to the action data specific to the IACL entry.
 *
 * \return XP_STATUS
*/
XP_STATUS xpsIaclWriteRaclData(xpsDevice_t devId, uint32_t camIndex, xpsIaclData_t *iaclData);

//Writing IACL Entry (Key + Data)
/**
 * \brief This API enables user to write both key portion and action data
 *        portion of the IACL entry for the port on the HW. 
 *
 *
 * \param [in] devId
 * \param [in] camIndex Its index at which the IACL entry is written on the Hw 
 * \param [in] *fldData Its pointer to a IACL key structure which consists information about key
 *             Fields list,keyType,validity bit(0-valid,1-invalid) and number of fields in the key.The key field List
 *             consists of various flds which is to be filled based on the keyType.Possible values for flds are values
 *             in the following enums xpIaclV4KeyFlds and xpIaclV6KeyFlds.Along with each key fields the value and mask
 *             should be provided which is to be programed on the TCAM.
 * \param [in] *iaclData Its pointer to the action data specific to the IACL entry.
 *
 * \return XP_STATUS
*/
XP_STATUS xpsIaclWritePaclEntry(xpsDevice_t devId, uint32_t camIndex, xpsIaclkeyFieldList_t *fldData, xpsIaclData_t *iaclData);

/**
 * \brief This API enables user to write both key portion and action data
 *        portion of the IACL entry for the vlan interface on the HW.
 *
 *
 * \param [in] devId
 * \param [in] camIndex Its index at which the IACL entry is written on the HW
 * \param [in] *fldData Its pointer to a IACL key structure which consists information about key
 *             Fields list,keyType,validity bit(0-valid,1-invalid) and number of fields in the key.The key field List
 *             consists of various flds which is to be filled based on the keyType.Possible values for flds are values
 *             in the following enums xpIaclV4KeyFlds and xpIaclV6KeyFlds.Along with each key fields the value and mask
 *             should be provided which is to be programed on the TCAM.
 * \param [in] *iaclData Its pointer to the action data specific to the IACL entry.
 *
 * \return XP_STATUS
*/
XP_STATUS xpsIaclWriteBaclEntry(xpsDevice_t devId, uint32_t camIndex, xpsIaclkeyFieldList_t *fldData, xpsIaclData_t *iaclData);

/**
 * \brief This API enables user to write both key portion and action data
 *        portion of the IACL entry for the router interface on the HW.
 *
 *
 * \param [in] devId
 * \param [in] camIndex Its index at which the IACL entry is written on the HW
 * \param [in] *fldData Its pointer to a IACL key structure which consists information about key
 *             Fields list,keyType,validity bit(0-valid,1-invalid) and number of fields in the key.The key field List
 *             consists of various flds which is to be filled based on the keyType.Possible values for flds are values
 *             in the following enums xpIaclV4KeyFlds and xpIaclV6KeyFlds.Along with each key fields the value and mask
 *             should be provided which is to be programed on the TCAM.
 * \param [in] *iaclData Its pointer to the action data specific to the IACL entry.
 *
 * \return XP_STATUS
*/
XP_STATUS xpsIaclWriteRaclEntry(xpsDevice_t devId, uint32_t camIndex, xpsIaclkeyFieldList_t *fldData, xpsIaclData_t *iaclData);



#ifdef __cplusplus
}
#endif

#endif // _OPENXPSIACL_H_
