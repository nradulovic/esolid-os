/*************************************************************************************************
 * This file is part of eSolid
 *
 * Copyright (C) 2011, 2012 - Nenad Radulovic
 *
 * eSolid is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * eSolid is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with eSolid; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301  USA
 *
 * web site:    http://blueskynet.dyndns-server.com
 * e-mail  :    blueskyniss@gmail.com
 *//******************************************************************************************//**
 * @file
 * @author  	Nenad Radulovic
 * @brief       Skup STP poruka
 * ------------------------------------------------------------------------------------------------
 * @addtogroup  stp_messages
 * @brief		Skup STP poruka
 ****************************************************************************************//** @{ */


#ifndef STP_MESSAGES_H_
#define STP_MESSAGES_H_

/*============================================================================  INCLUDE FILES  ==*/
/*==================================================================================  DEFINES  ==*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Definition group
 * @brief       brief description
 * @{ *//*---------------------------------------------------------------------------------------*/
#define STP_FILT_EPA_STATUS             (1U << 0)
#define STP_FILT_EPA_TRANSITION         (1U << 1)
#define STP_FILT_EVT                    (1U << 2)
#define STP_FILT_KERN_STATUS            (1U << 3)

/** @} *//*--------------------------------------------------------------------------------------*/

/*==================================================================================  MACRO's  ==*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Macro group
 * @brief       brief description
 * @{ *//*---------------------------------------------------------------------------------------*/
/** @} *//*--------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*===============================================================================  DATA TYPES  ==*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Data types group
 * @brief       brief description
 * @{ *//*---------------------------------------------------------------------------------------*/
/** @} *//*--------------------------------------------------------------------------------------*/

/*=========================================================================  GLOBAL VARIABLES  ==*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Variables group
 * @brief       brief description
 * @{ *//*---------------------------------------------------------------------------------------*/
/* STP_FILT_EPA_STATUS */
const char * const txtEpaCreate = "create EPA %s (addr: %p, size: %u, prio: %u, init: %p)\n"; /* ime, adresa, velicina, prio, adresa f-je.  */
const char * const txtEpaDestroy = "destroy EPA %p\n";                          /* adresa                                                   */
const char * const txtEpaDispatch = "dispach EPA %p (state: %p, event: %u)\n";  /* adresa, adresa, identifikator                      */
const char * const txtEpaInit = "init EPA %s (addr: %p, size: %u, prio: %u, init: %p) with buffers: state %p, evt %p"; /* ime, adresa, velicina, prio, adresa f-je, adresa state bufera, adresa reda za cekanje */

/* STP_FILT_EVT */
const char * const txtEvtCreate = "create evt %u (addr: %p, size %u)\n";        /* identifikator, adresa, velicina                          */
const char * const txtEvtPost = "post evt %u to EPA %p\n";                      /* identifikator, adresa                                    */
const char * const txtEvtPostAhead = "post ahead evt %u to EPA %p\n";           /* identifikator, adresa                                    */
const char * const txtEvtDestroy = "destroy evt %u\n";                          /* identifikator                                            */

/* STP_FILT_EPA_TRANSITION */
const char * const txtEpaSMTransition = "transition %p -> %p:";                   /* adresa, adresa                                           */
const char * const txtEpaSMSuperState = " super(%p),";                            /* adresa                                                   */
const char * const txtEpaSMEntry = " entry(%p)";                                  /* adresa                                                   */
const char * const txtEpaSMExit = " exit(%p)";                                    /* adresa                                                   */
const char * const txtEpaSMInit = " init(%p)";                                    /* adresa                                                   */

/* STP_FILT_KERN_STATUS */
const char * const txtKernStart = "\n ** Starting kernel **\n";

/** @} *//*--------------------------------------------------------------------------------------*/

/*======================================================================  FUNCTION PROTOTYPES  ==*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Function group
 * @brief       brief description
 * @{ *//*---------------------------------------------------------------------------------------*/
/** @} *//*--------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*===================================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/

/** @endcond *//** @} *//*************************************************************************
 * END of stp_messages.h
 *************************************************************************************************/
#endif /* STP_MESSAGES_H_ */
