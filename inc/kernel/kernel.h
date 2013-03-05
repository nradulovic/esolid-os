/*************************************************************************************************
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
 *//******************************************************************************************//**
 * @file
 * @author      Nenad Radulovic
 * @brief       Interfejs eSolid operativnog sistema.
 * @addtogroup  kernel_intf
 ****************************************************************************************//** @{ */


#ifndef KERNEL_H_
#define KERNEL_H_

/*============================================================================  INCLUDE FILES  ==*/
#include "hal/hal.h"
#include "../config/kernel_config.h"

/*==================================================================================  DEFINES  ==*/

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

/*==================================================================================  MACRO's  ==*/
/*-------------------------------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*===============================================================================  DATA TYPES  ==*/

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

/*-------------------------------------------------------------------------------------------*//**
 * @name        Karakteristicni tipovi podataka
 * @{ *//*---------------------------------------------------------------------------------------*/

/**
 * @brief       Memorijska klasa alokatora
 */
typedef struct esMemClass esMemClass_T;

/**
 * @brief       Zaglavlje dogadjaja
 */
typedef struct esEvtHeader esEvtHeader_T;

/**
 * @brief       Definiciona struktura koja opisuje jedan EPA objekat
 */
typedef struct esEpaDef esEpaDef_T;

/**
 * @brief       Zaglavlje Event Processing Agent objekta
 * @details     Ova struktura se koristi prilikom kreiranja strukture Workspace
 *              EPA objekta. Struktura se koristi i za referenciranje EPA
 *              objekata.
 */
typedef struct esEpaHeader esEpaHeader_T;

/** @} *//*--------------------------------------------------------------------------------------*/

#include "kernel/mm.h"
#include "kernel/evt.h"
#include "kernel/smp.h"
#include "kernel/core.h"

/*=========================================================================  GLOBAL VARIABLES  ==*/
/*======================================================================  FUNCTION PROTOTYPES  ==*/

/*-------------------------------------------------------------------------------------------*//**
 * @name        Osnovne funkcije kernel-a
 * @brief       Ove funkcije se koriste za upravljanjem kernel-om
 * @{ *//*---------------------------------------------------------------------------------------*/

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

/** @} *//*--------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*===================================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/

/** @endcond *//** @} *//*************************************************************************
 * END of kernel.h
 *************************************************************************************************/
#endif /* KERNEL_H_ */
