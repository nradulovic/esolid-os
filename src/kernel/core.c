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
 * @brief       Implementacija jezgra
 * @addtogroup  core_impl
 ****************************************************************************************//** @{ */

/*============================================================================  INCLUDE FILES  ==*/
#define CORE_PKG_H_VAR
#include "kernel_private.h"

/*============================================================================  LOCAL DEFINES  ==*/

/**
 * @brief       Local debug define macro.
 */
CORE_DBG_DEFINE_MODULE(Kernel Core);

/*============================================================================  LOCAL MACRO's  ==*/
/*=========================================================================  LOCAL DATA TYPES  ==*/

/**
 * @brief       Trenutno izvrsavana EPA jedinica
 */
struct currCtx {
/**
 * @brief       Pokazivac na trenutni EPA objekat koji se izvrsava
 */
    esEpaHeader_T   * epa;

/**
 * @brief       Trenutno stanje schedulera
 */
    esKernelStatus_T status;
};

/*================================================================  LOCAL FUNCTION PROTOTYPES  ==*/

C_INLINE_ALWAYS void schedRdyRmEpaI_(
    esEpaHeader_T       * aEpa);

C_INLINE_ALWAYS bool_T schedRdyIsEmptyI_(
    void);

C_INLINE_ALWAYS esEpaHeader_T * schedRdyGetEpaI_(
    void);

C_INLINE_ALWAYS bool_T schedRdyIsEpaRdy_(
    const esEpaHeader_T * aEpa);

C_INLINE_ALWAYS void schedRdyRegI_(
    const esEpaHeader_T * aEpa);

C_INLINE_ALWAYS void schedRdyUnRegI_(
    const esEpaHeader_T * aEpa);

static void schedInit(
    void);

/*==========================================================================  LOCAL VARIABLES  ==*/

/**
 * @brief       Podaci o trenutnom kontekstu
 */
static struct currCtx currCtx;

/*=========================================================================  GLOBAL VARIABLES  ==*/
/*===============================================================  LOCAL FUNCTION DEFINITIONS  ==*/

/*-------------------------------------------------------------------------------------------*//**
 * @name        Funkcije za rad sa redom za cekanje na izvrsavanje
 * @{ *//*---------------------------------------------------------------------------------------*/

/**
 * @brief       Izbacuje EPA objekat iz reda za cekanje
 */
C_INLINE_ALWAYS void schedRdyRmEpaI_(
    esEpaHeader_T       * aEpa) {

    unative_T indxGroup;
    unative_T indx;

    indx = aEpa->prio & (~((unative_T)0U) >> (ES_CPU_UNATIVE_BITS - PRIO_INDX_PWR));

#if (OPT_KERNEL_EPA_PRIO_MAX < ES_CPU_UNATIVE_BITS)
    indxGroup = (unative_T)0U;
#else
    indxGroup = aEpa->internals.prio >> PRIO_INDX_PWR;
#endif
    rdyBitmap.bit[indxGroup] &= ~((unative_T)1U << indx);

    if ((unative_T)0U == rdyBitmap.bit[indxGroup]) {
        rdyBitmap.bitGroup &= ~((unative_T)1U << indxGroup);
    }
}

/**
 * @brief       Vraca stanje reda za cekanje.
 * @return      Boolean vrednost koja karakterise stanje reda za cekanje
 *  @retval     TRUE - ne postoji EPA objekat koji ceka izvrsavanje,
 *  @retval     FALSE - postoji barem jedan EPA objekat koji ceka izvrsavanje.
 */
C_INLINE_ALWAYS bool_T schedRdyIsEmptyI_(
    void) {
    bool_T answer;

    if ((unative_T)0U == rdyBitmap.bitGroup) {
        answer = TRUE;
    } else {
        answer = FALSE;
    }

    return (answer);
}

/**
 * @brief       Vraca pokazivac na sledeci EPA objekat sa najvecim prioritetom.
 * @return      EPA objekat sa najvecim prioritetom koji ceka na izvrsenje.
 */
