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
 * @brief       Implementacija eS_isr_ProcessInputEvent().
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


/** @endcond *//*--------------------------------------------------------------------------------*
 * Local user defines
 *-----------------------------------------------------------------------------------------------*/
#define EPP_TYPE                ((secondaryRxBuffer[5] << 8) | secondaryRxBuffer[6])
#define EPP_ID                  ((secondaryRxBuffer[7] << 8) | secondaryRxBuffer[8])


/*************************************************************************************************
 * LOCAL MACRO's
 *************************************************************************************************/

#define GET_FCS_FROM_SECONDARY_BUFFER(storeVariable)                            \
    storeVariable =                                                             \
    (uint32_t)(((secondaryRxBuffer[secondaryIndex - (uint32_t)4]) << 24) |      \
    ((uint32_t)(secondaryRxBuffer[secondaryIndex - (uint32_t)3]) << 16)  |      \
    ((uint32_t)(secondaryRxBuffer[secondaryIndex - (uint32_t)2]) << 8)   |      \
    (uint32_t)secondaryRxBuffer[secondaryIndex - (uint32_t)1]);

#define DECODE_BYTESTUFFING_7E7D()                                              \
          if((primaryRxBuffer[primaryIndex] != (uint8_t)0x7D)) {                \
              secondaryRxBuffer[secondaryIndex] = primaryRxBuffer[primaryIndex];\
          } else {                                                              \
              if(primaryRxBuffer[primaryIndex] == (uint8_t)0x5E) {              \
                  secondaryRxBuffer[secondaryIndex] = (uint8_t)0x7E;            \
                  primaryIndex++;                                               \
              } else {                                                          \
                  secondaryRxBuffer[secondaryIndex] = (uint8_t)0x7D;            \
                  primaryIndex++;                                               \
              }                                                                 \
          }

#define GET_EPP_PACKET_SIZE()                                                   \
              (uint32_t)(((secondaryRxBuffer[1]) << 24) |                       \
              ((uint32_t)(secondaryRxBuffer[2]) << 16)  |                       \
              ((uint32_t)(secondaryRxBuffer[3]) << 8)   |                       \
              (uint32_t)secondaryRxBuffer[4])

#define SET_PENDING_RESPONSE()                                                  \
              if(secondaryCRC32Store != primaryCRC32Store) {                    \
                  pEppResponseFrame =  EPP_NACK_FRAME;                          \
              } else {                                                          \
                  pEppResponseFrame =  EPP_ACK_FRAME;                           \
              }                                                                 \
//              eS_ettTaskChecker();

/*************************************************************************************************
 * LOCAL CONSTANTS
 *************************************************************************************************/


/*************************************************************************************************
 * LOCAL DATA TYPES
 *************************************************************************************************/


/*************************************************************************************************
 * LOCAL TABLES
 *************************************************************************************************/


/*************************************************************************************************
 * SHARED GLOBAL VARIABLES
 *************************************************************************************************/

volatile uint8_t secondaryRxBuffer[50];
const uint8_t volatile * pEppResponseFrame;


/*************************************************************************************************
 * LOCAL GLOBAL VARIABLES
 *************************************************************************************************/

static uint8_t * pEvt;
static uint8_t * pEvtTmp;


/*************************************************************************************************
 * GLOBAL FUNCTION PROTOTYPES
 *************************************************************************************************/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Obrada ulaznih podataka po EET protokolu.
 */
/*-----------------------------------------------------------------------------------------------*/
void eS_isr_ProcessInputEvent(void);


/*************************************************************************************************
 * LOCAL FUNCTION PROTOTYPES
 *************************************************************************************************/


/*************************************************************************************************
 ***                                I M P L E M E N T A T I O N                                ***
 *************************************************************************************************/


/*************************************************************************************************
 * GLOBAL FUNCTION DEFINITIONS
 *************************************************************************************************/

void eS_isr_ProcessInputEvent(void) {

    uint32_t primaryIndex = (uint32_t)0x0;
    uint32_t secondaryIndex = (uint32_t)0x0;
    uint32_t secondaryCRC32Store;
    uint32_t primaryCRC32Store;
    uint32_t i = 0x0;

  if (primaryRxBufferFilled != 0x0) {                                           /* Ovaj uslov je privremeno uveden */

    while (primaryIndex < (uint32_t)primaryRxBufferFilled) {                    /* Kopiranje sadrzaja primaryRxBuffer-a u secondaryRxBuffer */
        DECODE_BYTESTUFFING_7E7D();                                             /* Runtime byte stuffing za 0x7E i kopiranje elemenata u Bafer */
        primaryIndex++;
        secondaryIndex++;
    }
    GET_FCS_FROM_SECONDARY_BUFFER(primaryCRC32Store);                           /* Uzmi primljenu vrednost CRC-a  */
    secondaryCRC32Store = HAL_CRC_CALC_BLOCK_SW32(secondaryRxBuffer,            /* Izracunaj CRC32 za primljene podate (CMD i DATA) */
                                (secondaryIndex - (uint32_t)0x4));
    SET_PENDING_RESPONSE();                                                     /* Setuj ACK ili NACK da bi se poslali kada se oslobodi uart */

    pEvt = (uint8_t *)eS_evtExtCreate(                                          /* F-ji za kreiranje dogadjaja se predaju dva argumenta: pointer na clasu i ID */
                (eot_evtClass_T *)evtTypeLkUp[EPP_TYPE],
                (uint16_t)EPP_ID);

    pEvtTmp = (uint8_t *)(pEvt + sizeof(eot_evtExt_T));
    for (i = 0x1; i < (uint32_t)((GET_EPP_PACKET_SIZE()) - (uint32_t)19); i++) {

        pEvtTmp[i] = secondaryRxBuffer[i + (uint32_t)20];
        DBG_PRINT_HEX(pEvtTmp[i]);
    }
    primaryRxBufferFilled = (uint16_t)0x0;
  }
}


/*************************************************************************************************
 * LOCAL FUNCTION DEFINITIONS
 *************************************************************************************************/


/*************************************************************************************************
 * CONFIGURATION ERRORS
 *************************************************************************************//** @cond */


/** @endcond *//** @} *//*************************************************************************
 * END of eS_isr_ProcessInputEvent.c
 *************************************************************************************************/
