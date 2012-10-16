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
 * ------------------------------------------------------------------------------------------------
 * @addtogroup  kernel_cfg
 ****************************************************************************************//** @{ */


#ifndef KERNEL_CONFIG_H_
#define KERNEL_CONFIG_H_

/*============================================================================  INCLUDE FILES  ==*/
/*==================================================================================  DEFINES  ==*/

#define OPT_KERNEL_ENABLE

/*=================================================================================  SETTINGS  ==*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Debagiranje Kernel-a
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Debug podrska Kernel modula
 * @details     Ukoliko je ova opcija:
 *              - definisana: omoguceno je debagiranje modula,
 *              - nedefinisana: nije omoguceno debagiranje modula.
 * @note        Ova opcija mora biti definisana ukoliko se zeli debagiranje
 *              pojedinih podmodula.
 *//*--------------------------------------------------------------------------------------------*/
#if defined(__DOXYGEN__)
# define OPT_KERNEL_DBG
#endif

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Debug podrska State Processor (SPROC) podmodula
 * @details     Ukoliko je ova opcija:
 *              - definisana: omoguceno je debagiranje modula,
 *              - nedefinisana: nije omoguceno debagiranje modula.
 *//*--------------------------------------------------------------------------------------------*/
#if defined(__DOXYGEN__)
# define OPT_KERNEL_DBG_SPROC
#endif

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Debug podrska CORE podmodula
 * @details     Ukoliko je ova opcija:
 *              - definisana: omoguceno je debagiranje modula,
 *              - nedefinisana: nije omoguceno debagiranje modula.
 *//*--------------------------------------------------------------------------------------------*/
#if defined(__DOXYGEN__)
# define OPT_KERNEL_DBG_CORE
#endif

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Debug podrska Event Management (EVT) podmodula
 * @details     Ukoliko je ova opcija:
 *              - definisana: omoguceno je debagiranje modula,
 *              - nedefinisana: nije omoguceno debagiranje modula.
 *//*--------------------------------------------------------------------------------------------*/
#if defined(__DOXYGEN__)
# define OPT_KERNEL_DBG_EVT
#endif

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Debug podrska Memory Management (MM) podmodula
 * @details     Ukoliko je ova opcija:
 *              - definisana: omoguceno je debagiranje modula,
 *              - nedefinisana: nije omoguceno debagiranje modula.
 *//*--------------------------------------------------------------------------------------------*/
#if defined(__DOXYGEN__)
# define OPT_KERNEL_DBG_MM
#endif

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Podesavanje Kernel-a
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Da li se koriste dinamicki objekti
 * @details     Ukoliko je promenljiva:
 *              - definisana: za alokaciju memorije se koristi memorijska klasa
 *              koja je data u argumentu esEpaCreate().
 *              - nedefinisana: za alokaciju memorije se koristi memStaticClass
 *              klasa memorije
 *//*--------------------------------------------------------------------------------------------*/
#if defined(__DOXYGEN__)
# define OPT_KERNEL_USE_DYNAMIC
#endif

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Maksimalan prioritet EPA objekata u sistemu
 * @details     Ova opcija omogucava da se ujedno definise maksimalan broj EPA
 *              objekata u sistemu. Podrazumevano podesavanje je 64 sto je i
 *              maksimalan broj prioriteta na 8-bitnoj arhitekturi. 16-bitne i
 *              32-bitne arhitekture podrazavaju vide od 64 EPA objekata.
 * @note        Podrazumevano podesavanje: 8 nivoa
 *//*--------------------------------------------------------------------------------------------*/
#if !defined(OPT_KERNEL_EPA_PRIO_MAX) || defined(__DOXYGEN__)
# define OPT_KERNEL_EPA_PRIO_MAX                  8U
#endif

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Maksimalan prioritet prekidnih rutina kernela
 * @details     Ovim se ogranicava prioritet prekidnih rutina koje jezgro
 *              koristi. Ovo podesavanje je izuzetno korisno kada postoji
 *              potreba da se pojedini hardverski prekidi ne prekidaju od strane
 *              jezgra.
 *//*--------------------------------------------------------------------------------------------*/
#if !defined(OPT_KERNEL_INTERRUPT_PRIO_MAX)
# define OPT_KERNEL_INTERRUPT_PRIO_MAX  ES_PRIO_REALTIME
#endif

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Planiranje zadataka bez istiskivanja sa fiksiranim prioritetom.
 * @note        Ovo je podrazumevano podesavanje
 *//*--------------------------------------------------------------------------------------------*/
