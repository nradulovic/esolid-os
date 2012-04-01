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
 * @author      Dejan
 *
 * @brief       Implementacija HAL USART modula.
 *
 * ------------------------------------------------------------------------------------------------
 *
 * @addtogroup  hal_usart_impl
 *
 ****************************************************************************************//** @{ */


/*************************************************************************************************
 * INCLUDE FILES
 *************************************************************************************************/

#include "port/variant/stm32f10x_ld_vl/hal/usart.h"


/*************************************************************************************************
 * LOCAL DEFINES
 *************************************************************************************************/

/*-----------------------------------------------------------------------------------------------*
 * Local debug defines
 *-----------------------------------------------------------------------------------*//** @cond */


/** @endcond *//*--------------------------------------------------------------------------------*
 * Local user defines
 *-----------------------------------------------------------------------------------------------*/


/*************************************************************************************************
 * LOCAL MACRO's
 *************************************************************************************************/


/*************************************************************************************************
 * LOCAL CONSTANTS
 *************************************************************************************************/


/*************************************************************************************************
 * LOCAL DATA TYPES
 *************************************************************************************************/

const uint32_t usartLkUp[] = {
    USART1_BASE,
    USART2_BASE,
    USART3_BASE
};

const uint32_t RCC_USART_APBPeriphLkUp[] = {
    RCC_APB2Periph_USART1,
    RCC_APB1Periph_USART2,
    RCC_APB1Periph_USART3
};

const uint32_t HAL_USART_IO_txPinsLkUp[] = {
    GPIO_Pin_9,
    GPIO_Pin_2,
    GPIO_Pin_10
};

const uint32_t HAL_USART_IO_rxPinsLkUp[] = {
    GPIO_Pin_10,
    GPIO_Pin_3,
    GPIO_Pin_11
};

const uint8_t HAL_USART_IO_portLkUp[] = {
    0,
    0,
    1
};

const uint32_t HAL_USART_NVIC_IRQ_ChanelLkUp[] = {
    USART1_IRQn,
    USART2_IRQn
};


/*************************************************************************************************
 * LOCAL TABLES
 *************************************************************************************************/


/*************************************************************************************************
 * SHARED GLOBAL VARIABLES
 *************************************************************************************************/

volatile uint16_t softwareItPendingMaskLkUp[5];


/*************************************************************************************************
 * LOCAL GLOBAL VARIABLES
 *************************************************************************************************/


/*************************************************************************************************
 * GLOBAL FUNCTION PROTOTYPES
 *************************************************************************************************/


/*************************************************************************************************
 * LOCAL FUNCTION PROTOTYPES
 *************************************************************************************************/


/*************************************************************************************************
 ***                                I M P L E M E N T A T I O N                                ***
 *************************************************************************************************/


/*************************************************************************************************
 * GLOBAL FUNCTION DEFINITIONS
 *************************************************************************************************/

ITStatus HAL_USART_GetItPendingBitStatus(
    uint16_t usart,
    uint16_t usartItFlag) {

    ITStatus bitstatus = RESET;

    if (((USART_GetITStatus(((USART_TypeDef *)usartLkUp[usart]), usartItFlag))  \
       != RESET) || (softwareItPendingMaskLkUp[usart] == usartItFlag)) {

        softwareItPendingMaskLkUp[usart] = (uint16_t)0x0;
        bitstatus = SET;
    } else {
        bitstatus = RESET;
    }

    return (bitstatus);
}


/*************************************************************************************************
 * LOCAL FUNCTION DEFINITIONS
 *************************************************************************************************/


/*************************************************************************************************
 * CONFIGURATION ERRORS
 *************************************************************************************//** @cond */


/** @endcond *//** @} *//*************************************************************************
 * END of usart.c
 *************************************************************************************************/
