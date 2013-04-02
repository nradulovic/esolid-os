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
 * @brief       Implementacija EPA objekta
 * @addtogroup  epa_impl
 *********************************************************************//** @{ */

/*=========================================================  INCLUDE FILES  ==*/
#define EPA_PKG_H_VAR
#include "kernel_private.h"

/*=========================================================  LOCAL DEFINES  ==*/

/**
 * @brief       Konstanta za potpis EPA objekta
 * @details     Konstanta se koristi prilikom debag procesa kako bi funkcije
 *              koje prihvate pokazivac na EPA objekat bile sigurne da je EPA
 *              objekat validan. EPA objekti koji su obrisani nemaju ovaj potpis.
 * @pre         Opcija @ref OPT_KERNEL_DBG mora da bude aktivna kako bi bila
 *              omogucena provera pokazivaca.
 */
#define EPA_SIGNATURE                   (0xDEEF)

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/

/**
 * @brief       Objekat - EPA
 */
typedef struct epaObject {
#if (OPT_MM_DISTRIBUTION != ES_MM_DYNAMIC_ONLY)                                 \
    && (OPT_MM_DISTRIBUTION != ES_MM_STATIC_ONLY)                               \
    || defined(__DOXYGEN__)
/**
 * @brief       Pokazivac na klasu memorijskog alokatora
 */
    const C_ROM struct esMemClass * memClass;
#endif

/**
 * @brief       Instanca EPA
 */
    struct esEpa    epa;
} epaObject_T;

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

static C_INLINE void epaInit_(
    esEpa_T *       epa,
    const C_ROM esEpaDef_T * definition);

static C_INLINE void epaDeInit_(
    esEpa_T *       epa);

/*=======================================================  LOCAL VARIABLES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Inicijalizacija i deinicijalizacija EPA objekta
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Inicijalizuje EPA objekat
 * @param       [out] epa               Pokazivac na strukturu EPA objekta,
 * @param       [in] definition         pokazivac na definicionu strukturu EPA
 *                                      objekta.
 * @notapi
 */
static C_INLINE void epaInit_(
    esEpa_T *       epa,
    const C_ROM esEpaDef_T * definition) {

    ES_CRITICAL_DECL();
    
    KERN_LOG_DBG(
        (definition->epaPrio <= OPT_KERNEL_EPA_PRIO_MAX),
        gKernelLog,
        LOG_FILT_EPA,
        LOG_EPA_INIT,
        ES_ERR_ARG_OUT_OF_RANGE);
    epa->prio = definition->epaPrio;
    epa->name = definition->epaName;
    ES_CRITICAL_ENTER(
        OPT_KERNEL_INTERRUPT_PRIO_MAX);
    evtQPutI_(
        &epa->evtQueue,
        (esEvt_T *)&evtSignal[SIG_INIT]);                                       /* Postavi dogadjaj INIT u redu cekanja ovog automata.      */
    schedRdyRegI(
        epa,
        epa->prio);
    schedRdyInsertI_(
        epa->prio);
    ES_CRITICAL_EXIT();

#if (OPT_KERNEL_ENABLE_LOG <= LOG_DBG)
    epa->signature = EPA_SIGNATURE;
#endif
}

/**
 * @brief       DeInicijalizuje EPA objekat
 * @param       [out] epa               Pokazivac na strukturu EPA objekta.
 * @notapi
 */
static C_INLINE void epaDeInit_(
    esEpa_T *       epa) {

    ES_CRITICAL_DECL();

    ES_CRITICAL_ENTER(
        OPT_KERNEL_INTERRUPT_PRIO_MAX);
    schedRdyRmI_(
        epa->prio);
    schedRdyUnRegI(
        epa->prio);
    ES_CRITICAL_EXIT();

    while (FALSE == evtQIsEmptyI_(&epa->evtQueue)) {
        esEvt_T * evt;

        evt = evtQGetI_(
            &epa->evtQueue);
        ES_CRITICAL_ENTER(
            OPT_KERNEL_INTERRUPT_PRIO_MAX);
        esEvtDestroyI(
            evt);
        ES_CRITICAL_EXIT();
    }

#if (OPT_KERNEL_ENABLE_LOG <= LOG_DBG)
    epa->signature = ~EPA_SIGNATURE;
#endif
}

