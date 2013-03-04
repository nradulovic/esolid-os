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
 * @brief       Implementacija Event Object podmodula.
 * @addtogroup  evt_impl
 ****************************************************************************************//** @{ */

/*============================================================================  INCLUDE FILES  ==*/
#define EVT_PKG_H_VAR
#include "kernel_private.h"

/*============================================================================  LOCAL DEFINES  ==*/
/*============================================================================  LOCAL MACRO's  ==*/
/*=========================================================================  LOCAL DATA TYPES  ==*/

typedef struct evtQueue evtQueue_T;

/*================================================================  LOCAL FUNCTION PROTOTYPES  ==*/

C_INLINE_ALWAYS void evtInit_(
    esEvtHeader_T       * aNewEvt,
    size_t              aDataSize,
    esEvtId_T           aEvtId);

C_INLINE void evtQPutI_(
    esEpaHeader_T       * aEpa,
    esEvtHeader_T       * aEvt);

C_INLINE void evtQPutAheadI_(
    esEpaHeader_T       * aEpa,
    esEvtHeader_T       * aEvt);

/*==========================================================================  LOCAL VARIABLES  ==*/
/*=========================================================================  GLOBAL VARIABLES  ==*/

/**
 * @brief       Tabela signalnih dogadjaja
 */
const C_ROM esEvtHeader_T evtSignal[] = {
    {(esEvtId_T)SIG_EMPTY,
    EVT_RESERVED_MASK | EVT_CONST_MASK,
#if defined(OPT_KERNEL_DBG_EVT) && defined(OPT_DBG_USE_CHECK) || defined(__DOXYGEN__)
    EVT_SIGNATURE
#endif
    },
    {(esEvtId_T)SIG_ENTRY,
    EVT_RESERVED_MASK | EVT_CONST_MASK,
#if defined(OPT_KERNEL_DBG_EVT) && defined(OPT_DBG_USE_CHECK) || defined(__DOXYGEN__)
    EVT_SIGNATURE
#endif
    },
    {(esEvtId_T)SIG_EXIT,
    EVT_RESERVED_MASK | EVT_CONST_MASK,
#if defined(OPT_KERNEL_DBG_EVT) && defined(OPT_DBG_USE_CHECK) || defined(__DOXYGEN__)
    EVT_SIGNATURE
#endif
    },
    {(esEvtId_T)SIG_INIT,
    EVT_RESERVED_MASK | EVT_CONST_MASK,
#if defined(OPT_KERNEL_DBG_EVT) && defined(OPT_DBG_USE_CHECK) || defined(__DOXYGEN__)
    EVT_SIGNATURE
#endif
    },
    {(esEvtId_T)SIG_SUPER,
    EVT_RESERVED_MASK | EVT_CONST_MASK,
#if defined(OPT_KERNEL_DBG_EVT) && defined(OPT_DBG_USE_CHECK) || defined(__DOXYGEN__)
    EVT_SIGNATURE
#endif
    }
};

/*===============================================================  LOCAL FUNCTION DEFINITIONS  ==*/

/**
 * @brief       Inicijalizator funkcija dogadjaja.
 * @param       aNewEvt                 Pokazivac na dogadjaj koji se konstruise,
 * @param       aDataSize               velicina dogadjaja,
 * @param       aEvtId                  identifikator dogadjaja.
 */
C_INLINE_ALWAYS void evtInit_(
    esEvtHeader_T       * aNewEvt,
    size_t              aDataSize,
    esEvtId_T           aEvtId) {

    aNewEvt->id = aEvtId;
    aNewEvt->dynamic = (uint_fast8_t)0U;                                        /* Dogadjaj je dinamican, sa 0 korisnika.                   */

#if defined(OPT_KERNEL_DBG_EVT) && defined(OPT_DBG_USE_CHECK)
    aNewEvt->signature = EVT_SIGNATURE;
#endif

#if defined(OPT_EVT_USE_GENERATOR)
    aNewEvt->generator = esEpaHeaderGet();
#endif

#if defined(OPT_EVT_USE_SIZE)
    aNewEvt->size = (esEvtSize_T)aDataSize;
#else
    (void)aDataSize;
#endif
}

/*-------------------------------------------------------------------------------------------*//**
 * @name        Direktan, interni pristup redu za cekanje
 * @{ *//*---------------------------------------------------------------------------------------*/