#if !defined(OPT_KERNEL_SCHEDULER_COOPERATIVE) && !defined(OPT_KERNEL_SCHEDULER_PREEMPTIVE) || defined(__DOXYGEN__)
# define OPT_KERNEL_SCHEDULER_COOPERATIVE
#endif

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Planiranje zadataka sa istiskivanjem sa Round-Robin tehnikom.
 * @details     Ukoliko je ova opcija:
 *              - definisana: omoguceno je istiskivanje zadataka
 *              - nedefinisana: nije omoguceno istiskivanje zadataka
 *              (koristi se neki drugi rezim scheduler-a).
 *              Ukoliko je omoguceno istiskivanje zadataka, onda mogu da postoje
 *              vise EPA objekata sa istim nivoom prioriteta. Tada se vrsi
 *              planiranje zadataka po Round-Robin algoritmu.
 * @note        Ostale opcije za rezim rada OPT_KERNEL_SCHEDULER_xxx ne smeju
 *              biti istovremeno definisane.
 *//*--------------------------------------------------------------------------------------------*/
#if defined(__DOXYGEN__)
# define OPT_KERNEL_SCHEDULER_PREEMPTIVE
#endif

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Podesavanje Memory Management (MM) modula
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Odabir algoritma alokatora koji se koristi.
 * @details     Postoje vise implementacija memorijskog alokatora. Ovom opcijom
 *              se moze konfigurisati koja implementacija memorijskog alokatora
 *              se koristi u zavisnosti od potrebe aplikacije. Videti
 *              @ref doc_mem.
 *              Postoje dve implementacije:
 *              - MM_IS_FF             @ref mem_ff_alloc
 *              - MM_IS_TLSF           @ref mem_tlsf_alloc
 * @note        Podrazumevano podesavanje: koristi se FF
 *//*--------------------------------------------------------------------------------------------*/
#if !defined(OPT_MM_TYPE) || defined(__DOXYGEN__)
# define OPT_MM_TYPE                   MM_IS_FF
#endif

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Podesavanje Event Management (EVT) podmodula
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Koristi se atribut o velicini dogadjaja
 * @details     Moguce vrednosti:
 *              - nedefinisano - ne koristi se atribut o velicini
 *              - definisano   - koristi se atribut o velicini
 *//*--------------------------------------------------------------------------------------------*/
#if defined(__DOXYGEN__)
# define OPT_EVT_USE_SIZE
#endif

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Koristi se pokazivac na proizvodjaca dogadjaja.
 * @details     Moguce vrednosti:
 *              - nedefinisano - pokazivac na objekat se ne koristi
 *              - definisano   - pokazivac na objekat se koristi
 *//*--------------------------------------------------------------------------------------------*/
#if defined(__DOXYGEN__)
# define OPT_EVT_USE_TRANSMITTER
#endif

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Koristi se vremenski marker dogadjaja.
 * @details     Moguce vrednosti:
 *              - nedefinisano - vrem. marker se ne koristi
 *              - definisano   - vrem. marker se koristi
 *              Podesavanje tipa se vrsi pomocu @ref OPT_EVT_TIME_T.
 *//*--------------------------------------------------------------------------------------------*/
#if defined(__DOXYGEN__)
# define OPT_EVT_USE_TIME
#endif

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Koristi se tip dogadjaja.
 * @details     Moguce vrednosti:
 *              - nedefinisano - tip dogadjaja se ne koristi
 *              - definisano   - tip dogadjaja se koristi
 *              Podesavanje tipa se vrsi pomocu @ref OPT_EVT_TYPE_T.
 *//*--------------------------------------------------------------------------------------------*/
#if defined(__DOXYGEN__)
# define OPT_EVT_USE_TYPE
#endif

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Tip promenljive za identifikator dogadjaja.
 * @details     Tip podatka treba da dovoljne velicine moze da predstavi sve
 *              dogadjaje u sistemu. Na primer, ako u sistemu postoje manje od
 *              255 identifikatora dogadjaja onda ovaj tip moze najmanje biti
 *              uint8_t tipa. Ukoliko sistem poseduje vise od 255 razlicitih
 *              dogadjaja onda ovaj tip treba da bude najmanje uint16_t. Naravno,
 *              treba voditi racuna o zauzecu strukture koja opisuje dogadjaj i
 *              njenom poravnjanju (alignement) u memoriji, gde se moze javiti
 *              potreba za vecim tipom od minimalnog.
 *//*--------------------------------------------------------------------------------------------*/
