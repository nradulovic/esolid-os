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
 * @brief       Implementacija Event Object modula.
 * @todo        Treba da se srede inline i obicne funkcije. Ideja je da oba seta
 *              funkcija imaju isti interfejs, pa bi se prostim makroima
 *              ukljucivalo/iskljucivalo inline funkcija.
 * ------------------------------------------------------------------------------------------------
 * @addtogroup  evt_impl
 ****************************************************************************************//** @{ */


/*============================================================================  INCLUDE FILES  ==*/
#define EVT_PKG_H_VAR
#include "core_private.h"

/*============================================================================  LOCAL DEFINES  ==*/
/*-------------------------------------------------------------------------------------------*//**
 * @brief       Local debug define macro.
 *//*--------------------------------------------------------------------------------------------*/
EO_DBG_DEFINE_MODULE(Event Object);

/*============================================================================  LOCAL MACRO's  ==*/
/*=========================================================================  LOCAL DATA TYPES  ==*/
/*================================================================  LOCAL FUNCTION PROTOTYPES  ==*/

C_INLINE void evtQStatEvtRemoved_(
    evtQueue_T    * aEvtQueue);

C_INLINE void evtQStatEvtAdded_(
    evtQueue_T    * aEvtQueue);

C_INLINE void evtQPutAheadI_(
    evtQueue_T          * aEvtQueue,
    esEvtHeader_T       * aEvt);

C_INLINE void evtQPutI_(
    evtQueue_T          * aEvtQueue,
    esEvtHeader_T       * aEvt);

C_INLINE void evtInit_(
    esEvtHeader_T       * aNewEvt,
    size_t              aDataSize,
    esEvtId_T           aEvtId);

/*==========================================================================  LOCAL VARIABLES  ==*/
/*=========================================================================  GLOBAL VARIABLES  ==*/
/*-------------------------------------------------------------------------------------------*//**
 * @brief       Tabela signalnih dogadjaja
 *//*--------------------------------------------------------------------------------------------*/
const C_ROM evtIntr_T evtSignal[] = {
    {(esEvtId_T)SIG_EMPTY,
    EVT_RESERVED_MASK | EVT_STATIC_MASK,
#if defined(OPT_DBG_EO) && defined(OPT_DBG_USE_CHECK) || defined(__DOXYGEN__)
    EVT_SIGNATURE
#endif
    },
    {(esEvtId_T)SIG_ENTRY,
    EVT_RESERVED_MASK | EVT_STATIC_MASK,
#if defined(OPT_DBG_EO) && defined(OPT_DBG_USE_CHECK) || defined(__DOXYGEN__)
    EVT_SIGNATURE
#endif
    },
    {(esEvtId_T)SIG_EXIT,
    EVT_RESERVED_MASK | EVT_STATIC_MASK,
#if defined(OPT_DBG_EO) && defined(OPT_DBG_USE_CHECK) || defined(__DOXYGEN__)
    EVT_SIGNATURE
#endif
    },
    {(esEvtId_T)SIG_INIT,
    EVT_RESERVED_MASK | EVT_STATIC_MASK,
#if defined(OPT_DBG_EO) && defined(OPT_DBG_USE_CHECK) || defined(__DOXYGEN__)
    EVT_SIGNATURE
#endif
    },
    {(esEvtId_T)SIG_SUPER,
    EVT_RESERVED_MASK | EVT_STATIC_MASK,
#if defined(OPT_DBG_EO) && defined(OPT_DBG_USE_CHECK) || defined(__DOXYGEN__)
    EVT_SIGNATURE
#endif
    },
    {(esEvtId_T)SIG_NOEX,
     EVT_RESERVED_MASK | EVT_STATIC_MASK,
#if defined(OPT_DBG_EO) && defined(OPT_DBG_USE_CHECK) || defined(__DOXYGEN__)
     EVT_SIGNATURE
#endif
    },
};

/*===============================================================  LOCAL FUNCTION DEFINITIONS  ==*/

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Konstruktor funkcija dogadjaja.
 * @param       aNewEvt                 Pokazivac na dogadjaj koji se konstruise,
 * @param       aDataSize               velicina dogadjaja,
 * @param       aEvtId                  identifikator dogadjaja.
 * @notapi
 *//*--------------------------------------------------------------------------------------------*/
