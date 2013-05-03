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
 * @author  	Nenad Radulovic
 * @brief       Konfiguracija eSolid Kernel-a
 * @addtogroup  kernel_cfg
 *********************************************************************//** @{ */

#ifndef KERNEL_CONFIG_H_
#define KERNEL_CONFIG_H_

/*=========================================================  INCLUDE FILES  ==*/
#include "log_config.h"

/*===============================================================  DEFINES  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Predefinisane konstante za odabir opcija
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Koristi se za opciju @ref OPT_SMP_SM_TYPES : Omoguceni su samo
 *              FSM tipovi automata
 */
#define ES_SMP_FSM_ONLY                 1U

/**
 * @brief       Koristi se za opciju @ref OPT_SMP_SM_TYPES : Omoguceni su samo
 *              HSM tipovi automata
 */
#define ES_SMP_HSM_ONLY                 2U

/**
 * @brief       Koristi se za opciju @ref OPT_SMP_SM_TYPES : Omoguceni su FSM i
 *              HSM tipovi automata
 */
#define ES_SMP_FSM_AND_HSM              3U

/** @} *//*-------------------------------------------------------------------*/
/*==============================================================  SETTINGS  ==*/
/*------------------------------------------------------------------------*//**
 * @name        Podesavanje Kernel-a
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Maksimalan prioritet EPA objekata u sistemu
 * @details     Ova opcija omogucava da se ujedno definise maksimalan broj EPA
 *              objekata u sistemu. Podrazumevano podesavanje je 64 sto je i
 *              maksimalan broj prioriteta na 8-bitnoj arhitekturi. 16-bitne i
 *              32-bitne arhitekture podrazavaju vide od 64 EPA objekata.
 * @note        Podrazumevano podesavanje: 8 nivoa
 */
#if !defined(OPT_KERNEL_EPA_PRIO_MAX) || defined(__DOXYGEN__)
# define OPT_KERNEL_EPA_PRIO_MAX        8U
#endif

/** @} *//*-------------------------------------------------------------------*/
/*------------------------------------------------------------------------*//**
 * @name        Podesavanje State Machine Processor (SMP) modula
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Omogucavanje vise tipova automata
 * @details     Ovom opcijom se definisu moguci tipovi automata u aplikaciji:
 *              - @ref ES_SMP_FSM_ONLY - omoguceni su samo FSM automati
 *              - @ref ES_SMP_HSM_ONLY - omoguceni su samo HSM automati
 *              - @ref ES_SMP_FSM_AND_HSM - omogucena su oba tipa automata,
 *              selekcija dispecera se vrsi dinamicki.
 *
 * @note        Podrazumevano podesavanje: @ref ES_SMP_FSM_AND_HSM
 */
#if !defined(OPT_SMP_SM_TYPES)
# define OPT_SMP_SM_TYPES               ES_SMP_FSM_AND_HSM
#endif

/** @} *//*-------------------------------------------------------------------*/
/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/

#if defined(ES_HAL_ENABLE_CPU)
# error "Kernel precondition is not satisfied: enable CPU module with option: OPT_HAL_CPU"
#endif

#if defined(ES_HAL_ENABLE_INTERRUPT)
# error "Kernel precondition is not satisfied: enable Interrupt module with option: OPT_HAL_INTERRUPT"
#endif

/** @endcond *//** @} *//******************************************************
 * END of kernel_cfg.h
 ******************************************************************************/
#endif /* KERNEL_CONFIG_H_ */
