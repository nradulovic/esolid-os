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
 * @author  	Nenad Radulovic
 * @brief       Privatni interfejs State processor podmodul.
 * ------------------------------------------------------------------------------------------------
 * @addtogroup  sproc_impl
 ****************************************************************************************//** @{ */


#ifndef SPROC_PKG_H_
#define SPROC_PKG_H_


/*************************************************************************************************
 * INCLUDE FILES
 *************************************************************************************************/


/*-----------------------------------------------------------------------------------------------*
 * EXTERNS
 *-----------------------------------------------------------------------------------*//** @cond */

#ifdef SPROC_PKG_H_VAR
# define SPROC_PKG_H_EXT
#else
# define SPROC_PKG_H_EXT extern
#endif


/** @endcond*//***********************************************************************************
 * DEFINES
 *************************************************************************************************/

/*-------------------------------------------------------------------------------------------*//**
 * @name        Definition group
 *
 * @brief       brief description
 * @{ *//*---------------------------------------------------------------------------------------*/

/** @} *//*--------------------------------------------------------------------------------------*/


/*************************************************************************************************
 * MACRO's
 *************************************************************************************************/

/*-------------------------------------------------------------------------------------------*//**
 * @name        Debug podrska
 * @{ *//*---------------------------------------------------------------------------------------*/
#if defined(OPT_KERNEL_DBG_SPROC)
# define SP_ASSERT                      DBG_ASSERT
# define SP_ASSERT_ALWAYS               DBG_ASSERT_ALWAYS
# define SP_ASSERT_COMPILE              DBG_COMPILE_ASSERT
# define SP_DBG_DECL                    DBG_DECL
# define SP_DBG_DEFINE_MODULE           DBG_DEFINE_MODULE
# define SP_DBG_MACRO                   DBG_MACRO
# define SP_DBG_CHECK                   DBG_CHECK
#else
# define SP_ASSERT(expr)                DBG_EMPTY_MACRO()
# define SP_ASSERT_ALWAYS(expr)         DBG_EMPTY_MACRO()
# define SP_ASSERT_COMPILE(expr)        DBG_EMPTY_DECL()
# define SP_DBG_DECL(expr)              DBG_EMPTY_DECL()
# define SP_DBG_DEFINE_MODULE(expr)     DBG_EMPTY_DECL()
# define SP_DBG_MACRO(expr)             DBG_EMPTY_MACRO()
# define SP_DBG_CHECK(expr)             DBG_EMPTY_MACRO()
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

/*-------------------------------------------------------------------------------------------*//**
 * @name        Data types group
 *
 * @brief       brief description
 * @{ *//*---------------------------------------------------------------------------------------*/



/** @} *//*--------------------------------------------------------------------------------------*/


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
 * @brief       Vraca kolika je potrebna velicina memorijskog prostora za
 *              cuvanje bafera stanja.
 * @param       aStateDept              Maksimalna hijerarhijska dubina stanja
 *                                      automata.
 * @return      Potreban memorijski prostor u bajtovima.
 *//*--------------------------------------------------------------------------------------------*/
C_INLINE_ALWAYS size_t hsmReqSize_(
    size_t              aStateDept) {

    return (aStateDept * (size_t)2U * sizeof(esPtrState_T));
}

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Konstruise HSM automat
 * @param       aEpa                    Pokazivac na tek kreiran EPA objekat,
 * @param       aInitState              inicijalno stanje automata,
 * @param       aStateBuff              pokazivac na memorijski bafer za stanja,
 * @param       aStateDepth             maksimalna hijerarhijska dubina stanja
 *                                      automata.
 *//*--------------------------------------------------------------------------------------------*/
void hsmInit(
    esEpaHeader_T       * aEpa,
    esPtrState_T        aInitState,
    esPtrState_T        * aStateBuff,
    size_t              aStateDepth);

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Dekonstruise HSM automat
 * @param       aEpa                    Pokazivac na kreiran EPA objekat.
 *//*--------------------------------------------------------------------------------------------*/
void hsmDeInit(
    esEpaHeader_T       * aEpa);

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
 * END of sproc_pkg.h
 *************************************************************************************************/
#endif /* SPROC_PKG_H_ */
