/************************************************************************/
/*  Copyright (c) [2016] Cavium, Inc. All rights reserved.              */
/*  Unpublished - rights reserved under the Copyright Laws of the       */
/*  United States.  Use, duplication, or disclosure by the              */
/*  Government is subject to restrictions as set forth in               */
/*  subparagraph (c)(1)(ii) of the Rights in Technical Data and         */
/*  Computer Software clause at 252.227-7013.                           */
/************************************************************************/
/*
 * This software is licensed to you under the terms of the GNU General Public
 * License version 2 (the "GPL"). 
 * TBD: need to update the GPL banner from Cavium Legal .
 */


#ifndef _XP_HEADER_H
#define _XP_HEADER_H

#include <linux/types.h>

/**
 * \struct _xphTxHdrMetaData__
 * \brief Tx metadata in Tx header. Max size is 56 bits.
 */
typedef struct __attribute__((__packed__)) _xphTxHdrMetaData__
{
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    uint8_t reserved[2];           /* Reserved                          */
    uint8_t bridgeDomain[2];       /* bridgeDomain[0]      bridgeDomain[1]
                                         MSB                  LSB       */
    uint32_t portAclId              :8;

    uint32_t portState              :2;
    uint32_t setEgressPortFilter    :1;
    uint32_t macSAmissCmd           :2;
    uint32_t bypassBridgeRouter     :1;
    uint32_t portACLEn              :1;
    uint32_t portDebugEn            :1;

    uint32_t setIngressVif          :1;
    uint32_t setBridgeDomain        :1;
    uint32_t acceptedFrameType      :2;
    uint32_t bypassTunnelVif        :1;
    uint32_t bypassACLsPBR          :1;
    uint32_t samplerEn              :1;
    uint32_t policerEn              :1;
#else    
    uint8_t reserved[2];           /* Reserved                          */
    uint8_t bridgeDomain[2];       /* bridgeDomain[0]      bridgeDomain[1]
                                         MSB                  LSB       */
    uint32_t portAclId              :8;   

    uint32_t portDebugEn            :1;
    uint32_t portACLEn              :1;
    uint32_t bypassBridgeRouter     :1;
    uint32_t macSAmissCmd           :2;
    uint32_t setEgressPortFilter    :1;
    uint32_t portState              :2;

    uint32_t policerEn              :1;
    uint32_t samplerEn              :1;
    uint32_t bypassACLsPBR          :1;
    uint32_t bypassTunnelVif        :1;
    uint32_t acceptedFrameType      :2;
    uint32_t setBridgeDomain        :1;
    uint32_t setIngressVif          :1;
#endif
} xphTxHdrMetaData;

/**
 * \struct _xphRxHdrMetaData__
 * \brief  Rx metadata in Rx header. Max size is 64 bits.
 */