C_INLINE void evtInit_(
    esEvtHeader_T       * aNewEvt,
    size_t              aDataSize,
    esEvtId_T           aEvtId) {

    aNewEvt->internals.dynamic = (evtDynamic_T)0U;                              /* Dogadjaj je dinamican, sa 0 korisnika.                   */

#if defined(OPT_DBG_EO) && defined(OPT_DBG_USE_CHECK)
    aNewEvt->internals.signature = EVT_SIGNATURE;
#endif
    aNewEvt->internals.id = aEvtId;

#if defined(OPT_EVT_USE_TRANSMITTER)
    aNewEvt->transmitter = esEpaGetId();
#endif

#if defined(OPT_EVT_USE_TIME)
    aNewEvt->time = EVT_TIMESTAMP_GET();
#endif

#if defined(OPT_EVT_USE_SIZE)
    aNewEvt->size = (esEvtSize_T)aDataSize;
#endif

#if defined(OPT_EVT_USE_TYPE)
    aNewEvt->type = (esEvtType_T)EVT_TYPE_UNSPEC;
#endif
}

/*-------------------------------------------------------------------------------------------*//**
 * @name        Direktan, interni pristup redu za cekanje
 * @{ *//*---------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @brief       Dobavlja dogadjaj iz reda za cekanje
 * @param       aEvtQueue               Pokazivac na red za cekanje.
 * @return      Dogadjaj sa pocetka reda za cekanje.
 *//*--------------------------------------------------------------------------------------------*/
C_INLINE esEvtHeader_T * evtQGetI_(
    evtQueue_T          * aEvtQueue) {

    esEvtHeader_T * tmpEvt;

    EO_ASSERT((evtQueue_T *)0U != aEvtQueue);                                   /* Provera par: da li je aEvtQueue inicijalizovan?          */
    EO_ASSERT(FALSE == esQpIsEmpty(&(aEvtQueue->queue)));                       /* Provera: da li red sadrzi barem jedan element?           */
    tmpEvt = (esEvtHeader_T *)esQpGetI(
        &(aEvtQueue->queue));

#if defined(OPT_KERNEL_ENABLE)

    if (!(EVT_STATIC_MASK & tmpEvt->internals.dynamic)) {
        evtDynamic_T tmpR;
        evtDynamic_T tmpU;

        tmpR = tmpEvt->internals.dynamic & ~EVT_USERS_MASK;
        tmpU = (tmpEvt->internals.dynamic - 1U) & EVT_USERS_MASK;
        tmpEvt->internals.dynamic = tmpR | tmpU;
    }

# if defined(OPT_KERNEL_USE_REGISTRY) || defined(__DOXYGEN__)
    evtQStatEvtRemoved_(
        aEvtQueue);
# endif
#endif
    return (tmpEvt);
}

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Postavlja dogadjaj na kraju reda za cekanje
 * @param       aEvtQueue               Pokazivac na red za cekanje
 * @param       aEvt                    dogadjaj koji se postavlja u red.
 *//*--------------------------------------------------------------------------------------------*/
