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
 * @brief       Implementacija ---. (POGRESNO I VAN PRAVILA NAZVANA FUNKCIJA I FAJL)
 *
 * ------------------------------------------------------------------------------------------------
 *
 * @addtogroup  ett_impl
 *
 ****************************************************************************************//** @{ */


/*************************************************************************************************
 * INCLUDE FILES
 *************************************************************************************************/

#include "ett_private.h"


/*************************************************************************************************
 * LOCAL DEFINES
 *************************************************************************************************/

/*-----------------------------------------------------------------------------------------------*
 * Local debug defines
 *-----------------------------------------------------------------------------------*//** @cond */
DBG_DEFINE_MODULE(ett_task_mangment);


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

typedef uint32_t (* pCurrentettTask_T) (void);


/*************************************************************************************************
 * LOCAL TABLES
 *************************************************************************************************/


/*************************************************************************************************
 * SHARED GLOBAL VARIABLES
 *************************************************************************************************/


/*************************************************************************************************
 * LOCAL GLOBAL VARIABLES
 *************************************************************************************************/

static uint32_t bufferFilled;
static uint8_t rbfTemp;
static uint8_t ettRelativeBuffersLkUp[3];
static pCurrentettTask_T pCurentettTask;
static const pCurrentettTask_T  ettHandlerLkUp[3] = {
    &eS_PutStpByte,
    &eS_PutEppByte,
    &eS_SendPendingResponse
};


/*************************************************************************************************
 * GLOBAL FUNCTION PROTOTYPES
 *************************************************************************************************/

void eS_isr_ettTaskExecutor(void);
void eS_ettTaskChecker(void);


/*************************************************************************************************
 * LOCAL FUNCTION PROTOTYPES
 *************************************************************************************************/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Ova funkcija vrsi planiranje toka slanja izlaznih podataka na
 *              osnovu prioriteta, tj. popunjenosti datih bafera.
 *              @c responsePrendingStatus preko USART modula.
 *
 * @note        Ova funkcija se poziva od strane funkcije eS_isr_ettTaskExecutor.
 */
/*-----------------------------------------------------------------------------------------------*/
static void ettTaskManager(void);


/*************************************************************************************************
 ***                                I M P L E M E N T A T I O N                                ***
 *************************************************************************************************/


/*************************************************************************************************
 * GLOBAL FUNCTION DEFINITIONS
 *************************************************************************************************/

void eS_isr_ettTaskExecutor(void) {

    if (HAL_USART_GET_IT_STATUS(HAL_USART1, USART_IT_TC)) {                     /* Slanje podataka preko USART1 preriferij   */
        if (bufferFilled != (uint8_t)0x0) {
            ettTaskManager();
        }
        if((pCurrentettTask_T)0 != pCurentettTask) {
            bufferFilled = (uint32_t)(*pCurentettTask)();
            HAL_USART_CLEAR_IT_PENDING_BIT(HAL_USART1, USART_IT_TC);
        }
    }
    if (HAL_USART_GET_IT_STATUS(HAL_USART1, USART_IT_RXNE)) {                   /* Primanje podataka preko USART1 periferije */
        eS_GetByte();
        eS_isr_ProcessInputEvent();
        HAL_USART_CLEAR_IT_PENDING_BIT(HAL_USART1, USART_IT_RXNE);
    }
}

void eS_ettTaskChecker(void) {

    if(bufferFilled == (uint8_t)0x0) {
        HAL_USART_SET_IT_PENDING_BIT(HAL_USART1, USART_IT_TC);                  /* Setuj USART_IT_TC pending bit, nakon ovog setovanja dogadja se USART1_IRQn glavni interapt */
    }
}


/*************************************************************************************************
 * LOCAL FUNCTION DEFINITIONS
 *************************************************************************************************/

static void ettTaskManager(void) {

    rbfTemp = (uint8_t)0x0;
    uint8_t i;

    ettRelativeBuffersLkUp[0] = (uint8_t)((100 * GET_STP_BUFF_FVALUE())/STP_BUFFER_SIZE);
    ettRelativeBuffersLkUp[1] = (uint8_t)((100 * GET_EPP_BUFF_FVALUE())/EPP_BUFFER_SIZE);
    ettRelativeBuffersLkUp[2] = (pEppResponseFrame > (uint8_t *)0 ? 100 : 0);

    for(i = 0x0; i < 3; i++) {
        if(rbfTemp < ettRelativeBuffersLkUp[i]) {
            pCurentettTask = ettHandlerLkUp[i];
            rbfTemp = (uint8_t)ettRelativeBuffersLkUp[i];
        }
    }
    if(rbfTemp == (uint8_t)0x0) {
        bufferFilled = (uint8_t)0x0;
        pCurentettTask = (pCurrentettTask_T)0;
    }
}


/*************************************************************************************************
 * CONFIGURATION ERRORS
 *************************************************************************************//** @cond */


/** @endcond *//** @} *//*************************************************************************
 * END of -------
 *************************************************************************************************/
