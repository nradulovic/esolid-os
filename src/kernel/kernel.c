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
 * @brief       Implementacija kernel-a.
 * @addtogroup  kernel_impl
 *********************************************************************//** @{ */
                                                                                                  
/*=========================================================  INCLUDE FILES  ==*/
#define KERNEL_PRIVATE_H_VAR
#include "kernel_private.h"

/*===============================================================  DEFINES  ==*/
/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/

/**
 * @brief       Trenutno stanje kernel-a
 */
static esKernelStatus_T gKernelStatus;

/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

/*----------------------------------------------------------------------------*/
esEpa_T * esKernelEpaGet(
    void) {

    return (schedEpaGetCurrent_());
}

/*----------------------------------------------------------------------------*/
void kernelInit(
    void) {

    esSmpInit();
    schedInit();

#if defined(OPT_STP_ENABLE)
    esStpInit();
#endif
    gKernelStatus = KERNEL_STOPPED;
}

/*----------------------------------------------------------------------------*/
C_NAKED void esKernelStart(
    void) {

    gKernelStatus = KERNEL_RUNNING;

    sched();
}

/*----------------------------------------------------------------------------*/
esKernelStatus_T esKernelStatus(
    void) {

    return (gKernelStatus);
}
/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/

/** @endcond *//** @} *//******************************************************
 * END of kernel.c
 ******************************************************************************/
