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
 * @brief       Generiše ko je proizvodžač događaja
 * @details     Ova callback funkcija se poziva ukoliko je uključena opcija
 *              @ref OPT_EVT_USE_GENERATOR. Tada je potrebno tek kreirani
 *              događaj popuniti identifikatorom (pokazivač) proizvodžača
 *              događaja.
 *
 * @note        Podrazumevano podesavanje: poziva se kernel funkcija
 */
#if !defined(OPT_EVT_GENERATOR_CALLBACK)
# define OPT_EVT_GENERATOR_CALLBACK()   esKernelEpaGet()
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
 * @brief       Generiše vremenski marker događaja
 * @details     Ova callback funkcija se poziva ukoliko je uključena opcija
 *              @ref OPT_EVT_USE_TIMESTAMP. Tada je potrebno tek kreirani
 *              događaj popuniti vremenskim markerom nastanka događaja.
 *
 * @note        Podrazumevano podesavanje: 0UL (Ne poziva se funkcija)
 */
#if !defined(OPT_EVT_TIMESTAMP_CALLBACK)
# define OPT_EVT_TIMESTAMP_CALLBACK()   0UL
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
 * @note        Podrazumevano podesavanje: @ref C_PACKED - pakovana struktrua
 */
#if !defined(OPT_EVT_STRUCT_ATTRIB) || defined(__DOXYGEN__)
# define OPT_EVT_STRUCT_ATTRIB          C_PACKED
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
