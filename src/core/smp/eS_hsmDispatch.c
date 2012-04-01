/*************************************************************************************************
 * This file is part of eSolid
 *
 * Copyright (C) 2011 - Nenad Radulovic
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
 *************************************************************************************************/


/*********************************************************************************************//**
 * @file
 *
 * @author      nesa
 *
 * @brief       Implementacija eS_hsmDispatch()
 *
 * ------------------------------------------------------------------------------------------------
 *
 * @addtogroup  smp_impl
 *
 ****************************************************************************************//** @{ */


/*************************************************************************************************
 * INCLUDE FILES
 *************************************************************************************************/

#include "../core_private.h"


/*************************************************************************************************
 * LOCAL DEFINES
 *************************************************************************************************/

/*-----------------------------------------------------------------------------------------------*
 * Local debug defines
 *-----------------------------------------------------------------------------------*//** @cond */
SMP_DBG_DEFINE_MODULE(eS_hsmDispatch);


/** @endcond *//*--------------------------------------------------------------------------------*
 * Local user defines
 *-----------------------------------------------------------------------------------------------*/


/*************************************************************************************************
 * LOCAL MACRO's
 *************************************************************************************************/


/*************************************************************************************************
 * LOCAL CONSTANTS
 *************************************************************************************************/


/*************************************************************************************************
 * LOCAL DATA TYPES
 *************************************************************************************************/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Tip za brojac stanja automata.
 */
/*-----------------------------------------------------------------------------------------------*/
typedef uint8_t dspaStCnt_T;


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
 ***                                I M P L E M E N T A T I O N                                ***
 *************************************************************************************************/


/*************************************************************************************************
 * GLOBAL FUNCTION DEFINITIONS
 *************************************************************************************************/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @ingroup     smp_intf
 */
