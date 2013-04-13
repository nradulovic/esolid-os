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
 * @brief       Implementacija LOGer-a
 * @addtogroup  log_impl
 *********************************************************************//** @{ */

/*=========================================================  INCLUDE FILES  ==*/
#define log_VAR
#include "kernel/log.h"

/*===============================================================  DEFINES  ==*/
/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/

struct logMsg {
    uint16_t        rid;
    uint8_t         type;
    uint8_t         size;
    uint32_t        timestamp;
    uint32_t        var;
};


/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

/*----------------------------------------------------------------------------*/
void esLogInit(esLog_T * log,
    const C_ROM esLogDescriptor_T * C_ROM_VAR logDescriptor) {

    (void)log;
    (void)logDescriptor;
}

/*----------------------------------------------------------------------------*/
void esLogSwitchSetOn(
    esLog_T *       log,
    uint32_t        switches) {

    log->switches |= switches;
}

/*----------------------------------------------------------------------------*/
uint16_t logMsg(
    const esLog_T * log,
    logType_T       type,
    uint32_t        msg,
    uint32_t        val) {

    const C_ROM char * text;
    size_t size;

    if (msg <= log->logDescriptor->entries) {
        text = log->logDescriptor->textTable[msg];
        size = log->logDescriptor->sizeTable[msg];
    } else {
        text = "UNKNOWN TEXT";
    }

    switch (type) {
        case LOG_TYPE_ERR :
        case LOG_TYPE_DBG : {

            esCpuStop();
            break;
        }

        default : {

            break;
        }
    }

    return (0U);
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of log.c
 ******************************************************************************/
