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
 * @brief       Implementcija eS_GetByte().
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

#define MAX_TIMEOUT                     300U                                     /* time out ~ 300ms */
#define TIMEOUT_COUNTER_MAX_VALUE       4294967295lU                             /* 2^24 jer je sysTick 24bitni brojac */


/*-----------------------------------------------------------------------------------------------*
 * Local debug defines
 *-----------------------------------------------------------------------------------*//** @cond */


/** @endcond *//*--------------------------------------------------------------------------------*
 * Local user defines
 *-----------------------------------------------------------------------------------------------*/


/*************************************************************************************************
 * LOCAL MACRO's
 *************************************************************************************************/

#define WRITE_PRIMARY_RX_BUFFER()                                               \
              if(rxStatus != (RxStatus_T)ETT_STOP) {                            \
                  primaryRxBuffer[index] = (uint8_t)byte;                       \
                  index++;                                                      \
              }

#define COMPUTE_TIME_OUT(timeout)                                               \
    if(timeOut <= timeOutCounter) {                                             \
        timeout = (timeOutCounter - timeOut);                                   \
    } else {                                                                    \
        timeout = ((TIMEOUT_COUNTER_MAX_VALUE - timeOut) + timeOutCounter);     \
    }


/*************************************************************************************************
 * LOCAL CONSTANTS
 *************************************************************************************************/


/*************************************************************************************************
 * LOCAL DATA TYPES
 *************************************************************************************************/

typedef enum RxStatus_tag {
     ETT_STOP,
     ETT_START
}RxStatus_T;


/*************************************************************************************************
 * LOCAL TABLES
 *************************************************************************************************/


/*************************************************************************************************
 * SHARED GLOBAL VARIABLES
 *************************************************************************************************/

volatile uint8_t primaryRxBuffer[50];
volatile uint16_t primaryRxBufferFilled;


/*************************************************************************************************
 * LOCAL GLOBAL VARIABLES
 *************************************************************************************************/

static uint8_t index;
static uint8_t byte;
static RxStatus_T rxStatus;
static uint32_t timeOut;
static uint32_t timeOutCounter;


/*************************************************************************************************
 * GLOBAL FUNCTION PROTOTYPES
 *************************************************************************************************/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @ingroup     ett_intf
 */
/*-----------------------------------------------------------------------------------------------*/
void eS_GetByte(
    void);


/*************************************************************************************************
 * LOCAL FUNCTION PROTOTYPES
 *************************************************************************************************/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Vrsi incrementiranje TimeOutCounter-a koji se koristi u funkciji
 *              isr_getByte().
 *
 * @note        Ova funkcija se poziva od strane SysTick interapt rutine.
 */
/*-----------------------------------------------------------------------------------------------*/
void eS_isr_UpdateTimeOutCounter(
    void);


/*************************************************************************************************
 ***                                I M P L E M E N T A T I O N                                ***
 *************************************************************************************************/


/*************************************************************************************************
 * GLOBAL FUNCTION DEFINITIONS
 *************************************************************************************************/

void eS_GetByte(void) {

    byte = HAL_USART_RECEIVE(HAL_USART1);
    DBG_PRINT_HEX((uint32_t)byte);

    if ((uint8_t)0x7E == byte) {                                                /* Detekcija pocetka ili kraja EEP frejma */
        if ((RxStatus_T)ETT_STOP == rxStatus) {
            index = (uint8_t)0x0;
            rxStatus = (RxStatus_T)ETT_START;
            timeOut = timeOutCounter;                                           /* Abdejtuj timeOut promenljivu */
        } else {
            rxStatus = (RxStatus_T)ETT_STOP;
            primaryRxBufferFilled = (uint16_t)index;
            timeOut = (uint32_t)0x0;                                            /* Resetuj timeOut promenljivu */
        }
    } else {
        COMPUTE_TIME_OUT(timeOut);
        if (timeOut > (uint32_t)MAX_TIMEOUT) {                                  /* Proveravanje TimeOut-a */
            rxStatus = (RxStatus_T)ETT_STOP;
            primaryRxBufferFilled = (uint16_t)0x0;
            timeOut = (uint32_t)0x0;                                            /* Resetuj timeOut promenljivu */
            pEppResponseFrame = EPP_NACK_FRAME;
            eS_ettTaskChecker();
        } else {
            WRITE_PRIMARY_RX_BUFFER();                                          /* Upisi primljeni bajt u bafer i incrementiraj index */
            timeOut = timeOutCounter;
        }
    }
}

void eS_isr_UpdateTimeOutCounter(void) {

    timeOutCounter++;
}


/*************************************************************************************************
 * LOCAL FUNCTION DEFINITIONS
 *************************************************************************************************/


/*************************************************************************************************
 * CONFIGURATION ERRORS
 *************************************************************************************//** @cond */


/** @endcond *//** @} *//*************************************************************************
 * END of IMPLEMENTATION FILE
 *************************************************************************************************/
