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
 * @brief       Koristi se za opciju @ref OPT_MM_DISTRIBUTION : Ukljucen je samo
 *              dinamicki alokator
 */
#define ES_MM_DYNAMIC_ONLY              0

/**
 * @brief       Koristi se za opciju @ref OPT_MM_DISTRIBUTION : Ukljucen je samo
 *              staticki alokator
 */
#define ES_MM_STATIC_ONLY               -1

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

/** @} *//*-------------------------------------------------------------------*/
/*------------------------------------------------------------------------*//**
 * @name        Podesavanje Memory Management (MM) modula
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Managed RAM size.
 * @details     Size of the RAM area to be managed by the eSolid. If set to zero
 *              then the whole available RAM is used. The core memory is made
 *              available to the heap allocator and/or can be used directly
 *              through the simplified core memory allocator.
 * @pre         In order to let the OS manage the whole RAM the linker script
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
#if !defined(OPT_MM_DISTRIBUTION)
# define OPT_MM_DISTRIBUTION            ES_MM_DYNAMIC_ONLY
#endif

/**
 * @brief       Static memory allocator override
 */
#if defined(__DOXYGEN__)
# define OPT_MM_STATIC_ALLOC
#endif

/**
 * @brief       Dynamic memory allocator override
 */
#if defined(__DOXYGEN__)
# define OPT_MM_DYNAMIC_ALLOC
# define OPT_MM_DYNAMIC_DEALLOC
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
/*------------------------------------------------------------------------*//**
 * @name        Podesavanje Event Management (EVT) modula
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Koristi se atribut o velicini dogadjaja
 * @details     Moguce vrednosti:
 *                  - nedefinisano - ne koristi se atribut o velicini
 *                  - definisano   - koristi se atribut o velicini
 *
 *              Podesavanje tipa se vrsi pomocu @ref OPT_EVT_SIZE_T.
 * @note        Podrazumevano podesavanje: Dogadjaji ne koriste atribut o
 *              velicini.
 */
#if defined(__DOXYGEN__)
# define OPT_EVT_USE_SIZE
#endif

/**
 * @brief       Koristi se pokazivac na proizvodjaca dogadjaja.
 * @details     Moguce vrednosti:
 *                  - nedefinisano - pokazivac na objekat se ne koristi
 *                  - definisano   - pokazivac na objekat se koristi
 *
 * @note        Podrazumevano podesavanje: Dogadjaji ne koriste atribut o
 *              generatoru.
 */
#if defined(__DOXYGEN__)
# define OPT_EVT_USE_GENERATOR
#endif

/**
 * @brief       Koristi se vremenski marker dogadjaja.
 * @details     Moguce vrednosti:
 *              - nedefinisano - vrem. marker se ne koristi
 *              - definisano   - vrem. marker se koristi
 *
 *              Podesavanje tipa se vrsi pomocu @ref OPT_EVT_TIMESTAMP_T.
 * @note        Podrazumevano podesavanje: Dogadjaji ne koriste atribut o
 *              vremenskom markeru.
 */
#if defined(__DOXYGEN__)
# define OPT_EVT_USE_TIMESTAMP
#endif

/**
 * @brief       Tip promenljive za identifikator dogadjaja.
 * @details     Tip podatka treba da dovoljne velicine moze da predstavi sve
 *              dogadjaje u sistemu. Na primer, ako u sistemu postoje manje od
 *              255 identifikatora dogadjaja onda ovaj tip moze najmanje biti
 *              uint8_t tipa. Ukoliko sistem poseduje vise od 255 razlicitih
 *              dogadjaja onda ovaj tip treba da bude najmanje uint16_t. Naravno,
 *              treba voditi racuna o zauzecu strukture koja opisuje dogadjaj i
 *              njenom poravnjanju (alignement) u memoriji, gde se moze javiti
 *              potreba za vecim tipom od minimalnog.
 * @note        Podrazumevano podesavanje: uint32_t
 */
#if !defined(OPT_EVT_ID_T) || defined(__DOXYGEN__)
# define OPT_EVT_ID_T                   uint16_t
#endif

/**
 * @brief       Tip podataka za cuvanje velicine prosirenog dogadjaja.
 * @note        Podrazumevano podesavanje: size_t
 */
#if !defined(OPT_EVT_SIZE_T) || defined(__DOXYGEN__)
# define OPT_EVT_SIZE_T                 size_t
#endif

/**
 * @brief       Tip za cuvanje vremenskih atributa dogadjaja.
 * @note        Podrazumevano podesavanje: uint32_t
 */
#if !defined(OPT_EVT_TIMESTAMP_T) || defined(__DOXYGEN__)
# define OPT_EVT_TIMESTAMP_T            uint32_t
#endif

/**
 * @brief       Atribut za strukture dogadaja
 * @details     Prilikom slanja dogadjaja drugim sistemima javlja se problem
 *              pakovanja podataka unutar strukture dogadjaja.
 *              Ovom promenljivom se moze definisati koja direktiva ce se
 *              koristiti za strukture dogadjaja (poravnjanje, pakovanje).
 * @note        Podrazumevano podesavanje: /
 */
#if !defined(OPT_EVT_STRUCT_ATTRIB) || defined(__DOXYGEN__)
# define OPT_EVT_STRUCT_ATTRIB
#endif

/** @} *//*-------------------------------------------------------------------*/
/*------------------------------------------------------------------------*//**
 * @name        Tipovi podataka koje korisnik definise
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Tip podataka za identifikator dogadjaja
 */
typedef OPT_EVT_ID_T                    esEvtId_T;

/**
 * @brief       Tip podataka za vremenski marker dogadjaja
 */
#if defined(OPT_EVT_USE_TIMESTAMP) || defined(__DOXYGEN__)
typedef OPT_EVT_TIMESTAMP_T             esEvtTimestamp_T;
#endif

/**
 * @brief       Tip podataka za atribut velicine dogadjaja
 */
#if defined(OPT_EVT_USE_SIZE) || defined(__DOXYGEN__)
typedef OPT_EVT_SIZE_T                  esEvtSize_T;
#endif

/** @} *//*-------------------------------------------------------------------*/
/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/

#if !defined(ES_HAL_ENABLE_STARTUP)
# if (OPT_MM_MANAGED_SIZE == 0UL)
#  error "Kernel precondition is not satisfied: either enable OPT_HAL_STARTUP or define managed memory size with OPT_MM_MANAGED_SIZE"
# endif
#endif

#if !defined(ES_HAL_ENABLE_CPU)
# error "Kernel precondition is not satisfied: enable CPU module with option: OPT_HAL_CPU"
#endif

#if !defined(ES_HAL_ENABLE_INTERRUPT)
# error "Kernel precondition is not satisfied: enable Interrupt module with option: OPT_HAL_INTERRUPT"
#endif

/** @endcond *//** @} *//******************************************************
 * END of kernel_cfg.h
 ******************************************************************************/
#endif /* KERNEL_CONFIG_H_ */
