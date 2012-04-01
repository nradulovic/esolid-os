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
 * @brief       Implementacija ett_ProcessingEPPdata().
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
DBG_DEFINE_MODULE(processingEPPdata);


/** @endcond *//*--------------------------------------------------------------------------------*
 * Local user defines
 *-----------------------------------------------------------------------------------------------*/


/*************************************************************************************************
 * LOCAL MACRO's
 *************************************************************************************************/

/**
 *  @brief Kopiranje podataka iz strukture Dogadjaja u bafer
 */
#define COPY_EVENT_TO_BUFFER()                                                  \
       do{  uint32_t temp;                                                      \
            uint32_t extSize;                                                   \
            uint32_t extIndex = (uint32_t)0x0;                                  \
            temp = (uint32_t)(pEPPdata->dataSize);                              \
            eppWordByteStuffingCRC32BS7E7D(temp);                               \
            temp = (uint32_t)((pEPPdata->type << 16) | (pEPPdata->id));         \
            eppWordByteStuffingCRC32BS7E7D(temp);                               \
            temp = (uint32_t)(pEPPdata->timeStamp);                             \
            eppWordByteStuffingCRC32BS7E7D(temp);                               \
            temp = (uint32_t)(pEPPdata->transmitter);                           \
            eppWordByteStuffingCRC32BS7E7D(temp);                               \
            eppWordByteStuffingCRC32BS7E7D((0x0));                              \
            extSize = (pEPPdata->dataSize - (uint32_t)20);                      \
            while(((uint32_t)extSize) > (uint32_t)0x0) {                        \
                PUSH_EPP_BYTE_CRC32_BS7E7D(pEPPdata->ext[extIndex]);            \
                extIndex++;                                                     \
                extSize--;                                                      \
            }                                                                   \
         } while(0)

#define PUSH_EPP_BYTE_CRC32_BS7E7D(data)                                        \
                {eppByteStuffing7E7D(data);                                     \
                 HAL_CRC_SHARED_CALC_SW32(data, eppCRC32metaResult);}


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

volatile uint8_t EPPcircularTxBuffer[EPP_BUFFER_SIZE];
volatile uint32_t eppRdIndex;
volatile uint32_t eppWrIndex;



/*************************************************************************************************
 * LOCAL GLOBAL VARIABLES
 *************************************************************************************************/

static volatile uint8_t EppParsedWord[4];
static volatile uint32_t eppCRC32metaResult;


/*************************************************************************************************
 * GLOBAL FUNCTION PROTOTYPES
 *************************************************************************************************/

BufferStatus_T eS_ProcessingEppData(EPPdata_T * pEPPdata);


/*************************************************************************************************
 * LOCAL FUNCTION PROTOTYPES
 *************************************************************************************************/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Razbijanje reci na 4 bajta sa bytestufing-om i upisvanje u
 *              EPP bafer.
 *
 * @param       aWord               Rec koja se procesira.
 */
/*-----------------------------------------------------------------------------------------------*/
static void eppWordByteStuffing7E7D(uint32_t aWord);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Bytestufing i upisvanje u bayta u EPP bafer.
 *
 * @param       aByte               Bajt koja se procesira.
 */
/*-----------------------------------------------------------------------------------------------*/
static void eppByteStuffing7E7D(uint8_t aByte);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Razbijanje reci na 4 bajta sa bytestufing-om i CRC32 proverom
 *              kao i upisvanje u EPP bafer.
 *
 * @param       aWord               Rec koja se procesira.
 */
/*-----------------------------------------------------------------------------------------------*/
static void eppWordByteStuffingCRC32BS7E7D(uint32_t aWord);


/*************************************************************************************************
 ***                                I M P L E M E N T A T I O N                                ***
 *************************************************************************************************/


/*************************************************************************************************
 * GLOBAL FUNCTION DEFINITIONS
 *************************************************************************************************/

BufferStatus_T eS_ProcessingEppData(EPPdata_T * pEPPdata) {

     uint16_t eppBufferFilled = (uint16_t)GET_EPP_BUFF_FVALUE();

     if((pEPPdata->dataSize + eppBufferFilled) < EPP_BUFFER_FILL_LIMIT) {       /* Provera dali niz moze da se smesti u Bafer uracunat i byte stuffing */
         PUSH_EPP_BUFFER(0x7E);                                                 /* Upisivanje preambule paketa EEP protokola */
         PUSH_EPP_BYTE_CRC32_BS7E7D(CMD_EPP);                                   /* Upisivanje CMD bajta u bafer */
         eS_ettTaskChecker();
         HAL_CRC_SHARED_RESET_CALC_SW32(eppCRC32metaResult);
         COPY_EVENT_TO_BUFFER();
         eppWordByteStuffing7E7D(HAL_CRC_SHARED_GET_SW32(eppCRC32metaResult));  /* Upisivanje CRC32 vrednosti u bafer */
         PUSH_EPP_BUFFER(0x7E);                                                 /* Upisivanje postambule paketa EEP protokola */

         return ((BufferStatus_T)ETT_NO_FILLED);

     } else {

         return ((BufferStatus_T)ETT_FILLED);
     }
}


/*************************************************************************************************
 * LOCAL FUNCTION DEFINITIONS
 *************************************************************************************************/

static void eppWordByteStuffing7E7D(uint32_t aWord) {

    parseWordToBytes(aWord, &EppParsedWord[0]);

    eppByteStuffing7E7D(EppParsedWord[0]);
    eppByteStuffing7E7D(EppParsedWord[1]);
    eppByteStuffing7E7D(EppParsedWord[2]);
    eppByteStuffing7E7D(EppParsedWord[3]);
}

static void eppByteStuffing7E7D(uint8_t aByte) {

    if((aByte != (uint8_t)0x7E) && (aByte != (uint8_t)0x7D)) {
        PUSH_EPP_BUFFER(aByte);
     } else {
        PUSH_EPP_BUFFER(0x7D);

        if(0x7E == aByte) {
             PUSH_EPP_BUFFER(0x5E);
        } else {
             PUSH_EPP_BUFFER(0x5D);
        }
    }
}

static void eppWordByteStuffingCRC32BS7E7D(uint32_t aWord) {

    eppWordByteStuffing7E7D(aWord);
    HAL_CRC_SHARED_CALC_SW32(EppParsedWord[0], eppCRC32metaResult);
    HAL_CRC_SHARED_CALC_SW32(EppParsedWord[1], eppCRC32metaResult);
    HAL_CRC_SHARED_CALC_SW32(EppParsedWord[2], eppCRC32metaResult);
    HAL_CRC_SHARED_CALC_SW32(EppParsedWord[3], eppCRC32metaResult);
}


/*************************************************************************************************
 * CONFIGURATION ERRORS
 *************************************************************************************//** @cond */


/** @endcond *//** @} *//*************************************************************************
 * END of ett_ProcessingEPPdata.c
 *************************************************************************************************/
