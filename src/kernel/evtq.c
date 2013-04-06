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
 * @brief       Implementacija Event Queue modula
 * @addtogroup  evtq_impl
 *********************************************************************//** @{ */
                                                                                                  
/*=========================================================  INCLUDE FILES  ==*/
#define EVTQ_PKG_H_VAR
#include "kernel_private.h"


/*===============================================================  DEFINES  ==*/
/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/

/*----------------------------------------------------------------------------*/
size_t evtQReqSize(
    uint8_t         levels) {

    return ((size_t)(levels * sizeof(void **)));
}

/*----------------------------------------------------------------------------*/
void evtQInit(
    evtQueue_T *    evtQ,
    esEvt_T **      evtQBuff,
    uint8_t         size) {

    if (ES_LOG_IS_DBG(&gKernelLog, LOG_FILT_EPA)) {
        ES_LOG_DBG_IF_INVALID(&gKernelLog, size >= 1UL, LOG_EVTQ_INIT, ES_ARG_OUT_OF_RANGE);
    }

    esQpInit_(
        &evtQ->queue,
        (void **)evtQBuff,
        size);

#if (OPT_LOG_LEVEL <= LOG_INFO) || defined(__DOXYGEN__)
    evtQ->free = esQpFreeSpace_(
        &evtQ->queue);
    evtQ->freeMin = evtQ->free;
#endif
}

/*----------------------------------------------------------------------------*/
void evtQDeInit(
    evtQueue_T *    evtQ) {

    esQpDeInit_(
        &evtQ->queue);

#if (OPT_LOG_LEVEL <= LOG_INFO) || defined(__DOXYGEN__)
    evtQ->freeMin = evtQ->free = 0U;
#endif
}

/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/
/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/

/** @endcond *//** @} *//******************************************************
 * END of evtq.c
 ******************************************************************************/
