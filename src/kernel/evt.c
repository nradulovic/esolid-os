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
#include "kernel_private.h"

/*============================================================================  LOCAL DEFINES  ==*/
/*-------------------------------------------------------------------------------------------*//**
 * @brief       Local debug define macro.
 *//*--------------------------------------------------------------------------------------------*/
EVT_DBG_DEFINE_MODULE(Event Object);

/*============================================================================  LOCAL MACRO's  ==*/
/*=========================================================================  LOCAL DATA TYPES  ==*/
/*================================================================  LOCAL FUNCTION PROTOTYPES  ==*/

C_INLINE_ALWAYS void evtInit_(
    esEvtHeader_T       * aNewEvt,
    size_t              aDataSize,
    esEvtId_T           aEvtId);

C_INLINE_ALWAYS esEvtHeader_T * evtQGetI_(
    evtQueue_T          * aEvtQueue);

C_INLINE_ALWAYS void evtQPutI_(
    evtQueue_T          * aEvtQueue,
    esEvtHeader_T       * aEvt);

C_INLINE_ALWAYS void evtQPutAheadI_(
    evtQueue_T          * aEvtQueue,
    esEvtHeader_T       * aEvt);

static uint32_t evtQQueryEmptyI(
    evtQueue_T          * aEvtQueue);

/*==========================================================================  LOCAL VARIABLES  ==*/
/*=========================================================================  GLOBAL VARIABLES  ==*/
/*-------------------------------------------------------------------------------------------*//**
 * @brief       Tabela signalnih dogadjaja
 *//*--------------------------------------------------------------------------------------------*/
const C_ROM evtIntr_T evtSignal[] = {
    {(esEvtId_T)SIG_EMPTY,
    EVT_RESERVED_MASK | EVT_STATIC_MASK,
#if defined(OPT_KERNEL_DBG_EVT) && defined(OPT_DBG_USE_CHECK) || defined(__DOXYGEN__)
    EVT_SIGNATURE
#endif
    },
    {(esEvtId_T)SIG_ENTRY,
    EVT_RESERVED_MASK | EVT_STATIC_MASK,
#if defined(OPT_KERNEL_DBG_EVT) && defined(OPT_DBG_USE_CHECK) || defined(__DOXYGEN__)
    EVT_SIGNATURE
#endif
    },
    {(esEvtId_T)SIG_EXIT,
    EVT_RESERVED_MASK | EVT_STATIC_MASK,
#if defined(OPT_KERNEL_DBG_EVT) && defined(OPT_DBG_USE_CHECK) || defined(__DOXYGEN__)
    EVT_SIGNATURE
#endif
    },
    {(esEvtId_T)SIG_INIT,
    EVT_RESERVED_MASK | EVT_STATIC_MASK,
#if defined(OPT_KERNEL_DBG_EVT) && defined(OPT_DBG_USE_CHECK) || defined(__DOXYGEN__)
    EVT_SIGNATURE
#endif
    },
    {(esEvtId_T)SIG_SUPER,
    EVT_RESERVED_MASK | EVT_STATIC_MASK,
#if defined(OPT_KERNEL_DBG_EVT) && defined(OPT_DBG_USE_CHECK) || defined(__DOXYGEN__)
    EVT_SIGNATURE
#endif
    }
};

/*===============================================================  LOCAL FUNCTION DEFINITIONS  ==*/

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Konstruktor funkcija dogadjaja.
 * @param       aNewEvt                 Pokazivac na dogadjaj koji se konstruise,
 * @param       aDataSize               velicina dogadjaja,
 * @param       aEvtId                  identifikator dogadjaja.
 * @notapi
 *//*--------------------------------------------------------------------------------------------*/
