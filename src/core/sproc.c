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
 *************************************************************************************************/


/*********************************************************************************************//**
 * @file
 * @author      Nenad Radulovic
 * @brief       Implementacija State Processor modula.
 * ------------------------------------------------------------------------------------------------
 * @addtogroup  sproc_impl
 ****************************************************************************************//** @{ */


/*************************************************************************************************
 * INCLUDE FILES
 *************************************************************************************************/

#define SPROC_PKG_H_VAR
#include "core_private.h"


/*************************************************************************************************
 * LOCAL DEFINES
 *************************************************************************************************/

/*-----------------------------------------------------------------------------------------------*
 * Local debug defines
 *-----------------------------------------------------------------------------------*//** @cond */

SP_DBG_DEFINE_MODULE(State Processor);


/** @endcond *//*--------------------------------------------------------------------------------*
 * Local user defines
 *-----------------------------------------------------------------------------------------------*/


/*************************************************************************************************
 * LOCAL MACRO's
 *************************************************************************************************/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Posalji predefinisan dogadjaj @c evt automatu @c hsm.
 * @param       epa                     Pokazivac na strukturu EPA objekta,
 * @param       state                   pokazivac na funkciju stanja,
 * @param       evt                     redni broj (enumerator) rezervisanog
 *                                      dogadjaj.
 */
/*-----------------------------------------------------------------------------------------------*/
#define EVT_SIGNAL_SEND(epa, state, evt)                                        \
    (* state)((epa), (esEvtHeader_T *)&evtSignal[evt])

/*-------------------------------------------------------------------------------------------*//**
 * @name        Debug podrska
 *
 * @brief       Makroi za proveru validnosti internih podataka
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Izvrsi validaciju odgovora stanja funkcije stanja HSM automata.
 */
/*-----------------------------------------------------------------------------------------------*/
#define HSM_VALID_STATE(expr)                                                   \
    (RETN_SUPER == (expr))   ||                                                 \
    (RETN_TRAN == (expr))    ||                                                 \
    (RETN_HANDLED == (expr)) ||                                                 \
    (RETN_NOEX == (expr)) ||                                                    \
    (RETN_DEFERRED == (expr))||                                                 \
    (RETN_IGNORED == (expr))

/** @} *//*--------------------------------------------------------------------------------------*/


/*************************************************************************************************
 * LOCAL CONSTANTS
 *************************************************************************************************/


/*************************************************************************************************
 * LOCAL DATA TYPES
 *************************************************************************************************/


/*************************************************************************************************
 * LOCAL TABLES
 *************************************************************************************************/


/*************************************************************************************************
 * SHARED GLOBAL VARIABLES
 *************************************************************************************************/


/*************************************************************************************************
 * LOCAL GLOBAL VARIABLES
 *************************************************************************************************/


/*************************************************************************************************
 * LOCAL FUNCTION PROTOTYPES
 *************************************************************************************************/


/*************************************************************************************************
 ***                                I M P L E M E N T A T I O N                                ***
 *************************************************************************************************/


/*************************************************************************************************
 * GLOBAL FUNCTION DEFINITIONS
 *************************************************************************************************/

/*-------------------------------------------------------------------------------------------*//**
 * @ingroup         sproc_intf
 * @{ *//*---------------------------------------------------------------------------------------*/

