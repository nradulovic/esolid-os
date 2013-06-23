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
 * @brief       Implementacija Event Object podmodula.
 * @addtogroup  evt_impl
 *********************************************************************//** @{ */

/*=========================================================  INCLUDE FILES  ==*/
#define EVT_PKG_H_VAR
#include "kernel_private.h"

/*=========================================================  LOCAL DEFINES  ==*/
/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

C_INLINE_ALWAYS void evtInit_(
    esEvt_T *       evt,
    size_t          size,
    esEvtId_T       id);

C_INLINE_ALWAYS void evtDeInit_(
    esEvt_T *       evt);

/*=======================================================  LOCAL VARIABLES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

/**
 * @brief       Inicijalizator funkcija dogadjaja.
 * @param       evt                     Pokazivac na dogadjaj koji se konstruise,
 * @param       size                    velicina dogadjaja,
 * @param       id                      identifikator dogadjaja.
 * @notapi
 * @inline
 */
C_INLINE_ALWAYS void evtInit_(
    esEvt_T *       evt,
    size_t          size,
    esEvtId_T       id) {

    evt->id = id;
    evt->dynamic = (uint_fast8_t)0U;                                            /* Dogadjaj je dinamican, sa 0 korisnika.                   */

#if defined(OPT_EVT_USE_TIMESTAMP)
        evt->timestamp = uTimestampGet();
#endif

#if defined(OPT_EVT_USE_GENERATOR)

# if (OPT_KERNEL_API_LEVEL < 2)
    evt->generator = uGeneratorGet();
# else
    evt->generator = esEpaGet();
# endif
#endif

#if defined(OPT_EVT_USE_SIZE)
    evt->size = (esEvtSize_T)size;
#else
    (void)size;
#endif

    ES_KERN_API_OBLIGATION(evt->signature = EVT_SIGNATURE);
}

/**
 * @brief       DeInicijalizator funkcija dogadjaja
 * @param       [in] evt                Pokazivac na EPA objekat koji se
 *                                      unistava.
 * @notapi
 * @inline
 */
C_INLINE_ALWAYS void evtDeInit_(
    esEvt_T *       evt) {

    evt->id = 0;

    ES_KERN_API_OBLIGATION(evt->signature = 0U);
}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

/*------------------------------------------------------------------------*//**
 * @ingroup         evt_intf
 * @{ *//*--------------------------------------------------------------------*/

esEvt_T * esEvtCreate(
    size_t          size,
    esEvtId_T       id) {

    ES_CRITICAL_DECL();
    esEvt_T * newEvt;

    ES_CRITICAL_ENTER(
        OPT_KERNEL_INTERRUPT_PRIO_MAX);
    newEvt = esDmemAllocI(size);                                                /* Dobavi potreban memorijski prostor za dogadjaj           */
    ES_CRITICAL_EXIT();
    evtInit_(
        newEvt,
        size,
        id);

    return (newEvt);
}

/*----------------------------------------------------------------------------*/
esEvt_T * esEvtCreateI(
    size_t          size,
    esEvtId_T       id) {

    esEvt_T * newEvt;

    ES_KERN_API_REQUIRE(ES_KERN_ARG_OUT_OF_RANGE, sizeof(esEvt_T) <= size);

    newEvt = esDmemAllocI(size);                                                /* Dobavi potreban memorijski prostor za dogadjaj           */
    evtInit_(
        newEvt,
        size,
        id);

    return (newEvt);
}

/*----------------------------------------------------------------------------*/
void esEvtReserve(
    esEvt_T *       evt) {

    ES_KERN_API_REQUIRE(ES_KERN_ARG_NULL, NULL != evt);
    ES_KERN_API_REQUIRE(ES_KERN_ARG_NOT_VALID, EVT_SIGNATURE == evt->signature);

    evt->dynamic |= EVT_RESERVED_MASK;
}

/*----------------------------------------------------------------------------*/
void esEvtUnReserve(
    esEvt_T *       evt) {

    ES_KERN_API_REQUIRE(ES_KERN_ARG_NULL, NULL != evt);
    ES_KERN_API_REQUIRE(ES_KERN_ARG_NOT_VALID, EVT_SIGNATURE == evt->signature);

    evt->dynamic &= ~EVT_RESERVED_MASK;
}

/*----------------------------------------------------------------------------*/
void esEvtDestroy(
    esEvt_T *       evt) {

    ES_CRITICAL_DECL();

    ES_CRITICAL_ENTER(
        OPT_KERNEL_INTERRUPT_PRIO_MAX);
    esEvtDestroyI(
        evt);
    ES_CRITICAL_EXIT();
}

/*----------------------------------------------------------------------------*/
void esEvtDestroyI(
    esEvt_T *       evt) {

    ES_KERN_API_REQUIRE(ES_KERN_ARG_NULL, NULL != evt);
    ES_KERN_API_REQUIRE(ES_KERN_ARG_NOT_VALID, EVT_SIGNATURE == evt->signature);

    if ((uint_fast8_t)0U == evt->dynamic) {
        evtDeInit_(
            evt);
        esDmemDeAllocI(
            evt);
    }
}

/** @} *//*-------------------------------------------------------------------*/
/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of evt.c
 ******************************************************************************/
