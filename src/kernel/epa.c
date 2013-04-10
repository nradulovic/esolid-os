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
    
    if (ES_LOG_IS_DBG(&gKernelLog, LOG_FILT_EPA)) {
        ES_LOG_DBG_IF_INVALID(&gKernelLog, definition->epaPrio <= OPT_KERNEL_EPA_PRIO_MAX, LOG_EPA_INIT, ES_ARG_OUT_OF_RANGE);
    }

    epa->prio = definition->epaPrio;
    epa->name = definition->epaName;
    ES_CRITICAL_ENTER(
        OPT_KERNEL_INTERRUPT_PRIO_MAX);
    evtQPutI_(
        &epa->evtQueue,
        (esEvt_T *)&esEvtSignal[SIG_INIT]);                                       /* Postavi dogadjaj INIT u redu cekanja ovog automata.      */
    schedRdyRegI(
        epa,
        epa->prio);
    schedRdyInsertI_(
        epa->prio);
    ES_CRITICAL_EXIT();

#if (OPT_LOG_LEVEL <= LOG_DBG)
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

#if (OPT_LOG_LEVEL <= LOG_DBG)
    epa->signature = (uint16_t)~EPA_SIGNATURE;
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
        ES_LOG_DBG_IF_INVALID(&gKernelLog, (NULL != epa) && (0U != evt), LOG_EPA_EVTPOST, ES_ARG_NULL);
        ES_LOG_DBG_IF_INVALID(&gKernelLog, EPA_SIGNATURE == epa->signature, LOG_EPA_EVTPOST, ES_ARG_NOT_VALID);
    }

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
        ES_LOG_IF_WARN(&gKernelLog, LOG_FILT_EPA, LOG_EPA_EVTPOST, ES_NOT_ENOUGH_MEM);
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
        ES_LOG_DBG_IF_INVALID(&gKernelLog, (NULL != epa) && (NULL != evt), LOG_EPA_EVTPOSTA, ES_ARG_NULL);
        ES_LOG_DBG_IF_INVALID(&gKernelLog, EPA_SIGNATURE == epa->signature, LOG_EPA_EVTPOSTA, ES_ARG_NOT_VALID);
    }

    if (FALSE == evtQIsFullI_(&epa->evtQueue)) {
        evtUsrAddI_(
            evt);
        evtQPutAheadI_(
            &epa->evtQueue,
            evt);
        schedRdyInsertI_(
            epa->prio);
    } else {
        ES_LOG_IF_WARN(&gKernelLog, LOG_FILT_EPA, LOG_EPA_EVTPOSTA, ES_NOT_ENOUGH_MEM);
        esEvtDestroyI(
            evt);
    }
}

/*----------------------------------------------------------------------------*/
esEpa_T * esEpaCreate(
    const C_ROM esMemClass_T *  memClass,
    const C_ROM esEpaDef_T *    definition) {

    esEpa_T * newEpa;
    size_t coreSize;
    size_t stateQSize;
    size_t evtQSize;

    if (ES_LOG_IS_DBG(&gKernelLog, LOG_FILT_EPA)) {
        ES_LOG_DBG_IF_INVALID(&gKernelLog, (NULL != memClass) && (NULL != definition), LOG_EPA_CREATE, ES_ARG_NULL);
    }

#if !defined(ES_CPU_ATTRIB_UNALIGNED_ACCESS) || defined(OPT_OPTIMIZE_SPEED)         /* Ukoliko port ne podrzava UNALIGNED ACCESS ili je ukljuce-*/
                                                                                /* na optimizacija za brzinu vrsi se zaokruzivanje velicina */
                                                                                /* radi brzeg pristupa memoriji.                            */
    coreSize = sizeof(esEpa_T);
    coreSize += ES_ALIGN(
        definition->epaWorkspaceSize,
        ES_CPU_ATTRIB_ALIGNMENT);
    stateQSize = ES_ALIGN(
        stateQReqSize(
            definition->smLevels),
        ES_CPU_ATTRIB_ALIGNMENT);
    evtQSize = ES_ALIGN(
        evtQReqSize(
            definition->evtQueueLevels),
        ES_CPU_ATTRIB_ALIGNMENT);
#else
    coreSize = sizeof(esEpa_T);
    coreSize += definition->epaWorkspaceSize;
    stateQSize = stateQReqSize(
        definition->smLevels);
    evtQSize = evtQReqSize(
        definition->evtQueueLevels);
#endif
    newEpa = mmObjCreate(
        memClass,
        coreSize + stateQSize + evtQSize);
    smInit(
        &newEpa->sm,
        definition->smInitState,
        (esState_T *)((uint8_t *)newEpa + coreSize),
        definition->smLevels);
    evtQInit(
        &newEpa->evtQueue,
        (esEvt_T **)((uint8_t *)newEpa + coreSize + stateQSize),
        definition->evtQueueLevels);
    epaInit_(
        newEpa,
        definition);

    return (newEpa);
}

/*----------------------------------------------------------------------------*/
void esEpaDestroy(
    esEpa_T *       epa) {

    if (ES_LOG_IS_DBG(&gKernelLog, LOG_FILT_EPA)) {
        ES_LOG_DBG_IF_INVALID(&gKernelLog, NULL != epa, LOG_EPA_DESTROY, ES_ARG_NULL);
        ES_LOG_DBG_IF_INVALID(&gKernelLog, EPA_SIGNATURE == epa->signature, LOG_EPA_DESTROY, ES_ARG_NOT_VALID);
    }

    epaDeInit_(
        epa);
    evtQDeInit(
        &epa->evtQueue);
    smDeInit(
        &epa->sm);
    mmObjDestroy(
        epa);
}

/*----------------------------------------------------------------------------*/
uint8_t esEpaPrioGet(
    const esEpa_T * epa) {

    if (ES_LOG_IS_DBG(&gKernelLog, LOG_FILT_EPA)) {
        ES_LOG_DBG_IF_INVALID(&gKernelLog, NULL != epa, LOG_EPA_PRIOGET, ES_ARG_NULL);
        ES_LOG_DBG_IF_INVALID(&gKernelLog, EPA_SIGNATURE == epa->signature, LOG_EPA_PRIOGET, ES_ARG_NOT_VALID);
    }

    return (epa->prio);
}

/*----------------------------------------------------------------------------*/
void esEpaPrioSet(
    esEpa_T *       epa,
    uint8_t         newPrio) {

    ES_CRITICAL_DECL();
    bool_T status;

    if (ES_LOG_IS_DBG(&gKernelLog, LOG_FILT_EPA)) {
        ES_LOG_DBG_IF_INVALID(&gKernelLog, NULL != epa, LOG_EPA_PRIOGET, ES_ARG_NULL);
        ES_LOG_DBG_IF_INVALID(&gKernelLog, EPA_SIGNATURE == epa->signature, LOG_EPA_PRIOGET, ES_ARG_NOT_VALID);
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
