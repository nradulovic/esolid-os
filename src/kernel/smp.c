/******************************************************************************
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
 *//***********************************************************************//**
 * @file
 * @author      Nenad Radulovic
 * @brief       Implementacija State Machine Processor modula.
 * @addtogroup  smp_impl
 *********************************************************************//** @{ */

/*=========================================================  INCLUDE FILES  ==*/
#define SMP_PKG_H_VAR
#include "kernel_private.h"

/*=========================================================  LOCAL DEFINES  ==*/
/*=========================================================  LOCAL MACRO's  ==*/

/**
 * @brief       Posalji predefinisan dogadjaj @c evt automatu @c hsm.
 * @param       epa                     Pokazivac na strukturu EPA objekta,
 * @param       state                   pokazivac na funkciju stanja,
 * @param       evt                     redni broj (enumerator) rezervisanog
 *                                      dogadjaj.
 */
#define EVT_SIGNAL_SEND(epa, state, evt)                                        \
    (*state)((epa), (esEvt_T *)&evtSignal[evt])

/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

static esState_T * disTranFindPath(
    esSm_T *        sm,
    esState_T *     exit,
    esState_T *     entry);

static void disTranEnter(
    esSm_T *        sm,
    esState_T *     entry);

static void disTranExit(
    esSm_T *        sm,
    esState_T *     exit);

/*=======================================================  LOCAL VARIABLES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/

/**
 * @brief       Tabela signalnih dogadjaja
 */