C_INLINE_ALWAYS esEpaHeader_T * schedRdyGetEpaI_(
    void) {

    uint_fast8_t prio;
    unative_T indxGroup;
    unative_T indx;

    #if (OPT_KERNEL_INTERRUPT_PRIO_MAX < ES_CPU_UNATIVE_BITS)
        indxGroup = (unative_T)0U;
    #else
        indxGroup = ES_CPU_FLS(rdyBitmap.bitGroup);
    #endif
        indx = ES_CPU_FLS(rdyBitmap.bit[indxGroup]);
        prio = indx | (indxGroup << PRIO_INDX_PWR);

    return (rdyBitmap.list[prio]);
}

/**
 * @brief       Ispituje da li je EPA objekat u listi reda za cekanje.
 * @param       [in] aEpa               Pokazivac na EPA objekat cije stanje
 *                                      trazi.
 * @return      Stanje navedenog EPA objekta
 *  @retval     TRUE - EPA objekat ceka na izvrsenje
 *  @retval     FALSE - EPA objekat ne ceka na izvrsenje
 */
C_INLINE_ALWAYS bool_T schedRdyIsEpaRdy_(
    const esEpaHeader_T * aEpa) {

    unative_T indxGroup;
    unative_T indx;

    KERN_ASSERT((esEpaHeader_T *)0U != rdyBitmap.list[aEpa->internals.prio]);          /* Provera: da li je dati EPA prijavljen u sistem?          */
    indx = aEpa->prio & (~((unative_T)0U) >> (ES_CPU_UNATIVE_BITS - PRIO_INDX_PWR));

#if (OPT_KERNEL_EPA_PRIO_MAX < ES_CPU_UNATIVE_BITS)
    indxGroup = (unative_T)0U;
#else
    indxGroup = aEpa->internals.prio >> PRIO_INDX_PWR;
#endif

    if (rdyBitmap.bit[indxGroup] & ((unative_T)1U << indx)) {

        return (TRUE);
    } else {

        return (FALSE);
    }
}

/**
 * @brief       Prijavljuje EPA objekat u red za cekanje.
 */
C_INLINE_ALWAYS void schedRdyRegI_(
    const esEpaHeader_T * aEpa) {

    KERN_ASSERT((esEpaHeader_T *)0U == rdyBitmap.list[aEpa->internals.prio]);  /* Provera: da nije ovaj prioritet vec zauzet?              */
    rdyBitmap.list[aEpa->prio] = (esEpaHeader_T *)aEpa;
}

/**
 * @brief       Odjavljuje EPA objekat iz reda za cekanje.
 */
C_INLINE_ALWAYS void schedRdyUnRegI_(
    const esEpaHeader_T * aEpa) {

    schedRdyRmEpaI_(
        (esEpaHeader_T *)aEpa);
    rdyBitmap.list[aEpa->prio] = (esEpaHeader_T *)0U;
}

/** @} *//*--------------------------------------------------------------------------------------*/

/**
 * @brief       Inicijalizacija scheduler-a
 */
static void schedInit(
    void) {

    currCtx.epa = (esEpaHeader_T *)0U;
    currCtx.status = KERNEL_STOPPED;
}

/*==============================================================  GLOBAL FUNCTION DEFINITIONS  ==*/

/*-------------------------------------------------------------------------------------------*//**
 * @ingroup     core_intf
 * @{ *//*---------------------------------------------------------------------------------------*/
void esEpaInit(
    esEpaHeader_T       * aEpa,
    esPtrState_T        * aStateBuff,
    esEvtHeader_T       ** aEvtBuff,
    const C_ROM esEpaDef_T * aDescription) {

    ES_CRITICAL_DECL();
    CORE_DBG_CHECK((uint8_t)OPT_KERNEL_EPA_PRIO_MAX > aDescription->epaPrio);   /* Provera par: prioritet EPA ne sme da bude veci od zada-  */
                                                                                /* te granice OPT_KERNEL_EPA_PRIO_MAX.                             */
    CORE_DBG_CHECK((uint8_t)0U != aDescription->epaPrio);                       /* Prioritet 0 je rezervisan.                               */
    ES_TRACE(
        STP_FILT_EPA_STATUS_0,
        txtEpaInit,
        aDescription->epaName,
        aEpa,
        aDescription->epaWorkspaceSize,
        aDescription->epaPrio);
    hsmInit(
        aEpa,
        aDescription->hsmInitState,
        aStateBuff,
        aDescription->hsmStateDepth);
    evtQInitI(
        aEpa,
        aEvtBuff,
        aDescription->evtQueueDepth);
    ES_CRITICAL_ENTER(OPT_KERNEL_INTERRUPT_PRIO_MAX);
    evtQPutAheadI(                                                              /* Postavi dogadjaj INIT u redu cekanja ovog automata.      */
        aEpa,
        (esEvtHeader_T *)&evtSignal[SIG_INIT]);
    aEpa->prio = aDescription->epaPrio;
    aEpa->name = aDescription->epaName;
    schedRdyRegI_(
        aEpa);
    schedRdyInsertI_(
        aEpa);

#if defined(OPT_KERNEL_SCHEDULER_PREEMPTIVE)
    if (KERNEL_RUNNING == esKernelStatus()) {
        EPE_SCHED_NOTIFY_RDY();
    }
#endif
    ES_CRITICAL_EXIT();
}

