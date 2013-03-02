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
 * @author      Nenad Radulovic
 * @brief       Implementacija State Machine Processor modula.
 * @addtogroup  smp_impl
 ****************************************************************************************//** @{ */

/*============================================================================  INCLUDE FILES  ==*/
#define SMP_PKG_H_VAR
#include "kernel_private.h"

/*============================================================================  LOCAL DEFINES  ==*/
/*============================================================================  LOCAL MACRO's  ==*/
/*-------------------------------------------------------------------------------------------*//**
 * @brief       Posalji predefinisan dogadjaj @c evt automatu @c hsm.
 * @param       epa                     Pokazivac na strukturu EPA objekta,
 * @param       state                   pokazivac na funkciju stanja,
 * @param       evt                     redni broj (enumerator) rezervisanog
 *                                      dogadjaj.
 *//*--------------------------------------------------------------------------------------------*/
#define EVT_SIGNAL_SEND(epa, state, evt)                                        \
    (* state)((epa), (esEvtHeader_T *)&evtSignal[evt])

/*=========================================================================  LOCAL DATA TYPES  ==*/
/*================================================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*==========================================================================  LOCAL VARIABLES  ==*/
/*=========================================================================  GLOBAL VARIABLES  ==*/
/*===============================================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*======================================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*-----------------------------------------------------------------------------------------------*/
void hsmDispatch(
    esEpaHeader_T       * aEpa,
    const esEvtHeader_T * aEvt) {

    esPtrState_T        * srcState;
    esPtrState_T        * dstState;
    esState_T           state;
    uint_fast8_t        srcEnd;
    uint_fast8_t        dstEnd;

    srcState = aEpa->exec.pSrcStates;
    srcEnd = (uint_fast8_t)0U;

    do {
        srcState[srcEnd] = aEpa->exec.ptrState;
        state = (esState_T)(* srcState[srcEnd])(aEpa, (esEvtHeader_T *)aEvt);
        ++srcEnd;
    } while (state == RETN_SUPER);

    switch (state) {

        case RETN_TRAN : {
            dstState = aEpa->exec.pDstStates;

            do {
                dstState[0] = aEpa->exec.ptrState;                                     /* sacuvaj destinaciju                                      */
                dstEnd = (uint_fast8_t)1U;

                if (srcState[srcEnd - 1U] != dstState[0]) {                     /* tran: a) src ?!= dst                                     */
                    (void)EVT_SIGNAL_SEND(aEpa, dstState[0], SIG_SUPER);
                    dstState[1] = aEpa->exec.ptrState;
                    --srcEnd;


                    if (srcState[srcEnd] != dstState[1]) {                      /* tran: b) src ?!= super(dst)                              */
                        (void)EVT_SIGNAL_SEND(aEpa, srcState[srcEnd], SIG_SUPER);
                        ++srcEnd;
                        srcState[srcEnd] = aEpa->exec.ptrState;

                        if (srcState[srcEnd] != dstState[1]) {                  /* tran: c) super(src) ?!= super(dst)                       */

                            if (srcState[srcEnd] == dstState[0]) {              /* tran: d) super(src) ?== dst                              */
                                dstEnd = (uint_fast8_t)0U;                      /* Ne treba ni da se udje u novu hijerarhiju.               */
                            } else {                                            /* tran: e) src ?== ...super(super(dst))                    */
                                --srcEnd;

                                while ((esPtrState_T)&esSMTopState != dstState[dstEnd]) { /* tran: e) src ?== ...super(super(dst))         */
                                    (void)EVT_SIGNAL_SEND(aEpa, dstState[dstEnd], SIG_SUPER);

                                    if (srcState[srcEnd] == aEpa->exec.ptrState) {
                                        goto TRANSITION_EXECUTION;
                                    }
                                    ++dstEnd;
                                    dstState[dstEnd] = aEpa->exec.ptrState;
                                }
                                ++srcEnd;
                                dstEnd = (uint_fast8_t)1U;

                                while ((esPtrState_T)&esSMTopState != dstState[dstEnd]) { /* tran: f) super(src) ?== ...super(super(dst))  */

                                    if (srcState[srcEnd] == dstState[dstEnd]) {
                                        goto TRANSITION_EXECUTION;
                                    }
                                    ++dstEnd;
                                }

                                while (TRUE) {
                                    (void)EVT_SIGNAL_SEND(aEpa, srcState[srcEnd], SIG_SUPER);
                                    dstEnd = (uint_fast8_t)0U;
                                    ++srcEnd;
                                    srcState[srcEnd] = aEpa->exec.ptrState;

                                    while ((esPtrState_T)&esSMTopState != dstState[dstEnd]) { /* tran: f) super(src) ?== ...super(super(dst))*/

                                        if (srcState[srcEnd] == dstState[dstEnd]) {
                                            goto TRANSITION_EXECUTION;
                                        }
                                        ++dstEnd;
                                    }
                                }
                            }
                        }
                    }
                }

TRANSITION_EXECUTION:
                {
                    uint_fast8_t        stateCnt;

                    stateCnt = (uint_fast8_t)0U;

                    while (stateCnt != srcEnd) {
#if defined(OPT_KERNEL_DBG_SMP)
                        state = (esState_T)EVT_SIGNAL_SEND(aEpa, srcState[stateCnt], SIG_EXIT);
                        SMP_ASSERT((RETN_SUPER == state) || (RETN_HANDLED == state));
#else
                        (void)EVT_SIGNAL_SEND(aEpa, srcState[stateCnt], SIG_EXIT);
#endif
                        ++stateCnt;
                    }
                }

                while ((uint_fast8_t)0U != dstEnd) {                            /* Udji u novu hijerarhiju.                                 */
                    --dstEnd;
#if defined(OPT_KERNEL_DBG_SMP)
                    state = (esState_T)EVT_SIGNAL_SEND(aEpa, srcState[stateCnt], SIG_EXIT);
                    SMP_ASSERT((RETN_SUPER == state) || (RETN_HANDLED == state));
#else
                    (void)EVT_SIGNAL_SEND(aEpa, dstState[dstEnd], SIG_ENTRY);
#endif
                }
                state = (esState_T)EVT_SIGNAL_SEND(aEpa, dstState[0], SIG_INIT);
                srcState[0] = dstState[0];
                srcEnd = (uint_fast8_t)1U;
            } while (RETN_TRAN == state);
            aEpa->exec.ptrState = dstState[0];

            return;
        }

        case RETN_HANDLED : {
            aEpa->exec.ptrState = srcState[0];

            return;
        }

        case RETN_DEFERRED : {
            aEpa->exec.ptrState = srcState[0];
            esEvtPost(
                aEpa,
                (esEvtHeader_T *)aEvt);

            return;
        }

        case RETN_IGNORED : {
            aEpa->exec.ptrState = srcState[0];

            return;
        }

        default : {

            return;
        }
    }
}

