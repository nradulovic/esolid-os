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
 * @brief       Interfejs CRC modul.
 *
 * ------------------------------------------------------------------------------------------------
 *
 * @addtogroup  hal_crc
 *
 ****************************************************************************************//** @{ */


#ifndef HAL_CRC_H_
#define HAL_CRC_H_


/*************************************************************************************************
 * INCLUDE FILES
 *************************************************************************************************/

/*-----------------------------------------------------------------------------------------------*
 * Module dependencies
 *-----------------------------------------------------------------------------------------------*/

#include "port/port.h"


/*-----------------------------------------------------------------------------------------------*
 * Module configuration and adapter
 *-----------------------------------------------------------------------------------------------*/

#include PORT_VARIANT_HEADER(hal/crc)


/*-----------------------------------------------------------------------------------------------*
 * EXTERNS
 *-----------------------------------------------------------------------------------*//** @cond */

#ifdef HAL_CRC_H_VAR
#define HAL_CRC_H_EXT
#else
#define HAL_CRC_H_EXT extern
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
# define HAL_CRC_CALC_HW32(data)
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Izracunava 32bit-nu vrednost nad 32bit-nim blokom podataka.
 *
 * @param       pbuffer                 pointer na blok sa podacima.
 * @param       bufferlength            velicina bloka izrazena u recima (word).
 * @return      Izracunata 32bitna vrednost CRC-a.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_CRC_CALC_BLOCK_HW32) || defined(__DOXYGEN__)
# define HAL_CRC_CALC_BLOCK_HW32(pbuffer, bufferlength)
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Resetuje HAL_CRC_CALC_HW32 makro, tj. postavlja njegov
 *              data registar na inicijalnu vrednost.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_CRC_RESET_CALC_HW32) || defined(__DOXYGEN__)
# define HAL_CRC_RESET_CALC_HW32()
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Vraca izracunatu vrednost CRC-a.
 * @return      Izracunatu vrednost CRC-a.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_CRC_GET_HW32) || defined(__DOXYGEN__)
#define HAL_CRC_GET_HW32()
#endif


/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Softwerski realizovani CRC.
 *
 * @brief       Makroi za izracunavanje CRC-a posredstvom softverskog modula.
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Izraqcunava 32bitnu vrednost CRCa nad 32bitnim podatkom.
 *
 * @param       data                    32bitni podatak
 * @return      Izracunata 32bitna vrednost CRCa.
 *
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_CRC_CALC_SW32) || defined(__DOXYGEN__)
# define HAL_CRC_CALC_SW32(data)
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Izracunava 32bitnu vrednost CRCa nad 32bitnim podatkom.
 *
 * @param       data                    32bitni podatak
 * @param       metaresult              NEDOKUMENTOVANO
 * @return      Izracunata 32bitna vrednost CRCa.
 * @note        Ovaj makro racuna CRC32 i smesta podatak u zadatu promenljivu.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_CRC_SHARED_CALC_SW32) || defined(__DOXYGEN__)
# define HAL_CRC_SHARED_CALC_SW32(data, metaresult)
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Izracunava 32bitnu vrednost nad 32bitnim blokom podataka.
 *
 * @param       pbuffer                 pointer na blok sa podacima.
 * @param       bufferlength            velicina bloka izrazena u recima (word).
 * @return      Izracunata 32bitna vrednost CRCa.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_CRC_CALC_BLOCK_SW32) || defined(__DOXYGEN__)
# define HAL_CRC_CALC_BLOCK_SW32(pbuffer, bufferlength)
#endif

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Konfiguracija CRCa za interakciju sa softverom i,
 *              ostalom periferijom mikrokontrolera,
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
# define HAL_CRC_RESET_CALC_HW32()
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Resetuje HAL_CSHARED_RC_CALC_HW32 makro, tj. postavlja njegov
 *              data registar na inicijalnu vrednost.
 *
 * @param       metaresult              NEDOKUMENTOVANO
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_CRC_SHARED_RESET_CALC_SW32) || defined(__DOXYGEN__)
# define HAL_CRC_SHARED_RESET_CALC_SW32(metaresult)
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Vraca izracunatu vrednost CRC-a.
 *
 * @return      Izracunatu vrednost CRC-a.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_CRC_GET_SW32) || defined(__DOXYGEN__)
# define HAL_CRC_GET_SW32()
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Vraca izracunatu vrednost deljenog CRC-a.
 *
 * @param       metaresult              NEDOKUMENTOVANO
 * @return      Izracunatu vrednost CRC-a.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_CRC_SHARED_GET_SW32) || defined(__DOXYGEN__)
# define HAL_CRC_SHARED_GET_SW32(metaresult)
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
 * END of hal_crc.h
 *************************************************************************************************/
#endif /* HAL_CRC_H_ */
