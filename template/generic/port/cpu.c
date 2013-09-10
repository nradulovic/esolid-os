/******************************************************************************
 * This file is part of esolid-rtos
 *
 * Copyright (C) 2011, 2012 - Nenad Radulovic
 *
 * esolid-rtos is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * esolid-rtos is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with esolid-rtos; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301  USA
 *
 * web site:    http://blueskynet.dyndns-server.com
 * e-mail  :    blueskyniss@gmail.com
 *//***********************************************************************//**
 * @file
 * @author      Nenad Radulovic
 * @brief       Implementation of CPU port - Template
 * @addtogroup  template_cpu_impl
 *********************************************************************//** @{ */

/*=========================================================  INCLUDE FILES  ==*/
#include "kernel.h"

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/

uint8_t portReg_T gPortIsrNesting_;

/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

esReg_T portIntGet_(
    void) {

    return (0U);
}

void portIntDisable_(
    void) {

}

void portIntSet_(
    esReg_T         status) {

    (void)status;
}

/*
 * This is a generic implementation of FLS algorthm and it should work on every
 * port available.
 */
uint_fast8_t portFindLastSet_(
    esReg_T     val) {

    esReg_T tmp;
    uint_fast8_t rtn;

    rtn = 0U;

#if (32U == PORT_DATA_WIDTH)
    tmp = val >> 16;

    if (0U != tmp) {
        val = tmp;
        rtn = 16U;
    }
#endif

#if (16U <= PORT_DATA_WIDTH)
    tmp = val >> 8;

    if (0U != tmp) {
        val = tmp;
        rtn += 8U;
    }
#endif
    tmp = val >> 4;

    if (0U != tmp) {
        val = tmp;
        rtn += 4U;
    }
    tmp = val >> 2;

    if (0U != tmp) {
        val = tmp;
        rtn += 2U;
    }
    tmp = val >> 1;

    if (0U != tmp) {
        val = tmp;
        rtn += 1U;
    }

    return (rtn);
}

void portSysTmrInit_(
    void) {

}

void portSysTmrEnable_(
    void) {

}

void portSysTmrDisable_(
    void) {

}

void portThdStart_(
    void) {

}

void * portCtxInit_(
    void *          stck,
    size_t          stckSize,
    void (* thdf)(void *),
    void *          arg) {

    (void)stck;
    (void)stckSize;
    (void)thdf;
    (void)arg;

    return (NULL);
}

void portCtxSw_(
    void) {

}

void portCtxSwIsr_(
    void) {

}

void portInitEarly_(
    void) {

}

void portInit_(
    void) {

}

void portInitLate_(
    void) {

}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of cpu.c
 ******************************************************************************/