C_INLINE_ALWAYS void evtInit_(
    esEvtHeader_T       * aNewEvt,
    size_t              aDataSize,
    esEvtId_T           aEvtId) {

    aNewEvt->internals.dynamic = (evtDynamic_T)0U;                              /* Dogadjaj je dinamican, sa 0 korisnika.                   */

#if defined(OPT_KERNEL_DBG_EVT) && defined(OPT_DBG_USE_CHECK)
    aNewEvt->internals.signature = EVT_SIGNATURE;
#endif
    aNewEvt->internals.id = aEvtId;

#if defined(OPT_EVT_USE_TRANSMITTER)
    aNewEvt->transmitter = esEpaGetHeader();
#endif

#if defined(OPT_EVT_USE_TIME)
    aNewEvt->time = EVT_TIMESTAMP_GET();
#endif

#if defined(OPT_EVT_USE_SIZE)
    aNewEvt->size = (esEvtSize_T)aDataSize;
#else
    (void)aDataSize;
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
C_INLINE_ALWAYS esEvtHeader_T * evtQGetI_(
    evtQueue_T          * aEvtQueue) {

    esEvtHeader_T * tmpEvt;

    EVT_ASSERT((evtQueue_T *)0U != aEvtQueue);                                   /* Provera par: da li je aEvtQueue inicijalizovan?          */
    EVT_ASSERT(FALSE == esQpIsEmpty(&(aEvtQueue->queue)));                       /* Provera: da li red sadrzi barem jedan element?           */
    ES_TRACE(
        STP_FILT_EVT_Q_0,
        txtEvtQget,
        aEvtQueue);
    tmpEvt = (esEvtHeader_T *)esQpGetI(
        &(aEvtQueue->queue));
    ES_TRACE(
        STP_FILT_EVT_Q_0,
        txtEvtQFree,
        evtQQueryEmptyI(aEvtQueue));

    if ((evtDynamic_T)0U == (EVT_STATIC_MASK & tmpEvt->internals.dynamic)) {
        evtDynamic_T tmpR;
        evtDynamic_T tmpU;

        /*
         * TODO: Mora da se postavi granicnih = 0, jer moze da dodje do overflow-a dok je dogadjaj rezervisan, a ovo se dekrementira
         */
        tmpR = tmpEvt->internals.dynamic & ~EVT_USERS_MASK;
        tmpU = (tmpEvt->internals.dynamic - 1U) & EVT_USERS_MASK;
        tmpEvt->internals.dynamic = tmpR | tmpU;
    }

    return (tmpEvt);
}

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Postavlja dogadjaj na kraju reda za cekanje
 * @param       aEvtQueue               Pokazivac na red za cekanje
 * @param       aEvt                    dogadjaj koji se postavlja u red.
 *//*--------------------------------------------------------------------------------------------*/
C_INLINE_ALWAYS void evtQPutI_(
    evtQueue_T          * aEvtQueue,
    esEvtHeader_T       * aEvt) {

    EVT_DBG_CHECK((evtQueue_T *)0U != aEvtQueue);                               /* Provera par: da li je aEvtQueue inicijalizovan?          */
    EVT_DBG_CHECK((esEvtHeader_T *)0U != aEvt);                                 /* Provera par: da li je aEvt inicijalizovan?               */
    EVT_ASSERT(FALSE == esQpIsFull(&(aEvtQueue->queue)));                       /* Provera: da li red moze da primi nov element?            */
    EVT_DBG_CHECK(EVT_SIGNATURE == aEvt->internals.signature);                  /* Provera par: da li je dogadjaj validan?                  */
    ES_TRACE(
        STP_FILT_EVT_Q_0,
        txtEvtQput,
        aEvtQueue,
        aEvt->internals.id);
    esQpPutI(
        &(aEvtQueue->queue),
        (void *)aEvt);
    ES_TRACE(
        STP_FILT_EVT_Q_0,
        txtEvtQFree,
        evtQQueryEmptyI(aEvtQueue));

    if ((evtDynamic_T)0U == (EVT_STATIC_MASK & aEvt->internals.dynamic)) {
        evtDynamic_T tmpR;
        evtDynamic_T tmpU;

        tmpR = aEvt->internals.dynamic & ~EVT_USERS_MASK;
        tmpU = (aEvt->internals.dynamic + 1U) & EVT_USERS_MASK;
        aEvt->internals.dynamic = tmpR | tmpU;
    }
}

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Postavlja dogadjaj na pocetku reda za cekanje
 * @param       aEvtQueue               Pokazivac na red za cekanje
 * @param       aEvt                    dogadjaj koji se postavlja u red.
 *//*--------------------------------------------------------------------------------------------*/
C_INLINE_ALWAYS void evtQPutAheadI_(
    evtQueue_T          * aEvtQueue,
    esEvtHeader_T       * aEvt) {

    EVT_DBG_CHECK((evtQueue_T *)0U != aEvtQueue);                               /* Provera par: da li je aEvtQueue inicijalizovan?          */
    EVT_DBG_CHECK((esEvtHeader_T *)0U != aEvt);                                 /* Provera par: da li je aEvt inicijalizovan?               */
    EVT_ASSERT(FALSE == esQpIsFull(&(aEvtQueue->queue)));                       /* Provera: da li red moze da primi nov element?            */
    EVT_DBG_CHECK(EVT_SIGNATURE == aEvt->internals.signature);                  /* Provera par: da li je dogadjaj validan?                  */
    ES_TRACE(
        STP_FILT_EVT_Q_0,
        txtEvtQputAhead,
        aEvtQueue,
        aEvt->internals.id);
    esQpPutAheadI(
        &(aEvtQueue->queue),
        (void *)aEvt);
    ES_TRACE(
        STP_FILT_EVT_Q_0,
        txtEvtQFree,
        evtQQueryEmptyI(aEvtQueue));

    if ((evtDynamic_T)0U == (EVT_STATIC_MASK & aEvt->internals.dynamic)) {
        evtDynamic_T tmpR;
        evtDynamic_T tmpU;

        tmpR = aEvt->internals.dynamic & ~EVT_USERS_MASK;
        tmpU = (aEvt->internals.dynamic - 1U) & EVT_USERS_MASK;
        aEvt->internals.dynamic = tmpR | tmpU;
    }
}

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Vraca informaciju o slobodnim mestima u redu za cekanje
 * @param       aEvtQueue               Pokazivac na red za cekanje
 * @param       aEvt                    dogadjaj koji se postavlja u red.
 * @return      Broj slobodnih mesta
 *//*--------------------------------------------------------------------------------------------*/
static uint32_t evtQQueryEmptyI(
    evtQueue_T          * aEvtQueue) {

    return (esQpFreeSpace(&(aEvtQueue->queue)));
}

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

    ES_CRITICAL_DECL();

    ES_CRITICAL_ENTER(OPT_KERNEL_INTERRUPT_PRIO_MAX);
    evtQPutAheadI_(
        &(aEpa->internals.evtQueue),
        aEvt);
    ES_CRITICAL_EXIT();
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

    ES_CRITICAL_DECL();

    ES_CRITICAL_ENTER(OPT_KERNEL_INTERRUPT_PRIO_MAX);
    evtQPutI_(
        &(aEpa->internals.evtQueue),
        aEvt);
    ES_CRITICAL_EXIT();
}

