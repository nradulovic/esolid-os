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
 * @brief       Implementacija ett_ProcessingSTPdata.
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
DBG_DEFINE_MODULE(processingSTPdata);



/** @endcond *//*--------------------------------------------------------------------------------*
 * Local user defines
 *-----------------------------------------------------------------------------------------------*/


/*************************************************************************************************
 * LOCAL MACRO's
 *************************************************************************************************/

#define PUSH_STP_WORD_CRC32_BS3E3F7E7D(word)                                    \
                {stpWordBytestufing3E3F7E7D(word);                              \
                 HAL_CRC_SHARED_CALC_SW32(StpParsedWord[0], stpCRC32metaResult);\
                 HAL_CRC_SHARED_CALC_SW32(StpParsedWord[1], stpCRC32metaResult);\
                 HAL_CRC_SHARED_CALC_SW32(StpParsedWord[2], stpCRC32metaResult);\
                 HAL_CRC_SHARED_CALC_SW32(StpParsedWord[3], stpCRC32metaResult);}

#define PUSH_STP_BYTE_CRC32_BS3E3F7E7D(data)                                    \
                {stpByteStuffing3E3F7E7D(data);                                 \
                 HAL_CRC_SHARED_CALC_SW32(data, stpCRC32metaResult);}           \

#define GET_STP_BUFFER_FREE_SPACE()                                             \
                STP_BUFFER_SIZE - GET_STP_BUFF_FVALUE()


/*************************************************************************************************
 * LOCAL CONSTANTS
 *************************************************************************************************/


/*************************************************************************************************
 * LOCAL DATA TYPES
 *************************************************************************************************/

typedef enum ProcessingStatus_tag {
     STOP_PROCESSING,
     START_PROCESSING
}ProcessingStatus_T;


/*************************************************************************************************
 * LOCAL TABLES
 *************************************************************************************************/


/*************************************************************************************************
 * SHARED GLOBAL VARIABLES
 *************************************************************************************************/

volatile uint8_t STPcircularTxBuffer[STP_BUFFER_SIZE];
volatile uint32_t stpRdIndex;
volatile uint32_t stpWrIndex;
volatile uint32_t stpCRC32StoreResult;


/*************************************************************************************************
 * LOCAL GLOBAL VARIABLES
 *************************************************************************************************/

static volatile ProcessingStatus_T processingStatus;
static volatile uint8_t StpParsedWord[4];
static volatile uint32_t stpCRC32metaResult;
static volatile uint32_t stpWriteIndexOrigin;
static volatile uint8_t rid;


/*************************************************************************************************
 * GLOBAL FUNCTION PROTOTYPES
 *************************************************************************************************/

void eS_StartProcesingStpData(void);
void eS_ProcessingStpData(uint8_t * pSTPdata);
void eS_StopProcessingStpData(void);
void stpWordBytestufing7E7D(uint32_t word);


/*************************************************************************************************
 * LOCAL FUNCTION PROTOTYPES
 *************************************************************************************************/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Bytestufing i upisvanje u bayta u STP bafer.
 *
 * @param       aByte               Bajt koja se procesira.
 */
/*-----------------------------------------------------------------------------------------------*/
static void stpByteStuffing7E7D(uint8_t data);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Bytestufing za 7E7D i 3E3F i upisvanje u bayta u STP bafer.
 *
 * @param       aByte               Bajt koja se procesira.
 */
/*-----------------------------------------------------------------------------------------------*/
static void stpByteStuffing3E3F7E7D(uint8_t data);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Razbijanje reci na 4 bajta sa bytestufing-om za 7E7D i
 *              3E3F i upisvanje u STP bafer.
 *
 * @param       aWord               Rec koja se procesira.
 */
/*-----------------------------------------------------------------------------------------------*/
static void stpWordBytestufing3E3F7E7D(uint32_t word);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Izracunava duzinu stringa, uracunava bytestuffing ako je neki od
 *              karaktera 7E, 7D, 3E ili 3F i upisvanje u bayta u STP bafer.
 *
 * @param       aByte               Bayt koja se procesira.
 */
/*-----------------------------------------------------------------------------------------------*/
static uint16_t stringLen(uint8_t * pString);


/*************************************************************************************************
 ***                                I M P L E M E N T A T I O N                                ***
 *************************************************************************************************/


/*************************************************************************************************
 * GLOBAL FUNCTION DEFINITIONS
 *************************************************************************************************/

