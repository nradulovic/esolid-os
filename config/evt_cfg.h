/*
 * This file is part of eSolid
 *
 * Template version: 1.1.15 (20.05.2012)
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
 * @brief       Event Object Configuration
 * @defgroup    evt_cfg Event Object Configuration
 * @brief       Konfiguracija
 * @details     Each configuration option or setting has its own default value
 *              when not defined by the application. When application needs to
 *              change a setting it just needs to define a configuration macro
 *              with another value and the default configuration macro will be
 *              overridden.
 *********************************************************************//** @{ */

#ifndef EVT_CONFIG_H_
#define EVT_CONFIG_H_

/*=========================================================  INCLUDE FILES  ==*/
/*===============================================================  DEFINES  ==*/
/** @cond */
/*
 * U ovom segmentu (DEFINES) treba definisati svoje opcije, primer:
 *
 * #define CFG_EVT_STRUCT_ATTRIB    C_PACKED
 *
 * Kada se opcije definisu u segmentu DEFINES, podrazumevane opcije ce postati
 * neaktivne. Preporucuje se da se donji kod ne modifikuje na bilo kakav nacin
 * jer to moze dovesti do lose konfiguracije eSolid-a. Donji kod se koristi samo
 * kao polazna tacka ili kao primer kako treba definisati, odnosno,
 * konfigurisati opcije.
 */

/** @endcond */
/*==============================================================  SETTINGS  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Podesavanje Event objekta
 * @brief       Specifikacija @ref esEvt_T strukture dogadjaja
 * @details     Opcijom @ref CFG_EVT_STRUCT_ATTRIB se definise kakav layout
 *              struktura treba da ima u memoriji. Ovde se najpre misli na
 *              poravnjanje (@c alignment) elemenata u strukturi. Ostale opcije
 *              specificiraju koje elemente struktura poseduje.
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Attributes of event header structure
 * @details     The user can easily specify here additional attributes for event
 *              header structure to control the layout of the structure in
 *              memory. The layout of structure can be important when event are
 *              sent across networked devices.
 * @note        Default settings: empty (no additional structure attribute)
 */
#if !defined(CFG_EVT_STRUCT_ATTRIB)
# define CFG_EVT_STRUCT_ATTRIB
#endif

/**
 * @brief       Koristi se atribut o velicini dogadjaja
 * @details     Moguce vrednosti:
 *              - 0 - ne koristi se atribut o velicini
 *              - 1 - koristi se atribut o velicini
 *
 *              Podesavanje tipa se vrsi pomocu @ref CFG_EVT_SIZE_T.
 * @note        Podrazumevano podesavanje: 0 (dogadjaji ne koriste atribut o
 *              velicini)
 */
#if !defined(OPT_EVT_USE_SIZE) || defined(__DOXYGEN__)
# define OPT_EVT_USE_SIZE               1U
#endif

/**
 * @brief       Koristi se pokazivac na proizvodjaca dogadjaja.
 * @details     Moguce vrednosti:
 *              - 0 - pokazivac na objekat se ne koristi
 *              - 1 - pokazivac na objekat se koristi
 *
 * @note        Podrazumevano podesavanje: 0 (dogadjaji ne koriste atribut o
 *              generatoru)
 */
#if !defined(OPT_EVT_USE_GENERATOR) || defined(__DOXYGEN__)
# define CFG_EVT_USE_GENERATOR          1U
#endif

/**
 * @brief       Koristi se vremenski marker dogadjaja.
 * @details     Moguce vrednosti:
 *              - 0 - vrem. marker se ne koristi
 *              - 1 - vrem. marker se koristi
 *
 *              Podesavanje tipa se vrsi pomocu @ref CFG_EVT_TIMESTAMP_T.
 * @note        Podrazumevano podesavanje: 0 (dogadjaji ne koriste atribut o
 *              vremenskom markeru)
 */
#if !defined(OPT_EVT_USE_TIMESTAMP) || defined(__DOXYGEN__)
# define CFG_EVT_USE_TIMESTAMP          1U
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
 * @note        Ovo podesavanje ima znacaja samo ako je ukljucena opcija
 *              @ref OPT_EVT_USE_SIZE
 */
#if !defined(CFG_EVT_SIZE_T) || defined(__DOXYGEN__)
# define CFG_EVT_SIZE_T                 size_t
#endif

/**
 * @brief       Tip za cuvanje vremenskih atributa dogadjaja.
 * @note        Podrazumevano podesavanje: uint32_t
 * @note        Ovo podesavanje ima znacaja samo ako je ukljucena opcija
 *              @ref CFG_EVT_USE_TIMESTAMP
 */
