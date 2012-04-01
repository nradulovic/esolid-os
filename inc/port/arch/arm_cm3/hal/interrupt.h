/*************************************************************************************************
 * Standard header file: interrupt.h
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
 * @author      Nenad Radulovic
 *
 * @brief       Interfejs Interrupt modula za ARM-CM3 arhitekturu, port klasa:
 *              arhitektura.
 *
 * ------------------------------------------------------------------------------------------------
 *
 * @addtogroup  intr_intf
 *
 ****************************************************************************************//** @{ */


#ifndef INTERRUPT_H_
#define INTERRUPT_H_


/*************************************************************************************************
 * INCLUDE FILES
 *************************************************************************************************/

/*-----------------------------------------------------------------------------------------------*
 * Module dependencies
 *-----------------------------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------------------------*
 * Module configuration and adapter
 *-----------------------------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------------------------*
 * EXTERNS
 *-----------------------------------------------------------------------------------*//** @cond */


#ifdef INTERRUPT_H_VAR
#define INTERRUPT_H_EXT
#else
#define INTERRUPT_H_EXT extern
#endif


/** @endcond*//***********************************************************************************
 * DEFINES
 *************************************************************************************************/

/*-------------------------------------------------------------------------------------------*//**
 * @name        Konstante za ARM Cortex-M3 arhitekturu
 *
 * @brief       Konstante koje definisu konfiguraciju eSolid Core modula.
 *
 * @{ *//*---------------------------------------------------------------------------------------*/

#if !defined(OPT_MAX_INTERRUPT_PRIO) || defined(__DOXYGEN__)
# define OPT_MAX_INTERRUPT_PRIO         0x40
#endif

/** @} *//*--------------------------------------------------------------------------------------*/


/*************************************************************************************************
 * MACRO's
 *************************************************************************************************/

/*-------------------------------------------------------------------------------------------*//**
 * @name        Implementacija za ARM Cortex-M3 arhitekturu
 *
 *              Preferiraju se asembler komande za implementaciju makroa.
 *
 * @{ *//*---------------------------------------------------------------------------------------*/

#if !defined(HAL_INT_ENABLE) || defined(__DOXYGEN__)
# define HAL_INT_ENABLE()                                                       \
    __asm volatile ("cpsie i")
#endif

#if !defined(HAL_INT_DISABLE) || defined(__DOXYGEN__)
# define HAL_INT_DISABLE()                                                      \
    __asm volatile ("cpsid i")
#endif

#if !defined(HAL_INT_PRIO_MASK_SET) || defined(__DOXYGEN__)
# define HAL_INT_PRIO_MASK_SET(prio)                                            \
    do {                                                                        \
        register uint32_t reg = (prio);                                         \
        __asm volatile (                                                        \
            "msr basepri, %0"                                                   \
            :: "r"(reg) : "memory", "cc");                                      \
    } while (0)
#endif

#if !defined(HAL_INT_PRIO_MASK_GET) || defined(__DOXYGEN__)
# define HAL_INT_PRIO_MASK_GET()                                                \
    C_EXT ({                                                                    \
        register uint32_t reg;                                                  \
        __asm volatile (                                                        \
            "mrs %0, basepri"                                                   \
            : "=r"(reg) :: "memory", "cc");                                     \
        reg;                                                                    \
    })
#endif

#if !defined(HAL_INT_PRIO_MASK_SAVE_SET) || defined(__DOXYGEN__)
# define HAL_INT_PRIO_MASK_SAVE_SET(variable, prio)                             \
    do {                                                                        \
        (variable) = HAL_INT_PRIO_MASK_GET();                                   \
        HAL_INT_PRIO_MASK_SET(prio);                                            \
    } while (0)
#endif

#if !defined(HAL_INT_PRIO_MASK_RESTORE) || defined(__DOXYGEN__)
# define HAL_INT_PRIO_MASK_RESTORE(variable)                                    \
    do {                                                                        \
        __asm volatile (                                                        \
            "msr basepri, %0"                                                   \
            :: "r"(variable) : "memory", "cc");                                 \
    } while (0)
#endif

#if !defined(HAL_CRITICAL_DECL) || defined(__DOXYGEN__)
# define HAL_CRITICAL_DECL()													\
	uint32_t irqLock_
#endif

#if !defined(HAL_CRITICAL_ENTER) || defined(__DOXYGEN__)
# define HAL_CRITICAL_ENTER()													\
	HAL_INT_PRIO_MASK_SAVE_SET(irqLock_, OPT_MAX_INTERRUPT_PRIO)
#endif

#if !defined(HAL_CRITICAL_EXIT) || defined(__DOXYGEN__)
# define HAL_CRITICAL_EXIT()													\
	HAL_INT_PRIO_MASK_RESTORE(irqLock_)
#endif

#if !defined(HAL_CRITICAL_GET) || defined(__DOXYGEN__)
# define HAL_CRITICAL_GET()                                                     \
    irqLock_ = HAL_INT_PRIO_MASK_GET()
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
 * END of interrupt.h
 *************************************************************************************************/
#endif /* INTERRUPT_H_ */