C_INLINE void evtQPutI_(
    evtQueue_T          * aEvtQueue,
    esEvtHeader_T       * aEvt) {

    EO_DBG_CHECK((evtQueue_T *)0U != aEvtQueue);                                /* Provera par: da li je aEvtQueue inicijalizovan?          */
    EO_DBG_CHECK((esEvtHeader_T *)0U != aEvt);                                  /* Provera par: da li je aEvt inicijalizovan?               */
    EO_ASSERT(FALSE == esQpIsFull(&(aEvtQueue->queue)));                        /* Provera: da li red moze da primi nov element?            */
    EO_DBG_CHECK(EVT_SIGNATURE == aEvt->internals.signature);                   /* Provera par: da li je dogadjaj validan?                  */
    esQpPutI(
        &(aEvtQueue->queue),
        (void *)aEvt);

#if defined(OPT_KERNEL_ENABLE)

    if ((evtDynamic_T)0U != (EVT_STATIC_MASK & aEvt->internals.dynamic)) {
        evtDynamic_T tmpR;
        evtDynamic_T tmpU;

        tmpR = aEvt->internals.dynamic & ~EVT_USERS_MASK;
        tmpU = (aEvt->internals.dynamic - 1U) & EVT_USERS_MASK;
        aEvt->internals.dynamic = tmpR | tmpU;
    }
                                                                                /* Povecaj info o zauzecu reda cekanja.                     */
# if defined(OPT_KERNEL_USE_REGISTRY) || defined(__DOXYGEN__)
    evtQStatEvtAdded_(
        aEvtQueue);
# endif
#endif
}

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Postavlja dogadjaj na pocetku reda za cekanje
 * @param       aEvtQueue               Pokazivac na red za cekanje
 * @param       aEvt                    dogadjaj koji se postavlja u red.
 *//*--------------------------------------------------------------------------------------------*/
C_INLINE void evtQPutAheadI_(
    evtQueue_T          * aEvtQueue,
    esEvtHeader_T       * aEvt) {

    EO_DBG_CHECK((evtQueue_T *)0U != aEvtQueue);                                /* Provera par: da li je aEvtQueue inicijalizovan?          */
    EO_DBG_CHECK((esEvtHeader_T *)0U != aEvt);                                  /* Provera par: da li je aEvt inicijalizovan?               */
    EO_ASSERT(FALSE == esQpIsFull(&(aEvtQueue->queue)));                        /* Provera: da li red moze da primi nov element?            */
    EO_DBG_CHECK(EVT_SIGNATURE == aEvt->internals.signature);                   /* Provera par: da li je dogadjaj validan?                  */
    esQpPutAheadI(
        &(aEvtQueue->queue),
        (void *)aEvt);

#if defined(OPT_KERNEL_ENABLE)

    if ((evtDynamic_T)0U != (EVT_STATIC_MASK & aEvt->internals.dynamic)) {
        evtDynamic_T tmpR;
        evtDynamic_T tmpU;

        tmpR = aEvt->internals.dynamic & ~EVT_USERS_MASK;
        tmpU = (aEvt->internals.dynamic - 1U) & EVT_USERS_MASK;
        aEvt->internals.dynamic = tmpR | tmpU;
    }
                                                                                /* Povecaj info o zauzecu reda cekanja.                     */
# if defined(OPT_KERNEL_USE_REGISTRY) || defined(__DOXYGEN__)
    evtQStatEvtAdded_(
        aEvtQueue);
# endif
#endif
}

#if defined(OPT_KERNEL_ENABLE) && defined(OPT_KERNEL_USE_REGISTRY) || defined(__DOXYGEN__)
/*-------------------------------------------------------------------------------------------*//**
 * @brief       Statistika reda za cekanje - dodavanje dogadjaja
 * @param       aEvtQueue               Pokazivac na strukturu reda za cekanje.
 * @notapi
 *//*--------------------------------------------------------------------------------------------*/
C_INLINE void evtQStatEvtAdded_(
    evtQueue_T    * aEvtQueue) {

    --aEvtQueue->freeCurr;

    if (aEvtQueue->freeCurr < aEvtQueue->freeMin) {
        aEvtQueue->freeMin = aEvtQueue->freeCurr;
    }
}

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Statistika reda za cekanje - uklanjanje dogadjaja
 * @param       aEvtQueue               Pokazivac na strukturu reda za cekanje.
 * @notapi
 *//*--------------------------------------------------------------------------------------------*/
C_INLINE void evtQStatEvtRemoved_(
    evtQueue_T    * aEvtQueue) {

    ++aEvtQueue->freeCurr;
}
#endif
/** @} *//*--------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
esEvtHeader_T * evtQGetI(
    esEpaHeader_T       * aEpa) {

    esEvtHeader_T * tmpEvt;

    tmpEvt = evtQGetI_(
        &(aEpa->internals.evtQueue));

    return (tmpEvt);
}

/*-----------------------------------------------------------------------------------------------*/
void evtQPutAheadI(
    esEpaHeader_T       * aEpa,
    esEvtHeader_T       * aEvt) {

    evtQPutAheadI_(
        &(aEpa->internals.evtQueue),
        aEvt);
}

