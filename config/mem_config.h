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
 * @brief       Konfiguracija memorijskog menadzmenta
 * @addtogroup  mem_cfg
 *********************************************************************//** @{ */

#ifndef MEM_CONFIG_H_
#define MEM_CONFIG_H_

/*=========================================================  INCLUDE FILES  ==*/
/*===============================================================  DEFINES  ==*/
#define ES_MM_DYNAMIC_ONLY 1
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

/**
 * @brief       Distribution of static and dynamic memory
 * @details     Size of the RAM area that is given to the dynamic memory manager.
 *              Here you can specify how much memory is given to dynamic memory
 *              manager and to the static memory manager.
 *
 *              If dynamic memory size value is set to zero the dynamic memory
 *              manager will use all static memory effectively disabling static
 *              memory manager. All calls to static memory manager will be
 *              redirected to dynamic memory manager.
 *
 *              If you wish to disable dynamic memory manager and use only
 *              static memory manager enter here -1.
 *
 *              Options:
 *              - @ref ES_MM_STATIC_ONLY - Only static memory management is
 *              enabled
 *              - @ref ES_MM_DYNAMIC_ONLY - Only dynamic memory management is
 *              enabled
 *              - any other value - both memory managers are enabled.
 *
 * @note        DEFAULT: ES_MM_DYNAMIC_ONLY (Only dynamic memory manager is
 *              enabled)
 */
#if !defined(OPT_MEM_HEAP_SIZE)
# define OPT_MEM_HEAP_SIZE              ES_MM_DYNAMIC_ONLY
#endif

/** @} *//*-------------------------------------------------------------------*/
/*------------------------------------------------------------------------*//**
 * @name        eSolid memory allocator override
 * @{ *//*--------------------------------------------------------------------*/

#if !defined(OPT_SYS_ENABLE_MEM)
# define OPT_MEM_ALLOC                  malloc
# define OPT_MEM_FREE                   free
#endif

/** @} *//*-------------------------------------------------------------------*/
/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/

#if !defined(ES_HAL_ENABLE_STARTUP)
# if (OPT_MEM_CORE_SIZE == 0UL)
#  error "Memory manager precondition is not satisfied: either enable OPT_HAL_STARTUP or define managed memory size with OPT_MEM_MANAGED_SIZE"
# endif
#endif

/** @endcond *//** @} *//******************************************************
 * END of mem_config.h
 ******************************************************************************/
#endif /* MEM_CONFIG_H_ */
