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
 * @brief       Osnovne funkcije kernel-a
 * ------------------------------------------------------------------------------------------------
 * @addtogroup  kernel_intf
 ****************************************************************************************//** @{ */


#ifndef CORE_H_
#define CORE_H_

/*============================================================================  INCLUDE FILES  ==*/
/*==================================================================================  DEFINES  ==*/
/*==================================================================================  MACRO's  ==*/
/*-------------------------------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif


/*===============================================================================  DATA TYPES  ==*/
/*-------------------------------------------------------------------------------------------*//**
 * @brief       Stanje kernel-a
 *//*--------------------------------------------------------------------------------------------*/
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
 * @brief       Kontrolna struktura kernel-a
 * @notapi
 *//*--------------------------------------------------------------------------------------------*/
typedef struct kernelCtrl {
#if defined(OPT_KERNEL_USE_ROUND_ROBIN) || defined(__DOXYGEN__)
/**
 * @brief       Lista EPA objekata sa istim prioritetom
 */
    esDlsList_T     list;
#endif

/**
 * @brief       Prioritet EPA objekta.
 * @details     Ova promenljiva odredjuje prioritet datog EPA objekta.
 */
    uint_fast8_t    prio;
} kernelCtrl_T;

/*=========================================================================  GLOBAL VARIABLES  ==*/
/*======================================================================  FUNCTION PROTOTYPES  ==*/
/*-------------------------------------------------------------------------------------------*//**
 * @brief       Inicijalizacija kernel-a
 * @param       aMemory                 Pokazivac na memorijski prostor
 *                                      rezervisan za rad eSolid kernel-a,
 * @param       aMemorySize             velicina rezervisanog memorijskog
 *                                      prostora,
 * @param       aHeapSize               deo rezervisanog memorijskog prostora
 *                                      koji treba da se odvoji za dinamicki
 *                                      alokator.
 * @api
 *//*--------------------------------------------------------------------------------------------*/
void esKernelInit(
    void                * aMemory,
    size_t              aMemorySize,
    size_t              aHeapSize);

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Pokrece izvrsavanje jezgra
 * @pre         Da bi se koristila kernel funkcija mora se definisati
 * @api
 *//*--------------------------------------------------------------------------------------------*/
void esKernelStart(
    void);

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Vraca odgovor da li trenutno radi kernel
 * @return      Status izvrsavanja kernel-a.
 *  @retval     KERNEL_STOPPED - kernel se ne izvrsava,
 *  @retval     KERNEL_RUNNING - kernel se izvrsava,
 * @api
 *//*--------------------------------------------------------------------------------------------*/
esKernelStatus_T esKernelStatus(
    void);

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Vraca znakovni niz koji identifikuje eSolid kernel
 * @return      Verzija kernel-a u obliku znakovnog niza.
 * @api
 *//*--------------------------------------------------------------------------------------------*/
const C_ROM char * esKernelSysVer(
    void);

/** @} *//*--------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*===================================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/

/** @endcond *//** @} *//*************************************************************************
 * END of core.h
 *************************************************************************************************/
#endif /* CORE_H_ */
