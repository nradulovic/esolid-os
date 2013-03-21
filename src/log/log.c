/******************************************************************************
 * This file is part of esolid-kernel
 *
 * Template version: 1.1.14 (10.03.2013)
 *
 * Copyright (C) 2011, 2012 - Nenad Radulovic
 *
 * esolid-kernel is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * esolid-kernel is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with esolid-kernel; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301  USA
 *
 * web site:    http://blueskynet.dyndns-server.com
 * e-mail  :    blueskyniss@gmail.com
 *//***********************************************************************//**
 * @file
 * @author      nenad
 * @brief       Implementation of log.
 * @details     Detailed description
 * @note        Notes
 * @addtogroup  module_impl
 * @brief		Implementation of log module.
 * @section		sec_name Section name
 *********************************************************************//** @{ */
                                                                                                  
/*=========================================================  INCLUDE FILES  ==*/
#define log_VAR
#include "log/log.h"

/*===============================================================  DEFINES  ==*/
/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

void logInit(esLog_T * log,
    const C_ROM esLogDescriptor_T * C_ROM_VAR logDescriptor) {

    (void)log;
    (void)logDescriptor;
}

void logSetSwitches(
    esLog_T *       log,
    uint32_t        switches) {

    (void)log;
    (void)switches;
}

void logMsg(
    const esLog_T * log,
    uint32_t        msg,
    uint32_t        val) {

    (void)log;
    (void)msg;
    (void)val;
}
/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/

/** @endcond *//** @} *//******************************************************
 * END of log.c
 ******************************************************************************/