/*-----------------------------------------------------------------------------------------------*/
esEpaHeader_T * esEpaCreate(
    const C_ROM esMemClass_T * aMemClass,
    const C_ROM esEpaDef_T * aDescription) {

    esEpaHeader_T * newEpa;
    size_t epaSize;
    size_t stateBuff;
    size_t evtBuff;

    CORE_DBG_CHECK((const C_ROM esEpaDef_T *)0U != aDescription);               /* Provera par: da li je aDescription inicijalizovan?       */
    CORE_DBG_CHECK(sizeof(esEpaHeader_T) <= aDescription->epaWorkspaceSize);    /* Provera par: zahtevana memorija se koristi za cuvanje ove*/
                                                                                /* strukture.                                               */
    ES_TRACE(
        STP_FILT_EPA_STATUS_0,
        txtEpaCreate,
        aDescription->epaName,
        aDescription->epaWorkspaceSize,
        aDescription->epaPrio);
    epaSize = aDescription->epaWorkspaceSize;
    stateBuff = epaSize + 1U;
    epaSize += hsmReqSize_(
        aDescription->hsmStateDepth);
    evtBuff = epaSize + 1U;
    epaSize += evtQReqSize_(
        aDescription->evtQueueDepth);
    ES_TRACE(
        STP_FILT_EPA_STATUS_0,
        txtEpaCreateMemNeeded,
        epaSize);

#if defined(OPT_KERNEL_USE_DYNAMIC) || defined(__DOXYGEN__)
    CORE_DBG_CHECK((const C_ROM esMemClass_T *)0U != aMemClass);                 /* Provera par: da li je aMemClass inicijalizovan?          */
    newEpa = (esEpaHeader_T *)(* aMemClass->pAlloc)(epaSize);
#else
    CORE_DBG_CHECK(&esMemStaticClass != aMemClass);                                /* Provera par: da li je aMemClass ispravno inicijalizovan? */
    (void)aMemClass;
    newEpa = (esEpaHeader_T *)esHmemAlloc(epaSize);
#endif
    ES_TRACE(
        STP_FILT_EPA_STATUS_0,
        txtEpaCreateMemAddr,
        newEpa);
    CORE_ASSERT((esEpaHeader_T *)0U != newEpa);
    esEpaInit(
        newEpa,
        (esPtrState_T *)((uint8_t *)newEpa + stateBuff),
        (esEvtHeader_T **)((uint8_t *)newEpa + evtBuff),
        aDescription);

#if defined(OPT_KERNEL_USE_DYNAMIC)
    newEpa->internals.memClass = aMemClass;
#endif

    return (newEpa);
}

/*-----------------------------------------------------------------------------------------------*/
void esEpaDeInit(
    esEpaHeader_T       * aEpa) {

    ES_CRITICAL_DECL();

    ES_CRITICAL_ENTER(OPT_KERNEL_INTERRUPT_PRIO_MAX);
    schedRdyUnRegI_(
        aEpa);
    evtQDeInitI(
        aEpa);
    ES_CRITICAL_EXIT();
    hsmDeInit(
        aEpa);
}

