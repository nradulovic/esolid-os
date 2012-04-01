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
 * @brief       Privatni objekti ETT modula.
 *
 * ------------------------------------------------------------------------------------------------
 *
 * @addtogroup  ett_impl
 *
 ****************************************************************************************//** @{ */


#ifndef ETT_PRIVATE_H_
#define ETT_PRIVATE_H_


/*************************************************************************************************
 * INCLUDE FILES
 *************************************************************************************************/

/*-----------------------------------------------------------------------------------------------*
 * Module dependencies
 *-----------------------------------------------------------------------------------------------*/
#include "ett\ett.h"
#include "core\eot.h"

/*
 * TODO: Pojasni zasto ETT modul mora da ukljuci "events.h".
 */
#include "events.h"


/*-----------------------------------------------------------------------------------------------*
 * Module configuration and adapter
 *-----------------------------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------------------------*
 * EXTERNS
 *-----------------------------------------------------------------------------------*//** @cond */

#ifdef ETT_PRIVATE_H_VAR
#define ETT_PRIVATE_H_EXT
#else
#define ETT_PRIVATE_H_EXT extern
#endif


/** @endcond*//***********************************************************************************
 * DEFINES
 *************************************************************************************************/

#define CMD_STP                         0x81
#define CMD_EPP                         0x80
#define STP_BUFFER_SIZE                 1024
#define EPP_BUFFER_SIZE                 512
#define EPP_BUFFER_FILL_LIMIT           (EPP_BUFFER_SIZE * 95)/100              /* 95% velicine Bafera */
#define STP_BUFFER_MASK                 (STP_BUFFER_SIZE - 1)
#define EPP_BUFFER_MASK                 (EPP_BUFFER_SIZE - 1)


/*************************************************************************************************
 * MACRO's
 *************************************************************************************************/

#define PUSH_STP_BUFFER(data)                                                   \
            {STPcircularTxBuffer[stpWrIndex & STP_BUFFER_MASK] = data;          \
            stpWrIndex++;}

#define POP_STP_BUFFER(data)                                                    \
            {data = STPcircularTxBuffer[stpRdIndex];                            \
            stpRdIndex++;}

#define PUSH_EPP_BUFFER(data)                                                   \
            if(eppWrIndex != (eppRdIndex - (uint32_t)0x1))                      \
              {EPPcircularTxBuffer[(eppWrIndex & EPP_BUFFER_MASK)] = data;      \
              eppWrIndex++;}

#define POP_EPP_BUFFER(data)                                                    \
            if(eppRdIndex <= eppWrIndex)                                        \
            {data =  EPPcircularTxBuffer[eppRdIndex];                           \
            eppRdIndex++;}

#define GET_STP_BUFF_FVALUE()                                                   \
                (stpWrIndex - stpRdIndex)

#define GET_EPP_BUFF_FVALUE()                                                   \
                (eppWrIndex - eppRdIndex)

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

extern volatile uint8_t STPcircularTxBuffer[];
extern volatile uint32_t stpRdIndex;
extern volatile uint32_t stpWrIndex;

extern volatile uint8_t EPPcircularTxBuffer[];
extern volatile uint32_t eppRdIndex;
extern volatile uint32_t eppWrIndex;

extern volatile uint8_t primaryRxBuffer[];
extern volatile uint16_t primaryRxBufferFilled;

extern const uint8_t EPP_ACK_FRAME[];
extern const uint8_t EPP_NACK_FRAME[];
extern const uint8_t volatile * pEppResponseFrame;

extern EPPdata_T Eppdata;

/*************************************************************************************************
 * FUNCTION PROTOTYPES
 *************************************************************************************************/

void eS_GetByte(void);
uint32_t eS_PutStpByte(void);
uint32_t eS_PutEppByte(void);
uint32_t eS_SendPendingResponse(void);
void eS_ettTaskChecker(void);
void parseWordToBytes(uint32_t aWord, volatile uint8_t * pStoreBuffer);
void stpWordBytestufing7E7D(uint32_t word);

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
 * END of ett_private.h
 *************************************************************************************************/
#endif /* ETT_PRIVATE_H_ */
