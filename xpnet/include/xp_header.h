/************************************************************************
* Copyright (C) 2016, Cavium, Inc.
* All Rights Reserved.
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; version 2
* of the License.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* A copy of the GNU General Public License, version 2 is available in the file 
* named LICENSE-GPLv2.md either in this directory or its root. 
* Alernatively to obtain a copy, write to the Free Software Foundation, Inc., 
* 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*
* File: xp_header.h
* 
* Abstract: This file contains the XPH header definition required for XPliant.
************************************************************************/
#ifndef _XP_HEADER_H
#define _XP_HEADER_H

#include <linux/types.h>

/**
 * \struct _xphRxHdrMetaData__
 * \brief  Rx metadata in Rx header. Max size is 64 bits.
 */
typedef struct __attribute__((__packed__)) _xphRxHdrMetaData__
{
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    uint8_t rsvd0                   :8;
    uint8_t rsvd2[1];
    uint8_t bdId[2];                   /* bdId[0]      bdId[1]
                                            MSB          LSB   */
    uint8_t rsvd3[4];
#else
    uint8_t rsvd1                   :8;
    uint8_t rsvd2[1];
    uint8_t bdId[2];                   /* bdId[0]      bdId[1]
                                            MSB          LSB   */
    uint8_t rsvd3[4];
#endif
} xphRxHdrMetaData;

/**
 * \struct _xphTxHdr__
 * \brief Tx Header, to be prefixed while Packet Tx when encap 
 *        is enabled.
 */
typedef struct  __attribute__((__packed__)) _xphTxHdr__
{
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__

    uint8_t reserved0[8];                /* reserved0[0]   reserved0[7]
                                          MSB                   LSB          */
    uint64_t reserved1              :8;

    uint64_t reserved2              :8;

    uint64_t reserved3              :8;

    uint64_t ingressVifLsbByte2     :8;  /* MSB of ingressVif                */
    uint64_t ingressVifLsbByte1     :8;  /* Intermidiate byte of ingressVif  */

    uint64_t egressVifLsbByte2      :4;  /* MSN of egressVif                 */
    uint64_t ingressVifLsbByte0     :4;  /* LSN of ingressVif                */

    uint64_t egressVifLsbByte1      :8;  /* Intermidiate byte of egressVif   */
    uint64_t egressVifLsbByte0      :8;  /* LSB of egressVif                 */

    uint64_t nextEngine             :8;
    uint64_t metadata               :56;
#else
    uint8_t reserved0[8];               /* reserved0[0]   reserved0[7]
                                          MSB                   LSB          */
    uint64_t reserved1              :8;

    uint64_t reserved2              :8;

    uint64_t reserved3              :8;

    uint64_t ingressVifLsbByte2     :8;  /* MSB of ingressVif                */
    uint64_t ingressVifLsbByte1     :8;  /* Intermidiate byte of ingressVif  */

    uint64_t ingressVifLsbByte0     :4;  /* LSN of ingressVif                */
    uint64_t egressVifLsbByte2      :4;  /* MSN of egressVif                 */

    uint64_t egressVifLsbByte1      :8;  /* Intermidiate byte of egressVif   */
    uint64_t egressVifLsbByte0      :8;  /* LSB of egressVif                 */

    uint64_t nextEngine             :8;
    uint64_t metadata               :56;
#endif
} xphTxHdr;

/**
 * \struct _xphRxHdr__
 * \brief Rx Header, to be parsed while Packet Rx.
 */
typedef struct  __attribute__((__packed__)) _xphRxHdr__
{
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    uint8_t reserved0[8];               /* reserved0[0]   reserved0[7]
                                          MSB                   LSB          */
    uint64_t reserved1              :8;

    uint64_t ingressPortNum         :8;
    uint64_t ingressVifLsbByte2     :8;  /* MSB of ingressVif                */
    uint64_t ingressVifLsbByte1     :8;  /* Intermidiate byte of ingressVif  */

    uint64_t r0                     :4;
    uint64_t ingressVifLsbByte0     :4;  /* LSN of ingressVif                */

    uint64_t reasonCodeMSB          :8;  /* MSB of reasonCode                */
    uint64_t r2                     :3;
    uint64_t reserved3              :1;
    uint64_t r1                     :2;
    uint64_t reasonCodeLsbs         :2;  /* Lsbs of reasonCode               */

    uint64_t r3                     :8;

    xphRxHdrMetaData metadata;
#else
    uint8_t reserved0[8];               /* reserved0[0]   reserved0[7]
                                          MSB                   LSB          */
    uint64_t reserved1              :8;

    uint64_t ingressPortNum         :8;
    uint64_t ingressVifLsbByte2     :8;  /* MSB of ingressVif                */
    uint64_t ingressVifLsbByte1     :8;  /* Intermidiate byte of ingressVif  */

    uint64_t ingressVifLsbByte0     :4;  /* LSN of ingressVif                */
    uint64_t r0                     :4;

    uint64_t reasonCodeMSB          :8;  /* MSB of reasonCode                */
    uint64_t reasonCodeLsbs         :2;  /* Lsbs of reasonCode               */
    uint64_t r1                     :2;
    uint64_t reserved3              :1;
    uint64_t r2                     :3;

    uint64_t r3                     :8;

    xphRxHdrMetaData metadata;
#endif
} xphRxHdr;

#endif /* _XP_HEADER_H */

