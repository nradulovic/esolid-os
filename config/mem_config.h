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
 * @name        Podesavanje Memory Management modula
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Managed RAM size by CORE allocator.
 * @details     Size of the RAM area to be managed by the eSolid. If set to zero
 *              then the whole available RAM is used. The core memory is made
 *              available to the heap allocator and/or can be used directly
 *              through the simplified core memory allocator.
 * @pre         In order to let the OS manage the whole RAM the linker script
 *              must provide the @p _sheap and @p _eheap symbols.
 * @note        DEFAULT: 0 (All memory)
 */
#if !defined(OPT_MEM_CORE_SIZE) || defined(__DOXYGEN__)
# define OPT_MEM_CORE_SIZE              0U
#endif

/** @} *//*-------------------------------------------------------------------*/
/*------------------------------------------------------------------------*//**
 * @name        eSolid memory allocator override
 * @brief       Ovim opcijama se moze koristiti drugi alokator
 * @{ *//*--------------------------------------------------------------------*/

#if !defined(OPT_SYS_ENABLE_MEM)
# define OPT_MEM_ALLOC                  malloc
# define OPT_MEM_FREE                   free
#endif

/** @} *//*-------------------------------------------------------------------*/
/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/

#if defined(OPT_SYS_ENABLE_MEM)
# if !defined(ES_HAL_ENABLE_STARTUP)
#  if (OPT_MEM_CORE_SIZE == 0UL)
#   error "Memory manager precondition is not satisfied: either enable OPT_HAL_STARTUP or define managed memory size with OPT_MEM_MANAGED_SIZE"
#  endif
# endif
#endif

/** @endcond *//** @} *//******************************************************
 * END of mem_config.h
 ******************************************************************************/
#endif /* MEM_CONFIG_H_ */
