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
 * @author      nesa
 *
 * @brief   	Konfiguracija State Machine Processor (SMP) modula.
 *
 * ------------------------------------------------------------------------------------------------
 *
 * @addtogroup	core_cfg
 *
 ****************************************************************************************//** @{ */

#ifndef SMP_CFG_H_
#define SMP_CFG_H_


/*************************************************************************************************
 * INCLUDE FILES
 *************************************************************************************************/


/*************************************************************************************************
 * DEFINES
 *************************************************************************************//** @cond */


/** @endcond *//**********************************************************************************
 * SETTINGS
 *************************************************************************************************/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Debug podrska SP modula
 *
 *              Ukoliko je ova opcija:
 *              - definisana: omoguceno je debagiranje modula,
 *              - nedefinisana: nije omoguceno debagiranje modula.
 *
 *              Omogucavanjem debagiranja mogu da se ukljucuje sledeci makroi:
 *              - @ref DBG_ASSERT
 *              - @ref DBG_ASSERT_ALWAYS
 *              - @ref DBG_ASSERT_COMPILE
 *              - @ref DBG_CHECK
 *              - @ref DBG_ENTRY
 *              - @ref DBG_EXIT
 *
 * @see         dbg_intf
 */
/*-----------------------------------------------------------------------------------------------*/
#if defined(__DOXYGEN__)
# define OPT_DBG_SP
#endif


/*************************************************************************************************
 * CONFIGURATION RESOLUTION
 *************************************************************************************//** @cond */


/** @endcond *//** @} *//*************************************************************************
 * END of smp_cfg.h
 *************************************************************************************************/
#endif /* SMP_CFG_H_ */