/*-----------------------------------------------------------------------------------------------*/
void evtQPutAhead(
    esEpaHeader_T       * aEpa,
    esEvtHeader_T       * aEvt) {

    HAL_CRITICAL_DECL();

    HAL_CRITICAL_ENTER();

#if defined(OPT_OPTIMIZE_SIZE)
    evtQPutAheadI(
        aEpa,
        aEvt);
#else
    evtQPutAheadI_(
        &(aEpa->internals.evtQueue),
        aEvt);
#endif
    HAL_CRITICAL_EXIT();
}

/*-----------------------------------------------------------------------------------------------*/
void evtQPutI(
    esEpaHeader_T       * aEpa,
    esEvtHeader_T       * aEvt) {

    evtQPutI_(
        &(aEpa->internals.evtQueue),
        aEvt);
}

/*-----------------------------------------------------------------------------------------------*/
void evtQPut(
    esEpaHeader_T       * aEpa,
    esEvtHeader_T       * aEvt) {

    HAL_CRITICAL_DECL();

    HAL_CRITICAL_ENTER();

#if defined(OPT_OPTIMIZE_SIZE)
    evtQPutI(
        aEpa,
        aEvt);
#else
    evtQPutI_(
        &(aEpa->internals.evtQueue),
        aEvt);
#endif
    HAL_CRITICAL_EXIT();
}

/*-----------------------------------------------------------------------------------------------*/
void evtQInit(
    esEpaHeader_T       * aEpa,
    esEvtHeader_T       ** aEvtBuff,
    size_t              aQueueSize) {

    EO_DBG_CHECK((size_t)1U <= aQueueSize);                                     /* Provera par: da li je zahtevana velicina barem za 1      */
                                                                                /* element?                                                 */
    EO_DBG_CHECK((esEvtHeader_T **)0U != aEvtBuff);                             /* Provera: da li je rezervisana memorija?                  */
    esQpInit(
        &(aEpa->internals.evtQueue.queue),
        (void **)aEvtBuff,
        aQueueSize);

#if defined(OPT_DBG_EO) || (defined(OPT_KERNEL_ENABLE) && defined(OPT_KERNEL_USE_REGISTRY)) || defined(__DOXYGEN__)
    aEpa->internals.evtQueue.freeCurr = esQpFreeSpace(
        &(aEpa->internals.evtQueue.queue));
    aEpa->internals.evtQueue.freeMin = aEpa->internals.evtQueue.freeCurr;
#endif
}

/*-----------------------------------------------------------------------------------------------*/
void evtQDeInit(
    esEpaHeader_T       * aEpa) {

    HAL_CRITICAL_DECL();
    esEvtHeader_T * tmpEvt;

    HAL_CRITICAL_ENTER();

    while (FALSE == esQpIsEmpty(&(aEpa->internals.evtQueue.queue))) {

#if defined(OPT_OPTIMIZE_SPEED) || defined(__DOXYGEN__)
        tmpEvt = evtQGetI_(
            &(aEpa->internals.evtQueue));
#else
        tmpEvt = evtQGetI(aEpa);
#endif
        evtDestroyI_(
            tmpEvt);
    }
    HAL_CRITICAL_EXIT();
    esQpDeInit(
        &(aEpa->internals.evtQueue.queue));
}

/*==============================================================  GLOBAL FUNCTION DEFINITIONS  ==*/
/*-------------------------------------------------------------------------------------------*//**
 * @ingroup         evt_intf
 * @{ *//*---------------------------------------------------------------------------------------*/
