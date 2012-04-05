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
 * @author      Nenad Radulovic
 * @brief       Konfiguracija Kernel podmodula.
 * ------------------------------------------------------------------------------------------------
 * @addtogroup  core_cfg
 ****************************************************************************************//** @{ */


#ifndef KERNEL_CFG_H_
#define KERNEL_CFG_H_


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
 * @name        Podesavanje KERNEL modula i EPA objekata
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Debug podrska KERNEL modula
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
# define OPT_DBG_KERNEL
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Omogucavanje/onemogucavanje registra
 *
 *              Ukoliko je ova promenljiva definisana koriste se registar
 *              funkcije i dodatni podaci u strukturi EPA objekata za njihov
 *              poboljasani opis.
 */
/*-----------------------------------------------------------------------------------------------*/
#if defined(__DOXYGEN__)
# define OPT_KERNEL_USE_REGISTRY
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Da li se koriste dinamicki EPA objekti
 *
 *              Ukoliko je promenljiva:
 *              - definisana: za alokaciju memorije se koristi memorijska klasa
 *              koja je data u argumentu eS_epaCreate().
 *              - nedefinisana: za alokaciju memorije se koristi memStaticClass
 *              klasa memorije
 */
/*-----------------------------------------------------------------------------------------------*/
#if defined(__DOXYGEN__)
# define OPT_KERNEL_USE_DYNAMIC
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Maksimalan prioritet EPA objekata u sistemu
 *
 *              Ova opcija omogucava da se ujedno definise maksimalan broj EPA
 *              objekata u sistemu. Podrazumevano podesavanje je 64 sto je i
 *              maksimalan broj prioriteta na 8-bitnoj arhitekturi. 16-bitne i
 *              32-bitne arhitekture podrazavaju vide od 64 EPA objekata.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(OPT_KERNEL_EPA_PRIO_MAX) || defined(__DOXYGEN__)
# define OPT_KERNEL_EPA_PRIO_MAX                  64U
#endif

/** @} *//*--------------------------------------------------------------------------------------*/


/*************************************************************************************************
 * CONFIGURATION RESOLUTION
 *************************************************************************************//** @cond */


/** @endcond *//** @} *//*************************************************************************
 * END of kernel_cfg.h
 *************************************************************************************************/
#endif /* KERNEL_CFG_H_ */
