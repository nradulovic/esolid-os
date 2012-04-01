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
 * @brief       Interfejs za GPIO modul STM32F10x Low Density Value Line serija,
 *              port klasa: varijanta.
 *
 * ------------------------------------------------------------------------------------------------
 *
 * @addtogroup  hal_gpio
 *
 ****************************************************************************************//** @{ */


#ifndef VARNT_GPIO_H_
#define VARNT_GPIO_H_


/*************************************************************************************************
 * INCLUDE FILES
 *************************************************************************************************/

/*-----------------------------------------------------------------------------------------------*
 * Module dependencies
 *-----------------------------------------------------------------------------------------------*/

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"


/*-----------------------------------------------------------------------------------------------*
 * Module configuration and adapter
 *-----------------------------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------------------------*
 * EXTERNS
 *-----------------------------------------------------------------------------------*//** @cond */

#ifdef VARNT_GPIO_H_VAR
#define VARNT_GPIO_H_EXT
#else
#define VARNT_GPIO_H_EXT extern
#endif


/** @endcond*//***********************************************************************************
 * DEFINES
 *************************************************************************************************/

/*-------------------------------------------------------------------------------------------*//**
 * @name        Konstante za STM32F10x LD_VL seriju
 * @{ *//*---------------------------------------------------------------------------------------*/

#if !defined(HAL_GPIO_IN) || defined(__DOXYGEN__)
# define HAL_GPIO_IN                    0x04
#endif

#if !defined(HAL_GPIO_OUT) || defined(__DOXYGEN__)
# define HAL_GPIO_OUT                   0x10
#endif

#if !defined(HAL_GPIO_AF) || defined(__DOXYGEN__)
# define HAL_GPIO_AF                    0x18
#endif

#if !defined(HAL_GPIO_AN) || defined(__DOXYGEN__)
# define HAL_GPIO_AN                    0x00
#endif

#if !defined(HAL_GPIO_PP) || defined(__DOXYGEN__)
# define HAL_GPIO_PP                    0x00
#endif

#if !defined(HAL_GPIO_OD) || defined(__DOXYGEN__)
# define HAL_GPIO_OD                    0x04
#endif

#if !defined(HAL_GPIO_PU) || defined (__DOXYGEN__)
# define HAL_GPIO_PU                    0x048
#endif

#if !defined(HAL_GPIO_PD) || defined (__DOXYGEN__)
# define HAL_GPIO_PD                    0x028
#endif

#if !defined(HAL_GPIO_WPU) || defined (__DOXYGEN__)
# define HAL_GPIO_WPU                   0x00                                    /* Ne koristi se, nije implementirano u hardveru mikrokontrolera */
#endif

#if !defined(HAL_GPIO_WPD) || defined(__DOXYGEN__)                              /* Ne koristi se, nije implementirano u hardveru mikrokontrolera */
# define HAL_GPIO_WPD                   0x00
#endif

/**
 * @todo        Proveriti tacnost ove konstante i slaganje sa biblioteckim
 *              funkcijama
 */
#if !defined(HAL_GPIO_NP) || defined(_DOXYGEN__)
# define HAL_GPIO_NP                    0xFF
#endif

/** @} *//*--------------------------------------------------------------------------------------*/


/*************************************************************************************************
 * MACRO's
 *************************************************************************************************/

/*-------------------------------------------------------------------------------------------*//**
 * @name        Implementacija za STM32F10x LD_VL seriju
 * @{ *//*---------------------------------------------------------------------------------------*/

#if !defined(HAL_GPIO_INIT) || defined(__DOXYGEN__)
# define HAL_GPIO_INIT(port, pins, direction, omode, imode, speed)              \
    do {                                                                        \
        RCC->APB2ENR |= HAL_GPIO_APB2PeriphLkUp[port];                          \
        GPIO_InitTypeDef GPIO_init;                                             \
        if(direction != HAL_GPIO_IN) {                                          \
            GPIO_init.GPIO_Mode = (direction | omode);                          \
        } else {                                                                \
            GPIO_init.GPIO_Mode = (direction & imode);                          \
        }                                                                       \
        GPIO_init.GPIO_Pin = pins;                                              \
        GPIO_init.GPIO_Speed = speed;                                           \
        GPIO_Init((GPIO_TypeDef *)HAL_GPIO_portLkUp[port], &GPIO_init);         \
    } while (0)

#endif

#if !defined(HAL_GPIO_DEINIT) || defined(__DOXYGEN__)
# define HAL_GPIO_DEINIT(port, pin)
#endif

#if !defined(HAL_GPIO_LOW) || defined(__DOXYGEN__)
# define HAL_GPIO_LOW(port, pin)                                                \
    port->BSRRH = pin

#endif

#if !defined(HAL_GPIO_HIGH) || defined(__DOXYGEN__)
# define HAL_GPIO_HIGH(port, pin)                                               \
    port->BSRRL = pin

#endif

#if !defined(HAL_GPIO_TOGGLE) || defined(__DOXYGEN__)
# define HAL_GPIO_TOGGLE(port, pin)                                             \
    port->ODR ^= pin

#endif

#if !defined(HAL_GPIO_RD_BIT) || defined(__DOXYGEN__)
# define HAL_GPIO_RD_BIT(port, bit)
#endif

#if !defined(HAL_GPIO_RD_U8) || defined(__DOXYGEN__)
# define HAL_GPIO_RD_U8(port)
#endif

#if !defined(HAL_GPIO_RD_U16) || defined(__DOXYGEN__)
# define HAL_GPIO_RD_U16(port)
#endif

#if !defined(HAL_GPIO_RD_U32) || defined(__DOXYGEN__)
# define HAL_GPIO_RD_U32(port)
#endif

#if !defined(HAL_GPIO_WR_BIT) || defined(__DOXYGEN__)
# define HAL_GPIO_WR_BIT(port, bit, value)
#endif

#if !defined(HAL_GPIO_WR_U8) || defined(__DOXYGEN__)
# define HAL_GPIO_WR_U8(port, value)
#endif

#if !defined(HAL_GPIO_WR_U16) || defined(__DOXYGEN__)
# define HAL_GPIO_WR_U16(port, value)
#endif

#if !defined(HAL_GPIO_WR_U32) || defined(__DOXYGEN__)
# define HAL_GPIO_WR_U32(port, value)
#endif

#if !defined(HAL_GPIO_REMAP_CMD) || defined(__DOXYGEN__)
# define HAL_GPIO_REMAP_CMD(gpioremap, command)                                 \
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);                        \
    GPIO_PinRemapConfig(gpioremap, command)

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

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Niz za konverziju rednog broja porta u njegov pokazivac koji se
 *              predaje funkcijama.
 */
 /*-----------------------------------------------------------------------------------------------*/
extern const uint32_t HAL_GPIO_portLkUp[];

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Niz za konverziju rednog broja porta u odgovarajuci RCC registar.
 */
/*-----------------------------------------------------------------------------------------------*/
extern const uint32_t HAL_GPIO_APB2PeriphLkUp[];


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
 * END of gpio.h
 *************************************************************************************************/
#endif /* VARNT_GPIO_H_ */
