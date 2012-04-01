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
 * @brief       Interfejs za CRC modul STM32F10x Low Density Value Line serija,
 *              port klasa: varijanta.
 *
 * ------------------------------------------------------------------------------------------------
 *
 * @addtogroup  hal_crc
 *
 ****************************************************************************************//** @{ */


#ifndef VARNT_CRC_H_
#define VARNT_CRC_H_


/*************************************************************************************************
 * INCLUDE FILES
 *************************************************************************************************/

/*-----------------------------------------------------------------------------------------------*
 * Module dependencies
 *-----------------------------------------------------------------------------------------------*/
#include "hal/hal_compiler.h"


/*-----------------------------------------------------------------------------------------------*
 * Module configuration and adapter
 *-----------------------------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------------------------*
 * EXTERNS
 *-----------------------------------------------------------------------------------*//** @cond */

#ifdef VARNT_CRC_H_VAR
#define VARNT_CRC_H_EXT
#else
#define VARNT_CRC_H_EXT extern
#endif


/** @endcond*//***********************************************************************************
 * DEFINES
 *************************************************************************************************/

/*************************************************************************************************
 * MACRO's
 *************************************************************************************************/

/*-------------------------------------------------------------------------------------------*//**
 * @name        Hardverski realizovani CRC.
 *
 * @brief       Makroi za izracunavanje CRC-a posredstvom hardverskog modula.
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Izracunava 32bitnu vrednost CRCa nad 32bitnim podatkom.
 *
 * @param       data                    32bitni podatak
 * @return      Izracunata 32bitna vrednost CRCa.
 *
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_CRC_CALC_HW32) || defined(__DOXYGEN__)
# define HAL_CRC_CALC_HW32(data)                                                \
    (uint32_t)CRC_CalcCRC((uint32_t)data)
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Izracunava 32bitnu vrednost nad 32bitnim blokom podataka.
 *
 * @param       pbuffer                 pointer na blok sa podacima.
 * @param       bufferlenght            velicina bloka izrazena u recima (word).
 * @return      Izracunata 32bitna vrednost CRCa.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_CRC_CALC_BLOCK_HW32) || defined(__DOXYGEN__)
# define HAL_CRC_CALC_BLOCK_HW32(pbuffer, bufferlength)                         \
    (uint32_t)CRC_CalcBlockCRC((uint32_t *)pbuffer, (uint32_t)bufferlength)
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Resetuje HAL_CRC_CALC_HW32 makro, tj. postavnja njegov
 *              data registar na inicijalnu vrednost.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_CRC_RESET_CALC_HW32) || defined(__DOXYGEN__)
# define HAL_CRC_RESET_CALC_HW32()                                              \
    CRC->CR = crcreset
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Vraca izracunatu vrednost CRC-a.
 * @return      Izracunatu vrednost CRC-a.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_CRC_GET_HW32) || defined(__DOXYGEN__)
#define HAL_CRC_GET_HW32()                                                      \
    (uint32_t)CRC->DR
#endif

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Softwerski realizovani CRC.
 *
 * @brief       Makroi za izracunavanje CRC-a posredstvom softverskog modula.
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Izracunava 32bitnu vrednost CRCa nad 32bitnim podatkom.
 *
 * @param       data                    32bitni podatak
 * @return      Izracunata 32bitna vrednost CRCa.
 *
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_CRC_CALC_SW32) || defined(__DOXYGEN__)
# define HAL_CRC_CALC_SW32(data)                                                \
    (uint32_t)crc32calc(data)
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Izracunava 32bitnu vrednost CRCa nad 32bitnim podatkom.
 *
 * @param       data                    32bitni podatak
 * @return      Izracunata 32bitna vrednost CRCa.
 * @note        Ovaj makro racuna CRC32 i smesta podatak u zadatu promenljivu.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_CRC_SHARED_CALC_SW32) || defined(__DOXYGEN__)
# define HAL_CRC_SHARED_CALC_SW32(data, metaresult)                             \
    metaresult = sharedCRC32calc(data, metaresult)
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Izracunava 32bitnu vrednost nad 8bitnim blokom podataka.
 *
 * @param       pbuffer                 pointer na blok sa podacima.
 * @param       bufferlenght            velicina bloka izrazena u recima (word).
 * @return      Izracunata 32bitna vrednost CRCa.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_CRC_CALC_BLOCK_SW32) || defined(__DOXYGEN__)
# define HAL_CRC_CALC_BLOCK_SW32(pbuffer, bufferlength)                         \
    (uint32_t)crc32calcBlock((uint8_t *) pbuffer, (uint32_t)bufferlength)
#endif

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Konfiguracija CRC-a za interakciju sa softverom i ostalom periferijom mikrokontrolera,
 *
 * @brief       Citanje, brisanje interapta, flagova, DMA requesta.
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Resetuje HAL_CRC_CALC_HW32 makro, tj. postavnja njegov
 *              data registar na inicijalnu vrednost.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_CRC_RESET_CALC_SW32) || defined(__DOXYGEN__)
# define HAL_CRC_RESET_CALC_SW32()                                              \
    crc32reset()
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Resetuje HAL_CSHARED_RC_CALC_HW32 makro, tj. postavnja njegov
 *              data registar na inicijalnu vrednost.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_CRC_SHARED_RESET_CALC_SW32) || defined(__DOXYGEN__)
# define HAL_CRC_SHARED_RESET_CALC_SW32(metaresult)                             \
    metaresult = 0xFFFFFFFF
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Vraca izracunatu vrednost deljenog CRC-a.
 *
 * @return      Izracunatu vrednost CRC-a.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_CRC_GET_SW32) || defined(__DOXYGEN__)
#define HAL_CRC_GET_SW32()                                                      \
    getCRC32()
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Vraca izracunatu vrednost deljenog CRC-a.
 *
 * @return      Izracunatu vrednost CRC-a.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_CRC_SHARED_GET_SW32) || defined(__DOXYGEN__)
#define HAL_CRC_SHARED_GET_SW32(metaresult)                                     \
    (metaresult ^ 0xFFFFFFFF)
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

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       NEDOKUMENTOVANO
 * @todo        dokumentovati
 */
/*-----------------------------------------------------------------------------------------------*/
void crc32reset(void);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       NEDOKUMENTOVANO
 * @todo        dokumentovati
 */
/*-----------------------------------------------------------------------------------------------*/
uint32_t crc32calc(
    uint32_t data);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       NEDOKUMENTOVANO
 * @todo        dokumentovati
 */
/*-----------------------------------------------------------------------------------------------*/
uint32_t sharedCRC32calc(
    uint32_t data,
    uint32_t metaResultStore);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       NEDOKUMENTOVANO
 * @todo        dokumentovati
 */
/*-----------------------------------------------------------------------------------------------*/
uint32_t crc32calcBlock(
    uint8_t * input,
    uint32_t size);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       NEDOKUMENTOVANO
 * @todo        dokumentovati
 */
/*-----------------------------------------------------------------------------------------------*/
uint32_t getCRC32(
    void);


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
 * END of crc.h
 *************************************************************************************************/
#endif /* VARNT_CRC_H_ */
