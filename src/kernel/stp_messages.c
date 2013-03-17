/*************************************************************************************************
 * This file is part of esolid-kernel
 *
 * Template version: 1.1.13 (21.10.2012)
 *
 * Copyright (C) 2011, 2012 - Nenad Radulovic
 *
 * esolid-kernel is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * esolid-kernel is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with esolid-kernel; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301  USA
 *
 * web site:    http://blueskynet.dyndns-server.com
 * e-mail  :    blueskyniss@gmail.com
 *//******************************************************************************************//**
 * @file
 * @author      Nenad Radulovic
 * @brief       Skup STP poruka
 * @addtogroup  stp_messages
 ****************************************************************************************//** @{ */
                                                                                                  
/*============================================================================  INCLUDE FILES  ==*/
#include "kernel_private.h"

/*============================================================================  LOCAL DEFINES  ==*/
/*============================================================================  LOCAL MACRO's  ==*/
/*=========================================================================  LOCAL DATA TYPES  ==*/
/*================================================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*==========================================================================  LOCAL VARIABLES  ==*/
/*=========================================================================  GLOBAL VARIABLES  ==*/
/* STP_FILT_EPA_STATUS 0 */

const char C_ROM * C_ROM_VAR const txtEpaCreateMemAddr =
    " - mem addr: %p";                                                          /* adresa EPA header-a                                      */
const char C_ROM * C_ROM_VAR const txtEpaDestroy =
    "EPA: destroy addr: %p";                                                    /* adresa                                                   */
const char C_ROM * C_ROM_VAR const txtEpaInit =
    "EPA: init name: %s, addr: %p, (size: %u, prio: %u)";                       /* ime, adresa, velicina, prio                              */

/* STP_FILT_EVT 0 */
const char C_ROM * C_ROM_VAR const txtEvtCreate =
    "evt: create id: %u (size %uB)";                                            /* identifikator, adresa, velicina                          */
const char C_ROM * C_ROM_VAR const txtEvtPost =
    "evt: post id: %u to EPA addr: %p";                                         /* identifikator, adresa                                    */
const char C_ROM * C_ROM_VAR const txtEvtPostAhead =
    "evt: post ahead id: %u to EPA addr: %p";                                   /* identifikator, adresa                                    */
const char C_ROM * C_ROM_VAR const txtEvtDestroy =
    "evt: destroy id: %u";                                                      /* identifikator                                            */
const char C_ROM * C_ROM_VAR const txtEvtReserve =
    "evt: reserve id: %u";
const char C_ROM * C_ROM_VAR const txtEvtUnReserve =
    "evt: unReserve id: %u";

/* STP_FILT_EVT 1 */
const char C_ROM * C_ROM_VAR const txtEvtCreateMemNeeded =
    " - mem required: %uB";                                                     /* velicina memorije                                        */
const char C_ROM * C_ROM_VAR const txtEvtCreateMemAddr =
    " - mem addr: %p";                                                          /* adrea EVT header-a                                       */

/* STP_FILT_EVT_Q_0 */
const char C_ROM * C_ROM_VAR const txtEvtQinit =
    "evtQ: initializing evtQ addr: %p, buff addr: %p, buff size: %u units";
const char C_ROM * C_ROM_VAR const txtEvtQput =
    "evtQ: put evt id: %u into queue addr: %p";                                 /* adresa reda za dogadjaje, identifikator                  */
const char C_ROM * C_ROM_VAR const txtEvtQputAhead =
    "evtQ: put ahead evt id: %u into queue addr: %p";                           /* adresa reda za dogadjaje, identifikator                  */
const char C_ROM * C_ROM_VAR const txtEvtQget =
    "evtQ: get evt addr: %p";                                                   /* adresa reda za dogadjaje                                 */
const char C_ROM * C_ROM_VAR const txtEvtQFree =
    " - queue free: %u";                                                        /* broj slobodnih pozicija                                  */
const char C_ROM * C_ROM_VAR const txtEvtQFull =
    " ** WARNING: QUEUE is FULL!";                                              /* red cekanja za dogadjaje je pun                          */

/* STP_FILT_EPA_EXECUTION 0 */
const char C_ROM * C_ROM_VAR const txtEpaSMinitialization =
    "SM: initializing SM %p, state addr: %p, state buff addr: %p, state depth: %u"; /* adresa automata i stanja, velicina bafera stanja     */
const char C_ROM * C_ROM_VAR const txtEpaSMexec =
    "SM: exec SM addr %p, start state addr: %p, evt id: %u";                    /* adresa EPA header-a, adresa f-je stanja, id dogadjaja    */
const char C_ROM * C_ROM_VAR const txtEpaSMtran =
    "SM: transition addr: %p -> addr: %p:";                                     /* adresa stanja izvora, adresa stanja odredista            */
