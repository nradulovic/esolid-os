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
 * ----------------------------------------------------------------------------
 *
 * Project settings:
 * - include paths:
 *      1. ./esolid-os/inc
 * - source paths:
 *      1. ./esolid-hal, exclude: doc/
 * - references
 *      1. esolid-hal
 * - exported include paths:
 *      1. ./esolid-os/inc
 * - exported lib include paths:
 *      1. ./esolid-os/<profile name>
 * - exported lib:
 *      1. esolid-os
 *//***********************************************************************//**
 * @file
 * @author      Nenad Radulovic
 * @brief       Interfejs eSolid operativnog sistema.
 * @addtogroup  kernel_intf
 *********************************************************************//** @{ */


#ifndef KERNEL_H_
#define KERNEL_H_

/*=========================================================  INCLUDE FILES  ==*/
#include "kernel/core.h"

/*===============================================================  DEFINES  ==*/

/**
 * @brief       Znakovna konstanta koja pokazuje puno ime i verziju eSolid-a.
 */
#define ES_KERNEL_ID                    "eSolid v1.0"

/**
 * @brief       Numericka vrednost major verzije eSolid-a.
 */
#define ES_KERNEL_VERSION_MAJOR         1U

/**
 * @brief       Numericka vrednost minor verzije eSolid-a.
 */
#define ES_KERNEL_VERSION_MINOR         0U

/*===============================================================  MACRO's  ==*/
/*------------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/**
 * @brief       Stanje kernel-a
 */
typedef enum esKernelStatus {
/**
 * @brief       Kernel se ne izvrsava
 */
    KERNEL_STOPPED,

/**
 * @brief       Kernel se izvrsava
 */
    KERNEL_RUNNING
} esKernelStatus_T;

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Osnovne funkcije kernel-a
 * @brief       Ove funkcije se koriste za upravljanjem kernel-om
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Inicijalizacija kernel-a
 * @details     Ova funkcija vrsi najpre inicijalizaciju HAL-a, zatim
 *              memorijskog menadzera i prelazi na inicijalizaciju samog kernela.
 * @api
 */
void esKernelInit(
    void);

/**
 * @brief       Pokrece izvrsavanje jezgra
 * @details     Pokrecu se svi prethodno kreirani EPA objekti.
 * @api
 */
void esKernelStart(
    void);

/**
 * @brief       Vraca odgovor da li trenutno radi kernel
 * @return      Status izvrsavanja kernel-a.
 *  @retval     KERNEL_STOPPED - kernel se ne izvrsava,
 *  @retval     KERNEL_RUNNING - kernel se izvrsava,
 * @api
 */
esKernelStatus_T esKernelStatus(
    void);

/** @} *//*-------------------------------------------------------------------*/
/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of kernel.h
 ******************************************************************************/
#endif /* KERNEL_H_ */
