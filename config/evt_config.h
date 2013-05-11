/******************************************************************************
 * This file is part of esolid-kernel
 *
 * Template version: 1.1.15 (20.05.2012)
 *
 * Copyright (C) 2011, 2012 - Nenad Radulovic
 *
 * esolid-kernel is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * esolid-kernel is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with esolid-kernel; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301  USA
 *
 * web site:    http://blueskynet.dyndns-server.com
 * e-mail  :    blueskyniss@gmail.com
 *//***********************************************************************//**
 * @file
 * @author  	Nenad Radulovic
 * @brief       Event Object Configuration
 * @defgroup    evt_cfg Event Object Configuration
 * @brief       Konfiguracija
 * @details     Podesavanja Event objekta.
 *********************************************************************//** @{ */

#ifndef EVT_CONFIG_H_
#define EVT_CONFIG_H_

/*=========================================================  INCLUDE FILES  ==*/
/*===============================================================  DEFINES  ==*/
/*==============================================================  SETTINGS  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Podesavanje Event objekta
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Atribut za strukturu dogadaja
 * @details     Prilikom slanja dogadjaja drugim sistemima javlja se problem
 *              pakovanja podataka unutar strukture dogadjaja.
 *              Ovom promenljivom se moze definisati koja direktiva ce se
 *              koristiti za strukture dogadjaja (poravnjanje, pakovanje).
 * @note        Podrazumevano podesavanje: @ref C_PACKED - pakovana struktrua
 */
#if !defined(OPT_EVT_STRUCT_ATTRIB)
# define OPT_EVT_STRUCT_ATTRIB
#endif

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
 * @details     Ova callback funkcija se poziva samo ako je uključena opcija
 *              @ref OPT_EVT_USE_GENERATOR. Tada je potrebno tek kreirani
 *              događaj popuniti identifikatorom (pokazivač) proizvodžača
 *              događaja.
 * @pre         Opcija @ref OPT_EVT_USE_GENERATOR mora da bude ukljucena pre ove
 *              opcije.
 * @note        Podrazumevano podesavanje: ne koristi se callback funkcija
 */
#if defined(__DOXYGEN__)
# define OPT_EVT_GENERATOR_CALLBACK
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
 * @pre         Opcija @ref OPT_EVT_USE_TIMESTAMP mora da bude ukljucena pre ove
 *              opcije.
 * @note        Podrazumevano podesavanje: ne koristi se callback funkcija
 */
#if defined(__DOXYGEN__)
# define OPT_EVT_TIMESTAMP_CALLBACK
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
#if !defined(OPT_EVT_ID_T)
# define OPT_EVT_ID_T                   uint16_t
#endif

/**
 * @brief       Tip podataka za cuvanje velicine prosirenog dogadjaja.
 * @note        Podrazumevano podesavanje: size_t
 * @note        Ovo podesavanje ima znacaja samo ako je ukljucena opcija
 *              @ref OPT_EVT_USE_SIZE
 */
#if !defined(OPT_EVT_SIZE_T)
# define OPT_EVT_SIZE_T                 size_t
#endif

/**
 * @brief       Tip za cuvanje vremenskih atributa dogadjaja.
 * @note        Podrazumevano podesavanje: uint32_t
 * @note        Ovo podesavanje ima znacaja samo ako je ukljucena opcija
 *              @ref OPT_EVT_USE_TIMESTAMP
 */
#if !defined(OPT_EVT_TIMESTAMP_T)
# define OPT_EVT_TIMESTAMP_T            uint32_t
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
/** @endcond *//** @} *//******************************************************
 * END of evt_config.h
 ******************************************************************************/
#endif /* EVT_CONFIG_H_ */
