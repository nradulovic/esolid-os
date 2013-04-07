/******************************************************************************
 * This file is part of eSolid
 *
 * Template version: 1.1.14 (10.03.2013)
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
 * @brief       Implementacija scheduler modula.
 * @addtogroup  sched_impl
 *********************************************************************//** @{ */
                                                                                                  
/*=========================================================  INCLUDE FILES  ==*/
#define SCHED_PKG_H_VAR
#include "kernel_private.h"

/*===============================================================  DEFINES  ==*/
/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

static C_INLINE bool_T schedRdyIsEmpty_(
    void);

static C_INLINE esEpa_T * schedRdyGetEpaI_(
    void);

/*=======================================================  LOCAL VARIABLES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

/**
 * @brief       Vraca stanje reda za cekanje.
 * @return      Boolean vrednost koja karakterise stanje reda za cekanje
 *  @retval     TRUE - ne postoji EPA objekat koji ceka izvrsavanje,
 *  @retval     FALSE - postoji barem jedan EPA objekat koji ceka izvrsavanje.
 */
static C_INLINE bool_T schedRdyIsEmpty_(
    void) {

#if (OPT_KERNEL_EPA_PRIO_MAX <= ES_CPU_UNATIVE_BITS)
    bool_T answer;

    if ((unative_T)0U == gRdyBitmap.bit[0]) {
        answer = TRUE;
    } else {
        answer = FALSE;
    }

    return (answer);
#else
    bool_T answer;

    if ((unative_T)0U == gRdyBitmap.bitGroup) {
        answer = TRUE;
    } else {
        answer = FALSE;
    }

    return (answer);
#endif
}

/**
 * @brief       Vraca pokazivac na sledeci EPA objekat sa najvecim prioritetom.
 * @return      EPA objekat sa najvecim prioritetom koji ceka na izvrsenje.
 */
static C_INLINE esEpa_T * schedRdyGetEpaI_(
    void) {

#if (OPT_KERNEL_INTERRUPT_PRIO_MAX <= ES_CPU_UNATIVE_BITS)
    esEpa_T * epa;
    uint_fast8_t prio;

    prio = esCpuFindLastSet(gRdyBitmap.bit[0]);
    epa = gRdyBitmap.list[prio];

    return (epa);
#else
    unative_T indxGroup;
    unative_T indx;
    esEpa_T * epa;

    indxGroup = esCpuFindLastSet(gRdyBitmap.bitGroup);
    indx = esCpuFindLastSet(gRdyBitmap.bit[indxGroup]);
    epa = gRdyBitmap.list[indx | (indxGroup << PRIO_INDX_PWR)];

    return (epa);
#endif
}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/

/*----------------------------------------------------------------------------*/
void schedInit(
    void) {

    gRdyBitmap.current = (esEpa_T *)0U;
}

/*----------------------------------------------------------------------------*/
void schedRdyRegI(
    const esEpa_T * epa,
    uint_fast8_t    prio) {

    if (ES_LOG_IS_DBG(&gKernelLog, LOG_FILT_SCHED)) {
        ES_LOG_DBG_IF_INVALID(&gKernelLog, OPT_KERNEL_EPA_PRIO_MAX >= prio, LOG_SCHED_REG, ES_ARG_OUT_OF_RANGE);
        ES_LOG_DBG_IF_INVALID(&gKernelLog, 0UL == gRdyBitmap.list[prio], LOG_SCHED_REG, ES_USAGE_FAILURE);
    }

    gRdyBitmap.list[prio] = (esEpa_T *)epa;
}


/*----------------------------------------------------------------------------*/
void schedRdyUnRegI(
    uint_fast8_t    prio) {

    gRdyBitmap.list[prio] = (esEpa_T *)0U;
}

/*----------------------------------------------------------------------------*/
bool_T schedEpaIsRdyI(
    uint_fast8_t    prio) {

#if (OPT_KERNEL_EPA_PRIO_MAX <= ES_CPU_UNATIVE_BITS)
    bool_T answer;

    if (gRdyBitmap.bit[0] & ((unative_T)1U << prio)) {
        answer = TRUE;
    } else {
        answer = FALSE;
    }

    return (answer);
#else
    unative_T indxGroup;
    unative_T indx;

    indx = prio & (~((unative_T)0U) >> (ES_CPU_UNATIVE_BITS - PRIO_INDX_PWR));
    indxGroup = prio >> PRIO_INDX_PWR;

    if (gRdyBitmap.bit[indxGroup] & ((unative_T)1U << indx)) {
        answer = TRUE;
    } else {
        answer = FALSE;
    }

    return (answer);
#endif
}

/*----------------------------------------------------------------------------*/
void sched(
    void) {

    ES_CRITICAL_DECL();

    ES_CRITICAL_ENTER(
            OPT_KERNEL_INTERRUPT_PRIO_MAX);

    while (TRUE) {

        while (FALSE == schedRdyIsEmpty_()) {
            esEvt_T * evt;
            esEpa_T * epa;
            esStatus_T status;

            gRdyBitmap.current = epa = schedRdyGetEpaI_();
            evt = evtFetchI(
                epa);
            ES_CRITICAL_EXIT();
            status = EPA_DISPATCH(
                epa,
                evt);
            ES_CRITICAL_ENTER(
                OPT_KERNEL_INTERRUPT_PRIO_MAX);

            if (RETN_DEFERRED == status) {
                evtPushBackI(
                    epa,
                    evt);
            } else {
                esEvtDestroyI(
                    evt);
            }
        }
        gRdyBitmap.current = (esEpa_T *)0U;

#if (OPT_KERNEL_ENABLE_SLEEP == 1U)
        esCpuSleep();
#endif
        ES_CRITICAL_EXIT();
        ES_CRITICAL_ENTER(
            OPT_KERNEL_INTERRUPT_PRIO_MAX);
    }
}

/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/
/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/

/** @endcond *//** @} *//******************************************************
 * END of sched.c
 ******************************************************************************/