/*-----------------------------------------------------------------------------------------------*/
eot_evt_T * eS_hsmDispatch (
    smp_exec_T      * aHsm,
    const eot_evt_T * aEvt) {

    SMP_FVAR smp_ptrState_T * srcState;                                  /* Cuvanje adresa izvorista.                                */
    SMP_FVAR smp_ptrState_T * dstState;                                  /* Cuvanje adresa odredista.                                */
    SMP_FVAR smp_ptrState_T tmpState;
    SMP_FVAR smp_state_T     state;
    SMP_FVAR uint_fast8_t    srcEnd;
    SMP_FVAR uint_fast8_t    dstEnd;
    SMP_FVAR uint_fast8_t    stateCnt;

    srcState = aHsm->pSrcStates;
    srcEnd = (dspaStCnt_T)0;

    while (TRUE) {                                                              /* Pozivanje f-ja sve dok vracaju SUPER.                    */
        tmpState = aHsm->pStateHandler;
        state = (smp_state_T)(*tmpState)(                                       /* Pozovi funkciju stanja:                                  */
            aHsm,                                                               /* Automat koji se obradjuje.                               */
            aEvt);                                                              /* Dobavi nov dogadjaj.                                     */
        SMP_ASSERT(HSM_VALID_STATE(state));

        if (RETN_SUPER != state) {
            break;                                                              /* break se koristi zbog brzog izvrsavanja petlje, FIXME: ovo nije tacna tvrdnja! */
        }
        srcState[srcEnd] = tmpState;
        ++srcEnd;
    };

    if (RETN_TRAN == state) {                                                   /* Da li treba izvrsiti tranziciju?                         */
        dstState = aHsm->pDstStates;
        dstState[0] = aHsm->pStateHandler;                                      /* sacuvaj destinaciju                                      */
        dstEnd = (dspaStCnt_T)1;

        if (tmpState == dstState[0]) {                                          /* tran: a) src ?== dst                                     */
            srcState[srcEnd] = tmpState;
            ++srcEnd;
        } else {                                                                /* tran: a) src != dst                                      */
            (void)SMP_RSRVEVT_SEND(aHsm, dstState[0], SIG_SUPER);
            dstState[1] = aHsm->pStateHandler;

            if (tmpState != dstState[1]) {                                      /* tran: b) src ?== super(dst)                              */
                /*dstEnd = (dspaStCnt_T)1;*/
            /*} else {*/                                                        /* tran: b) src !== super(dst)                              */
                (void)SMP_RSRVEVT_SEND(aHsm, tmpState, SIG_SUPER);
                srcState[srcEnd] = tmpState;
                tmpState = aHsm->pStateHandler;
                ++srcEnd;

                if (tmpState != dstState[1]) {                                  /* tran: c) super(src) ?== super(dst)                       */
                    /*dstEnd = (dspaStCnt_T)1;*/
                /*} else {*/                                                    /* tran: c) super(src) !== super(dst)                       */

                    if (tmpState == dstState[0]) {                              /* tran: d) super(src) ?== dst                              */
                        dstEnd = (dspaStCnt_T)0;
                    } else {
                        srcState[srcEnd] = tmpState;                            /* tran: e) src ?== ...super(super(dst))                    */
                        --srcEnd;
                        dstEnd = (dspaStCnt_T)2;
                        tmpState = dstState[1];

                        while ((smp_ptrState_T)&eS_hsmTopState != tmpState) {  /* dobavi super(dst) i sacuvaj u niz                  */
                            (void)SMP_RSRVEVT_SEND(aHsm, tmpState, SIG_SUPER);
                            tmpState = aHsm->pStateHandler;

                            if (srcState[srcEnd] == tmpState) {

                                goto EXECUTE_PATH;

                            }
                            dstState[dstEnd] = tmpState;
                            ++dstEnd;
                        }

                        ++srcEnd;
                        stateCnt = (dspaStCnt_T)2;
                        tmpState = srcState[srcEnd];

                        while (stateCnt != dstEnd) {                            /* tran: f) super(src) ?== ...super(super(dst))             */

                            if (tmpState == dstState[stateCnt]) {
                                dstEnd = stateCnt;

                                goto EXECUTE_PATH;

                            }
                            ++stateCnt;
                        };

                        while (TRUE) {                                          /* tran: g i h) ...super(super(src) ?== super(super(dst)    */
                            (void)SMP_RSRVEVT_SEND(aHsm, tmpState, SIG_SUPER);
                            tmpState = aHsm->pStateHandler;
                            ++srcEnd;
                            stateCnt = (dspaStCnt_T)0;

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

        stateCnt = (dspaStCnt_T)0;

        while (stateCnt != srcEnd) {                                            /* Izadji iz hijerarhije.                                   */

#if defined(OPT_DBG_SMP)

            state = (smp_state_T)SMP_RSRVEVT_SEND(aHsm, srcState[stateCnt], SIG_EXIT);
            SMP_ASSERT((RETN_SUPER == state) || (RETN_HANDLED == state));

#else

            (void)SMP_RSRVEVT_SEND(aHsm, srcState[stateCnt], SIG_EXIT);

#endif
            ++stateCnt;
        }

        while (TRUE) {

            while ((dspaStCnt_T)0 != dstEnd) {                                  /* Udji u novu hijerarhiju.                                 */
                --dstEnd;
                state = (smp_state_T)SMP_RSRVEVT_SEND(aHsm, dstState[dstEnd], SIG_ENTRY);
                SMP_ASSERT(((RETN_SUPER == state) || (RETN_NOEX == state) || (RETN_HANDLED == state)));
            }

            if (RETN_NOEX == state) {
                aHsm->pStateHandler = dstState[0];

                return ((eot_evt_T *)&smp_controlEvt[SIG_NOEX]);
            }
            state = (smp_state_T)SMP_RSRVEVT_SEND(aHsm, dstState[0], SIG_INIT);
            SMP_ASSERT((RETN_TRAN == state) || (RETN_SUPER == state));

            if (RETN_TRAN != state) {

                break;
            }
            dstEnd = (dspaStCnt_T)0;
            tmpState = dstState[0];
            dstState[0] = aHsm->pStateHandler;

            while (dstState[dstEnd] != tmpState) {
                SMP_ASSERT((dspaStCnt_T)SMP_STATE_DEPTH != dstEnd);
                (void)SMP_RSRVEVT_SEND(aHsm, dstState[dstEnd], SIG_SUPER);
                ++dstEnd;
                dstState[dstEnd] = aHsm->pStateHandler;
            }
        }
        aHsm->pStateHandler = dstState[0];
    } else if (RETN_HANDLED == state) {
        aHsm->pStateHandler = srcState[0];                                      /* Vrati izvorno stanje.                                    */
    } else if (RETN_NOEX == state) {                                            /* dst ?== flowchart                                        */

        return ((eot_evt_T *)&smp_controlEvt[SIG_NOEX]);
    } else if (RETN_DEFERRED == state) {
        aHsm->pStateHandler = srcState[0];                                      /* Vrati izvorno stanje.                                    */

        return ((eot_evt_T *)aEvt);
    } else {                                                                    /* RETN_IGNORED                                             */
        aHsm->pStateHandler = srcState[0];                                      /* Vrati izvorno stanje.                                    */
    }

    return ((eot_evt_T *)0U);                                                   /* Oznaci da je dogadjaj obradjen.                          */
}


/*************************************************************************************************
 * LOCAL FUNCTION DEFINITIONS
 *************************************************************************************************/


/*************************************************************************************************
 * CONFIGURATION ERRORS
 *************************************************************************************//** @cond */


/** @endcond *//** @} *//*************************************************************************
 * END of eS_hsmDispatch.c
 *************************************************************************************************/