void hsmDispatch (
    esEpaHeader_T       * aEpa,
    const esEvtHeader_T * aEvt) {

    esPtrState_T * srcState;                                                    /* Cuvanje adresa izvorista.                                */
    esPtrState_T * dstState;                                                    /* Cuvanje adresa odredista.                                */
    esPtrState_T tmpState;
    esState_T    state;
    uint_fast8_t srcEnd;
    uint_fast8_t dstEnd;
    uint_fast8_t stateCnt;

    srcState = aEpa->internals.exec.pSrcStates;
    srcEnd = (uint_fast8_t)0U;

    while (TRUE) {                                                              /* Pozivanje f-ja sve dok vracaju SUPER.                    */
        tmpState = aEpa->pState;
        state = (esState_T)(*tmpState)(                                         /* Pozovi funkciju stanja:                                  */
            aEpa,                                                               /* EPA objekat koji se obradjuje.                           */
            (esEvtHeader_T *)aEvt);                                             /* Dobavi nov dogadjaj.                                     */
        SP_ASSERT(HSM_VALID_STATE(state));

        if (RETN_SUPER != state) {
            break;                                                              /* break se koristi zbog brzog izvrsavanja petlje, FIXME: ovo nije tacna tvrdnja! */
        }
        srcState[srcEnd] = tmpState;
        ++srcEnd;

#if defined(OPT_KERNEL_ENABLE) && defined(OPT_KERNEL_USE_REGISTRY) || defined(__DOXYGEN__)
        SP_ASSERT(srcEnd <= aEpa->internals.registry.description->hsmStateDepth);
#endif
    };

    if (RETN_TRAN == state) {                                                   /* Da li treba izvrsiti tranziciju?                         */
        dstState = aEpa->internals.exec.pDstStates;
        dstState[0] = aEpa->pState;                                             /* sacuvaj destinaciju                                      */
        dstEnd = (uint_fast8_t)1U;

        if (tmpState == dstState[0]) {                                          /* tran: a) src ?== dst                                     */
            srcState[srcEnd] = tmpState;
            ++srcEnd;
        } else {                                                                /* tran: a) src != dst                                      */
            (void)EVT_SIGNAL_SEND(aEpa, dstState[0], SIG_SUPER);
            dstState[1] = aEpa->pState;

            if (tmpState != dstState[1]) {                                      /* tran: b) src ?== super(dst)                              */
                (void)EVT_SIGNAL_SEND(aEpa, tmpState, SIG_SUPER);
                srcState[srcEnd] = tmpState;
                tmpState = aEpa->pState;
                ++srcEnd;

                if (tmpState != dstState[1]) {                                  /* tran: c) super(src) ?== super(dst)                       */

                    if (tmpState == dstState[0]) {                              /* tran: d) super(src) ?== dst                              */
                        dstEnd = (uint_fast8_t)0U;
                    } else {
                        srcState[srcEnd] = tmpState;                            /* tran: e) src ?== ...super(super(dst))                    */
                        --srcEnd;
                        dstEnd = (uint_fast8_t)2U;
                        tmpState = dstState[1];

                        while ((esPtrState_T)&esHsmTopState != tmpState) {      /* dobavi super(dst) i sacuvaj u niz                  */
                            (void)EVT_SIGNAL_SEND(aEpa, tmpState, SIG_SUPER);
                            tmpState = aEpa->pState;

                            if (srcState[srcEnd] == tmpState) {

                                goto EXECUTE_PATH;

                            }
                            dstState[dstEnd] = tmpState;
                            ++dstEnd;

#if (defined(OPT_DBG_SP) && defined(OPT_KERNEL_ENABLE) && defined(OPT_KERNEL_USE_REGISTRY)) || defined(__DOXYGEN__)
                            SP_ASSERT(dstEnd <= aEpa->internals.registry.description->hsmStateDepth);
#endif
                        }
                        ++srcEnd;
                        stateCnt = (uint_fast8_t)2U;
                        tmpState = srcState[srcEnd];

                        while (stateCnt != dstEnd) {                            /* tran: f) super(src) ?== ...super(super(dst))             */

                            if (tmpState == dstState[stateCnt]) {
                                dstEnd = stateCnt;

                                goto EXECUTE_PATH;

                            }
                            ++stateCnt;
                        };

                        while (TRUE) {                                          /* tran: g i h) ...super(super(src) ?== super(super(dst)    */
                            (void)EVT_SIGNAL_SEND(aEpa, tmpState, SIG_SUPER);
                            tmpState = aEpa->pState;
                            ++srcEnd;
                            stateCnt = (uint_fast8_t)0U;

                            while (stateCnt != dstEnd) {

                                if (tmpState == dstState[stateCnt]) {
                                    dstEnd = stateCnt;

                                    goto EXECUTE_PATH;

                                }
                                ++stateCnt;
                            }
                            srcState[srcEnd] = tmpState;
                        }
                    }
                }
            }
        }

        EXECUTE_PATH:

        stateCnt = (uint_fast8_t)0U;

        while (stateCnt != srcEnd) {                                            /* Izadji iz hijerarhije.                                   */

#if defined(OPT_DBG_SP)
            state = (esState_T)EVT_SIGNAL_SEND(aEpa, srcState[stateCnt], SIG_EXIT);
            SP_ASSERT((RETN_SUPER == state) || (RETN_HANDLED == state));
#else
            (void)EVT_SIGNAL_SEND(aEpa, srcState[stateCnt], SIG_EXIT);
#endif
            ++stateCnt;
        }

        while (TRUE) {

            while ((uint_fast8_t)0U != dstEnd) {                                  /* Udji u novu hijerarhiju.                                 */
                --dstEnd;
                state = (esState_T)EVT_SIGNAL_SEND(aEpa, dstState[dstEnd], SIG_ENTRY);
                SP_ASSERT(((RETN_SUPER == state) || (RETN_NOEX == state) || (RETN_HANDLED == state)));
            }

            if (RETN_NOEX == state) {
                aEpa->pState = dstState[0];
                evtQPutAhead(
                    aEpa,
                    (esEvtHeader_T *)&evtSignal[SIG_NOEX]);

                return;
            }
            state = (esState_T)EVT_SIGNAL_SEND(aEpa, dstState[0], SIG_INIT);
            SP_ASSERT((RETN_TRAN == state) || (RETN_SUPER == state));

            if (RETN_TRAN != state) {

                break;
            }
            dstEnd = (uint_fast8_t)0U;
            tmpState = dstState[0];
            dstState[0] = aEpa->pState;

            while (dstState[dstEnd] != tmpState) {
                (void)EVT_SIGNAL_SEND(aEpa, dstState[dstEnd], SIG_SUPER);
                ++dstEnd;
                dstState[dstEnd] = aEpa->pState;
            }
        }
        aEpa->pState = dstState[0];

        return;
    } else if (RETN_NOEX == state) {                                            /* dst ?== flowchart                                        */
        evtQPutAhead(
            aEpa,
            (esEvtHeader_T *)&evtSignal[SIG_NOEX]);

        return;
    } else if (RETN_DEFERRED == state) {
        evtQPut(
            aEpa,
            (esEvtHeader_T *)aEvt);
    }
    aEpa->pState = srcState[0];                                                 /* Vrati izvorno stanje.                                    */
}