const char C_ROM * C_ROM_VAR const txtEpaSMHandled =
    "SM: handled evt id: %u";                                                   /* identifikator dogadjaja                                  */
const char C_ROM * C_ROM_VAR const txtEpaSMDeffered =
    "SM: deffered evt id: %u";                                                  /* identifikator dogadjaja                                  */
const char C_ROM * C_ROM_VAR const txtEpaSMignored =
    "SM: ignored evt id: %u";                                                   /* identifikator dogadjaja                                  */

/* STP_FILT_EPA_EXECUTION 1 */
const char C_ROM * C_ROM_VAR const txtEpaSMtranStart =
    "SM: transition start";
const char C_ROM * C_ROM_VAR const txtEpaSMtranFinished =
    "SM: transition finish";
const char C_ROM * C_ROM_VAR const txtEpaSMgenTree =
    "SM: generating hierarchy tree:";
const char C_ROM * C_ROM_VAR const txtEpaSMsuper =
    " - super(%p)=%p";                                                          /* adresa stanja, adresa superstanja                        */
const char C_ROM * C_ROM_VAR const txtEpaSMentry =
    " - entry(%p)";                                                             /* adresa                                                   */
const char C_ROM * C_ROM_VAR const txtEpaSMexit =
    " - exit(%p)";                                                              /* adresa                                                   */
const char C_ROM * C_ROM_VAR const txtEpaSMinit =
    " - init(%p)";                                                              /* adresa                                                   */

/* STP_FILT_KERN_STATUS 0 */
const char C_ROM * C_ROM_VAR const txtKernStart =
    " ** Kernel start **";
const char C_ROM * C_ROM_VAR const txtKernSearch =
    "kern: search EPA to dispatch";
const char C_ROM * C_ROM_VAR const txtKernEnterIdle =
    "kern: enter IDLE loop";
const char C_ROM * C_ROM_VAR const txtKernExitIdle =
    "kern: exit IDLE loop";
const char C_ROM * C_ROM_VAR const txtKernEpaDispatch =
    "kern: dispatch EPA addr: %p (prio: %u)";                                   /* adresa, adresa                                           */

/* STP_FILE_MEM_0 */
const char C_ROM * C_ROM_VAR const txtMemHeapInit =
    "mem heap: initializing heap mem: addr: %p, size: %uB";                     /* pocetak heap memorije, velicina                          */
const char C_ROM * C_ROM_VAR const txtMemHeapFree =
    " - heap free: %u";
const char C_ROM * C_ROM_VAR const txtMemHeapAlloc =
    "mem heap: allocate %uB";
const char C_ROM * C_ROM_VAR const txtMemHeapAllocated =
    "mem heap: allocated addr: %p";
const char C_ROM * C_ROM_VAR const txtMemHeapDeAlloc =
    "mem heap: deallocate %p";

#define EXPAND_AS_ENUMERATOR(a,b) a,
#define EXPAND_AS_TEXT(a,b) [a] = b,
#define EXPAND_AS_SIZE(a,b) sizeof(b),
#define TEXT_TABLE(ENTRY)                                                       \
    ENTRY(LOG_EPA_CREATE,"EPA: create:")                                        \
    ENTRY(LOG_EPA__MEM," mem [B]: %x")                                          \
    ENTRY(LOG_EPA__ID, " id: %x")                                               \
    ENTRY(LOG_EPA__SIZE, " size [B]: %x")                                       \
    ENTRY(LOG_EPA__PRIO, " prio: %x")                                           \
    ENTRY(LOG_EPA_PRIOSET, "EPA: prioSet:")                                     \
    ENTRY(LOG_EPA_PRIOGET, "EPA: prioGet:")                                     \
    ENTRY(LOG_EPA_DESTROY, "EPA: destroy:")                                     \
    ENTRY(LOG_EPA_DISPATCH, "EPA: dispatch:")                                   \
    ENTRY(LOG_EPA_EVTPOST, "EPA: evtPost:")                                     \
    ENTRY(LOG_EPA_EVTPOSTA, "EPA: evtPostAhead:")                               \
    ENTRY(LOG_EPA_EVTFETCH, "EPA: evtFetch:")                                   \
    ENTRY(LOG_EPA_EVTPUSHBACK, "EPA: evtPushBack")

enum logMsgIndex {
    TEXT_TABLE(EXPAND_AS_ENUMERATOR)
    LOG_LAST_ENTRY
};

const char C_ROM * C_ROM_VAR const logMsgText[LOG_LAST_ENTRY] = {
    TEXT_TABLE(EXPAND_AS_TEXT)
};

const size_t logMsgSize[LOG_LAST_ENTRY] = {
    TEXT_TABLE(EXPAND_AS_SIZE)
};

/*===============================================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*======================================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*=======================================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/
/*===================================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/

/** @endcond *//** @} *//*************************************************************************
 * END of stp_messages.c
 *************************************************************************************************/