/**
 * @brief       Postavlja dogadjaj na kraju reda za cekanje
 * @param       aEpa                    Pokazivac na EPA objekat
 * @param       aEvt                    dogadjaj koji se postavlja u red.
 */
C_INLINE void evtQPutI_(
    esEpaHeader_T       * aEpa,
    esEvtHeader_T       * aEvt) {

    if (FALSE == esQpIsFull_(&(aEpa->evtQueue.queue))) {

#if defined(OPT_EVT_USE_TIMESTAMP)
        aEvt->timestamp = uTimestampGet();
#endif
        esQpPut_(
            &(aEpa->evtQueue.queue),
            (void *)aEvt);

        if ((uint_fast8_t)0U == (EVT_CONST_MASK & aEvt->dynamic)) {                 /* Da li je dogadjaj dinamičan?                             */
            uint_fast8_t tmpR;
            uint_fast8_t tmpU;

            tmpR = aEvt->dynamic & ~EVT_USERS_MASK;
            tmpU = aEvt->dynamic & EVT_USERS_MASK;
            ++tmpU;
            aEvt->dynamic = tmpR | tmpU;
        }

#if defined(OPT_KERNEL_DBG_EVT)
    --(aEpa->evtQueue.freeCurr);
    if (aEpa->evtQueue.freeCurr < aEpa->evtQueue.freeMin) {
        aEpa->evtQueue.freeMin = aEpa->evtQueue.freeCurr;
    }
#endif
    schedRdyInsertI_(
        aEpa);
    } else {
    }
}

/**
 * @brief       Postavlja dogadjaj na pocetku reda za cekanje
 * @param       aEpa                    Pokazivac na EPA objekat
 * @param       aEvt                    dogadjaj koji se postavlja u red.
 */
C_INLINE void evtQPutAheadI_(
    esEpaHeader_T       * aEpa,
    esEvtHeader_T       * aEvt) {

    if (FALSE == esQpIsFull_(&(aEpa->evtQueue.queue))) {

#if defined(OPT_EVT_USE_TIMESTAMP)
        aEvt->timestamp = uTimestampGet();
#endif
        esQpPutAhead_(
            &(aEpa->evtQueue.queue),
            (void *)aEvt);

        if ((uint_fast8_t)0U == (EVT_CONST_MASK & aEvt->dynamic)) {
            uint_fast8_t tmpR;
            uint_fast8_t tmpU;

            tmpR = aEvt->dynamic & ~EVT_USERS_MASK;
            tmpU = aEvt->dynamic & EVT_USERS_MASK;
            ++tmpU;
            aEvt->dynamic = tmpR | tmpU;
        }

#if defined(OPT_KERNEL_DBG_EVT)
    --(aEpa->evtQueue.freeCurr);
    if (aEpa->evtQueue.freeCurr < aEpa->evtQueue.freeMin) {
        aEpa->evtQueue.freeMin = aEpa->evtQueue.freeCurr;
    }
#endif
    schedRdyInsertI_(
        aEpa);
    } else {
    }

}

/** @} *//*--------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------*/
esEvtHeader_T * evtQGetI(
    esEpaHeader_T       * aEpa) {

    esEvtHeader_T * tmpEvt;

    tmpEvt = (esEvtHeader_T *)esQpGet_(
        &(aEpa->evtQueue.queue));

    if ((uint_fast8_t)0U == (EVT_CONST_MASK & tmpEvt->dynamic)) {               /* Da li je dogadjaj dinamičan?                             */
        uint_fast8_t tmpR;
        uint_fast8_t tmpU;

        tmpR = tmpEvt->dynamic & ~EVT_USERS_MASK;
        tmpU = tmpEvt->dynamic & EVT_USERS_MASK;
        --tmpU;
        tmpEvt->dynamic = tmpR | tmpU;
    }

    if (TRUE == esQpIsEmpty_(&(aEpa->evtQueue.queue))) {
        schedRdyRmI_(
            aEpa);
    }

#if defined(OPT_KERNEL_DBG_EVT)
    ++(aEpa->evtQueue.freeCurr);
#endif

    return (tmpEvt);
}

