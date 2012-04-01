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
 * @brief       Konfiguracija Memory Management modula
 *
 * ------------------------------------------------------------------------------------------------
 *
 * @addtogroup  core_cfg
 *
 ****************************************************************************************//** @{ */


#ifndef MM_CFG_H_
#define MM_CFG_H_


/*************************************************************************************************
 * INCLUDE FILES
 *************************************************************************************************/


/*************************************************************************************************
 * DEFINES
 *************************************************************************************//** @cond */

#define MM_IS_FF                       1
#define MM_IS_TLSF                     2


/** @endcond *//**********************************************************************************
 * SETTINGS
 *************************************************************************************************/

/*-------------------------------------------------------------------------------------------*//**
 * @name        Podesavanje Memory Management modula
 *
 *              Ovim opcijama se specificira:
 *              - debug podrska,
 *              - odabir algoritma memorijskog alokatora.
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Debug podrska MM modula
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
# define OPT_DBG_MM
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Odabir algoritma alokatora koji se koristi.
 *
 *              Postoje vise implementacija memorijskog alokatora. Ovom opcijom
 *              se moze konfigurisati koja implementacija memorijskog alokatora
 *              se koristi u zavisnosti od potrebe aplikacije. Videti
 *              @ref doc_mem.
 *
 *              Postoje dve implementacije:
 *              - MM_IS_FF             @ref mem_ff_alloc
 *              - MM_IS_TLSF           @ref mem_tlsf_alloc
 *
 * @note        Podrazumevano podesavanje: koristi se TLSF
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(OPT_MM_TYPE) || defined(__DOXYGEN__)
# define OPT_MM_TYPE                   MM_IS_FF
#endif

/** @} *//*--------------------------------------------------------------------------------------*/


/*************************************************************************************************
 * CONFIGURATION RESOLUTION
 *************************************************************************************//** @cond */


/** @endcond *//** @} *//*************************************************************************
 * END of mem_cfg.h
 *************************************************************************************************/
#endif /* MM_CFG_H_ */
