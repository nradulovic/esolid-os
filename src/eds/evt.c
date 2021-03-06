/*
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
 *********************************************************************//** @{ */

/*=========================================================  INCLUDE FILES  ==*/

#include "evt_pkg.h"
#include "base/mem.h"
#include "base/critical.h"

#if (3 == CFG_EVT_STORAGE)
# include <stdlib.h>
#endif

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/

#if (2 > CFG_EVT_STORAGE)
struct evtPools {
    esPMemHandle_T *    handle[CFG_EVT_STORAGE_NPOOL];
    uint_fast8_t        npool;
};
#endif

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

#if (2 > CFG_EVT_STORAGE)
static struct evtPools EvtPools;
#endif

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
#if (1U == CFG_EVT_USE_SIZE)
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

/*----------------------------------------------------------------------------*/
void esEvtPoolRegister(
    esPMemHandle_T *    handle) {

#if (2 > CFG_EVT_STORAGE)
    portReg_T           intrCtx;
    uint_fast8_t        cnt;
    size_t              size;

    ES_DBG_API_REQUIRE(ES_DBG_NOT_ENOUGH_MEM, CFG_EVT_STORAGE_NPOOL != EvtPools.npool);

    size = ES_PMEM_ATTR_BLOCK_SIZE_GET(handle);
    ES_CRITICAL_LOCK_ENTER(&intrCtx);
    cnt  = EvtPools.npool;

    while (0u < cnt) {
        size_t          currSize;

        EvtPools.handle[cnt] = EvtPools.handle[cnt - 1];
        currSize = ES_PMEM_ATTR_BLOCK_SIZE_GET(EvtPools.handle[cnt]);

        if (currSize <= size) {

            break;
        }
        cnt--;
    }
    EvtPools.handle[cnt] = handle;
    EvtPools.npool++;
    ES_CRITICAL_LOCK_EXIT(intrCtx);
#endif
}

/*----------------------------------------------------------------------------*/
void esEvtPoolUnregister(
    esPMemHandle_T *    handle) {

#if (2 > CFG_EVT_STORAGE)
    portReg_T           intrCtx;
    uint_fast8_t        cnt;

    ES_CRITICAL_LOCK_ENTER(&intrCtx);
    cnt = EvtPools.npool;

    while ((0u < cnt) && (handle != EvtPools.handle[cnt])) {
        cnt--;
    }
    EvtPools.npool--;

    ES_DBG_API_REQUIRE(ES_DBG_OUT_OF_RANGE, handle == EvtPools.handle[cnt]);

    while (cnt < EvtPools.npool) {
        EvtPools.handle[cnt] = EvtPools.handle[cnt + 1];
        cnt++;
    }
    EvtPools.handle[EvtPools.npool - 1] = NULL;
    ES_CRITICAL_LOCK_EXIT(intrCtx);
#endif
}

static PORT_C_INLINE esPMemHandle_T * poolFindI_(size_t size) {
    uint_fast8_t        cnt;

    cnt = 0u;

    while (cnt < EvtPools.npool) {
        size_t          currSize;

        currSize = ES_PMEM_ATTR_BLOCK_SIZE_GET(EvtPools.handle[cnt]);

        if (currSize >= size) {

            return (EvtPools.handle[cnt]);
        }
        cnt++;
    }

    return (NULL);
}

/*----------------------------------------------------------------------------*/
esEvt_T * esEvtCreate(
    size_t              size,
    esEvtId_T           id) {

    portReg_T           intrCtx;
    esEvt_T *           newEvt;

    ES_DBG_API_REQUIRE(ES_DBG_OUT_OF_RANGE, sizeof(esEvt_T) <= size);

    ES_CRITICAL_LOCK_ENTER(&intrCtx);
#if   (0 == CFG_EVT_STORAGE)
    {
        esPMemHandle_T * pool;

        pool = poolFindI_(
            size);

        if (NULL != pool) {
            newEvt = esPMemAllocI(
                pool);
        } else {
            newEvt = esDMemAllocI(
                &DefDMemHandle,
                size);
        }
    }
#elif (1 == CFG_EVT_STORAGE)
    {
        esPMemHandle_T * pool;

        pool = poolFindI_(
            size);
        newEvt = NULL;

        if (NULL != pool) {
            newEvt = esPMemAllocI(
                pool);
        }
    }
#elif (2 == CFG_EVT_STORAGE)
    newEvt = esDmemAllocI(
        &DefDMemHandle,
        size);
#elif (3 == CFG_EVT_STORAGE)
    newEvt = malloc(
        size);
#endif
    ES_CRITICAL_LOCK_EXIT(intrCtx);
    evtInit_(
        newEvt,
        size,
        id);

    return (newEvt);
}

/*----------------------------------------------------------------------------*/
esEvt_T * esEvtCreateI(
    size_t              size,
    esEvtId_T           id) {

    esEvt_T *           newEvt;

    ES_DBG_API_REQUIRE(ES_DBG_OUT_OF_RANGE, sizeof(esEvt_T) <= size);

    newEvt = esDMemAllocI(
        &DefDMemHandle,
        size);                                                                  /* Dobavi potreban memorijski prostor za dogadjaj           */
    evtInit_(
        newEvt,
        size,
        id);

    return (newEvt);
}

/*----------------------------------------------------------------------------*/
void esEvtReserve(
    esEvt_T *           evt) {

    ES_DBG_API_REQUIRE(ES_DBG_POINTER_NULL, NULL != evt);
    ES_DBG_API_REQUIRE(ES_DBG_OBJECT_NOT_VALID, EVT_SIGNATURE == evt->signature);

    evt->attrib |= EVT_RESERVED_Msk;
}

/*----------------------------------------------------------------------------*/
void esEvtUnReserve(
    esEvt_T *       evt) {

    ES_DBG_API_REQUIRE(ES_DBG_POINTER_NULL, NULL != evt);
    ES_DBG_API_REQUIRE(ES_DBG_OBJECT_NOT_VALID, EVT_SIGNATURE == evt->signature);

    evt->attrib &= ~EVT_RESERVED_Msk;
}

/*----------------------------------------------------------------------------*/
void esEvtDestroy(
    esEvt_T *           evt) {

    portReg_T           intrCtx;

    ES_CRITICAL_LOCK_ENTER(&intrCtx);
    esEvtDestroyI(
        evt);
    ES_CRITICAL_LOCK_EXIT(intrCtx);
}

/*----------------------------------------------------------------------------*/
void esEvtDestroyI(
    esEvt_T *           evt) {

    ES_DBG_API_REQUIRE(ES_DBG_POINTER_NULL, NULL != evt);
    ES_DBG_API_REQUIRE(ES_DBG_OBJECT_NOT_VALID, EVT_SIGNATURE == evt->signature);

    if (0U == evt->attrib) {
        evtDeInit_(
            evt);
        esDMemDeAllocI(
            &DefDMemHandle,
            evt);
    }
}

/** @} *//*-------------------------------------------------------------------*/

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of evt.c
 ******************************************************************************/
