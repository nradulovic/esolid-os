/******************************************************************************
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
 *//***********************************************************************//**
 * @file
 * @author      Nenad Radulovic
 * @brief       Memory Management Configuration
 * @addtogroup  mem_cfg
 * @brief       Konfiguracija
 * @details     Podesavanja memorijskog menadzera. Za detalje o radu alokatora
 *              pogledati @ref page_mem
 *********************************************************************//** @{ */

#ifndef MEM_CONFIG_H_
#define MEM_CONFIG_H_

/*=========================================================  INCLUDE FILES  ==*/
/*===============================================================  DEFINES  ==*/
/** @cond */
/*
 * U ovom segmentu (DEFINES) treba definisati svoje opcije, primer:
 *
 * #define OPT_MEM_SMEM_SIZE    1024U
 *
 * Kada se opcije definisu u segmentu DEFINES, podrazumevane opcije ce postati
 * neaktivne. Preporucuje se da se donji kod ne modifikuje na bilo kakav nacin
 * jer to moze dovesti do lose konfiguracije eSolid-a. Donji kod se koristi samo
 * kao polazna tacka ili kao primer kako treba definisati, odnosno,
 * konfigurisati opcije.
 */

/** @endcond */
/*==============================================================  SETTINGS  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Memory Management settings
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Managed RAM size by memory allocator.
 * @details     Size of the RAM area to be managed by the Memory Management. If
 *              set to zero then the whole available RAM is used. The memory is
 *              made available to the static allocator.
 * @pre         In order to let the Memory Manager manage the whole RAM the
 *              linker script must provide the @p _sheap and @p _eheap symbols.
 *              If eSolid HAL supports port startup sequence then the linker
 *              scripts will provide necessary symbols for whole memory
 *              allocation.
 * @note        DEFAULT: 1024B
 */
#if !defined(OPT_MEM_SMEM_SIZE) || defined(__DOXYGEN__)
# define OPT_MEM_SMEM_SIZE              1024U
#endif

/** @} *//*-------------------------------------------------------------------*/
/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/

#if (16U > OPT_MEM_SMEM_SIZE) && (0U != OPT_MEM_SMEM_SIZE)
# error "eSolid: MEM: Static memory is too small."
#endif

/** @endcond *//** @} *//******************************************************
 * END of mem_config.h
 ******************************************************************************/
#endif /* MEM_CONFIG_H_ */
