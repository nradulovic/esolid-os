/*************************************************************************************************
 * Standard configuration file: tem_def.h
 * This file is part of lcsw-eSolid
 *
 * Template version 1.1.3 (13.11.2011)
 *
 * Copyright (C) 2011 - Nenad Radulović
 *
 * lcsw-eSolid is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * lcsw-eSolid is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with lcsw-eSolid; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301  USA
 *************************************************************************************************/


/*********************************************************************************************//**
 * @file
 *
 * @version     {tem_version}
 * @date    	Nov 29, 2011
 * @author  	nesa
 *
 * @brief   	Podrazumevane vrednosti za TEM modul.
 *
 * ------------------------------------------------------------------------------------------------
 *
 * @addtogroup	tem_cfg
 *
 ****************************************************************************************//** @{ */
/*-------------------------------------------------------------------------------------------*//**
**//*--------------------------------------------------------------------------------------------*/


#ifndef TEM_DEF_H_
#define TEM_DEF_H_


/*************************************************************************************************
 * INCLUDE FILES
 *************************************************************************************************/


/*************************************************************************************************
 * DEFINES
 *************************************************************************************//** @cond */


/** @endcond *//**********************************************************************************
 * SETTINGS
 *************************************************************************************//** @cond */

#if !defined(ES_OPTIMIZE_SIZE) && defined(ES_OPTIMIZE_SPEED)

# if !defined(OPT_TMRCNTR_T)
#  define OPT_TMRCNTR_T                 uint16_t
# endif
#endif

#if defined(ES_OPTIMIZE_SIZE) && !defined(ES_OPTIMIZE_SPEED)

# if !defined(OPT_TMRCNTR_T)
#  define OPT_TMRCNTR_T                 uint16_t
# endif
#endif

#if !defined(ES_OPTIMIZE_SIZE) && !defined(ES_OPTIMIZE_SPEED)

# if !defined(OPT_TMRCNTR_T)
#  define OPT_TMRCNTR_T                 uint16_t
# endif
#endif


/** @endcond *//**********************************************************************************
 * END of HEADER FILE
 *************************************************************************************************/
/** @} *//****************************************************************************************/
#endif /* TEM_DEF_H_ */