typedef struct __attribute__((__packed__)) _xphRxHdrMetaData__
{
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    uint8_t rsvd0                   :6;
    uint8_t tunnelTerminate         :1;
    uint8_t rsvd1                   :1;
    uint8_t rsvd2[1];
    uint8_t bdId[2];                   /* bdId[0]      bdId[1]
                                            MSB          LSB   */
    uint8_t rsvd3[4];
#else
    uint8_t rsvd1                   :1;
    uint8_t tunnelTerminate         :1;
    uint8_t rsvd0                   :6;
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

    uint8_t pktRecvTimeStamp[8];   /* pktRecvTimeStamp[0]   pktRecvTimeStamp[7]
                                          MSB                   LSB          */
    uint64_t headerSize             :4;
    uint64_t headerType             :4;

    uint64_t DP                     :2;
    uint64_t TC                     :4;
    uint64_t useXPHTimeStamp        :1;
    uint64_t r0                     :1;

    uint64_t r1                     :5;
    uint64_t txSampleID             :2;
    uint64_t txSample               :1;

    uint64_t ingressVifLsbByte2     :8;  /* MSB of ingressVif                */
    uint64_t ingressVifLsbByte1     :8;  /* Intermidiate byte of ingressVif  */

    uint64_t egressVifLsbByte2      :4;  /* MSN of egressVif                 */
    uint64_t ingressVifLsbByte0     :4;  /* LSN of ingressVif                */

    uint64_t egressVifLsbByte1      :8;  /* Intermidiate byte of egressVif   */
    uint64_t egressVifLsbByte0      :8;  /* LSB of egressVif                 */

    uint64_t nextEngine             :8;
    xphTxHdrMetaData  metadata;
#else
    uint8_t pktRecvTimeStamp[8];   /* pktRecvTimeStamp[0]   pktRecvTimeStamp[7]
                                          MSB                   LSB          */
    uint64_t headerType             :4;
    uint64_t headerSize             :4;

    uint64_t r0                     :1;
    uint64_t useXPHTimeStamp        :1;
    uint64_t TC                     :4;
    uint64_t DP                     :2;

    uint64_t txSample               :1;
    uint64_t txSampleID             :2;
    uint64_t r1                     :5;

    uint64_t ingressVifLsbByte2     :8;  /* MSB of ingressVif                */
    uint64_t ingressVifLsbByte1     :8;  /* Intermidiate byte of ingressVif  */

    uint64_t ingressVifLsbByte0     :4;  /* LSN of ingressVif                */
    uint64_t egressVifLsbByte2      :4;  /* MSN of egressVif                 */

    uint64_t egressVifLsbByte1      :8;  /* Intermidiate byte of egressVif   */
    uint64_t egressVifLsbByte0      :8;  /* LSB of egressVif                 */

    uint64_t nextEngine             :8;
    xphTxHdrMetaData  metadata;
#endif
} xphTxHdr;

/**
 * \struct _xphRxHdr__
 * \brief Rx Header, to be parsed while Packet Rx.
 */
typedef struct  __attribute__((__packed__)) _xphRxHdr__
{
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    uint8_t pktRecvTimeStamp[8];   /* pktRecvTimeStamp[0]   pktRecvTimeStamp[7]
                                          MSB                   LSB          */
    uint64_t headerSize             :4;
    uint64_t headerType             :4;

    uint64_t ingressPortNum         :8;
    uint64_t ingressVifLsbByte2     :8;  /* MSB of ingressVif                */
    uint64_t ingressVifLsbByte1     :8;  /* Intermidiate byte of ingressVif  */

    uint64_t r0                     :4;
    uint64_t ingressVifLsbByte0     :4;  /* LSN of ingressVif                */

    uint64_t reasonCodeMSB          :8;  /* MSB of reasonCode                */
    uint64_t r2                     :3;
    uint64_t truncated              :1;
    uint64_t r1                     :2;
    uint64_t reasonCodeLsbs         :2;  /* Lsbs of reasonCode               */

    uint64_t r3                     :8;

    xphRxHdrMetaData metadata;
#else
    uint8_t pktRecvTimeStamp[8];   /* pktRecvTimeStamp[0]   pktRecvTimeStamp[7]
                                          MSB                   LSB          */
    uint64_t headerType             :4;
    uint64_t headerSize             :4;

    uint64_t ingressPortNum         :8;
    uint64_t ingressVifLsbByte2     :8;  /* MSB of ingressVif                */
    uint64_t ingressVifLsbByte1     :8;  /* Intermidiate byte of ingressVif  */

    uint64_t ingressVifLsbByte0     :4;  /* LSN of ingressVif                */
    uint64_t r0                     :4;

    uint64_t reasonCodeMSB          :8;  /* MSB of reasonCode                */
    uint64_t reasonCodeLsbs         :2;  /* Lsbs of reasonCode               */
    uint64_t r1                     :2;
    uint64_t truncated              :1;
    uint64_t r2                     :3;

    uint64_t r3                     :8;

    xphRxHdrMetaData metadata;
#endif
} xphRxHdr;

#endif /* _XP_HEADER_H */