void eS_StartProcesingStpData(void) {

    if(GET_STP_BUFFER_FREE_SPACE() > (uint32_t)8) {
        processingStatus = (ProcessingStatus_T)START_PROCESSING;                /* Oznaci pocetak obrade STP podataka */
        stpWriteIndexOrigin = stpWrIndex;
        PUSH_STP_BUFFER(0x7E);                                                  /* Kreiranje preambule paketa EEP protokola ako je detektovan start */
        //eS_ettTaskChecker();
        HAL_CRC_SHARED_RESET_CALC_SW32(stpCRC32metaResult);
        PUSH_STP_BYTE_CRC32_BS3E3F7E7D(CMD_STP);                                /* Upisivanje CMD bajta u bafer */
        PUSH_STP_BUFFER(0x3F);
        HAL_CRC_SHARED_CALC_SW32(0x3F, stpCRC32metaResult);
        PUSH_STP_BUFFER(rid++);
        HAL_CRC_SHARED_CALC_SW32(rid, stpCRC32metaResult);
        PUSH_STP_WORD_CRC32_BS3E3F7E7D((HAL_SYSTICK_GET_VALUE()));
    }
}

void eS_ProcessingStpData(uint8_t * pSTPdata) {

    if((GET_STP_BUFFER_FREE_SPACE() > stringLen(pSTPdata)) &&
                    processingStatus == STOP_PROCESSING) {                      /* Provera dali niz moze da se smesti u Bafer uracunat i byte stuffing */

        while((*pSTPdata) != (uint8_t)0x0) {                                    /* Kopiranje clanova niza koji sadrzi STP podatke */
            PUSH_STP_BYTE_CRC32_BS3E3F7E7D((*pSTPdata));
            pSTPdata++;
        }
    } else {                                                                    /* Podatak ne moze da se smesti u slobodan prostor bafera zato se vraca index na  pocetak upisivanja */
        stpWrIndex = stpWriteIndexOrigin;
        processingStatus = (ProcessingStatus_T)STOP_PROCESSING;
    }
}

void eS_StopProcessingStpData(void) {

    if(GET_STP_BUFFER_FREE_SPACE() > (uint32_t)5) {
        stpWordBytestufing7E7D(HAL_CRC_SHARED_GET_SW32(stpCRC32metaResult));
        PUSH_STP_BUFFER((uint8_t)0x7E);
        processingStatus = (ProcessingStatus_T)STOP_PROCESSING;
    }
}

void stpWordBytestufing7E7D(uint32_t word) {

    parseWordToBytes(word, &StpParsedWord[0]);
    stpByteStuffing7E7D(StpParsedWord[0]);
    stpByteStuffing7E7D(StpParsedWord[1]);
    stpByteStuffing7E7D(StpParsedWord[2]);
    stpByteStuffing7E7D(StpParsedWord[3]);
}


/*************************************************************************************************
 * LOCAL FUNCTION DEFINITIONS
 *************************************************************************************************/

static void stpByteStuffing7E7D(uint8_t data) {

    if((data != 0x7E) && (data != 0x7D)) {
        PUSH_STP_BUFFER(data);
     } else {
        PUSH_STP_BUFFER(0x7D);

        if(0x7E == data) {
             PUSH_STP_BUFFER(0x5E);
        } else {
             PUSH_STP_BUFFER(0x5D);
        }
    }
}

static void stpByteStuffing3E3F7E7D(uint8_t data) {

    if((data != 0x3E) && (data != 0x3F)) {
        stpByteStuffing7E7D(data);
     } else {
        PUSH_STP_BUFFER(0x3E);

        if(0x3E == data) {
             PUSH_STP_BUFFER(0x5D);
        } else {
             PUSH_STP_BUFFER(0x5E);
        }
    }
}

static void stpWordBytestufing3E3F7E7D(uint32_t word) {

    parseWordToBytes(word, &StpParsedWord[0]);
    stpByteStuffing3E3F7E7D(StpParsedWord[0]);
    stpByteStuffing3E3F7E7D(StpParsedWord[1]);
    stpByteStuffing3E3F7E7D(StpParsedWord[2]);
    stpByteStuffing3E3F7E7D(StpParsedWord[3]);
}

static uint16_t stringLen(uint8_t * pString) {

    uint16_t stringlen = 0x0;
    uint8_t* temp = pString;
    uint8_t c = *temp;
    while(c != '\0'){
        if((c == 0x7E) || (c == 0x7D) || (c == 0x3E) || (c == 0x3F)) {
            stringlen++;
        }
        stringlen++;
        temp++;
        c = *temp;
    }
    return stringlen;
}


/*************************************************************************************************
 * CONFIGURATION ERRORS
 *************************************************************************************//** @cond */


/** @endcond *//** @} *//*************************************************************************
 * END of ett_ProcessingSTPdata.c
 *************************************************************************************************/
