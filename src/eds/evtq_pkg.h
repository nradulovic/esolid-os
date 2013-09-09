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
 * @author  	Nenad Radulovic
 * @brief       Interface of evtq_pkg.
 * @details     Detailed description
 * @note        Notes
 * @addtogroup  module_intf
 * @brief		Interface of evtq_pkg module.
 * @section		sec_name Section name
 *********************************************************************//** @{ */

#ifndef EVTQ_PKG_H_
#define EVTQ_PKG_H_

/*=========================================================  INCLUDE FILES  ==*/
#include "primitive/queue.h"

/*===============================================================  DEFINES  ==*/
/*===============================================================  MACRO's  ==*/
/*------------------------------------------------------  C++ extern begin  --*/
#if defined(__cplusplus)
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

typedef struct evtQueue evtQueue_T;

/*======================================================  GLOBAL VARIABLES  ==*/


/*===================================================  FUNCTION PROTOTYPES  ==*/

static C_INLINE_ALWAYS bool_T evtQIsEmptyI_(
    evtQueue_T *    evtQ) {

    return (esQpIsEmpty_(&evtQ->queue));
}

static C_INLINE_ALWAYS bool_T evtQIsFullI_(
    evtQueue_T *    evtQ) {

    return (esQpIsFull_(&evtQ->queue));
}

static C_INLINE_ALWAYS esEvt_T * evtQGetI_(
    evtQueue_T *    evtQ) {

    esEvt_T * evt;

    evt = esQpGet_(
        &evtQ->queue);

#if defined(OPT_KERNEL_DBG_EVT)
    ++(epa->evtQueue.free);
#endif

    return (evt);
}

static C_INLINE_ALWAYS void evtQPutAheadI_(
    evtQueue_T *    evtQ,
    esEvt_T *       evt) {

    esQpPutAhead_(
        &evtQ->queue,
        evt);

#if defined(OPT_KERNEL_DBG_EVT)
    --(evtQ->free);
    if (evtQ->free < evtQ->freeMin) {
        evtQ->freeMin = evtQ->free;
    }
#endif
}

static C_INLINE_ALWAYS void evtQPutI_(
    evtQueue_T *    evtQ,
    esEvt_T *       evt) {

    esQpPut_(
        &evtQ->queue,
        evt);

#if defined(OPT_KERNEL_DBG_EVT)
    --(evtQ->free);
    if (evtQ->free < evtQ->freeMin) {
        evtQ->freeMin = evtQ->free;
    }
#endif
}

void evtQInit(
    evtQueue_T *    evtQ,
    esEvt_T **      evtQBuff,
    uint8_t         size);

void evtQDeInit(
    evtQueue_T *    evtQ);

size_t evtQReqSize(
    uint8_t         levels);

/*--------------------------------------------------------  C++ extern end  --*/
#if defined(__cplusplus)
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/

/** @endcond *//** @} *//******************************************************
 * END of evtq_pkg.h
 ******************************************************************************/
#endif /* EVTQ_PKG_H_ */
