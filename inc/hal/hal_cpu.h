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
 * @brief       Interfejs za CPU modul.
 *
 * ------------------------------------------------------------------------------------------------
 *
 * @addtogroup  hal_cpu
 *
 ****************************************************************************************//** @{ */

#ifndef HAL_CPU_H_
#define HAL_CPU_H_


/*************************************************************************************************
 * INCLUDE FILES
 *************************************************************************************************/

/*-----------------------------------------------------------------------------------------------*
 * Module dependencies
 *-----------------------------------------------------------------------------------------------*/
#include "port/port.h"


/*-----------------------------------------------------------------------------------------------*
 * Module configuration and adapter
 *-----------------------------------------------------------------------------------------------*/
#include PORT_ARCH_HEADER(hal/cpu)


/*-----------------------------------------------------------------------------------------------*
 * EXTERNS
 *-----------------------------------------------------------------------------------*//** @cond */

#ifdef HAL_CPU_H_VAR
#define HAL_CPU_H_EXT
#else
#define HAL_CPU_H_EXT extern
#endif


/** @endcond*//***********************************************************************************
 * DEFINES
 *************************************************************************************************/


/*************************************************************************************************
 * MACRO's
 *************************************************************************************************/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Zaustavi procesor
 *
 *              Ovaj makro se obicno poziva u slucaju neke katastrofalne greske
 *              i treba zaustaviti dalji rad procesora. Kada se jednom ovaj
 *              makro pokrene on se ne moze prekinuti.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_CPU_STOP) || defined(__DOXYGEN__)
# define HAL_CPU_STOP()                                                         \
    while (1)

#endif

/*-------------------------------------------------------------------------------------------*//**
 * @name        Brza manipulacija sa bitovima
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Find Last set Bit makro
 *
 * @param       expr1                   Broj koji se ispituje.
 * @return      y = log2(expr1)
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_CPU_FLS) || defined(__DOXYGEN__)
# define HAL_CPU_FLS(expr1)
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Find First set Bit makro
 *
 * @param       expr1                   Broj koji se ispituje.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_CPU_FFS) || defined(__DOXYGEN__)
# define HAL_CPU_FFS(expr1)
#endif

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Atributi za CPU
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Operacije za citanje/upis iz ROM memorije su identicne
 *              operacijama za citanje/upis iz RAM memorije.
 *
 *              Ova promeljiva bi trebala da bide definisana PORT_TRUE samo
 *              ukoliko su C_ROM i C_ROM_VAR promenljive, u compiler modulu,
 *              definisane kao prazne.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(PORT_SUPP_IDENTIC_ROM_RAM) || defined(__DOXYGEN__)
# define PORT_SUPP_IDENTIC_ROM_RAM      PORT_FALSE
#endif

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Manipulacija SysTick brojaca.
 *
 * @brief       Makroi za rad sa SysTick brojacem.
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Izbor kloka koji se dovodi SysTick brojacu.
 *
 * @param       clksource               Klok koji se dovodi SysTick modulu.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_SYSTICK_CLOCK_CONFIG) || defined(__DOXYGEN__)
# define HAL_SYSTICK_CLOCK_CONFIG(clksource)
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Uzimanje trenutne vrednosti broja tick-ova SysTick modula.
 *
 * @return      Trenutna vrednost broja tick-ova u jedinici vremena.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_SYSTICK_GET_TICK) || defined(__DOXYGEN__)
# define HAL_SYSTICK_GET_TICK()
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Izbor broja tick-ova SysTick modula.
 *
 * @param       tick                	Broj tick-ova koje generise SysTick modul.
 * @return      1 ako nije korektna tick vrednost ili 0 ako jeste.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_SYSTICK_CONFIG) || defined(__DOXYGEN__)
# define HAL_SYSTICK_CONFIG(tick)
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Izvrsi inicijalizaciju SysTick.
 *
 * @param       clksource               Klok koji se dovodi SysTick modulu.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_SYSTICK_CLOCK_CONFIG) || defined(__DOXYGEN__)
# define HAL_SYSTICK_CLOCK_CONFIG(clksource)
#endif

/** @} *//*--------------------------------------------------------------------------------------*/


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
 * END of HEADER FILE
 *************************************************************************************************/
#endif /* HAL_CPU_H_ */