/** @} *//*-------------------------------------------------------------------*/
/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/

/*----------------------------------------------------------------------------*/
esEvt_T * evtFetchI(
    esEpa_T *       epa) {

    esEvt_T * evt;

    evt = evtQGetI_(
        &epa->evtQueue);
    evtUsrRmI_(
        evt);

    if (TRUE == evtQIsEmptyI_(&epa->evtQueue)) {
        schedRdyRmI_(
            epa->prio);
    }

    return (evt);
}

/*----------------------------------------------------------------------------*/
void evtPushBackI(
    esEpa_T *       epa,
    esEvt_T *       evt) {

    evtQPutAheadI_(
        &epa->evtQueue,
        evt);
    evtUsrAddI_(
        evt);
    schedRdyRmI_(
        epa->prio);
}

/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

/*----------------------------------------------------------------------------*/
void esEvtPost(
    esEpa_T *       epa,
    esEvt_T *       evt) {

    ES_CRITICAL_DECL();

    ES_CRITICAL_ENTER(
        OPT_KERNEL_INTERRUPT_PRIO_MAX);
    esEvtPostI(
       epa,
       evt);
    ES_CRITICAL_EXIT();
}

/*----------------------------------------------------------------------------*/
void esEvtPostI(
    esEpa_T *       epa,
    esEvt_T *       evt) {

    if (ES_LOG_IS_DBG(&gKernelLog, LOG_FILT_EPA)) {

        if (ES_LOG_IS_INVALID((0U != epa) && (0U != evt))) {
            ES_LOG_DBG(
                &gKernelLog,
                LOG_EPA_EVTPOST,
                ES_ERR_ARG_NULL);
        }

        if (ES_LOG_IS_INVALID(EPA_SIGNATURE == epa->signature)) {
            ES_LOG_DBG(
                &gKernelLog,
                LOG_EPA_EVTPOST,
                ES_ERR_ARG_NOT_VALID);
        }
    } /* ES_LOG_IS_DBG */

    if (TRUE == evtQIsEmptyI_(&epa->evtQueue)) {
        evtUsrAddI_(
            evt);
        evtQPutI_(
            &epa->evtQueue,
            evt);
        schedRdyInsertI_(
            epa->prio);
    } else if (FALSE == evtQIsFullI_(&epa->evtQueue)) {
        evtUsrAddI_(
            evt);
        evtQPutI_(
            &epa->evtQueue,
            evt);
    } else {
        /* Greska! Red je pun. */
        esEvtDestroyI(
            evt);
    }
}

/*----------------------------------------------------------------------------*/
void esEvtPostAhead(
    esEpa_T *       epa,
    esEvt_T *       evt) {

    ES_CRITICAL_DECL();
    
    ES_CRITICAL_ENTER(
        OPT_KERNEL_INTERRUPT_PRIO_MAX);
    esEvtPostAheadI(
        epa,
        evt);
    ES_CRITICAL_EXIT();
}

/*----------------------------------------------------------------------------*/
void esEvtPostAheadI(
    esEpa_T *       epa,
    esEvt_T *       evt) {
    
    if (ES_LOG_IS_DBG(&gKernelLog, LOG_FILT_EPA)) {

        if (ES_LOG_IS_INVALID((0U != epa) && (0U != evt))) {
            ES_LOG_DBG(
                &gKernelLog,
                LOG_EPA_EVTPOSTA,
                ES_ERR_ARG_NULL);
        }

        if (ES_LOG_IS_INVALID(EPA_SIGNATURE == epa->signature)) {
            ES_LOG_DBG(
                &gKernelLog,
                LOG_EPA_EVTPOSTA,
                ES_ERR_ARG_NOT_VALID);
        }
    } /* ES_LOG_IS_DBG */

    if (FALSE == evtQIsFullI_(&epa->evtQueue)) {
        evtUsrAddI_(
            evt);
        evtQPutAheadI_(
            &epa->evtQueue,
            evt);
        schedRdyInsertI_(
            epa->prio);
    } else {
        /* Greska! Red je pun. */
        esEvtDestroyI(
            evt);
    }
}