const C_ROM esEvt_T evtSignal[] = {
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

/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

/**
 * @brief       Pronalazi putanju od izvorista do odredista
 * @param       [in,out] sm             Pokazivac na automat
 * @param       [in,out] exit           Pokazivac na pocetak reda stanja za izlaz
 * @param       [in,out] entry          Pokazivac na kraj reda stanja za ulaz
 * @return      Pokazivac na pocetak reda stanja za ulaz.
 * @notapi
 */
static esState_T * disTranFindPath(
    esSm_T *        sm,
    esState_T *     exit,
    esState_T *     entry) {

    /* tran: a) src ?== dst                                                   */
    if (*exit == *entry) {
        *(++exit) = (esState_T)0U;

        return (--entry);
    }
    /* tran: b) src ?== super(dst)                                            */
    (void)EVT_SIGNAL_SEND(sm, *entry, SIG_SUPER);
    *(--entry) = sm->state;                                                    /* super(dst)                                               */

    if (*exit == *entry) {
        *(++exit) = (esState_T)0U;

        return (entry);
    }
    /* tran: c) super(src) ?== super(dst)                                     */
    (void)EVT_SIGNAL_SEND(sm, *exit, SIG_SUPER);
    *(++exit) = sm->state;                                                     /* super(src)                                               */

    if (*exit == *entry) {
        *exit = (esState_T)0U;

        return (entry);
    }
    /* tran: d) super(src) ?== dst                                            */
    ++entry;                                                                    /* dst */

    if (*exit == *entry) {
        *exit = (esState_T)0U;

        return (entry);
    }
    /* tran: e) src ?== ...super(super(dst))                                  */
    --exit;                                                                     /* src                                                      */
    --entry;                                                                    /* super(dst)                                               */

    while (&esSmTopState != **entry) {
        (void)EVT_SIGNAL_SEND(sm, *entry, SIG_SUPER);
        *(--entry) = sm->state;

        if (*exit == *entry) {
            *exit = (esState_T)0U;

            return (entry);
        }
    }
    /* tran: f) super(src) ?== ...super(super(dst))                           */
    ++exit;                                                                     /* super(src)                                               */
    entry = sm->stateQEnd - 2U;                                                 /* super(dst)                                               */

    while (&esSmTopState != **entry) {

        if (*exit == *entry) {                                                  /* ...super(super(dst)) */
            *exit = (esState_T)0U;

            return (entry);
        }
        --entry;
    }
    /* tran: g) ...super(super(src)) ?== ...super(super(dst))                 */
    while (TRUE) {
        (void)EVT_SIGNAL_SEND(sm, *exit, SIG_SUPER);
        *(++exit) = sm->state;
        entry = sm->stateQEnd;

        while (&esSmTopState != **entry) {

            if (*exit == *entry) {
                *exit = (esState_T)0U;

                return (entry);
            }
            --entry;
        }
    }

    return ((esState_T *)0U);
}

/**
 * @brief       Ulaz u hijerarhiju
 * @param       [in] sm                 Pokazivac na konacni automat
 * @param       [in] entry              Pokazivac na pocetak reda stanja.
 * @details     Funkcija ulazi u hijerarhiju dok ne stigne do kraja reda za
 *              stanja.
 * @notapi
 */
static void disTranEnter(
    esSm_T *        sm,
    esState_T *     entry) {

    while (entry != sm->stateQEnd) {
        ++entry;
        (void)EVT_SIGNAL_SEND(sm, *entry, SIG_ENTRY);
    }
}

/**
 * @brief       Izlaz iz hijerarhije
 * @param       [in] sm                 Pokazivac na konacni automat
 * @param       [in] exit               Pokazivac na pocetak reda stanja
 * @details     Red stanja je terminisan nulom.
 * @notapi
 */
static void disTranExit(
    esSm_T *        sm,
    esState_T *     exit) {

    while (*exit != (esState_T)0U) {
        (void)EVT_SIGNAL_SEND(sm, *exit, SIG_EXIT);
        ++exit;
    }
}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/

/*----------------------------------------------------------------------------*/
void smInit (
    esSm_T *        sm,
    esState_T       initState,
    esState_T *     stateQueue,
    size_t          levels) {

#if (OPT_SMP_SM_TYPES == ES_SMP_FSM_ONLY)
    sm->state = initState;
#elif (OPT_SMP_SM_TYPES == ES_SMP_HSM_ONLY)
    sm->state = initState;
    sm->stateQBegin = stateQueue;
    sm->stateQEnd =
        (esState_T *)((uint8_t *)stateQueue + stateQReqSize(levels)) - 1U;
#else
    sm->state = initState;

    if (levels == 2) {
        sm->dispatch = &fsmDispatch;
        sm->stateQBegin = (esState_T *)0U;
        sm->stateQEnd = (esState_T *)0U;
    } else {
        sm->dispatch = &hsmDispatch;
        sm->stateQBegin = stateQueue;
        sm->stateQEnd =
            (esState_T *)((uint8_t *)stateQueue + stateQReqSize(levels)) - 1U;
    }
#endif
}

/*----------------------------------------------------------------------------*/
void smDeInit(
    esSm_T *        sm) {

    sm->stateQBegin = (esState_T *)0U;
    sm->stateQEnd = (esState_T *)0U;
}

/*----------------------------------------------------------------------------*/
size_t stateQReqSize(
    uint8_t         levels) {

#if (OPT_SMP_SM_TYPES == ES_SMP_FSM_ONLY)

    return (0U);
#elif (OPT_SMP_SM_TYPES == ES_SMP_HSM_ONLY)
    size_t needed;

    needed = levels * (size_t)2U * sizeof(esState_T);

    return (needed);
#else
    size_t needed;

    if (levels <= 2) {

        needed = 0U;
    } else {
        needed = levels * 2U * sizeof(esState_T *);
    }

    return (needed);
#endif
}

/*----------------------------------------------------------------------------*/
esStatus_T hsmDispatch(
    esSm_T *        sm,
    const esEvt_T * evt) {

    esState_T * stateQCurr;
    esStatus_T status;

    stateQCurr = sm->stateQBegin;
    *sm->stateQEnd = sm->state;

    do {
        *stateQCurr = sm->state;
        status = (esStatus_T)(**stateQCurr)(sm, (esEvt_T *)evt);
        ++stateQCurr;
    } while (RETN_SUPER == status);
    --stateQCurr;

    while (RETN_TRAN == status) {
        *sm->stateQEnd = sm->state;
        stateQCurr = disTranFindPath(
            sm,
            stateQCurr,
            sm->stateQEnd);
        disTranExit(
            sm,
            sm->stateQBegin);
        disTranEnter(
            sm,
            stateQCurr);
        status = (esStatus_T)EVT_SIGNAL_SEND(sm,*sm->stateQEnd, SIG_INIT);
        stateQCurr = sm->stateQBegin;
        *sm->stateQBegin = *sm->stateQEnd;
    }
    sm->state = *sm->stateQEnd;

    return (status);
}

/*----------------------------------------------------------------------------*/
/**
 * @todo        Pogledati da li je dobra ova funkcija!!!
 */
esStatus_T fsmDispatch(
    esSm_T *        sm,
    const esEvt_T * evt) {

    esStatus_T status;
    esState_T oldState;
    esState_T newState;

    oldState = sm->state;
    status = (esStatus_T)(*sm->state)(sm, (esEvt_T *)evt);
    newState = sm->state;

    while (RETN_TRAN == status) {
        (void)EVT_SIGNAL_SEND(sm, oldState, SIG_EXIT);
        (void)EVT_SIGNAL_SEND(sm, newState, SIG_ENTRY);
        status = EVT_SIGNAL_SEND(sm, newState, SIG_INIT);
        oldState = newState;
        newState = sm->state;
    }
    sm->state = oldState;

    return (status);
}

/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

/*------------------------------------------------------------------------*//**
 * @ingroup         smp_intf
 * @{ *//*--------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
esStatus_T esSmDispatch(
    esSm_T *        sm,
    const esEvt_T * evt) {

    esStatus_T status;

    status = SM_DISPATCH(sm, evt);

    return (status);
}

/*----------------------------------------------------------------------------*/
esSm_T * esSmCreate(
    esMemClass_T *  memClass,
    esState_T       initState,
    uint8_t         levels) {

#if (OPT_KERNEL_API_LEVEL == ES_KERNEL_API_SM)                                  /* Da li je ukljucen API nivo SM? */
    uint8_t * newSm;
    size_t smpSize;
    size_t stateQSize;

# if !defined(PORT_SUPP_UNALIGNED_ACCESS) || defined(OPT_OPTIMIZE_SPEED)        /* Ukoliko port ne podrzava UNALIGNED ACCESS ili je ukljuce-*/
                                                                                /* na optimizacija za brzinu vrsi se zaokruzivanje velicina */
                                                                                /* radi brzeg pristupa memoriji.                            */
    smpSize = ES_ALIGN(
        sizeof(esSm_T),
        ES_CPU_ATTRIB_ALIGNMENT);
    stateQSize = ES_ALIGN(
        stateQReqSize(
            levels),
        ES_CPU_ATTRIB_ALIGNMENT);
# else
    smpSize = sizeof(esSm_T);
    stateQSize = stateQReqSize(
        levels);
# endif

# if (OPT_MM_DISTRIBUTION == ES_MM_STATIC_ONLY)
    (void)aMemClass;
    {
        ES_CRITICAL_DECL();

        ES_CRITICAL_ENTER(
            OPT_KERNEL_INTERRUPT_PRIO_MAX);
        newSm = esSmemAllocI(
            smpSize + stateQSize);
        ES_CRITICAL_EXIT();
    }
# elif (OPT_MM_DISTRIBUTION == ES_MM_DYNAMIC_ONLY)
    (void)memClass;
    {
        ES_CRITICAL_DECL();

        ES_CRITICAL_ENTER(
            OPT_KERNEL_INTERRUPT_PRIO_MAX);
        newSm = esDmemAllocI(
            smpSize + stateQSize);
        ES_CRITICAL_EXIT();
    }
# else
    newSm = (* memClass->alloc)(smpSize + stateQSize);
    ((esSm_T *)newSm)->memClass = aMemClass;
# endif
    smInit(
        (esSm_T *)newSm,
        initState,
        (esState_T *)(newSm + smpSize),
        levels);

    return ((esSm_T *)newSm);
#else /* !(OPT_KERNEL_API_LEVEL == ES_KERNEL_API_SM) */
    /* Greska! Ne moze ova f-ja u ovom API nivou */
    (void)memClass;
    (void)initState;
    (void)levels;

    return ((esSm_T *)0U);
#endif
}

/*----------------------------------------------------------------------------*/
void esSmDestroy(
    esSm_T *        sm) {

#if (OPT_KERNEL_API_LEVEL == ES_KERNEL_API_SM)                                  /* Da li je ukljucen API nivo SM? */
    smDeInit(
        sm);

# if (OPT_MM_DISTRIBUTION == ES_MM_STATIC_ONLY)
    /* Greska! Staticni objekat */
# elif (OPT_MM_DISTRIBUTION == ES_MM_DYNAMIC_ONLY)
    {
        ES_CRITICAL_DECL();

        ES_CRITICAL_ENTER(
            OPT_KERNEL_INTERRUPT_PRIO_MAX);
        esDmemDeAllocI(
            sm);
        ES_CRITICAL_EXIT();
    }
# else
    (* sm->memClass->deAlloc)(sm);
# endif
#else /* !(OPT_KERNEL_API_LEVEL == ES_KERNEL_API_SM) */
    /* Greska! Ne moze ova f-ja u ovom API nivou */
    (void)sm;
#endif
}

/*----------------------------------------------------------------------------*/
void esSmpInit(
    void) {

    esMemInit();
}

/*----------------------------------------------------------------------------*/
esStatus_T esSmTopState (
    void *          sm,
    esEvt_T *       evt) {

    (void)sm;                                                                  /* Ukloni upozorenje o nekoriscenom parametru               */
    (void)evt;

    return ES_STATE_IGNORED();
}

/** @} *//*-------------------------------------------------------------------*/
/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/

/** @endcond *//** @} *//******************************************************
 * END of smp.c
 ******************************************************************************/
