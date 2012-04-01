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
 *************************************************************************************************/


/*********************************************************************************************//**
 * @file
 * @author  	Nenad Radulovic
 * @brief       Port Core modula, ARM Cortex-M3 arhitektura.
 * ------------------------------------------------------------------------------------------------
 * @addtogroup  core_impl
 ****************************************************************************************//** @{ */


#ifndef ARCH_CORE_H_
#define ARCH_CORE_H_

/*============================================================================  INCLUDE FILES  ==*/
/*==================================================================================  DEFINES  ==*/
/*==================================================================================  MACRO's  ==*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Port ARM Cortex-M3, makroi
 * @{ *//*---------------------------------------------------------------------------------------*/

#define EPE_SCHED_NOTIFY_RDY(epa)                                               \
    do {                                                                        \
        SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;                                    \
    } while (0)

/** @} *//*--------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*===============================================================================  DATA TYPES  ==*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Port ARM Cortex-M3, tipovi podataka
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Tip koji se koristi za cuvanje informacije o velicini bloka
 *
 *              Za ispravan rad alokatora potrebno je da velicina pokazivaca na
 *              podatke u RAM memoriji bude jednaka velicini tipa podataka koji
 *              cuva velicinu memorijskog bloka. Na nekim arhitekturama
 *              (uglavnom 16-bitne) @c sizeof(void *) se razlikuje od
 *              @c sizeof(size_t), pa se u tom  slucaju mora koristiti drugi tip
 *              podataka.
 *//*--------------------------------------------------------------------------------------------*/
typedef size_t                          hmemBlkSize_T;

/** @} *//*--------------------------------------------------------------------------------------*/

/*=========================================================================  GLOBAL VARIABLES  ==*/
/*======================================================================  FUNCTION PROTOTYPES  ==*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Port ARM Cortex-M3, port funkcije
 * @{ *//*---------------------------------------------------------------------------------------*/

#if defined(OPT_KERNEL_ENABLE) || defined(__DOXYGEN__)
C_NAKED void PendSV_Handler(
    void);
#endif

#if defined(OPT_KERNEL_ENABLE) || defined(__DOXYGEN__)
C_NAKED void SVC_Handler(
    void);
#endif

/** @} *//*--------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*===================================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/

/** @endcond *//** @} *//*************************************************************************
 * END of core.h
 *************************************************************************************************/
#endif /* ARCH_CORE_H_ */
