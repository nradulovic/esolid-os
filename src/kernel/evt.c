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
 * @brief       Implementacija Event objekta
 * @addtogroup  evt_impl
 * @brief       Implementacija
 *********************************************************************//** @{ */

/*=========================================================  INCLUDE FILES  ==*/
#define EVT_PKG_H_VAR
#include "kernel_private.h"
#include "kernel/mem.h"
#include "kernel/log.h"

/*=========================================================  LOCAL DEFINES  ==*/
/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

/**
 * @brief       Inicijalizator funkcija dogadjaja.
 * @param       evt                     Pokazivac na dogadjaj koji se konstruise,
 * @param       size                    velicina dogadjaja,
 * @param       id                      identifikator dogadjaja.
 * @inline
 */
static C_INLINE_ALWAYS void evtInit_(
    esEvt_T *       evt,
    size_t          size,
    esEvtId_T       id);

/**
 * @brief       DeInicijalizator funkcija dogadjaja
 * @param       [in] evt                Pokazivac na EPA objekat koji se
 *                                      unistava.
 * @inline
 */
static C_INLINE_ALWAYS void evtDeInit_(
    esEvt_T *       evt);

/*=======================================================  LOCAL VARIABLES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

/*----------------------------------------------------------------------------*/
static C_INLINE_ALWAYS void evtInit_(
    esEvt_T *       evt,
    size_t          size,
    esEvtId_T       id) {

    evt->id = id;
    evt->attrib = 0U;                                                        /* Dogadjaj je dinamican, sa 0 korisnika.                   */

#if defined(OPT_EVT_USE_TIMESTAMP)
    evt->timestamp = OPT_EVT_TIMESTAMP_CALLBACK();
#endif

#if (OPT_LOG_LEVEL <= LOG_DBG)
    evt->signature = EVT_SIGNATURE;
#endif

#if defined(OPT_EVT_USE_GENERATOR)
    evt->generator = OPT_EVT_GENERATOR_CALLBACK();
#endif

#if defined(OPT_EVT_USE_SIZE)
    evt->size = (esEvtSize_T)size;
#else
    (void)size;
#endif
}

/*----------------------------------------------------------------------------*/
static C_INLINE_ALWAYS void evtDeInit_(
    esEvt_T *       evt) {

#if (OPT_LOG_LEVEL <= LOG_DBG)
    evt->signature = (uint16_t)~EVT_SIGNATURE;                                  /* Postavljanje loseg potpisa                               */
#else
	(void)evt;    
#endif
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

    ES_CRITICAL_ENTER(OPT_SYS_INTERRUPT_PRIO_MAX);
    newEvt = esDMemAllocI(
        size);                                                                  /* Dobavi potreban memorijski prostor za dogadjaj           */
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

    newEvt = esDMemAllocI(
        size);                                                                  /* Dobavi potreban memorijski prostor za dogadjaj           */
    evtInit_(
        newEvt,
        size,
        id);

    return (newEvt);
}

/*----------------------------------------------------------------------------*/
void esEvtReserve(
    esEvt_T *       evt) {

    evt->attrib |= EVT_RESERVED_MASK;
}

/*----------------------------------------------------------------------------*/
void esEvtUnReserve(
    esEvt_T *       evt) {

    evt->attrib &= ~EVT_RESERVED_MASK;
}

/*----------------------------------------------------------------------------*/
void esEvtDestroy(
    esEvt_T *       evt) {

    if (0U == evt->attrib) {
    	ES_CRITICAL_DECL();
    	
        evtDeInit_(
            evt);
        ES_CRITICAL_ENTER(OPT_SYS_INTERRUPT_PRIO_MAX);
        esDMemDeAllocI(
            evt);
        ES_CRITICAL_EXIT();
    }
}

/*----------------------------------------------------------------------------*/
void esEvtDestroyI(
    esEvt_T *       evt) {

    if (0U == evt->attrib) {
        evtDeInit_(
            evt);
        esDMemDeAllocI(
            evt);
    }
}

/** @} *//*-------------------------------------------------------------------*/
/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of evt.c
 ******************************************************************************/
