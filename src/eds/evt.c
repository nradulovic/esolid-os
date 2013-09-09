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

#include "eds/evt.h"
#include "eds/mem.h"
#include "eds_private.h"

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
static PORT_C_INLINE_ALWAYS void evtInit_(
    esEvt_T *           evt,
    size_t              size,
    esEvtId_T           id);

/**
 * @brief       DeInicijalizator funkcija dogadjaja
 * @param       [in] evt                Pokazivac na EPA objekat koji se
 *                                      unistava.
 * @inline
 */
static PORT_C_INLINE_ALWAYS void evtDeInit_(
    esEvt_T *           evt);

/*=======================================================  LOCAL VARIABLES  ==*/

DECL_MODULE_INFO("EVT", "Event management", "Nenad Radulovic");

/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

/*----------------------------------------------------------------------------*/
static PORT_C_INLINE_ALWAYS void evtInit_(
    esEvt_T *       evt,
    size_t          size,
    esEvtId_T       id) {

    ES_DBG_API_REQUIRE(ES_DBG_OBJECT_NOT_VALID, evt->signature != EVT_SIGNATURE);

    evt->id = id;
    evt->attrib = 0U;                                                           /* Dogadjaj je dinamican, sa 0 korisnika.                   */

#if (1U == CFG_EVT_USE_TIMESTAMP)
# if (1U == OPT_EVT_TIMESTAMP_CALLBACK)
    evt->timestamp = appEvtTimestampGet();
#else
# endif
#endif
#if (1U == CFG_EVT_USE_GENERATOR)
# if (1U == OPT_EVT_GENERATOR_CALLBACK)
    evt->generator = appEvtGeneratorGet();
# endif
#endif
#if (1U == OPT_EVT_USE_SIZE)
    evt->size = (esEvtSize_T)size;
#else
    (void)size;
#endif
    ES_DBG_API_OBLIGATION(evt->signature = EVT_SIGNATURE);
}

/*----------------------------------------------------------------------------*/
static PORT_C_INLINE_ALWAYS void evtDeInit_(
    esEvt_T *       evt) {

    ES_DBG_API_REQUIRE(ES_DBG_OBJECT_NOT_VALID, evt->signature == EVT_SIGNATURE);
    ES_DBG_API_OBLIGATION(evt->signature = ~EVT_SIGNATURE);
}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

esEvt_T * esEvtCreate(
    size_t          size,
    esEvtId_T       id) {

    PORT_CRITICAL_DECL;
    esEvt_T *       newEvt;

    ES_DBG_API_REQUIRE(ES_DBG_OUT_OF_RANGE, sizeof(esEvt_T) <= size);

    PORT_CRITICAL_ENTER();
    newEvt = esDmemAllocI(
        &gDefDMemHandle,
        size);                                                                  /* Dobavi potreban memorijski prostor za dogadjaj           */
    PORT_CRITICAL_EXIT();
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

    esEvt_T *       newEvt;

    ES_DBG_API_REQUIRE(ES_DBG_OUT_OF_RANGE, sizeof(esEvt_T) <= size);

    newEvt = esDMemAllocI(
        &gDefDMemHandle,
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

    ES_DBG_API_REQUIRE(ES_DBG_POINTER_NULL, NULL != evt);
    ES_DBG_API_REQUIRE(ES_DBG_OBJECT_NOT_VALID, EVT_SIGNATURE == evt->signature);

    evt->attrib |= EVT_RESERVED_MASK;
}

/*----------------------------------------------------------------------------*/
void esEvtUnReserve(
    esEvt_T *       evt) {

    ES_DBG_API_REQUIRE(ES_DBG_POINTER_NULL, NULL != evt);
    ES_DBG_API_REQUIRE(ES_DBG_OBJECT_NOT_VALID, EVT_SIGNATURE == evt->signature);

    evt->attrib &= ~EVT_RESERVED_MASK;
}

/*----------------------------------------------------------------------------*/
void esEvtDestroy(
    esEvt_T *       evt) {

    PORT_CRITICAL_DECL;

    PORT_CRITICAL_ENTER();
    esEvtDestroyI(
        evt);
    PORT_CRITICAL_EXIT();
}

/*----------------------------------------------------------------------------*/
void esEvtDestroyI(
    esEvt_T *       evt) {

    ES_DBG_API_REQUIRE(ES_DBG_POINTER_NULL, NULL != evt);
    ES_DBG_API_REQUIRE(ES_DBG_OBJECT_NOT_VALID, EVT_SIGNATURE == evt->signature);

    if (0U == evt->attrib) {
        evtDeInit_(
            evt);
        esDMemDeAllocI(
            &gDefDMemHandle,
            evt);
    }
}

/** @} *//*-------------------------------------------------------------------*/

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of evt.c
 ******************************************************************************/