/*----------------------------------------------------------------------------*/
esEpa_T * esEpaCreate(
    const C_ROM esMemClass_T *  memClass,
    const C_ROM esEpaDef_T *    definition) {

    epaObject_T * newEpa;
    size_t coreSize;
    size_t stateQSize;
    size_t evtQSize;

    if (ES_LOG_IS_DBG(&gKernelLog, LOG_FILT_EPA)) {

        if (ES_LOG_IS_INVALID((0UL != memClass) && (0UL != definition))) {
            ES_LOG_DBG(
                &gKernelLog,
                LOG_EPA_CREATE,
                ES_ERR_ARG_NULL);
        }
    } /* ES_LOG_IS_DBG */

#if !defined(PORT_SUPP_UNALIGNED_ACCESS) || defined(OPT_OPTIMIZE_SPEED)         /* Ukoliko port ne podrzava UNALIGNED ACCESS ili je ukljuce-*/
                                                                                /* na optimizacija za brzinu vrsi se zaokruzivanje velicina */
                                                                                /* radi brzeg pristupa memoriji.                            */
    coreSize = sizeof(epaObject_T);
    coreSize += ES_ALIGN(
        definition->epaWorkspaceSize,
        ES_CPU_ATTRIB_ALIGNMENT);
    stateQSize = ES_ALIGN(
        stateQReqSize(
            definition->smLevels),
        ES_CPU_ATTRIB_ALIGNMENT);
    evtQSize = ES_ALIGN(
        evtQReqSize(
            definition->evtQueueDepth),
        ES_CPU_ATTRIB_ALIGNMENT);
#else
    coreSize = sizeof(epaObject_T);
    coreSize += definition->epaWorkspaceSize;
    stateQSize = stateQReqSize(
        definition->smLevels);
    evtQSize = evtQReqSize(
        definition->evtQueueDepth
#endif

#if (OPT_MM_DISTRIBUTION == ES_MM_DYNAMIC_ONLY)
    {
        ES_CRITICAL_DECL();

        (void)memClass;
        ES_CRITICAL_ENTER(
            OPT_KERNEL_INTERRUPT_PRIO_MAX);
        newEpa = esDmemAllocI(
            coreSize + stateQSize + evtQSize);
        ES_CRITICAL_EXIT();
    }
#elif (OPT_MM_DISTRIBUTION == ES_MM_STATIC_ONLY)
    {
        ES_CRITICAL_DECL();

        (void)memClass;
        ES_CRITICAL_ENTER(
            OPT_KERNEL_INTERRUPT_PRIO_MAX);
        newEpa = esSmemAllocI(
            coreSize + stateQSize + evtQSize);
        ES_CRITICAL_EXIT();
    }
#else
    newEpa = (* memClass->alloc)(coreSize + stateQSize + evtQSize);
    newEpa->memClass = memClass;
#endif
    smInit(
        &newEpa->epa.sm,
        definition->smInitState,
        (esState_T *)((uint8_t *)newEpa + coreSize),
        definition->smLevels);
    evtQInit(
        &newEpa->epa.evtQueue,
        (esEvt_T **)((uint8_t *)newEpa + coreSize + stateQSize),
        evtQSize / sizeof(esEvt_T **));
    epaInit_(
        &newEpa->epa,
        definition);

    return (&newEpa->epa);
}

/*----------------------------------------------------------------------------*/
void esEpaDestroy(
    esEpa_T *       epa) {

#if (OPT_MM_DISTRIBUTION == ES_MM_STATIC_ONLY)

    if (ES_LOG_IS_DBG(&gKernelLog, LOG_FILT_EPA)) {
        ES_LOG_DBG(
            &gKernelLog,
            LOG_EPA_DESTROY,
            ES_ERR_USAGE_FAILURE);
    }
#elif (OPT_MM_DISTRIBUTION == ES_MM_DYNAMIC_ONLY)
    ES_CRITICAL_DECL();

    if (ES_LOG_IS_DBG(&gKernelLog, LOG_FILT_EPA)) {

        if (ES_LOG_IS_INVALID(0UL != epa)) {
            ES_LOG_DBG(
                gKernelLog,
                LOG_EPA_DESTROY,
                ES_ERR_ARG_NULL);
        }

        if (ES_LOG_IS_INVALID(EPA_SIGNATURE == epa->signature)) {
            ES_LOG_DBG(
                gKernelLog,
                LOG_EPA_DESTROY,
                ES_ERR_ARG_NOT_VALID);
        }
    }

    epaDeInit_(
        epa);
    evtQDeInit(
        &epa->evtQueue);
    smDeInit(
        &epa->sm);
    ES_CRITICAL_ENTER(
        OPT_KERNEL_INTERRUPT_PRIO_MAX);
    esDmemDeAllocI(
        C_CONTAINER_OF(epa, epaObject_T, epa));
    ES_CRITICAL_EXIT();
#else
    {
        epaObject_T * epaObject;
        
        KERN_LOG_DBG(
            (0U != epa),
            gKernelLog,
            LOG_FILT_EPA,
            LOG_EPA_DESTROY,
            ES_ERR_ARG_NULL);
        KERN_LOG_DBG(
            (EPA_SIGNATURE == epa->signature),
            gKernelLog,
            LOG_FILT_EPA,
            LOG_EPA_DESTROY,
            ES_ERR_ARG_NOT_VALID);

        epaDeInit_(
            epa);
        epaObject = C_CONTAINER_OF(epa, epaObject_T, epa);
        (* epaObject->memClass->deAlloc)(epaObject);
    }
#endif
}

/*----------------------------------------------------------------------------*/
uint8_t esEpaPrioGet(
    const esEpa_T * epa) {

    KERN_LOG_DBG(
        (0U != epa),
        gKernelLog,
        LOG_FILT_EPA,
        LOG_EPA_PRIOGET,
        ES_ERR_ARG_NULL);
    KERN_LOG_DBG(
        (EPA_SIGNATURE == epa->signature),
        gKernelLog,
        LOG_FILT_EPA,
        LOG_EPA_PRIOGET,
        ES_ERR_ARG_NOT_VALID);

    return (epa->prio);
}

/*----------------------------------------------------------------------------*/
void esEpaPrioSet(
    esEpa_T *       epa,
    uint8_t         newPrio) {

    ES_CRITICAL_DECL();
    bool_T status;

    if (ES_LOG_IS_DBG(&gKernelLog, LOG_FILT_EPA)) {

        if (ES_LOG_IS_INVALID(0UL != epa)) {
            ES_LOG_DBG(
                &gKernelLog,
                LOG_EPA_PRIOSET,
                ES_ERR_ARG_NULL);
        }

        if (ES_LOG_IS_INVALID(EPA_SIGNATURE == epa->signature)) {
            ES_LOG_DBG(
                &gKernelLog,
                LOG_EPA_PRIOSET,
                ES_ERR_ARG_NOT_VALID);
        }
    }

    ES_CRITICAL_ENTER(
        OPT_KERNEL_INTERRUPT_PRIO_MAX);
    status = schedEpaIsRdyI(
        epa->prio);
    schedRdyRmI_(
        epa->prio);
    schedRdyUnRegI(
        epa->prio);
    epa->prio = (uint_fast8_t)newPrio;
    schedRdyRegI(
        epa,
        newPrio);

    if (TRUE == status) {
        schedRdyInsertI_(
            newPrio);
    }
    ES_CRITICAL_EXIT();
}


/** @} *//*-------------------------------------------------------------------*/
/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of epa.c
 ******************************************************************************/
