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
 *//******************************************************************************************//**
 * @file
 * @author  	Nenad Radulovic
 * @brief       Konfiguracija eSolid Kernel-a
 * @defgroup    kernel_cfg Kernel configuration
 ****************************************************************************************//** @{ */


#ifndef KERNEL_CONFIG_H_
#define KERNEL_CONFIG_H_

/*============================================================================  INCLUDE FILES  ==*/
/*==================================================================================  DEFINES  ==*/

/* Ovde se pisu podesavanja projekta ------------------------------------------------------------*/


/*=================================================================================  SETTINGS  ==*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Debagiranje Kernel-a
 * @{ *//*---------------------------------------------------------------------------------------*/

/**
 * @brief       Debug podrska Kernel-a
 * @details     Ukoliko je ova opcija:
 *                  - definisana: omoguceno je debagiranje kernela,
 *                  - nedefinisana: nije omoguceno debagiranje kernela.
 *
 * @note        Ova opcija mora biti definisana ukoliko se zeli debagiranje
 *              pojedinih modula.
 */
#if defined(__DOXYGEN__)
# define OPT_KERNEL_DBG
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
 * @brief       Debug podrska CORE podmodula
 * @details     Ukoliko je ova opcija:
 *                  - definisana: omoguceno je debagiranje modula,
 *                  - nedefinisana: nije omoguceno debagiranje modula.
 *
 * @pre         Da bi ova opcija bila aktivna mora se prethodno ukljuciti
 *              opcija @ref OPT_KERNEL_DBG.
 */
#if defined(__DOXYGEN__)
# define OPT_KERNEL_DBG_CORE
#endif

/**
 * @brief       Debug podrska Event Management (EVT) podmodula
 * @details     Ukoliko je ova opcija:
 *                  - definisana: omoguceno je debagiranje modula,
 *                  - nedefinisana: nije omoguceno debagiranje modula.
 *
 * @pre         Da bi ova opcija bila aktivna mora se prethodno ukljuciti
 *              opcija @ref OPT_KERNEL_DBG.
 */
#if defined(__DOXYGEN__)
# define OPT_KERNEL_DBG_EVT
#endif

/**
 * @brief       Debug podrska Memory Management (MM) podmodula
 * @details     Ukoliko je ova opcija:
 *                  - definisana: omoguceno je debagiranje modula,
 *                  - nedefinisana: nije omoguceno debagiranje modula.
 *
 * @pre         Da bi ova opcija bila aktivna mora se prethodno ukljuciti
 *              opcija @ref OPT_KERNEL_DBG.
 */
#if defined(__DOXYGEN__)
# define OPT_KERNEL_DBG_MM
#endif

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Podesavanje Kernel-a
 * @{ *//*---------------------------------------------------------------------------------------*/

/**
 * @brief       Da li se koriste dinamicki objekti
 * @details     Ukoliko je promenljiva:
 *                  - definisana: za alokaciju memorije se koristi memorijska
 *                  klasa koja je data u argumentu funkcija.
 *                  - nedefinisana: za alokaciju memorije se koristi
 *                  @ref esMemStaticClass klasa memorije
 */
#if defined(__DOXYGEN__)
# define OPT_KERNEL_USE_DYNAMIC
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
#if !defined(OPT_KERNEL_INTERRUPT_PRIO_MAX)
# define OPT_KERNEL_INTERRUPT_PRIO_MAX  ES_PRIO_REALTIME
#endif

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Podesavanje Memory Management (MM) modula
 * @{ *//*---------------------------------------------------------------------------------------*/

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
 * @brief       Initial heap memory size.
 * @details     Size of the RAM area that is initially given to the heap memory
 *              manager. If there is a need for more heap memory the system will
 *              automatically require aditional memory space from static memory
 *              manager.
 *
 *              If initial heap memory size value is set to zero the heap memory
 *              manager will be disabled. All calls to it will be redirected to
 *              static memory manager. Please note, that in this case you will
 *              not be able to free memory area once it is acquired.
 *
 *              If you wish to disable static memory manager and use only heap
 *              memory manager enter here -1.
 * @note        DEFAULT: -1 (Only heap memory manager enabled)
 */
#if !defined(OPT_MM_STATIC_SIZE) || defined(__DOXYGEN__)
# define OPT_MM_HEAP_SIZE               -1
#endif

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Podesavanje Event Management (EVT) modula
 * @{ *//*---------------------------------------------------------------------------------------*/

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
 * @note        Podrazumevano podesavanje: uint_fast8_t
 */
#if !defined(OPT_EVT_ID_T) || defined(__DOXYGEN__)
# define OPT_EVT_ID_T                   uint_fast8_t
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

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Tipovi podataka koje korisnik definise
 * @{ *//*---------------------------------------------------------------------------------------*/

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

/** @} *//*--------------------------------------------------------------------------------------*/

/*===================================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/

/** @endcond *//** @} *//*************************************************************************
 * END of kernel_cfg.h
 *************************************************************************************************/
#endif /* KERNEL_CONFIG_H_ */