/*-----------------------------------------------------------------------------------------------*/
void esEpaDestroy(
    esEpaHeader_T       * aEpa) {

#if !defined(OPT_KERNEL_USE_DYNAMIC)
    (void)aEpa;                                                                 /* Skloni upozorenje o neiskoriscenom parametru.            */
    CORE_ASSERT_ALWAYS("Trying to delete static EPA object!");
#else
    const esMemClass_T * tmpMemClass;

    CORE_DBG_CHECK((esEpaHeader_T *)0U != aEpa);                                 /* Provera par: da li je aEpa inicijalizovan?               */
    tmpMemClass = aEpa->memClass;
    CORE_DBG_CHECK(&esMemStaticClass != tmpMemClass);                              /* Provera par: osiguraj da ne brisemo "staticne" objekte.  */
    esEpaDeInit(
        aEpa);
    (*tmpMemClass->pDeAlloc)((void *)aEpa);
#endif
}

/*-----------------------------------------------------------------------------------------------*/
esEpaHeader_T * esEpaHeaderGet(
    void) {

    return (currCtx.epa);
}

/*-----------------------------------------------------------------------------------------------*/
uint8_t esEpaPrioGet(
    const esEpaHeader_T * aEpa) {

    CORE_DBG_CHECK((const esEpaHeader_T *)0U != aEpa);                           /* Provera par: da li je aEpa inicijalizovan?               */

    return (aEpa->prio);
}

/*-----------------------------------------------------------------------------------------------*/
void esEpaPrioSet(
    esEpaHeader_T       * aEpa,
    uint8_t             aNewPrio) {

    ES_CRITICAL_DECL();
    bool_T state;

    ES_CRITICAL_ENTER(OPT_KERNEL_INTERRUPT_PRIO_MAX);
    state = schedRdyIsEpaRdy_(
        aEpa);
    schedRdyUnRegI_(
        aEpa);
    aEpa->prio = (uint_fast8_t)aNewPrio;
    schedRdyRegI_(
        aEpa);

    if (TRUE == state) {
        schedRdyInsertI_(
            aEpa);
    }
    ES_CRITICAL_EXIT();
}

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @ingroup     kernel_intf
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
void esKernelInit(
    void) {

    esHalInit();
    hmemInit();
    schedInit();

#if defined(OPT_STP_ENABLE)
    esStpInit();
#endif
}

/*-----------------------------------------------------------------------------------------------*/
C_NORETURN void esKernelStart(void) {
    ES_CRITICAL_DECL();

    ES_TRACE(
        STP_FILT_KERN_STATUS_0,
        txtKernStart);
    currCtx.status = KERNEL_RUNNING;
    ES_CRITICAL_ENTER(OPT_KERNEL_INTERRUPT_PRIO_MAX);

    while (TRUE) {
        ES_TRACE(
            STP_FILT_KERN_STATUS_0,
            txtKernSearch);

        while (FALSE == schedRdyIsEmptyI_()) {
            esEvtHeader_T * newEvt;

            currCtx.epa = schedRdyGetEpaI_();
            newEvt = evtQGetI(
                currCtx.epa);
            ES_CRITICAL_EXIT();
            ES_TRACE(
                STP_FILT_KERN_STATUS_0,
                txtKernEpaDispatch,
                currCtx.epa,
                currCtx.epa->prio);
            hsmDispatch(
                currCtx.epa,
                newEvt);
            ES_CRITICAL_ENTER(OPT_KERNEL_INTERRUPT_PRIO_MAX);
            evtDestroyI_(
                newEvt);

            if (TRUE == evtQIsEmpty_(currCtx.epa)) {
                schedRdyRmEpaI_(currCtx.epa);
            }
        }
        currCtx.epa = (void *)0U;
        ES_TRACE(
            STP_FILT_KERN_STATUS_0,
            txtKernEnterIdle);
        ES_CRITICAL_EXIT();
        /* ES_CPU_SLEEP(); */
        ES_CRITICAL_ENTER(OPT_KERNEL_INTERRUPT_PRIO_MAX);
        ES_TRACE(
            STP_FILT_KERN_STATUS_0,
            txtKernExitIdle);
    }
}

/*-----------------------------------------------------------------------------------------------*/
esKernelStatus_T esKernelStatus(
    void) {

    return (currCtx.status);
}
/** @} *//*--------------------------------------------------------------------------------------*/

/*===================================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/

/** @endcond *//** @} *//*************************************************************************
 * END of core.c
 *************************************************************************************************/
