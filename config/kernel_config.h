/*
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
 * @defgroup    kernel_cfg Kernel configuration
 *********************************************************************//** @{ */

#ifndef KERNEL_CONFIG_H_
#define KERNEL_CONFIG_H_

/*=========================================================  INCLUDE FILES  ==*/
/*===============================================================  DEFINES  ==*/

#define ES_MM_DYNAMIC_ONLY              0
#define ES_MM_STATIC_ONLY               -1
#define ES_SMP_FSM_ONLY                 1
#define ES_SMP_HSM_ONLY                 2
#define ES_SMP_FSM_AND_HSM              3
#define ES_KERNEL_API_MM                0
#define ES_KERNEL_API_SM                1
#define ES_KERNEL_API_FULL              2

/* Ovde se pisu podesavanja projekta -----------------------------------------*/

# define OPT_EVT_USE_GENERATOR
# define OPT_KERNEL_EPA_PRIO_MAX        9U

/*==============================================================  SETTINGS  ==*/

/**
 * @brief       Vrsi validaciju API ugovora
 * @details     Ukljucivanjem ove opcija eSolid funkcije ce vrsiti proveru
 *              predatih argumenata.
 */
#if defined(__DOXYGEN__)
# define OPT_KERN_API_VALIDATION
#endif

/**
 * @brief       Debug podrska State Machine Processor (SMP) modula
 * @details     Ukoliko je ova opcija:
 *                  - definisana: omoguceno je debagiranje modula,
 *                  - nedefinisana: nije omoguceno debagiranje modula.
 *
 * @pre         Da bi ova opcija bila aktivna mora se prethodno ukljuciti
 *              opcija @ref OPT_KERNEL_DBG.
 */
#if defined(__DOXYGEN__)
# define OPT_KERNEL_DBG_SMP
#endif

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

/**@} *//*----------------------------------------------------------------*//**
 * @name        Podesavanje Kernel-a
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Nivo kernel interfejsa koji se koristi
 * @details     Ovom promenljivom se definise koji interfejs kernal se koristi.
 *              U zavisnosti od potrebe aplikacije moguce je koristiti sledece
 *              nivoe:
 *              - 0 - omogucen je samo Memory Management modul. Korisnik moze da
 *              koristi samo interfejs Memory Management modula. Ukljucivanje
 *              interfejsa ostalih modula je zabranjeno.
 *              - 1 - omogucen je State Machine Processor (SMP). Sa obzirom da
 *              SMP modul koristi funkcije MM modula onda je i MM modul, takodje,
 *              aktiviran. Ovaj nivo interfejsa se najcesce koristi kada je
 *              potrebno pokrenuti do nekoliko automata prekidnim rutinama ili u
 *              glavnoj programskoj petlji.
 *              - 2 - omogucen je kompletan interfejs kernel-a. Ovaj interfejs
 *              se koristi kada je potrebno konkuretno pokrenuti nekoliko
 *              razlicitih EPA objekata.
 * @note        Podrazumevano podesavanje: 2 (ukljucen je kompletan kernel
 *              interfejs)
 */
#if !defined(OPT_KERNEL_API_LEVEL) || defined(__DOXYGEN__)
# define OPT_KERNEL_API_LEVEL           ES_KERNEL_API_FULL
#endif

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

/**
 * @brief       Maksimalan prioritet prekidnih rutina kernela
 * @details     Ovim se ogranicava prioritet prekidnih rutina koje jezgro
 *              koristi. Ovo podesavanje je izuzetno korisno kada postoji
 *              potreba da se pojedini hardverski prekidi ne prekidaju od strane
 *              jezgra.
 * @note        Podrazumevano podesavanje: ES_PRIO_REALTIME
 */
#if !defined(OPT_KERNEL_INTERRUPT_PRIO_MAX) || defined(__DOXYGEN__)
# define OPT_KERNEL_INTERRUPT_PRIO_MAX  ES_PRIO_REALTIME
#endif

/**@} *//*----------------------------------------------------------------*//**
 * @name        Podesavanje Memory Management (MM) modula
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Managed RAM size.
 * @details     Size of the RAM area to be managed by the eSolid. If set to zero
 *              then the whole available RAM is used. The core memory is made
 *              available to the heap allocator and/or can be used directly
 *              through the simplified core memory allocator.
 * @note        In order to let the OS manage the whole RAM the linker script
 *              must provide the @p _sheap and @p _eheap symbols.
 * @note        DEFAULT: 0 (All memory)
 */
#if !defined(OPT_MM_MANAGED) || defined(__DOXYGEN__)
# define OPT_MM_MANAGED_SIZE            0U
#endif

/**
 * @brief       Distribution of static and dynamic memory
 * @details     Size of the RAM area that is given to the dynamic memory manager.
 *              Here you can specify how much memory is given to dynamic memory
 *              manager.
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
 * @note        DEFAULT: ES_MM_DYNAMIC_ONLY (Only dynamic memory manager is
 *              enabled)
 */
#if !defined(OPT_MM_STATIC_SIZE) || defined(__DOXYGEN__)
# define OPT_MM_DISTRIBUTION            ES_MM_DYNAMIC_ONLY
#endif

/**@} *//*----------------------------------------------------------------*//**
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
#if !defined(OPT_SMP_SM_TYPES) || defined(__DOXYGEN__)
# define OPT_SMP_SM_TYPES               ES_SMP_FSM_ONLY
#endif

/** @} *//*-------------------------------------------------------------------*/

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/

#if !defined(ES_HAL_ENABLE_INTERRUPT)
# error "Kernel precondition is not satisfied: enable Interrupt module with option: OPT_HAL_INTERRUPT"
#endif

/** @endcond *//** @} *//******************************************************
 * END of kernel_cfg.h
 ******************************************************************************/
#endif /* KERNEL_CONFIG_H_ */
