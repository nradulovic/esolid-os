/*************************************************************************************************
 * Standard header file: hal_support.h
 * This file is part of eSolid
 *
 * Template version 1.1.3 (14.11.2011)
 *
 * Copyright (C) 2011 - Nenad Radulović
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
 * @version     {hal_version}
 * @date        Dec 17, 2011
 * @author      nesa
 *
 * @brief       Podrska za HAL modul.
 *
 * ------------------------------------------------------------------------------------------------
 *
 * @addtogroup  hal_intf
 *
 ****************************************************************************************//** @{ */


#ifndef HAL_SUPPORT_H_
#define HAL_SUPPORT_H_


/*************************************************************************************************
 * INCLUDE FILES
 *************************************************************************************************/

/*-----------------------------------------------------------------------------------------------*
 * Module dependencies
 *-----------------------------------------------------------------------------------------------*/

#include "dbg/dbg.h"


/*-----------------------------------------------------------------------------------------------*
 * Module configuration and adapter
 *-----------------------------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------------------------*
 * EXTERNS
 *-----------------------------------------------------------------------------------*//** @cond */

#ifdef SUPPORT_H_VAR
#define SUPPORT_H_EXT
#else
#define SUPPORT_H_EXT extern
#endif


/** @endcond*//***********************************************************************************
 * DEFINES
 *************************************************************************************************/


/*************************************************************************************************
 * MACRO's
 *************************************************************************************************/

/*-------------------------------------------------------------------------------------------*//**
 * @name        Debug podrska
 *
 * @brief       Makroi za debug podrsku. Pogledati @ref dbg_intf.
 * @{ *//*---------------------------------------------------------------------------------------*/

#if defined(OPT_DBG_HAL) || defined(__DOXYGEN__)
# define HAL_ASSERT                     DBG_ASSERT
# define HAL_ASSERT_ALWAYS              DBG_ASSERT_ALWAYS
# define HAL_COMPILE_ASSERT             DBG_COMPILE_ASSERT
# define HAL_DBG_DECL                   DBG_DECL
# define HAL_DBG_DEFINE_MODULE          DBG_DEFINE_MODULE
# define HAL_DBG_ENTRY                  DBG_ENTRY
# define HAL_DBG_EXIT                   DBG_EXIT
# define HAL_DBG_MACRO                  DBG_MACRO
# define HAL_DBG_CHECK                  DBG_CHECK
#else
# define HAL_ASSERT(expr)               DBG_EMPTY_MACRO()
# define HAL_ASSERT_ALWAYS(expr)        DBG_EMPTY_MACRO()
# define HAL_COMPILE_ASSERT(expr)       DBG_EMPTY_DECL()
# define HAL_DBG_DECL(expr)             DBG_EMPTY_DECL()
# define HAL_DBG_DEFINE_MODULE(expr)    DBG_EMPTY_DECL()
# define HAL_DBG_ENTRY()                DBG_EMPTY_MACRO()
# define HAL_DBG_EXIT()                 DBG_EMPTY_MACRO()
# define HAL_DBG_MACRO(expr)            DBG_EMPTY_MACRO()
# define HAL_DBG_CHECK(expr)            DBG_EMPTY_MACRO()
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
 * END of HEADER FILE
 *************************************************************************************************/
#endif /* HAL_SUPPORT_H_ */
