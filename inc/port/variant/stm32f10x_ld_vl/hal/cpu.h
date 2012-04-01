/*************************************************************************************************
 * This file is part of eSolid
 *
 * Copyright (C) 2011 - Nenad Radulovic
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
 *************************************************************************************************/


/*********************************************************************************************//**
 * @file
 *
 * @author      nesa
 *
 * @brief       Interfejs za CPU modul STM32F10x Low Density Value Line serija,
 *              port klasa: varijanta.
 *
 * ------------------------------------------------------------------------------------------------
 *
 * @addtogroup  hal_cpu
 *
 ****************************************************************************************//** @{ */


#ifndef VARNT_CPU_H_
#define VARNT_CPU_H_


/*************************************************************************************************
 * INCLUDE FILES
 *************************************************************************************************/

/*-----------------------------------------------------------------------------------------------*
 * Module dependencies
 *-----------------------------------------------------------------------------------------------*/

#include "misc.h"

/*-----------------------------------------------------------------------------------------------*
 * Module configuration and adapter
 *-----------------------------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------------------------*
 * EXTERNS
 *-----------------------------------------------------------------------------------*//** @cond */


#ifdef VARNT_CPU_H_VAR
#define VARNT_CPU_H_EXT
#else
#define VARNT_CPU_H_EXT extern
#endif


/** @endcond*//***********************************************************************************
 * DEFINES
 *************************************************************************************************/


/*************************************************************************************************
 * MACRO's
 *************************************************************************************************/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Inicijalizacija SysTick modula.
 *
 * @brief       Makroi za inicijalizaciju SysTick modula.
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Izbor kloka koji se dovodi SysTick modulu.
 *
 * @param       clksource               Klok koji se dovodi SysTick modulu.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_SYSTICK_CLOCK_CONFIG) || defined(__DOXYGEN__)
# define HAL_SYSTICK_CLOCK_CONFIG(clksource)                                    \
    do {                                                                        \
        if (clksource == SysTick_CLKSource_HCLK) {                              \
            SysTick->CTRL |= SysTick_CLKSource_HCLK;                            \
        } else {                                                                \
            SysTick->CTRL &= SysTick_CLKSource_HCLK_Div8;                       \
        }                                                                       \
    } while (0)
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Izbor broja tick-ova SysTick modula.
 *
 * @param       ticks               Broj tick-ova koje generise SysTick modul.
 * @return      1 ako nije korektna tick vrednost ili 0 ako jeste.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_SYSTICK_CONFIG) || defined(__DOXYGEN__)
# define HAL_SYSTICK_CONFIG(ticks)                                              \
      (uint32_t)SysTick_Config((uint32_t)ticks)

#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Uzimanje trenutne vrednosti broja tick-ova SysTick modula.
 *
 * @param       none.
 * @return      Trenutna vrednost broja tick-ova u jedinici vremena.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_SYSTICK_GET_VALUE) || defined(__DOXYGEN__)
# define HAL_SYSTICK_GET_VALUE()                                                \
      (uint32_t)(SysTick->VAL)

#endif

/*-----------------------------------------------------------------------------------------------*
 * C/C++ #ifdef - open
 *-----------------------------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif


/*************************************************************************************************
 * DATA TYPES
 *************************************************************************************************/


/*************************************************************************************************
 * GLOBAL VARIABLES
 *************************************************************************************************/


/*************************************************************************************************
 * FUNCTION PROTOTYPES
 *************************************************************************************************/


/*-----------------------------------------------------------------------------------------------*
 * C/C++ #endif - close
 *-----------------------------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif


/*************************************************************************************************
 * CONFIGURATION ERRORS
 *************************************************************************************//** @cond */


/** @endcond *//** @} *//*************************************************************************
 * END of cpu.h
 *************************************************************************************************/
#endif /* VARNT_CPU_H_ */