/*-----------------------------------------------------------------------------------------------*/
void hsmInit (
    esEpaHeader_T       * aEpa,
    esPtrState_T        aInitState,
    esPtrState_T        * aStateBuff,
    size_t              aStateDepth) {

    aEpa->exec.ptrState = aInitState;
    aEpa->exec.pSrcStates = aStateBuff;
    aEpa->exec.pDstStates = aStateBuff + aStateDepth;
}

/*-----------------------------------------------------------------------------------------------*/
void hsmDeInit(
    esEpaHeader_T       * aEpa) {

    aEpa->exec.pSrcStates = (esPtrState_T *)0U;
    aEpa->exec.pDstStates = (esPtrState_T *)0U;
}

/*=======================================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

/*-------------------------------------------------------------------------------------------*//**
 * @ingroup         smp_intf
 * @{ *//*---------------------------------------------------------------------------------------*/
esState_T esSMTopState (
    esEpaHeader_T       * aEpa,
    esEvtHeader_T       * aEvt) {

    (void)aEpa;                                                                 /* Ukloni upozorenje o nekoriscenom parametru               */
    (void)aEvt;

    return ES_STATE_IGNORED();
}

/*-----------------------------------------------------------------------------------------------*/
bool_T esSMIsInState (
    esEpaHeader_T       * aEpa,
    esPtrState_T        aState) {

    ES_CRITICAL_DECL();
    esPtrState_T tmpState;
    esPtrState_T savedState;
    bool_T       ans;


    ES_CRITICAL_ENTER(OPT_KERNEL_INTERRUPT_PRIO_MAX);
    savedState = aEpa->exec.ptrState;                                                  /* sacuvaj trenutno stanje automata                         */
    (void)EVT_SIGNAL_SEND(aEpa, savedState, SIG_SUPER);
    tmpState = aEpa->exec.ptrState;
    ans = FALSE;

    while (&esSMTopState != tmpState) {

        if (aState == tmpState) {
            ans = TRUE;
            break;
        }
        (void)EVT_SIGNAL_SEND(aEpa, savedState, SIG_SUPER);
        tmpState = aEpa->exec.ptrState;
    }
    aEpa->exec.ptrState = savedState;                                                  /* vrati prethodno stanje automata */
    ES_CRITICAL_EXIT();

    return (ans);
}
/** @} *//*--------------------------------------------------------------------------------------*/
/*===================================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/

/** @endcond *//** @} *//*************************************************************************
 * END of smp.c
 *************************************************************************************************/
