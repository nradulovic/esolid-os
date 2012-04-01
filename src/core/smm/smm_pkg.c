/*************************************************************************************************
 * This file is part of eSolid
 *
 * Copyright (C) 2011. 2012 - Nenad Radulovic
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
 * @author  	nesa
 *
 * @brief   	Privatni objekti SMM modula
 *
 * ------------------------------------------------------------------------------------------------
 *
 * @addtogroup  smm_impl
 *
 * @brief		Implementacija SMM modula.
 *
 ****************************************************************************************//** @{ */


/*************************************************************************************************
 * INCLUDE FILES
 *************************************************************************************************/

#define SMM_PKG_H_VAR
#include "../core_private.h"


/*************************************************************************************************
 * LOCAL DEFINES
 *************************************************************************************************/


/*************************************************************************************************
 * LOCAL MACRO's
 *************************************************************************************************/


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

/*-------------------------------------------------------------------------------------------*//**
 * @name        Klase memorijskog alokatora
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Dinamicki memorijski alokator (heap memory)
 */
/*-----------------------------------------------------------------------------------------------*/
const C_ROM smm_memClass_T memHeapClass = {
    &eS_hmemAlloc,
    &eS_hmemDeAlloc,
    &eS_hmemBlockSize,
};

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Skladiste memorije (memory pool)
 */
/*-----------------------------------------------------------------------------------------------*/
const C_ROM smm_memClass_T memPoolClass = {
    &eS_hmemAlloc,
    &eS_hmemDeAlloc,
    &eS_hmemBlockSize,
};

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Staticki memorijski alokator (static memory)
 */
/*-----------------------------------------------------------------------------------------------*/
const C_ROM smm_memClass_T memStaticClass = {
    &eS_hmemAlloc,
    &eS_hmemDeAlloc,
    &eS_hmemBlockSize,
};

/** @} *//*--------------------------------------------------------------------------------------*/

/*************************************************************************************************
 * LOCAL GLOBAL VARIABLES
 *************************************************************************************************/


/*************************************************************************************************
 ***                                I M P L E M E N T A T I O N                                ***
 *************************************************************************************************/


/*************************************************************************************************
 * LOCAL FUNCTION DEFINITIONS
 *************************************************************************************************/


/*************************************************************************************************
 * GLOBAL FUNCTION DEFINITIONS
 *************************************************************************************************/


/*************************************************************************************************
 * CONFIGURATION ERRORS
 *************************************************************************************//** @cond */


/** @endcond *//** @} *//*************************************************************************
 * END of smm_private.c
 *************************************************************************************************/
