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
 * @author  	nenad
 *
 * @brief   	Konfiguracija Event Processing Network (EPN) Management modula.
 *
 * ------------------------------------------------------------------------------------------------
 *
 * @addtogroup	epn_cfg
 *
 ****************************************************************************************//** @{ */


#ifndef EPN_CFG_H_
#define EPN_CFG_H_


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

#if defined(OPT_DBG_EPN)
# define EPN_ASSERT                     DBG_ASSERT
# define EPN_ASSERT_ALWAYS              DBG_ASSERT_ALWAYS
# define EPN_COMPILE_ASSERT             DBG_COMPILE_ASSERT
# define EPN_DBG_DECL                   DBG_DECL
# define EPN_DBG_DEFINE_MODULE          DBG_DEFINE_MODULE
# define EPN_DBG_ENTRY                  DBG_ENTRY
# define EPN_DBG_EXIT                   DBG_EXIT
# define EPN_DBG_MACRO                  DBG_MACRO
# define EPN_DBG_CHECK                  DBG_CHECK
#else
# define EPN_ASSERT(expr)				DBG_EMPTY_MACRO()
# define EPN_ASSERT_ALWAYS(expr)		DBG_EMPTY_MACRO()
# define EPN_COMPILE_ASSERT(expr)		DBG_EMPTY_DECL()
# define EPN_DBG_DECL(expr)				DBG_EMPTY_DECL()
# define EPN_DBG_DEFINE_MODULE(expr)	DBG_EMPTY_DECL()
# define EPN_DBG_ENTRY()				DBG_EMPTY_MACRO()
# define EPN_DBG_EXIT()					DBG_EMPTY_MACRO()
# define EPN_DBG_MACRO(expt)			DBG_EMPTY_MACRO()
# define EPN_DBG_CHECK(expr)            DBG_EMPTY_MACRO()
#endif


/** @} *//*--------------------------------------------------------------------------------------*/


/*************************************************************************************************
 * CONFIGURATION RESOLUTION
 *************************************************************************************//** @cond */


/** @endcond *//** @} *//*************************************************************************
 * END of epn_cfg.h
 *************************************************************************************************/
#endif /* EPN_CFG_H_ */
