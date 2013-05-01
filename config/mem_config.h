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
 * @defgroup    mem_cfg Memory Management Configuration
 * @brief       Konfiguracija
 *********************************************************************//** @{ */

#ifndef MEM_CONFIG_H_
#define MEM_CONFIG_H_

/*=========================================================  INCLUDE FILES  ==*/
/*===============================================================  DEFINES  ==*/
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
 * @note        DEFAULT: 0 (All memory)
 */
#if !defined(OPT_MEM_SMEM_SIZE) || defined(__DOXYGEN__)
# define OPT_MEM_SMEM_SIZE              0U
#endif

/**
 * @brief       Omogucavanje dinamickog memorijskog menadzmenta
 * @details     Ukoliko se koristi eSolid dinamicki alokator ova opcija mora
 *              biti definisana.
 * @see         OPT_MEM_ALLOC
 * @see         OPT_MEM_FREE
 */
#if defined(__DOXYGEN__)
# define OPT_MEM_DMEM_ENABLE
#endif

/** @} *//*-------------------------------------------------------------------*/
/*------------------------------------------------------------------------*//**
 * @name        Memory Manager allocator override
 * @brief       When eSolid Memory Manager is not used these macros provide
 *              default fall back standard C library routines.
 * @{ *//*--------------------------------------------------------------------*/

#if !defined(OPT_MEM_DMEM_ENABLE)
/**
 * @brief       Fallback alokator funkcija
 * @details     Makro se koristi kada je zabranjen rad dinamickog alokatora,
 *              odnosno, kada opcija @ref OPT_MEM_DMEM_ENABLE nije definisana. U
 *              tom slucaju eSolid ce pozvati funkciju koja je navedena ovde.
 * @note        Default: malloc
 */
# define OPT_MEM_ALLOC                  malloc

/**
 * @brief       Fallback dealokator funkcija
 * @details     Makro se koristi kada je zabranjen rad dinamickog alokatora,
 *              odnosno, kada opcija @ref OPT_MEM_DMEM_ENABLE nije definisana. U
 *              tom slucaju eSolid ce pozvati funkciju koja je navedena ovde.
 * @note        Default: free
 */
# define OPT_MEM_FREE                   free
#endif

/** @} *//*-------------------------------------------------------------------*/
/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/

#if (OPT_MEM_SMEM_SIZE == 0U) && !defined(ES_HAL_ENABLE_STARTUP)
# error "Memory manager precondition is not satisfied: either enable OPT_HAL_STARTUP or define managed memory size with OPT_MEM_MANAGED_SIZE"
#endif

/** @endcond *//** @} *//******************************************************
 * END of mem_config.h
 ******************************************************************************/
#endif /* MEM_CONFIG_H_ */
