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
 *   FILE : openXpsQos.h
 * 
 *   Abstract: This file defines the apis for QoS Management in OpenXPS.
 ************************************************************************/
/**
 * \file openXpsQos.h
 * \brief This file contains API prototypes and type definitions
 *        for the openXps QoS Management
 * \copyright (c) 2016 Cavium Inc
 */

#ifndef _openXpsQos_h_
#define _openXpsQos_h_

#include "openXpsTypes.h"
#include "openXpsEnums.h"

#ifdef __cplusplus
extern "C"
{
#endif


/*
 * Q-Mapping Control APIs
 */

/**
 * \brief This API returns the qmap table index and the location
 *        of the queue within the entry for a given input set of
 *        data used for q mapping
 *
 * This API will take the following parameters to determine the
 * index of the q map table and the location of the queue within
 * the entry pointed to by the index of an absolute queue number
 * that corresponds to an input set
 *
 * This API can be used to remap an egress ports q's to an
 * arbitrary set of input data. For example we can remap a tc to
 * egress port mapping using this API and the ones listed below:
 *
 * \see xpsQosAqmGetQueueAbsoluteNumber
 * \see xpsQosAqmSetQueueAtQmapIndex
 *
 *
 * \param [in] devId Device id
 * \param [in] destPort Destination Port
 * \param [in] srcPort Source Port
 * \param [in] mc Multicast
 * \param [in] txqSelVec Txq select vector
 * \param [in] tc Traffic Class
 * \param [out] qmapIdx Qmap index
 * \param [out] queueLoc Queue location
 *
 * \return XP_STATUS
 */
XP_STATUS xpsQosAqmGetQmapTableIndex(xpsDevice_t devId, xpPort_t destPort, xpPort_t srcPort, uint32_t mc, uint32_t txqSelVec, uint32_t tc, uint32_t *qmapIdx, uint32_t *queueLoc);

/**
 * \brief This API will return the absolute queue number that
 *        corresponds to the destination port and queue
 *        combination
 *
 * This API is used to retrieve the absolute queue number that
 * corresponds to a port and queue combination
 *
 * The absolute queue number is used in many Traffic Manager
 * configurations including mapping of a port and traffic class
 * to egress queue
 *
 * If you wish to change this mapping of a traffic class to an
 * egress queue, you will need to find out the absolute queue
 * number for every port in the system for the queue you wish to
 * remap, then you will need to update the qmap table
 * accordingly using the indicies determined by tne expected
 * input set of bits
 *
 * \see xpwQosAqmGetQmapTableIndex
 * \see xpwQosAqmSetQueueAtQmapIndex
 *
 *
 * \param [in] devId Device id
 * \param [in] devPort Device Port
 * \param [in] queueNum Queue number
 * \param [out] absQNum Absolute queue number
 *
 * \return XP_STATUS
 */
XP_STATUS xpsQosAqmGetQueueAbsoluteNumber(xpsDevice_t devId, xpPort_t devPort, uint32_t queueNum, uint32_t *absQNum);

/**
 * \brief This API will update the qmap table with a new
 *        absolute queue number at the qmap index and location
 *        desired
 *
 * This API in conjuction with two others can be used to remap a
 * traffic class to an egress queue on a per port basis. This
 * API will need to be called for each Traffic Class that you
 * wish to remap for every egress port on the device
 *
 * \see xpQosAqmGetQmapTableIndex
 * \see xpQosAqmGetQueueAbsoluteNumber
 *
 *
 * \param [in] devId Device id
 * \param [in] qmapIdx Qmap index
 * \param [in] queueLoc Queue location
 * \param [in] absQNum Absolute queue number
 *
 * \return XP_STATUS
 */
XP_STATUS xpsQosAqmSetQueueAtQmapIndex(xpsDevice_t devId, uint32_t qmapIdx, uint32_t queueLoc, uint32_t absQNum);

/*
 * XPS Port Ingress Qos Map
 */

/**
 * \brief API to configure a traffic class for a given L2 QOS
 *        Profile
 *
 * This API will allow a user to set a Traffic Class for a
 * given L2 QOS Profile and combination of PCP and DEI values
 * (also known as COS)
 *
 * There are 8 QOS profiles for each type of QOS marking
 * (L2/L3/MPLS) supported by Xpliant. Each profile provides the
 * ability to configure PCP/DEI to traffic class mapping
 * independently
 *
 *
 * \param [in] devId Device id
 * \param [in] profile QOS Profile
 * \param [in] pcpVal PCP value
 * \param [in] deiVal DEI value
 * \param [in] tc Traffic Class
 *
 * \return XP_STATUS
 */
XP_STATUS xpsQosPortIngressSetTrafficClassForL2QosProfile(xpsDevice_t devId, xpQosMapPfl_t profile, uint32_t pcpVal, uint32_t deiVal, uint32_t tc);

/**
 * \brief API to configure a traffic class for a given L3 QOS
 *        Profile
 *
 * This API will allow a user to set a Traffic Class for a
 * given L3 QOS Profile and DSCP value
 *
 * There are 8 QOS profiles for each type of QOS marking
 * (L2/L3/MPLS) supported by Xpliant. Each profile provides the
 * ability to configure DSCP to traffic class mapping
 * independently
 *
 *
 * \param [in] devId Device id
 * \param [in] profile QOS Profile
 * \param [in] dscpVal DSCP value
 * \param [in] tc Traffic class
 *
 * \return XP_STATUS
 */
XP_STATUS xpsQosPortIngressSetTrafficClassForL3QosProfile(xpsDevice_t devId, xpQosMapPfl_t profile, uint32_t dscpVal, uint32_t tc);

/**
 * \brief API to retrieve the configured traffic class for a
 *        given L3 QOS Profile
 *
 * This API will allow a user to get a set Traffic Class for a
 * given L2 QOS Profile and DSCP value
 *
 * There are 8 QOS Profiles for each type of QOS marking
 * (L2/L3/MPLS) supported by Xpliant. Each profile provides the
 * ability to configure DSCP to traffic class mapping
 * independently
 *
 *
 * \param [in] devId Device id
 * \param [in] profile QOS Profile
 * \param [in] dscpVal DSCP value
 * \param [out] tc Traffic class
 *
 * \return XP_STATUS
 */
XP_STATUS xpsQosPortIngressGetTrafficClassForL3QosProfile(xpsDevice_t devId, xpQosMapPfl_t profile, uint32_t dscpVal, uint32_t *tc);

/**
 * \brief API to configure a drop precedence for a given L2 QOS
 *        Profile
 *
 * This API will allow a user to set a Drop Precedence for a
 * given L2 QOS Profile and combination of PCP and DEI values
 * (also known as COS)
 *
 * There are 8 QOS profiles for each type of QOS marking
 * (L2/L3/MPLS) supported by Xpliant. Each profile provides the
 * ability to configure PCP/DEI to drop precedence mapping
 * independently
 *
 *
 * \param [in] devId Device id
 * \param [in] profile QOS Profile
 * \param [in] pcpVal PCP value
 * \param [in] deiVal DEI value
 * \param [in] dp Drop Precedence
 *
 * \return XP_STATUS
 */
XP_STATUS xpsQosPortIngressSetDropPrecedenceForL2QosProfile(xpsDevice_t devId, xpQosMapPfl_t profile, uint32_t pcpVal, uint32_t deiVal, uint32_t dp);

/**
 * \brief API to configure a drop precedence for a given L3 QOS
 *        Profile
 *
 * This API will allow a user to set a Drop Precedence for a
 * given L3 QOS Profile and DSCP value
 *
 * There are 8 QOS profiles for each type of QOS marking
 * (L2/L3/MPLS) supported by Xpliant. Each profile provides the
 * ability to configure DSCP to drop precedence mapping
 * independently
 *
 *
 * \param [in] devId Device id
 * \param [in] profile QOS Profile
 * \param [in] dscpVal DSCP value
 * \param [in] dp Drop Precedence
 *
 * \return XP_STATUS
 */
XP_STATUS xpsQosPortIngressSetDropPrecedenceForL3QosProfile(xpsDevice_t devId, xpQosMapPfl_t profile, uint32_t dscpVal, uint32_t dp);

/**
 * \brief API to retrieve the configured drop precedence for a
 *        given L3 QOS Profile
 *
 * This API will allow a user to get a Drop Precedence for a
 * given L3 QOS Profile and DSCP value
 *
 * There are 8 QOS Profiles for each type of QOS marking
 * (L2/L3/MPLS) supported by Xpliant. Each profile provides the
 * ability to configure DSCP to drop precedence mapping
 * independently
 *
 *
 * \param [in] devId Device id
 * \param [in] profile QOS Profile
 * \param [in] dscpVal DSCP value
 * \param [out] dp Drop Precedence
 *
 * \return XP_STATUS
 */
XP_STATUS xpsQosPortIngressGetDropPrecedenceForL3QosProfile(xpsDevice_t devId, xpQosMapPfl_t profile, uint32_t dscpVal, uint32_t *dp);

/**
 * \brief API to remap QOS Priority values based on an ingress
 *        PCP/DEI received from a packet for a profile
 *
 * This API will allow a user to remap all QOS priority values
 * based off the values retrieved from a .1q tagged packet or
 * from the port default PCP/DEI values
 *
 * There are 8 QOS Profiles for each type of QOS marking
 * (L2/L3/MPLS) supported by Xpliant. Each profile provides the
 * ability to remap QOS priorities independently
 *
 *
 * \param [in] devId Device id
 * \param [in] profile QOS Profile
 * \param [in] pcpVal PCP value
 * \param [in] deiVal DEI value
 * \param [in] remapPcpVal Remap PCP value
 * \param [in] remapDeiVal Remap DEI value
 * \param [in] remapDscpVal Remap DSCP value
 * \param [in] remapExpVal Remap EXP value
 *
 * \return XP_STATUS
 */
XP_STATUS xpsQosPortIngressRemapPriorityForL2QosProfile(xpsDevice_t devId,        xpQosMapPfl_t profile,
                                                        uint32_t   pcpVal,       uint32_t      deiVal,
                                                        uint32_t   remapPcpVal,  uint32_t      remapDeiVal,
                                                        uint32_t   remapDscpVal, uint32_t      remapExpVal);

/**
 * \brief API to remap QOS Priority values based on an ingress
 *        DSCP received from a packet for a profile
 *
 * This API will allow a user to remap all QOS priority values
 * based off the value retrieved from an IP packet or from the
 * port default DSCP value
 *
 * There are 8 QOS Profiles for each type of QOS marking
 * (L2/L3/MPLS) supported by Xpliant. Each profile provides the
 * ability to remap QOS Priorities independently
 *
 *
 * \param [in] devId Device id
 * \param [in] profile QOS Profile
 * \param [in] dscpVal DSCP value
 * \param [in] remapPcpVal Remap PCP value
 * \param [in] remapDeiVal Remap DEI value
 * \param [in] remapDscpVal Remap DSCP value
 * \param [in] remapExpVal Remap EXP value
 *
 * \return XP_STATUS
 */
XP_STATUS xpsQosPortIngressRemapPriorityForL3QosProfile(xpsDevice_t devId, xpQosMapPfl_t profile, uint32_t dscpVal,
                                                        uint32_t remapPcpVal, uint32_t remapDeiVal, uint32_t remapDscpVal, uint32_t remapExpVal);

/**
 * \brief API to set the Port Default DSCP value
 *
 * This API will set the port default DSCP value for all
 * incoming IP packets
 *
 *
 * \param [in] devId Device id
 * \param [in] devPort Device Port
 * \param [in] dscpVal DSCP value
 *
 * \return XP_STATUS
 */
XP_STATUS xpsQosPortIngressSetPortDefaultL3QosPriority(xpsDevice_t devId, xpPort_t devPort, uint32_t dscpVal);

/**
 * \brief API to set the Port Default Traffic Class
 *
 * This API will set the port default Traffic Class for all
 * incoming packets
 *
 *
 * \param [in] devId Device id
 * \param [in] devPort Device Port
 * \param [in] tc Traffic Class
 *
 * \return XP_STATUS
 */
XP_STATUS xpsQosPortIngressSetPortDefaultTrafficClass(xpsDevice_t devId, xpPort_t devPort, uint32_t tc);

/**
 * \brief API to set the Port Default Drop Precedence
 *
 * This API will set the port default Drop Precedence for all
 * incoming packets
 *
 *
 * \param [in] devId Device id
 * \param [in] devPort Device Port
 * \param [in] dp Drop Precedence
 *
 * \return XP_STATUS
 */
XP_STATUS xpsQosPortIngressSetPortDefaultDropPrecedence(xpsDevice_t devId, xpPort_t devPort, uint32_t dp);

/**
 * \brief API to set the L2 QOS profile for a port
 *
 * This API will set a L2 QOS profile for a port as well as
 * enable or disable its application to all incoming packets
 *
 * There is a precedence that is followed for the QOS Remarking
 * scheme in Xpliant:
 *  - MPLS
 *  - IP
 *  - L2
 *  - Port default
 *
 * If MPLS Profile is enabled, all QOS priorities are taken from
 * the MPLS Profile entry that was hit
 *
 * This API enables the L2 QOS Profile application. If other,
 * higher in precedence, profiles are enabled, they take
 * precedence
 *
 * \param [in] devId Device id
 * \param [in] devPort Device Port
 * \param [in] profile QOS Profile
 * \param [in] enable Enable/Disable
 *
 * \return XP_STATUS
 */
XP_STATUS xpsQosPortIngressSetL2QosProfileForPort(xpsDevice_t devId, xpPort_t devPort, xpQosMapPfl_t profile, uint32_t enable);

/**
 * \brief API to set the L3 QOS profile for a port
 *
 * This API will set a L3 QOS profile for a port as well as
 * enable or disable its application to all incoming packets
 *
 * There is a precedence that is followed for the QOS Remarking
 * scheme in Xpliant:
 *  - MPLS
 *  - IP
 *  - L2
 *  - Port default
 *
 * If MPLS Profile is enabled, all QOS priorities are taken from
 * the MPLS Profile entry that was hit
 *
 * This API enables the L3 QOS Profile application. If other,
 * higher in precedence, profiles are enabled, they take
 * precedence
 *
 * \param [in] devId Device id
 * \param [in] devPort Device Port
 * \param [in] profile QOS Profile
 * \param [in] enable Enable/Disable
 *
 * \return XP_STATUS
 */
XP_STATUS xpsQosPortIngressSetL3QosProfileForPort(xpsDevice_t devId, xpPort_t devPort, xpQosMapPfl_t profile, uint32_t enable);

/**
 * \brief API to enable the L2 QOS Trust profile
 *
 * This profile by default trusts only the L2 Priorities and
 * keeps the incoming PCP/DEI. Traffic Class and Drop Precedence
 * are not set by this API, they will need to be set separately
 *
 *
 * \param [in] devId Device id
 * \param [in] devPort Device Port
 *
 * \return XP_STATUS
 */
XP_STATUS xpsQosPortIngressSetTrustL2ForPort(xpsDevice_t devId, xpPort_t devPort);

/**
 * \brief API to enable the L3 QOS Trust profile
 *
 * This profile by default trusts only the L3 Priorities and
 * keeps the incoming DSCP. Traffic Class and Drop Precedence
 * are not set by this API, they will need to be set separately
 *
 *
 * \param [in] devId Device id
 * \param [in] devPort Device Port
 *
 * \return XP_STATUS
 */
XP_STATUS xpsQosPortIngressSetTrustL3ForPort(xpsDevice_t devId, xpPort_t devPort);

/**
 * \brief This method enables DWRR scheduling for a particular port.
 *
 * \param [in] devId Device id
 * \param [in] devPort Device Port
 * \param [in] queueNum Queue Number
 * \param [in] enable Enable/Disable
 *
 * \return XP_STATUS
 */
XP_STATUS xpsQosSetQueueSchedulerDWRR(xpsDevice_t devId, xpsPort_t devPort, uint32_t queueNum, uint32_t enable);

/**
 * \brief This method sets the DWRR weights for a particular port.
 *
 * \param [in] devId Device id
 * \param [in] devPort Device Port
 * \param [in] queueNum Queue Number
 * \param [in] weight Weight
 *
 * \return XP_STATUS
 */
XP_STATUS xpsQosSetQueueSchedulerDWRRWeight(xpsDevice_t devId, xpsPort_t devPort, uint32_t queueNum, uint32_t weight);

/**
 * \brief This method enables Strict Priority scheduling for a particular port.
 *
 * \param [in] devId Device id
 * \param [in] devPort Device Port
 * \param [in] queueNum Queue Number
 * \param [in] enable Enable/Disable
 *
 * \return XP_STATUS
 */
XP_STATUS xpsQosSetQueueSchedulerSP(xpsDevice_t devId, xpsPort_t devPort, uint32_t queueNum, uint32_t enable);

/**
 * \brief This method sets SP priority for a particular port.
 *
 * \param [in] devId Device id
 * \param [in] devPort Device Port
 * \param [in] queueNum Queue Number
 * \param [in] priority Priority
 *
 * \return XP_STATUS
 */
XP_STATUS xpsQosSetQueueSchedulerSPPriority(xpsDevice_t devId, xpsPort_t devPort, uint32_t queueNum, uint32_t priority);

/**
 * \brief This method sets the Priority Flow Control priority for a specific port and queue.
 *
 * \param [in] devId Device id
 * \param [in] portNum Port number
 * \param [in] queueNum Queue number
 * \param [in] prio Priority
 *
 * \return XP_STATUS
 */
XP_STATUS xpsQosFcSetPfcPriority(xpsDevice_t devId, uint32_t portNum, uint32_t queueNum, uint8_t prio);

/**
 * \brief This method configures shaping for a specific port on a device
 *
 * This API is used only to configure a shape and does not
 * explicitly enable shaping.
 *
 * \see xpsQosShaperSetPortShaperEnable()
 *
 *
 * \param [in] devId Device id
 * \param [in] devPort Device Port
 * \param [in] rateKbps Rate
 * \param [in] maxBurstByteSize Maximum burst byte size
 *
 * \return XP_STATUS
 */
XP_STATUS xpsQosShaperConfigurePortShaper(xpsDevice_t devId, xpPort_t devPort, uint64_t rateKbps, uint32_t maxBurstByteSize);

/**
 * \brief This method enables or disables shaping for a specific port on a device
 *
 * \param [in] devId Device id
 * \param [in] devPort Device Port
 * \param [in] enableShaper Enable/Disable
 *
 * \return XP_STATUS
 */
XP_STATUS xpsQosShaperSetPortShaperEnable(xpsDevice_t devId, xpPort_t devPort, uint32_t enableShaper);

#ifdef __cplusplus
}
#endif

#endif //_openXpsQos_h_

