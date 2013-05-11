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
#include "evt_pkg.h"

/*=========================================================  LOCAL DEFINES  ==*/
/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/

struct evtDB {
    const C_ROM esEvtDBElem_T ** elem;

#if (OPT_LOG_LEVEL <= LOG_DBG)
    uint16_t        elements;
#endif
};

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

static struct evtDB gEvtDB;

/*======================================================  GLOBAL VARIABLES  ==*/

OPT_MEM_DYN_T gEvtDynStorage;

/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

/*----------------------------------------------------------------------------*/
static C_INLINE_ALWAYS void evtInit_(
    esEvt_T *       evt,
    size_t          size,
    esEvtId_T       id) {

#if (OPT_LOG_LEVEL <= LOG_DBG)
    evt->signature = EVT_SIGNATURE;
#endif

    evt->id = id;
    evt->attrib = 0U;                                                           /* Dogadjaj je dinamican, sa 0 korisnika.                   */

#if (1U == OPT_EVT_USE_TIMESTAMP)
# if defined(1U == OPT_EVT_TIMESTAMP_CALLBACK)
    evt->timestamp = appEvtTimestampGet();
# endif
#endif

#if (1U == OPT_EVT_USE_GENERATOR)
# if (1U == OPT_EVT_GENERATOR_CALLBACK)
    evt->generator = appEvtGeneratorGet();
# endif
#endif

#if (1U == OPT_EVT_USE_SIZE)
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

/*----------------------------------------------------------------------------*/
void esEvtDBRegister(
    const C_ROM esEvtDBElem_T * evtDB[],
    uint16_t        elements) {

    gEvtDB.elem = evtDB;

#if (OPT_LOG_LEVEL <= LOG_DBG)
    gEvtDB.elements = elements;                                                 /* Snimi broj elemenata kako bi ogranicili ulazni opseg id  */

# if (1U == OPT_EVT_USE_MEM_POOL) && (0U == OPT_MEM_POOL_EXTERN)                /* Proveri da li pool moze da sadrzi dogadjaje              */
    {
        uint16_t cntr;

        for (cntr = 0U; cntr < elements; cntr++) {
            ES_LOG_DBG_IF_INVALID(log, gEvtDB.elem[cntr] <= gEvtDB.elem[cntr]->pool.blockSize, "Pool size too small", 1U);
        }
    }
# endif
#endif
}

/*----------------------------------------------------------------------------*/
size_t esEvtDBQuerySize(
    esEvtId_T       id) {

    return (gEvtDB.elem[id]->size);
}

/*----------------------------------------------------------------------------*/
const C_ROM char * esEvtDBQueryName(
    esEvtId_T       id) {

    const C_ROM char * name;

#if (1U == OPT_EVT_DB_USE_DESC_DATA)
    name = gEvtDB.elem[id]->name;
#else
    (void)id;

    name = "unknown";
#endif

    return (name);
}

/*----------------------------------------------------------------------------*/
const C_ROM char * esEvtDBQueryType(
    esEvtId_T       id) {

    const C_ROM char * type;

#if (1U == OPT_EVT_DB_USE_DESC_DATA)
    type = gEvtDB.elem[id]->type;
#else
    (void)id;

    type = "unknown";
#endif

    return (type);
}

/*----------------------------------------------------------------------------*/
const C_ROM char * esEvtDBQueryDesc(
    esEvtId_T       id) {

    const C_ROM char * desc;

#if (1U == OPT_EVT_DB_USE_DESC_DATA)
    desc = gEvtDB.elem[id]->desc;

    if (NULL == desc) {
        desc = "no description";
    }
#else
    (void)id;

    desc = "no description";
#endif

    return (desc);
}

/*----------------------------------------------------------------------------*/
esEvt_T * esEvtCreate(
    esEvtId_T       id) {

    esEvt_T * newEvt;

#if (1U == OPT_EVT_USE_MEM_DYN) && (0U == OPT_EVT_USE_MEM_POOL)                 /* Koristi se samo dinamicki alokator                       */
    newEvt = OPT_MEM_DYN_ALLOC(
        &gEvtDynStorage,
        gEvtDB.elem[id]->size);                                                 /* Dobavi potreban memorijski prostor za dogadjaj           */
#elif (0U == OPT_EVT_USE_MEM_DYN) && (1U == OPT_EVT_USE_MEM_POOL)               /* Koristi se samo pool alokator                            */
    newEvt = OPT_MEM_POOL_ALLOC(
        gEvtDB.elem[id]->pool);
#else                                                                           /* Koriste se oba alokatora                                 */

    if (NULL != gEvtDB.elem[id]->pool) {                                        /* Ako je pokazivac != NULL onda je to pool alokator        */
        newEvt = OPT_MEM_POOL_ALLOC(
            gEvtDB.elem[id]->pool);
    } else {
        newEvt = OPT_MEM_DYN_ALLOC(
            &gEvtDynStorage,
            gEvtDB.elem[id]->size);                                             /* Dobavi potreban memorijski prostor za dogadjaj           */
    }
#endif
    evtInit_(
        newEvt,
        gEvtDB.elem[id]->size,
        id);

    return (newEvt);
}

/*----------------------------------------------------------------------------*/
esEvt_T * esEvtCreateI(
    esEvtId_T       id) {

    esEvt_T * newEvt;

#if (1U == OPT_EVT_USE_MEM_DYN) && (0U == OPT_EVT_USE_MEM_POOL)                 /* Koristi se samo dinamicki alokator                       */
    newEvt = OPT_MEM_DYN_ALLOCI(&gEvtDynStorage, gEvtDB.elem[id]->size);        /* Dobavi potreban memorijski prostor za dogadjaj           */
#elif (0U == OPT_EVT_USE_MEM_DYN) && (1U == OPT_EVT_USE_MEM_POOL)               /* Koristi se samo pool alokator                            */
    newEvt = OPT_MEM_POOL_ALLOCI(gEvtDB.elem[id]->pool);
#else                                                                           /* Koriste se oba alokatora                                 */

    if (NULL != gEvtDB.elem[id]->pool) {                                        /* Ako je pokazivac != NULL onda je to pool alokator        */
        newEvt = OPT_MEM_POOL_ALLOCI(gEvtDB.elem[id]->pool);
    } else {
        newEvt = OPT_MEM_DYN_ALLOCI(&gEvtDynStorage, gEvtDB.elem[id]->size);    /* Dobavi potreban memorijski prostor za dogadjaj           */
    }
#endif
    evtInit_(
        newEvt,
        gEvtDB.elem[id]->size,
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
        evtDeInit_(
            evt);

#if (1U == OPT_EVT_USE_MEM_DYN) && (0U == OPT_EVT_USE_MEM_POOL)                 /* Koristi se samo dinamicki alokator                       */
        OPT_MEM_DYN_DEALLOC(&gEvtDynStorage, evt);
#elif (0U == OPT_EVT_USE_MEM_DYN) && (1U == OPT_EVT_USE_MEM_POOL)               /* Koristi se samo pool alokator                            */
        OPT_MEM_POOL_DEALLOC(&gEvtDB.elem[evt->id]->pool, evt);
#else                                                                           /* Koriste se oba alokatora                                 */

        if (NULL != gEvtDB.elem[evt->id]->pool) {
            OPT_MEM_POOL_DEALLOC(gEvtDB.elem[evt->id]->pool, evt);
        } else {
            OPT_MEM_DYN_DEALLOC(&gEvtDynStorage, evt);
        }
#endif
    }
}

/*----------------------------------------------------------------------------*/
void esEvtDestroyI(
    esEvt_T *       evt) {

    if (0U == evt->attrib) {
        evtDeInit_(
            evt);

#if (1U == OPT_EVT_USE_MEM_DYN) && (0U == OPT_EVT_USE_MEM_POOL)                 /* Koristi se samo dinamicki alokator                       */
        OPT_MEM_DYN_DEALLOCI(&gEvtDynStorage, evt);
#elif (0U == OPT_EVT_USE_MEM_DYN) && (1U == OPT_EVT_USE_MEM_POOL)               /* Koristi se samo pool alokator                            */
        OPT_MEM_POOL_DEALLOCI(gEvtDB.elem[evt->id]->pool, evt);
#else                                                                           /* Koriste se oba alokatora                                 */

        if (NULL != gEvtDB.elem[evt->id]->pool) {
            OPT_MEM_POOL_DEALLOCI(gEvtDB.elem[evt->id]->pool, evt);
        } else {
            OPT_MEM_DYN_DEALLOCI(&gEvtDynStorage, evt);
        }
#endif
    }
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of evt.c
 ******************************************************************************/
