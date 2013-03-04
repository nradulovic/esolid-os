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

C_INLINE void epaInit_(
    esEpaHeader_T       * aEpa,
    esPtrState_T        * aStateBuff,
    esEvtHeader_T       ** aEvtBuff,
    const C_ROM esEpaDef_T * aDescription);

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

#if (OPT_KERNEL_INTERRUPT_PRIO_MAX < ES_CPU_UNATIVE_BITS)
    esEpaHeader_T * epa;

    epa = rdyBitmap.list[ES_CPU_FLS(rdyBitmap.bit[0])];

    return (epa);
#else
    unative_T indxGroup;
    unative_T indx;
    esEpaHeader_T * epa;

    indxGroup = ES_CPU_FLS(rdyBitmap.bitGroup);
    indx = ES_CPU_FLS(rdyBitmap.bit[indxGroup]);
    epa = rdyBitmap.list[indx | (indxGroup << PRIO_INDX_PWR)];

    return (epa);
#endif
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

#if (OPT_KERNEL_EPA_PRIO_MAX < ES_CPU_UNATIVE_BITS)
    unative_T indx;
    bool_T ans;

    indx = aEpa->prio & (~((unative_T)0U) >> (ES_CPU_UNATIVE_BITS - PRIO_INDX_PWR));

    if (rdyBitmap.bit[0] & ((unative_T)1U << indx)) {

        ans = TRUE;
    } else {

        ans = FALSE;
    }

    return (ans);
#else
    unative_T indxGroup;
    unative_T indx;

    indx = aEpa->prio & (~((unative_T)0U) >> (ES_CPU_UNATIVE_BITS - PRIO_INDX_PWR));
    indxGroup = aEpa->internals.prio >> PRIO_INDX_PWR;

    if (rdyBitmap.bit[indxGroup] & ((unative_T)1U << indx)) {

        ans = TRUE;
    } else {

        ans = FALSE;
    }

    return (ans);
#endif
}

/**
 * @brief       Prijavljuje EPA objekat u red za cekanje.
 */
C_INLINE_ALWAYS void schedRdyRegI_(
    const esEpaHeader_T * aEpa) {

    rdyBitmap.list[aEpa->prio] = (esEpaHeader_T *)aEpa;
}

/**
 * @brief       Odjavljuje EPA objekat iz reda za cekanje.
 */
C_INLINE_ALWAYS void schedRdyUnRegI_(
    const esEpaHeader_T * aEpa) {

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

/**
 * @brief       Inicijalizuje EPA objekat
 * @param       [out] aEpa              Pokazivac na strukturu EPA objekta,
 * @param       [in] aStateBuff         memorija za cuvanje stanja HSM automata,
 * @param       [in] aEvtBuff           memorija za cuvanje reda za cekanje,
 * @param       [in] aDescription       pokazivac na opisnu strukturu EPA
 *                                      objekta.
 * @notapi
 */
C_INLINE void epaInit_(
    esEpaHeader_T       * aEpa,
    esPtrState_T        * aStateBuff,
    esEvtHeader_T       ** aEvtBuff,
    const C_ROM esEpaDef_T * aDescription) {

    ES_CRITICAL_DECL();

    hsmInit(
        aEpa,
        aDescription->hsmInitState,
        aStateBuff,
        aDescription->hsmStateDepth);
    evtQInit(
        aEpa,
        aEvtBuff,
        aDescription->evtQueueDepth);
    aEpa->prio = aDescription->epaPrio;
    aEpa->name = aDescription->epaName;
    ES_CRITICAL_ENTER(
        OPT_KERNEL_INTERRUPT_PRIO_MAX);
    schedRdyRegI_(
        aEpa);
    esEvtPostAheadI(                                                              /* Postavi dogadjaj INIT u redu cekanja ovog automata.      */
        aEpa,
        (esEvtHeader_T *)&evtSignal[SIG_INIT]);
    ES_CRITICAL_EXIT();
}

/*======================================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*=======================================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/
/*-------------------------------------------------------------------------------------------*//**
 * @ingroup     core_intf
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
esEpaHeader_T * esEpaCreate(
    const C_ROM esMemClass_T * aMemClass,
    const C_ROM esEpaDef_T * aDescription) {

    uint8_t * newEpa;
    size_t coreSize;
    size_t smpQSize;
    size_t evtQSize;

#if !defined(PORT_SUPP_UNALIGNED_ACCESS) || defined(OPT_OPTIMIZE_SPEED)         /* Ukoliko port ne podrzava UNALIGNED ACCESS ili je ukljuce-*/
                                                                                /* na optimizacija za brzinu vrsi se zaokruzivanje velicina */
                                                                                /* radi brzeg pristupa memoriji.                            */
    coreSize = ES_ALIGN(
        aDescription->epaWorkspaceSize, ES_CPU_ATTRIB_ALIGNMENT);
    smpQSize = ES_ALIGN(hsmReqSize(
        aDescription->hsmStateDepth), ES_CPU_ATTRIB_ALIGNMENT);
    evtQSize = ES_ALIGN(evtQReqSize(
        aDescription->evtQueueDepth), ES_CPU_ATTRIB_ALIGNMENT);
#else
    coreSize = aDescription->epaWorkspaceSize;
    smpQSize = hsmReqSize(
        aDescription->hsmStateDepth);
    evtQSize = evtQReqSize(
        aDescription->evtQueueDepth);
#endif

#if (OPT_MM_DYNAMIC_SIZE == -1)                                                 /* Koristi se samo staticni memorijski menadzer             */
    (void)aMemClass;
    {
        ES_CRITICAL_DECL();

        ES_CRITICAL_ENTER(
            OPT_KERNEL_INTERRUPT_PRIO_MAX);
        newEpa = esSmemAllocI(coreSize + smpQSize + evtQSize);
        ES_CRITICAL_EXIT();
    }
#elif (OPT_MM_DYNAMIC_SIZE == 0U)                                               /* Koristi se samo dinamicni memorijski menadzer            */
    (void)aMemClass;
    {
        ES_CRITICAL_DECL();

        ES_CRITICAL_ENTER(
            OPT_KERNEL_INTERRUPT_PRIO_MAX);
        newEpa = esDmemAllocI(coreSize + smpQSize + evtQSize);
        ES_CRITICAL_EXIT();
    }
#else                                                                           /* Koriste se oba memorijska menadzera                      */
    newEpa = (* aMemClass->pAlloc)(coreSize + smpQSize + evtQSize);
#endif
    epaInit_(
        (esEpaHeader_T *)newEpa,
        (esPtrState_T *)(newEpa + coreSize),
        (esEvtHeader_T **)(newEpa + coreSize + smpQSize),
        aDescription);

#if (OPT_MM_DYNAMIC_SIZE > 0U)                                                  /* Koriste se oba memorijska menadzera                      */
    newEpa->memClass = aMemClass;
#endif

    return ((esEpaHeader_T *)newEpa);
}