/*-----------------------------------------------------------------------------------------------*/
void evtQInit(
    esEpaHeader_T       * aEpa,
    esEvtHeader_T       ** aEvtBuff,
    size_t              aQueueSize) {

    EVT_DBG_CHECK((size_t)1U <= aQueueSize);                                    /* Provera par: da li je zahtevana velicina barem za 1      */
                                                                                /* element?                                                 */
    EVT_DBG_CHECK((esEvtHeader_T **)0U != aEvtBuff);                            /* Provera: da li je rezervisana memorija?                  */
    ES_TRACE(
        STP_FILT_EVT_Q_0,txtEvtQinit,
        &(aEpa->internals.evtQueue.queue),
        aEvtBuff,
        aQueueSize);
    esQpInit(
        &(aEpa->internals.evtQueue.queue),
        (void **)aEvtBuff,
        aQueueSize);
    ES_TRACE(
        STP_FILT_EVT_Q_0,
        txtEvtQFree,
        evtQQueryEmptyI(&(aEpa->internals.evtQueue)));

#if defined(OPT_KERNEL_DBG_EVT) || defined(__DOXYGEN__)
    aEpa->internals.evtQueue.freeCurr = esQpFreeSpace(
        &(aEpa->internals.evtQueue.queue));
    aEpa->internals.evtQueue.freeMin = aEpa->internals.evtQueue.freeCurr;
#endif
}

