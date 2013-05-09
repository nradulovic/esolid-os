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

#define OPT_MEM_SMEM_ENABLE

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
#if !defined(OPT_MEM_SMEM_SIZE)
# define OPT_MEM_SMEM_SIZE              1024U
#endif

/** @} *//*-------------------------------------------------------------------*/
/*------------------------------------------------------------------------*//**
 * @name        Zastita od istovremenog pristupa
 * @brief       Ovim makroima se sprecava istovremeni pristup istom memorijskom
 *              prostoru.
 * @details     Koriscenjem ovih makroa moze da se specificira da se prilikom
 *              pristupa memorijskom prostoru prvo zakljuca pristup na odredjeni
 *              nacin. To se moze vrsiti na nekoliko nacina:
 *              - gasenje prekida,
 *              - podizanje prioriteta scheduler-a,
 *              - aktiviranjem mutex-a
 *              - aktiviranjem binarnog semaphore-a.
 * @p           Najjednostavniji nacin je gasenje prekida. Primer u takvom
 *              slucaju bi bio sledeci:
 *              1. Ostaviti makro @ref GUARD_T nedefinisan
 *              2. Ostaviti makro @ref GUART_INIT prazan (nema funkciju)
 *              3. Definisati makro @ref GUARD_LOCK kao: esIntDisable()
 *              4. Definisati makro @ref GUARD_UNLOCK kao: esIntEnable()
 * @note        Treba naglasiti da za gornji primer treba ucitati u datoteku
 *              @c "hal/hal_int.h".
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Tip podataka za zastitu memorijskog alokatora
 * @details     Ovde treba postaviti tip cuvara alokatora, kao na primer
 *              struktura mutex-a ili semaphore-a kada se koristi neki RTOS.
 */
#if defined(__DOXYGEN__)
#define GUARD_T
#endif

/**
 * @brief       Inicijalizacija cuvara memorijskog alokatora
 * @details     Ovde treba postaviti funkciju koja vrsi inicijalizaciju cuvara
 *              alokatora. Ona se poziva u esPMemInit() ili esDMemInit()
 *              funkcijama.
 */
#define GUARD_INIT(guard)               (void)guard

/**
 * @brief       Zakljucavanje memorijskog alokatora
 */
#define GUARD_LOCK(guard)               (void)guard

/**
 * @brief       Otkljucavanje memorijskog alokatora
 */
#define GUARD_UNLOCK(guard)             (void)guard

/** @} *//*-------------------------------------------------------------------*/
/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of mem_config.h
 ******************************************************************************/
#endif /* MEM_CONFIG_H_ */