#if !defined(CFG_EVT_TIMESTAMP_T) || defined(__DOXYGEN__)
# define CFG_EVT_TIMESTAMP_T            uint32_t
#endif

/** @} *//*-------------------------------------------------------------------*/
/*------------------------------------------------------------------------*//**
 * @name        Callback funkcije
 * @brief       Funkcije koje se pozivaju od strane EVT modula u odredjenim
 *              trenucima izvrsenja
 * @details     Ovde se nalaze sve callback funkcije koje EVT modul poziva. Njih
 *              treba da definise korisnik prema svojim potrebama.
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Generiše ko je proizvodžač događaja
 * @details     Ova callback funkcija se poziva samo ako je uključena opcija
 *              @ref CFG_EVT_USE_GENERATOR. Tada je potrebno tek kreirani
 *              događaj popuniti identifikatorom (pokazivač) proizvodžača
 *              događaja.
 *
 *              Moguce vrednosti:
 *              - 0 - ne koristi se callback funkcija
 *              - 1 - koristi se callback funkcija
 *
 * @pre         Opcija @ref CFG_EVT_USE_GENERATOR mora da bude ukljucena pre ove
 *              opcije.
 * @note        Podrazumevano podesavanje: 0 (ne koristi se callback funkcija)
 */
#if !defined(OPT_EVT_GENERATOR_CALLBACK) || defined(__DOXYGEN__)
# define OPT_EVT_GENERATOR_CALLBACK     1U
#endif

/**
 * @brief       Generiše vremenski marker događaja
 * @details     Ova callback funkcija se poziva ukoliko je uključena opcija
 *              @ref CFG_EVT_USE_TIMESTAMP. Tada je potrebno tek kreirani
 *              događaj popuniti vremenskim markerom nastanka događaja.
 *
 *              Moguce vrednosti:
 *              - 0 - ne koristi se callback funkcija
 *              - 1 - koristi se callback funkcija
 *
 * @pre         Opcija @ref CFG_EVT_USE_TIMESTAMP mora da bude ukljucena pre ove
 *              opcije.
 * @note        Podrazumevano podesavanje: 0 (ne koristi se callback funkcija)
 */
#if !defined(OPT_EVT_TIMESTAMP_CALLBACK) || defined(__DOXYGEN__)
# define OPT_EVT_TIMESTAMP_CALLBACK     1U
#endif

/** @} *//*-------------------------------------------------------------------*/
/*------------------------------------------------------------------------*//**
 * @name        Odabir memorijskog alokatora za dogadjaje
 * @brief       Moguce je izabrati koji memorijski alokator se koristi prilikom
 *              kreiranja dogadjaja.
 * @details     Ukoliko je potrebno specificirati koji memorijski alokator se
 *              koristi mogu se koristiti dole navedene opcije. Moze se
 *              koristiti @c pool memorijski alokator i @c dinamicki memorijski
 *              alokator.
 *              Postoje sledeci slucajevi:
 *              - koristi se iskljucivo @c pool alokator: ukljucena je opcija
 *              @ref OPT_EVT_USE_MEM_POOL, a iskljucena je opcija
 *              @ref OPT_EVT_USE_MEM_DYN.
 *              - koristi se iskljucivo @c dinamicki alokator: iskljucena je
 *              opcija @ref OPT_EVT_USE_MEM_POOL, a ukljucena je opcija
 *              @ref OPT_EVT_USE_MEM_DYN.
 *              - koriste se oba alokatora: ukljucene su obe opcije.
 *
 * @note        Zabranjeno je da obe opcije budu iskljucene, jedan od navedena
 *              dva alokatora se mora koristiti.
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Ukljucivanje/iskljucivanje @c pool alokatora
 * @details     Moguce vrednosti:
 *              - 0 - ne koristi se @c pool alokator
 *              - 1 - koristi se @c pool alokator
 *
 * @note        Podrazumevano podesavanje: 0 (ne koristi se @c pool alokator)
 */
#if !defined(OPT_EVT_USE_MEM_POOL) || defined(__DOXYGEN__)
# define OPT_EVT_USE_MEM_POOL           1U
#endif

/**
 * @brief       Ukljucivanje/iskljucivanje @c dinamickog alokatora
 * @details     Moguce vrednosti:
 *              - 0 - ne koristi se @c dinamicki alokator
 *              - 1 - koristi se @c dinamicki alokator
 *
 * @note        Podrazumevano podesavanje: 1 (koristi se @c dinamicki alokator)
 */