/*-----------------------------------------------------------------------------------------------*/
void evtQDeInit(
    esEpaHeader_T       * aEpa) {

    ES_CRITICAL_DECL();
    esEvtHeader_T * tmpEvt;

    ES_CRITICAL_ENTER(OPT_KERNEL_INTERRUPT_PRIO_MAX);

    while (FALSE == esQpIsEmpty(&(aEpa->internals.evtQueue.queue))) {
        tmpEvt = evtQGetI_(
            &(aEpa->internals.evtQueue));
        evtDestroyI_(
            tmpEvt);
    }
    ES_CRITICAL_EXIT();
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

    ES_CRITICAL_DECL();
    esEvtHeader_T * newEvt;

    EVT_DBG_CHECK(
        sizeof(esEvtHeader_T) <= dataSize);                           /* Provera par: da li dataSize moze da sadrzi zaglavlje?    */
    ES_TRACE(
        STP_FILT_EVT_0,
        txtEvtCreate,
        id,
        dataSize);
    ES_CRITICAL_ENTER(
        OPT_KERNEL_INTERRUPT_PRIO_MAX);
    newEvt = (esEvtHeader_T *)esHmemAllocI(dataSize);                           /* Dobavi potreban memorijski prostor za dogadjaj           */
    ES_CRITICAL_EXIT();
    EVT_ASSERT(
        (esEvtHeader_T *)0U != newEvt);                                  /* Provera: da li je mem. prostor rezervisan za dogadjaj?   */
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

    EVT_DBG_CHECK(
        sizeof(esEvtHeader_T) <= dataSize);                            /* Provera par: da li dataSize moze da sadrzi zaglavlje?    */
    ES_TRACE(
        STP_FILT_EVT_0,
        txtEvtCreate,
        id,
        dataSize);
    newEvt = (esEvtHeader_T *)esHmemAllocI(dataSize);                           /* Dobavi potreban memorijski prostor za dogadjaj           */
    EVT_ASSERT((esEvtHeader_T *)0 != newEvt);                                   /* Provera: da li je mem. prostor rezervisan za dogadjaj?   */
    evtInit_(
        newEvt,
        dataSize,
        id);

    return (newEvt);
}

/*-----------------------------------------------------------------------------------------------*/
void esEvtReserve(
    esEvtHeader_T       * aEvt) {

    EVT_DBG_CHECK((esEvtHeader_T *)0U != aEvt);                                  /* Provera par: da li je aEvt inicijalizovan?               */
    ES_TRACE(
        STP_FILT_EVT_0,
        txtEvtReserve,
        aEvt->internals.id);
    aEvt->internals.dynamic |= EVT_RESERVED_MASK;
}

/*-----------------------------------------------------------------------------------------------*/
void esEvtUnReserve(
    esEvtHeader_T       * aEvt) {

    EVT_DBG_CHECK((esEvtHeader_T *)0U != aEvt);                                  /* Provera par: da li je aEvt inicijalizovan?               */
    ES_TRACE(
        STP_FILT_EVT_0,
        txtEvtUnReserve,
        aEvt->internals.id);
    aEvt->internals.dynamic &= ~EVT_RESERVED_MASK;

    if ((evtDynamic_T)0U == aEvt->internals.dynamic) {

#if defined(OPT_KERNEL_DBG_EVT)
        aEvt->internals.signature = 0xDEAD;
#endif
        esHmemDeAlloc((void *)aEvt);
    }
}

/*-----------------------------------------------------------------------------------------------*/
void esEvtPost(
    esEpaHeader_T       * aEpa,
    esEvtHeader_T       * aEvt) {

    ES_CRITICAL_DECL();

    EVT_DBG_CHECK((esEpaHeader_T *)0U != aEpa);                                  /* Provera par: da li je aEpa inicijalizovan?               */
    ES_TRACE(
        STP_FILT_EVT_0,
        txtEvtPost,
        aEvt->internals.id,
        aEpa);
    ES_CRITICAL_ENTER(OPT_KERNEL_INTERRUPT_PRIO_MAX);
    evtQPutI_(
        &(aEpa->internals.evtQueue),
        aEvt);
    schedRdyInsertI_(
        aEpa);
    ES_CRITICAL_EXIT();

#if defined(OPT_KERNEL_SCHEDULER_ROUNDROBIN)
    EPE_SCHED_NOTIFY_RDY();
#endif
}

/*-----------------------------------------------------------------------------------------------*/
void esEvtPostAhead(
    esEpaHeader_T       * aEpa,
    esEvtHeader_T       * aEvt) {

    ES_CRITICAL_DECL();

    EVT_DBG_CHECK((esEpaHeader_T *)0U != aEpa);                                  /* Provera par: da li je aEpa inicijalizovan?               */
    ES_TRACE(
        STP_FILT_EVT_0,
        txtEvtPostAhead,
        aEvt->internals.id,
        aEpa);
    ES_CRITICAL_ENTER(OPT_KERNEL_INTERRUPT_PRIO_MAX);
    evtQPutAheadI_(
        &(aEpa->internals.evtQueue),
        aEvt);
    schedRdyInsertI_(
        aEpa);
    ES_CRITICAL_EXIT();

#if defined(OPT_KERNEL_SCHEDULER_ROUNDROBIN)
    EPE_SCHED_NOTIFY_RDY();
#endif
}

/** @} *//*--------------------------------------------------------------------------------------*/

/*===================================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/

/** @endcond *//** @} *//*************************************************************************
 * END of evt.c
 *************************************************************************************************/