/*-----------------------------------------------------------------------------------------------*/
void esEpaDestroy(
    esEpaHeader_T       * aEpa) {

#if (OPT_MM_DYNAMIC_SIZE == -1)                                                 /* Koristi se samo staticni memorijski menadzer             */
    /* Greska */
#elif (OPT_MM_DYNAMIC_SIZE == 0U)                                               /* Koristi se samo dinamicni memorijski menadzer            */
    ES_CRITICAL_DECL();

    ES_CRITICAL_ENTER(
        OPT_KERNEL_INTERRUPT_PRIO_MAX);
    schedRdyRmI_(
        aEpa);
    schedRdyUnRegI_(
        aEpa);
    ES_CRITICAL_EXIT();
    evtQDeInit(
        aEpa);
    hsmDeInit(
        aEpa);
    ES_CRITICAL_ENTER(
        OPT_KERNEL_INTERRUPT_PRIO_MAX);
    esDmemDeAllocI(
        (void *)aEpa);
    ES_CRITICAL_EXIT();
#else                                                                           /* Koriste se oba memorijska menadzera                      */
    ES_CRITICAL_DECL();
    const esMemClass_T * tmpMemClass;

    tmpMemClass = aEpa->memClass;
    ES_CRITICAL_ENTER(
        OPT_KERNEL_INTERRUPT_PRIO_MAX);
    schedRdyUnRegI_(
        aEpa);
    ES_CRITICAL_EXIT();
    evtQDeInit(
        aEpa);
    hsmDeInit(
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

    return (aEpa->prio);
}

/*-----------------------------------------------------------------------------------------------*/
void esEpaPrioSet(
    esEpaHeader_T       * aEpa,
    uint8_t             aNewPrio) {

    ES_CRITICAL_DECL();
    bool_T state;

    ES_CRITICAL_ENTER(
        OPT_KERNEL_INTERRUPT_PRIO_MAX);
    state = schedRdyIsEpaRdy_(
        aEpa);
    schedRdyRmI_(
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
    mmInit();
    schedInit();

#if defined(OPT_STP_ENABLE)
    esStpInit();
#endif
}

/*-----------------------------------------------------------------------------------------------*/
C_NORETURN void esKernelStart(void) {
    ES_CRITICAL_DECL();

    currCtx.status = KERNEL_RUNNING;
    ES_CRITICAL_ENTER(
        OPT_KERNEL_INTERRUPT_PRIO_MAX);

    while (TRUE) {

        while (FALSE == schedRdyIsEmptyI_()) {
            esEvtHeader_T * newEvt;
            esEpaHeader_T * newEpa;

            currCtx.epa = newEpa = schedRdyGetEpaI_();
            newEvt = evtQGetI(
                newEpa);
            ES_CRITICAL_EXIT();
            hsmDispatch(
                newEpa,
                newEvt);
            ES_CRITICAL_ENTER(
                OPT_KERNEL_INTERRUPT_PRIO_MAX);
            evtDestroyI_(
                newEvt);
        }
        currCtx.epa = (esEpaHeader_T *)0U;
        ES_CRITICAL_EXIT();
        /* ES_CPU_SLEEP(); */
        ES_CRITICAL_ENTER(
            OPT_KERNEL_INTERRUPT_PRIO_MAX);
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