#if defined(__DOXYGEN__)
# define OPT_EVT_ID_T
#endif

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Tip podataka za cuvanje velicine prosirenog dogadjaja.
 * @note        Ovo podesavanje moze da se zaobidje definisanjem OPT_EVT_SIZE_T.
 *//*--------------------------------------------------------------------------------------------*/
#if defined(__DOXYGEN__)
# define OPT_EVT_SIZE_T
#endif

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Tip za cuvanje vremenskih atributa dogadjaja.
 * @note        Ovo podesavanje moze da se zaobidje definisanjem OPT_EVT_TIME_T.
 *//*--------------------------------------------------------------------------------------------*/
#if defined(__DOXYGEN__)
# define OPT_EVT_TIME_T
#endif

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Tip za cuvanje tipa dogadjaja.
 * @note        Ovo podesavanje moze da se zaobidje definisanjem OPT_EVT_TYPE_T.
 *//*--------------------------------------------------------------------------------------------*/
#if defined(__DOXYGEN__)
# define OPT_EVT_TYPE_T
#endif

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Atribut za strukture dogadaja
 * @details     Prilikom slanja dogadjaja drugim sistemima javlja se problem
 *              pakovanja podataka unutar strukture dogadjaja.
 *              Ovom promenljivom se moze definisati koja direktiva ce se
 *              koristiti za strukture dogadjaja (poravnjanje, pakovanje).
 *//*--------------------------------------------------------------------------------------------*/
#if !defined(OPT_EVT_STRUCT_ATTRIB) || defined(__DOXYGEN__)
# define OPT_EVT_STRUCT_ATTRIB
#endif

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Dobavlja vremenski marker za dogadjaje
 * @details     Ovo je makro koji poziva callback funkciju za tacno vreme.
 *              Zadatak te funkcije je da pruzi informaciju o tacnom vremenu
 *              koje se utiskuje u tek kreirani dogadjaj. Korisnik ima za
 *              zadatak da u ovom makrou napise ime funkcije koja dobavlja
 *              trenutno vreme i sistem ce je pozivati u odgovarajucim trenucima.
 *//*--------------------------------------------------------------------------------------------*/
#if !defined(EVT_TIMESTAMP_GET) || defined(__DOXYGEN__)
# define EVT_TIMESTAMP_GET()            0U
#endif

/** @} *//*--------------------------------------------------------------------------------------*/

/*===================================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/*-------------------------------------------------------------------------------------------*//**
 * @brief       Tip podataka za identifikator dogadjaja
 *//*--------------------------------------------------------------------------------------------*/
#if defined(OPT_EVT_ID_T)
typedef OPT_EVT_ID_T                    esEvtId_T;
#else
typedef uint8_t                         esEvtId_T;
#endif

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Tip podataka za vremenski marker dogadjaja
 *//*--------------------------------------------------------------------------------------------*/
#if defined(OPT_EVT_USE_TIME) || defined(__DOXYGEN__)
# if defined(OPT_EVT_TIME_T)
typedef OPT_EVT_TIME_T                  esEvtTime_T;
# else
typedef uint32_t                        esEvtTime_T;
# endif
#endif

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Tip podataka za atribut velicine dogadjaja
 *//*--------------------------------------------------------------------------------------------*/
#if defined(OPT_EVT_USE_SIZE) || defined(__DOXYGEN__)
# if defined(OPT_EVT_SIZE_T)
typedef OPT_EVT_SIZE_T                  esEvtSize_T;
# else
typedef uint8_t                         esEvtSize_T;
# endif
#endif

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Tip podataka za atribut tipa dogadjaja
 *//*--------------------------------------------------------------------------------------------*/
#if defined(OPT_EVT_USE_TYPE) || defined(__DOXYGEN__)
# if defined(OPT_EVT_TYPE_T)
typedef OPT_EVT_TYPE_T                  esEvtType_T;
# else
typedef uint8_t                         esEvtType_T;
# endif
#endif

/*===================================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/

/** @endcond *//** @} *//*************************************************************************
 * END of kernel_cfg.h
 *************************************************************************************************/
#endif /* KERNEL_CONFIG_H_ */