esEvtHeader_T * esEvtCreate(
    size_t              dataSize,
    esEvtId_T           id) {

    esEvtHeader_T * newEvt;

    EO_DBG_CHECK(sizeof(esEvtHeader_T) <= dataSize);                            /* Provera par: da li dataSize moze da sadrzi zaglavlje?    */
    ES_ATOMIC(newEvt = (esEvtHeader_T *)esHmemAllocI(dataSize));                /* Dobavi potreban memorijski prostor za dogadjaj           */
    DBG_ASSERT((esEvtHeader_T *)0U != newEvt);                                  /* Provera: da li je mem. prostor rezervisan za dogadjaj?   */
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

    EO_DBG_CHECK(sizeof(esEvtHeader_T) <= dataSize);                            /* Provera par: da li dataSize moze da sadrzi zaglavlje?    */
    newEvt = (esEvtHeader_T *)esHmemAllocI(dataSize);                           /* Dobavi potreban memorijski prostor za dogadjaj           */
    DBG_ASSERT((esEvtHeader_T *)0 != newEvt);                                   /* Provera: da li je mem. prostor rezervisan za dogadjaj?   */
    evtInit_(
        newEvt,
        dataSize,
        id);

    return (newEvt);
}

/*-----------------------------------------------------------------------------------------------*/
void esEvtReserve(
    esEvtHeader_T       * aEvt) {

    EO_DBG_CHECK((esEvtHeader_T *)0U != aEvt);                                  /* Provera par: da li je aEvt inicijalizovan?               */
    aEvt->internals.dynamic |= EVT_RESERVED_MASK;
}

/*-----------------------------------------------------------------------------------------------*/
void esEvtUnReserve(
    esEvtHeader_T       * aEvt) {

    EO_DBG_CHECK((esEvtHeader_T *)0U != aEvt);                                  /* Provera par: da li je aEvt inicijalizovan?               */
    aEvt->internals.dynamic &= ~EVT_RESERVED_MASK;

    if ((evtDynamic_T)0U == aEvt->internals.dynamic) {

#if defined(OPT_DBG_EO) && defined(OPT_DBG_USE_CHECK)
        aEvt->internals.signature = 0xDEAD;
#endif
        esHmemDeAlloc((void *)aEvt);
    }
}

/*-----------------------------------------------------------------------------------------------*/
void esEvtPost(
    esEpaHeader_T       * aEpa,
    esEvtHeader_T       * aEvt) {

    HAL_CRITICAL_DECL();

    EO_DBG_CHECK((esEpaHeader_T *)0U != aEpa);                                  /* Provera par: da li je aEpa inicijalizovan?               */
    HAL_CRITICAL_ENTER();

#if defined(OPT_OPTIMIZE_SIZE)
    evtQPutI(
        aEpa,
        aEvt);
    schedRdyInsertI(
        aEpa);
#elif defined(OPT_OPTIMIZE_SPEED)
    evtQPutI_(
        &(aEpa->internals.evtQueue),
        aEvt);
    schedRdyInsertI_(
        aEpa);
#else
    evtQPutI(
        aEpa,
        aEvt);
    schedRdyInsertI_(
        aEpa);
#endif
    HAL_CRITICAL_EXIT();
    EPE_SCHED_NOTIFY_RDY();
}

/*-----------------------------------------------------------------------------------------------*/
void esEvtPostAhead(
    esEpaHeader_T       * aEpa,
    esEvtHeader_T       * aEvt) {

    HAL_CRITICAL_DECL();

    EO_DBG_CHECK((esEpaHeader_T *)0U != aEpa);                                  /* Provera par: da li je aEpa inicijalizovan?               */
    HAL_CRITICAL_ENTER();

#if defined(OPT_OPTIMIZE_SIZE)
    evtQPutAheadI(
        aEpa,
        aEvt);
    schedRdyInsertI(
        aEpa);
#elif defined(OPT_OPTIMIZE_SPEED)
    evtQPutAheadI_(
        &(aEpa->internals.evtQueue),
        aEvt);
    schedRdyInsertI_(
        aEpa);
#else
    evtQPutAheadI(
        aEpa,
        aEvt);
    schedRdyInsertI_(
        aEpa);
#endif
    HAL_CRITICAL_EXIT();
    EPE_SCHED_NOTIFY_RDY();
}

/** @} *//*--------------------------------------------------------------------------------------*/

/*===================================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/

/** @endcond *//** @} *//*************************************************************************
 * END of evt.c
 *************************************************************************************************/
