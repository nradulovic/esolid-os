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
 *************************************************************************************************/


/*********************************************************************************************//**
 * @file
 *
 * @author  	nenad
 *
 * @brief   	Konfiguracija Software Trace Recording (STR) modula.
 *
 * ------------------------------------------------------------------------------------------------
 *
 * @addtogroup	str_cfg
 *
 ****************************************************************************************//** @{ */


#ifndef STR_CFG_H_
#define STR_CFG_H_


/*************************************************************************************************
 * INCLUDE FILES
 *************************************************************************************************/


/*************************************************************************************************
 * DEFINES
 *************************************************************************************//** @cond */



/** @endcond *//**********************************************************************************
 * SETTINGS
 *************************************************************************************************/

/*-------------------------------------------------------------------------------------------*//**
 * @name        Debug podrska
 *
 * @brief       Makroi za debug podrsku. Pogledati @ref dbg_intf.
 *
 * @{ *//*---------------------------------------------------------------------------------------*/

#if defined(OPT_DBG_STR)
# define STR_ASSERT                     DBG_ASSERT
# define STR_ASSERT_ALWAYS              DBG_ASSERT_ALWAYS
# define STR_COMPILE_ASSERT             DBG_COMPILE_ASSERT
# define STR_DBG_DECL                   DBG_DECL
# define STR_DBG_DEFINE_MODULE          DBG_DEFINE_MODULE
# define STR_DBG_ENTRY                  DBG_ENTRY
# define STR_DBG_EXIT                   DBG_EXIT
# define STR_DBG_MACRO                  DBG_MACRO
#else
# define STR_ASSERT(expr)				DBG_EMPTY_MACRO()
# define STR_ASSERT_ALWAYS(expr)		DBG_EMPTY_MACRO()
# define STR_COMPILE_ASSERT(expr)		DBG_EMPTY_DECL()
# define STR_DBG_DECL(expr)				DBG_EMPTY_DECL()
# define STR_DBG_DEFINE_MODULE(expr)	DBG_EMPTY_DECL()
# define STR_DBG_ENTRY()				DBG_EMPTY_MACRO()
# define STR_DBG_EXIT()					DBG_EMPTY_MACRO()
# define STR_DBG_MACRO(expt)			DBG_EMPTY_MACRO()
#endif


/** @} *//*--------------------------------------------------------------------------------------*/


/*************************************************************************************************
 * CONFIGURATION RESOLUTION
 *************************************************************************************//** @cond */


/** @endcond *//** @} *//*************************************************************************
 * END of str_cfg.h
 *************************************************************************************************/
#endif /* STR_CFG_H_ */
