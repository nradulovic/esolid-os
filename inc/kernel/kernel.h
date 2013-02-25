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
 * ------------------------------------------------------------------------------------------------
 * @addtogroup  kernel_intf
 ****************************************************************************************//** @{ */


#ifndef KERNEL_H_
#define KERNEL_H_

/*============================================================================  INCLUDE FILES  ==*/
#include "hal/hal.h"
#include "../config/kernel_config.h"

/*==================================================================================  DEFINES  ==*/
/*==================================================================================  MACRO's  ==*/
/*-------------------------------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*===============================================================================  DATA TYPES  ==*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Deklaracije unapred
 * @{ *//*---------------------------------------------------------------------------------------*/

/**
 * @brief		Moguca stanja kernela
 */
typedef enum esKernelStatus esKernelStatus_T;

/**
 * @brief		Povratni tip funkcija stanja
 */
typedef enum state esState_T;

/**
 * @brief       Memorijska klasa alokatora
 */
typedef struct esMemClass esMemClass_T;

/**
 * @brief       Zaglavlje dogadjaja
 */
typedef struct esEvtHdr esEvtHdr_T;

/**
 * @brief       Zaglavlje Event Processing Agent objekta
 */
typedef struct esEpaHdr esEpaHdr_T;

/**
 * @brief		Definiciona struktura EPA objekta
 */
typedef struct esEpaDef esEpaDef_T;

/**
 * @brief       Tip pokazivaca na state handler funkcije.
 * @details		Funkcije vracaju esState_T , a kao parametar prihvataju
 *              pokazivac na strukturu izvrsne jedinice i pokazivac na
 *              dogadjaj.
 */
typedef esState_T (* esPtrState_T) (esEpaHdr_T *, esEvtHdr_T *);

/** @} *//*--------------------------------------------------------------------------------------*/

#include "kernel/core.h"
#include "kernel/smp.h"
#include "kernel/evt.h"
#include "kernel/mm.h"

/*=========================================================================  GLOBAL VARIABLES  ==*/
/*======================================================================  FUNCTION PROTOTYPES  ==*/
/*---------------------------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*===================================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/

/** @endcond *//** @} *//*************************************************************************
 * END of kernel.h
 *************************************************************************************************/
#endif /* KERNEL_H_ */