#if !defined(OPT_EVT_USE_MEM_DYN) || defined(__DOXYGEN__)
# define OPT_EVT_USE_MEM_DYN            1U
#endif

/** @} *//*-------------------------------------------------------------------*/
/*------------------------------------------------------------------------*//**
 * @name        Baza dogadjaja
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Ukljucivanje/iskljucivanje da li baza dogadjaja cuva podatke
 *              tipa znakovni niz
 * @details     U vecini slucajeva aplikacija ne zahteva da se cuvaju dodatni
 *              podaci o dogadjajima kao sto su imena, tip i opis dogadjaja.
 *              Ovom opcijom navedeni podaci se ukljucuju/iskljucuju.
 *
 *              Moguce vrednosti:
 *              - 0 - ne koriste se imena, tip i opis dogadjaja
 *              - 1 - koriste se imena, tip i opis dogadjaja
 *
 * @note        Podrazumevano podesavanje: 0 (ne koriste se imena, tip i opis
 *              dogadjaja)
 */
#if !defined(CFG_EVT_DB_USE_DESC_DATA)
# define CFG_EVT_DB_USE_DESC_DATA       1U
#endif

/** @} *//*-------------------------------------------------------------------*/
/*------------------------------------------------------------------------*//**
 * @name        Tipovi podataka u zaglavlju dogadjaja
 * @brief       Ovde su navedeni tipovi podataka koji se koriste u strukturi
 *              @ref esEvt_T
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Tip podataka za identifikator dogadjaja
 * @details     Identifikator dogadjaja je jedinstveni broj tipa dogadjaja u
 *              sistemu. Opcijom @ref OPT_EVT_ID_T se vrsi odabir koliko bajtova
 *              se dodeljuje promenljivi koja cuva pomenuti jedinstveni broj.
 *              Ovaj tip podataka se koristi u strukturi @ref esEvt_T i kao
 *              argument funkcije za kreiranje dogadjaja.
 */
typedef OPT_EVT_ID_T                    esEvtId_T;

/**
 * @brief       Tip podataka za vremenski marker dogadjaja
 * @details     Ovaj tip podataka treba da cuva informaciju o vremenskom markeru
 *              nastanka dogadjaja. eSolid moze da automatski generise vremenski
 *              marker nastanka dogadjaja, ali korisnik mora da odluci sta ce da
 *              uradi sa tom informacijom.
 * @see         CFG_EVT_TIMESTAMP_T
 * @see         OPT_EVT_TIMESTAMP_CALLBACK
 */
#if defined(CFG_EVT_USE_TIMESTAMP) || defined(__DOXYGEN__)
typedef CFG_EVT_TIMESTAMP_T             esTime_T;
#endif

/**
 * @brief       Tip podataka za atribut velicine dogadjaja
 * @details     Ovaj tip podataka cuva informaciju o velicini dogadjaja. Koristi
 *              se u strukturi @ref esEvt_T i od strane funkcija koje kreiraju
 *              dogadjaj.
 * @see         CFG_EVT_SIZE_T
 */
#if defined(OPT_EVT_USE_SIZE) || defined(__DOXYGEN__)
typedef CFG_EVT_SIZE_T                  esEvtSize_T;
#endif

/** @} *//*-------------------------------------------------------------------*/
/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/

#if (0U == OPT_EVT_USE_MEM_DYN) && (0U == OPT_EVT_USE_MEM_POOL)
# error "eSolid: EVT: enable at least one memory allocator. See options: OPT_EVT_USE_MEM_DYN and OPT_EVT_USE_MEM_POOL"
#endif

#if (1U == OPT_EVT_TIMESTAMP_CALLBACK) && (0U == CFG_EVT_USE_TIMESTAMP)
# error "eSolid: EVT: Option OPT_EVT_TIMESTAMP_CALLBACK is enabled while option OPT_EVT_USE_TIMESTAMP is disabled. Enable option OPT_EVT_TIMESTAMP_CALLBACK only if you use timestamp."
#endif

#if (1U == OPT_EVT_GENERATOR_CALLBACK) && (0U == CFG_EVT_USE_GENERATOR)
# error "eSolid: EVT: Option OPT_EVT_GENERATOR_CALLBACK is enabled while option OPT_EVT_USE_GENERATOR is disabled. Enable option OPT_EVT_GENERATOR_CALLBACK only if you use generator."
#endif

/** @endcond *//** @} *//******************************************************
 * END of evt_config.h
 ******************************************************************************/
#endif /* EVT_CONFIG_H_ */