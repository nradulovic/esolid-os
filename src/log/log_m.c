/******************************************************************************
 * This file is part of esolid-kernel
 *
 * Template version: 1.1.13 (21.10.2012)
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
 * @author      Nenad Radulovic
 * @brief       Skup LOG poruka
 * @addtogroup  logm
 *********************************************************************//** @{ */

/*=========================================================  INCLUDE FILES  ==*/
#define LOG_M_H_VAR
#include "../src/kernel/kernel_private.h"

/*=========================================================  LOCAL DEFINES  ==*/
/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/

/**
 * @brief       Tabela teksta
 */
static const char C_ROM * C_ROM_VAR const logMsgText[LOG_LAST_ENTRY] = {
    TEXT_TABLE(EXPAND_AS_TEXT)
};

/**
 * @brief       Tabela velicina
 */
static const size_t logMsgSize[LOG_LAST_ENTRY] = {
    TEXT_TABLE(EXPAND_AS_SIZE)
};

/*======================================================  GLOBAL VARIABLES  ==*/

/**
 * @brief       Deskriptor struktura za LOG sistem
 */
const C_ROM esLogDescriptor_T gKernelLogDescriptor = {
    .entries = LOG_LAST_ENTRY,
    .textTable = &logMsgText[0],
    .sizeTable = &logMsgSize[0],
    .getBlock = (void * (*)(size_t))0U,
    .pushBlock = (void (*)(void *))0U
};

/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/
/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of log_m.c
 ******************************************************************************/