/*-----------------------------------------------------------------------------------------------*/
void evtQInit(
    esEpaHeader_T       * aEpa,
    esEvtHeader_T       ** aEvtBuff,
    size_t              aQueueSize) {

    esQpInit_(
        &(aEpa->evtQueue.queue),
        (void **)aEvtBuff,
        aQueueSize);

#if defined(OPT_KERNEL_DBG_EVT)
    aEpa->evtQueue.freeCurr = esQpFreeSpace_(
        &(aEpa->evtQueue.queue));
    aEpa->evtQueue.freeMin = aEpa->evtQueue.freeCurr;
#endif
}

/*-----------------------------------------------------------------------------------------------*/
void evtQDeInit(
    esEpaHeader_T       * aEpa) {

    esEvtHeader_T * tmpEvt;

    while (FALSE == esQpIsEmpty_(&(aEpa->evtQueue.queue))) {
        tmpEvt = esQpGet_(
            &(aEpa->evtQueue.queue));
        evtDestroyI_(
            tmpEvt);
    }
    esQpDeInit_(
        &(aEpa->evtQueue.queue));

#if defined(OPT_KERNEL_DBG_EVT)
    aEpa->evtQueue.freeMin = aEpa->evtQueue.freeCurr = 0U;
#endif
}

/*==============================================================  GLOBAL FUNCTION DEFINITIONS  ==*/

/*-------------------------------------------------------------------------------------------*//**
 * @ingroup         evt_intf
 * @{ *//*---------------------------------------------------------------------------------------*/
esEvtHeader_T * esEvtCreate(
    size_t              dataSize,
    esEvtId_T           id) {

    ES_CRITICAL_DECL();
    esEvtHeader_T * newEvt;

    ES_CRITICAL_ENTER(
        OPT_KERNEL_INTERRUPT_PRIO_MAX);
    newEvt = (esEvtHeader_T *)esHmemAllocI(dataSize);                           /* Dobavi potreban memorijski prostor za dogadjaj           */
    ES_CRITICAL_EXIT();
    evtInit_(
        newEvt,
        dataSize,
        id);

    return (newEvt);
}

/*-----------------------------------------------------------------------------------------------*/
esEvtHeader_T * esEvtCreateI(
    size_t              dataSize,
    esEvtId_T           id) {

    esEvtHeader_T * newEvt;

    newEvt = (esEvtHeader_T *)esHmemAllocI(dataSize);                           /* Dobavi potreban memorijski prostor za dogadjaj           */
    evtInit_(
        newEvt,
        dataSize,
        id);

    return (newEvt);
}

/*-----------------------------------------------------------------------------------------------*/
void esEvtReserve(
    esEvtHeader_T       * aEvt) {

    aEvt->dynamic |= EVT_RESERVED_MASK;
}

/*-----------------------------------------------------------------------------------------------*/
void esEvtUnReserve(
    esEvtHeader_T       * aEvt) {

    aEvt->dynamic &= ~EVT_RESERVED_MASK;
}

/*-----------------------------------------------------------------------------------------------*/
void esEvtPost(
    esEpaHeader_T       * aEpa,
    esEvtHeader_T       * aEvt) {

    ES_CRITICAL_DECL();

    ES_CRITICAL_ENTER(OPT_KERNEL_INTERRUPT_PRIO_MAX);
    evtQPutI_(
       aEpa,
       aEvt);
    ES_CRITICAL_EXIT();
}

/*-----------------------------------------------------------------------------------------------*/
void esEvtPostI(
    esEpaHeader_T       * aEpa,
    esEvtHeader_T       * aEvt) {

    evtQPutI_(
       aEpa,
       aEvt);
}

/*-----------------------------------------------------------------------------------------------*/
void esEvtPostAhead(
    esEpaHeader_T       * aEpa,
    esEvtHeader_T       * aEvt) {

    ES_CRITICAL_DECL();

    ES_CRITICAL_ENTER(OPT_KERNEL_INTERRUPT_PRIO_MAX);
    evtQPutAheadI_(
        aEpa,
        aEvt);
    ES_CRITICAL_EXIT();
}

/*-----------------------------------------------------------------------------------------------*/
void esEvtPostAheadI(
    esEpaHeader_T       * aEpa,
    esEvtHeader_T       * aEvt) {

    evtQPutAheadI_(
        aEpa,
        aEvt);
}

/** @} *//*--------------------------------------------------------------------------------------*/

/*===================================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/

/** @endcond *//** @} *//*************************************************************************
 * END of evt.c
 *************************************************************************************************/