bool_T esHsmIsInState (
    esEpaHeader_T       * aEpa,
    esPtrState_T        aState) {

    ES_CRITICAL_DECL();
    esPtrState_T tmpState;
    esPtrState_T savedState;
    bool_T       ans;

#if (defined(OPT_DBG_SP) && defined(OPT_KERNEL_ENABLE) && defined(OPT_KERNEL_USE_REGISTRY)) || defined(__DOXYGEN__)
    uint8_t dbgCnt;

    dbgCnt = aEpa->internals.registry.description->hsmStateDepth;
#endif
    SP_ASSERT((esPtrState_T)0 != aState);

    ES_CRITICAL_ENTER(OPT_KERNEL_INTERRUPT_PRIO_MAX);
    savedState = aEpa->pState;                                                  /* sacuvaj trenutno stanje automata                         */
    (void)EVT_SIGNAL_SEND(aEpa, savedState, SIG_SUPER);
    tmpState = aEpa->pState;
    ans = FALSE;

    while (&esHsmTopState != tmpState) {

        if (aState == tmpState) {
            ans = TRUE;
            break;
        }

#if (defined(OPT_DBG_SP) && defined(OPT_KERNEL_ENABLE) && defined(OPT_KERNEL_USE_REGISTRY)) || defined(__DOXYGEN__)
        --dbgCnt;
        SP_ASSERT((uint8_t)0 != dbgCnt);
#endif
        (void)EVT_SIGNAL_SEND(aEpa, savedState, SIG_SUPER);
        tmpState = aEpa->pState;
    }
    aEpa->pState = savedState;                                                  /* vrati prethodno stanje automata */
    ES_CRITICAL_EXIT();

    return (ans);
}

esState_T esHsmTopState (
    esEpaHeader_T       * aEpa,
    esEvtHeader_T       * aEvt) {

#if defined(OPT_TRACE_ENABLE)
    /* trace struktura */
#else
    (void)aEpa;                                                                 /* Ukloni upozorenje o nekoriscenom parametru               */
    (void)aEvt;
#endif

    return SMP_STATE_IGNORED();
}

/** @} *//*--------------------------------------------------------------------------------------*/

void hsmInit (
    esEpaHeader_T       * aEpa,
    esPtrState_T        aInitState,
    esPtrState_T        * aStateBuff,
    size_t              aStateDepth) {

    SP_DBG_CHECK((size_t)1U < aStateDepth);                                     /* Provera par: da li je aStateDepth minimalne dubine?      */
    SP_DBG_CHECK((esPtrState_T)0U != aInitState);                               /* Provera par: da li je aInitState  minimalne dubine?      */
    aEpa->pState = aInitState;
    aEpa->internals.exec.pSrcStates = aStateBuff;
    aEpa->internals.exec.pDstStates = aStateBuff + aStateDepth;
}

/*-----------------------------------------------------------------------------------------------*/

void hsmDeInit(
    esEpaHeader_T       * aEpa) {

    aEpa->internals.exec.pSrcStates = (esPtrState_T *)0U;
    aEpa->internals.exec.pDstStates = (esPtrState_T *)0U;
}


/*************************************************************************************************
 * LOCAL FUNCTION DEFINITIONS
 *************************************************************************************************/


/*************************************************************************************************
 * CONFIGURATION ERRORS
 *************************************************************************************//** @cond */


/** @endcond *//** @} *//*************************************************************************
 * END of sproc.c
 *************************************************************************************************/
