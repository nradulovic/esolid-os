/*************************************************************************************************
 * This file is part of eSolid
 *
 * Copyright (C) 2011, 2012 - Nenad Radulovic
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
 *
 * web site:    http://blueskynet.dyndns-server.com
 * e-mail  :    blueskyniss@gmail.com
 *************************************************************************************************/


/*********************************************************************************************//**
 * @file
 *
 * @author      Nenad Radulovic
 *
 * @brief       Privatni interfejs Memory Management podmodul.
 *
 * ------------------------------------------------------------------------------------------------
 *
 * @addtogroup  mm_impl
 *
 ****************************************************************************************//** @{ */


#ifndef MM_PKG_H_
#define MM_PKG_H_


/*************************************************************************************************
 * INCLUDE FILES
 *************************************************************************************************/


/*-----------------------------------------------------------------------------------------------*
 * EXTERNS
 *-----------------------------------------------------------------------------------*//** @cond */

#ifdef MM_PKG_H_VAR
# define MM_PKG_H_EXT
#else
# define MM_PKG_H_EXT extern
#endif


/** @endcond*//***********************************************************************************
 * DEFINES
 *************************************************************************************************/


/*************************************************************************************************
 * MACRO's
 *************************************************************************************************/

/*-------------------------------------------------------------------------------------------*//**
 * @name        Debug podrska
 * @brief       Makroi za debug podrsku. Pogledati @ref dbg_intf.
 * @{ *//*---------------------------------------------------------------------------------------*/

#if defined(OPT_DBG_MM) || defined(__DOXYGEN__)
# define MM_ASSERT                      DBG_ASSERT
# define MM_ASSERT_ALWAYS               DBG_ASSERT_ALWAYS
# define MM_COMPILE_ASSERT              DBG_COMPILE_ASSERT
# define MM_DBG_DECL                    DBG_DECL
# define MM_DBG_DEFINE_MODULE           DBG_DEFINE_MODULE
# define MM_DBG_ENTRY                   DBG_ENTRY
# define MM_DBG_EXIT                    DBG_EXIT
# define MM_DBG_MACRO                   DBG_MACRO
# define MM_DBG_CHECK                   DBG_CHECK
#else
# define MM_ASSERT(expr)                DBG_EMPTY_MACRO()
# define MM_ASSERT_ALWAYS(expr)         DBG_EMPTY_MACRO()
# define MM_COMPILE_ASSERT(expr)        DBG_EMPTY_DECL()
# define MM_DBG_DECL(expr)              DBG_EMPTY_DECL()
# define MM_DBG_DEFINE_MODULE(expr)     DBG_EMPTY_DECL()
# define MM_DBG_ENTRY()                 DBG_EMPTY_MACRO()
# define MM_DBG_EXIT()                  DBG_EMPTY_MACRO()
# define MM_DBG_MACRO(expr)             DBG_EMPTY_MACRO()
# define MM_DBG_CHECK(expr)             DBG_EMPTY_MACRO()
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

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Klasa memorije
 */
/*-----------------------------------------------------------------------------------------------*/
struct esMemClass {
/**
 * @brief       Memorijski alokator
 */
    void * (*pAlloc)(size_t);

/**
 * @brief       Memorijski dealokator
 */
    void (*pDeAlloc)(void *);

/**
 * @brief       Velicina alocirane memorijske oblasti
 */
    size_t (*pBlockSize)(void *);
};

/*************************************************************************************************
 * GLOBAL VARIABLES
 *************************************************************************************************/

/*-------------------------------------------------------------------------------------------*//**
 * @name        Variables group
 *
 * @brief       brief description
 * @{ *//*---------------------------------------------------------------------------------------*/

/** @} *//*--------------------------------------------------------------------------------------*/


/*************************************************************************************************
 * FUNCTION PROTOTYPES
 *************************************************************************************************/

/*-------------------------------------------------------------------------------------------*//**
 * @name        Function group
 *
 * @brief       brief description
 * @{ *//*---------------------------------------------------------------------------------------*/

/** @} *//*--------------------------------------------------------------------------------------*/


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
 * END of mem_pkg.h
 *************************************************************************************************/
#endif /* MM_PKG_H_ */
