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
#define STP_FILT_EPA_STATUS_0           (1UL << 0)
#define STP_FILT_EPA_EXECUTION_0        (1UL << 1)
#define STP_FILT_EVT_0                  (1UL << 2)
#define STP_FILT_KERN_STATUS_0          (1UL << 3)

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
/* STP_FILT_EPA_STATUS 0 */
const char * const txtEpaCreate =
    "\nEPA: create name: %s (size: %uB, prio: %u, init addr: %p)";              /* ime, velicina, prio, adresa f-je.                        */
const char * const txtEpaCreateMemNeeded =
    "\n - mem required: %uB";                                                   /* velicina memorije                                        */
const char * const txtEpaCreateMemAddr =
    "\n - mem addr: %p";                                                        /* adresa EPA header-a                                      */
const char * const txtEpaDestroy =
    "\nEPA: destroy addr: %p";                                                  /* adresa                                                   */
const char * const txtEpaInit =
    "\nEPA: init name: %s, addr: %p, (size: %u, prio: %u, init: %p) buffers: state addr: %p, evt addr: %p";
                                                                                /* ime, adresa, velicina, prio, adresa f-je, adresa state   */
                                                                                /* bufera, adresa reda za cekanje                           */

/* STP_FILT_EVT 0 */
const char * const txtEvtCreate =
    "\nevt: create id: %u (size %uB)";                                          /* identifikator, adresa, velicina                          */
const char * const txtEvtPost =
    "\nevt: post id: %u to EPA addr: %p";                                       /* identifikator, adresa                                    */
const char * const txtEvtPostAhead =
    "\nevt: post ahead id: %u to EPA addr: %p";                                 /* identifikator, adresa                                    */
const char * const txtEvtDestroy =
    "\nevt: destroy id: %u";                                                    /* identifikator                                            */

/* STP_FILT_EVT 1 */
const char * const txtEvtCreateMemNeeded =
    "\n - mem required: %uB";                                                   /* velicina memorije                                        */
const char * const txtEvtCreateMemAddr =
    "\n - mem addr: %p";                                                        /* adrea EVT header-a                                       */

/* STP_FILT_EPA_EXECUTION 0 */
const char * const txtEpaSMexec =
    "\nSM: exec addr %p, start state addr: %p, evt id: %u";                     /* adresa EPA header-a, adresa f-je stanja, id dogadjaja    */
const char * const txtEpaSMtran =
    "\nSM: transition addr: %p -> addr: %p:";                                   /* adresa stanja izvora, adresa stanja odredista            */
const char * const txtEpaSMtranFinished =
    "\nSM: transition finished";

/* STP_FILT_EPA_EXECUTION 1 */
const char * const txtEpaSMsuper = " super(%p)";                                /* adresa                                                   */
const char * const txtEpaSMentry = " entry(%p)";                                /* adresa                                                   */
const char * const txtEpaSMexit = " exit(%p)";                                  /* adresa                                                   */
const char * const txtEpaSMinit = " init(%p)";                                  /* adresa                                                   */

/* STP_FILT_KERN_STATUS 0 */
const char * const txtKernStart = "\n ** Kernel start **\n";
const char * const txtKernSearch = "\nkern: search EPA to dispatch";
const char * const txtKernEnterIdle = "\nkern: enter IDLE loop";
const char * const txtKernExitIdle = "\nkern: exit IDLE loop";
const char * const txtKernEpaDispatch = "\nkern: dispatch EPA addr: %p (prio: %u)";   /* adresa, adresa                                     */
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
