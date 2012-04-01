/*************************************************************************************************
 * Standard configuration file: tem_cfg.h
 * This file is part of eSolid
 *
 * Template version 1.1.3 (13.11.2011)
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
 * @version     {tem_version}
 * @date    	Nov 18, 2011
 * @author  	nesa
 *
 * @brief   	Konfiguracija Time Event Management (TEM) modula.
 *
 * ------------------------------------------------------------------------------------------------
 *
 * @addtogroup	tem_cfg
 *
 ****************************************************************************************//** @{ */


#ifndef TEM_CFG_H_
#define TEM_CFG_H_


/*************************************************************************************************
 * INCLUDE FILES
 *************************************************************************************************/


/*************************************************************************************************
 * DEFINES
 *************************************************************************************//** @cond */


#include PORT_ARCH_HEADER(core/tem_def)


/** @endcond *//**********************************************************************************
 * SETTINGS
 *************************************************************************************************/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Tip podataka za kontrolne podatke jednog tajmera.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(OPT_TMRCNTR_T) || defined(__DOXYGEN__)
# define OPT_TMRCNTR_T                  uint8_t
#endif

/*-------------------------------------------------------------------------------------------*//**
 * @name        Debug podrska
 *
 * @brief       Makroi za debug podrsku. Pogledati @ref dbg_intf.
 *
 * @{ *//*---------------------------------------------------------------------------------------*/

#if defined(OPT_DBG_TEM)
# define TEM_ASSERT                     DBG_ASSERT
# define TEM_ASSERT_ALWAYS              DBG_ASSERT_ALWAYS
# define TEM_COMPILE_ASSERT             DBG_COMPILE_ASSERT
# define TEM_DBG_DECL                   DBG_DECL
# define TEM_DBG_DEFINE_MODULE          DBG_DEFINE_MODULE
# define TEM_DBG_ENTRY                  DBG_ENTRY
# define TEM_DBG_EXIT                   DBG_EXIT
# define TEM_DBG_MACRO                  DBG_MACRO
#else
# define TEM_ASSERT(expr)				DBG_EMPTY_MACRO()
# define TEM_ASSERT_ALWAYS(expr)		DBG_EMPTY_MACRO()
# define TEM_COMPILE_ASSERT(expr)		DBG_EMPTY_DECL()
# define TEM_DBG_DECL(expr)				DBG_EMPTY_DECL()
# define TEM_DBG_DEFINE_MODULE(expr)	DBG_EMPTY_DECL()
# define TEM_DBG_ENTRY()				DBG_EMPTY_MACRO()
# define TEM_DBG_EXIT()					DBG_EMPTY_MACRO()
# define TEM_DBG_MACRO(expt)			DBG_EMPTY_MACRO()
#endif


/** @} *//*--------------------------------------------------------------------------------------*/



/*************************************************************************************************
 * CONFIGURATION RESOLUTION
 *************************************************************************************//** @cond */

typedef OPT_TMRCNTR_T tem_tmrTick_T;


/** @endcond *//**********************************************************************************
 * END of HEADER FILE
 *************************************************************************************************/
/** @} *//****************************************************************************************/
#endif /* TEM_CFG_H_ */
