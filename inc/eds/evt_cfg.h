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
 * @addtogroup  evt
 *********************************************************************//** @{ */
/**@defgroup    evt_cfg Event Object Configuration
 * @brief       Konfiguracija
 * @details     Each configuration option or setting has its own default value
 *              when not defined by the application. When application needs to
 *              change a setting it just needs to define a configuration macro
 *              with another value and the default configuration macro will be
 *              overridden.
 * @{ *//*--------------------------------------------------------------------*/

#ifndef EVT_CFG_H__
#define EVT_CFG_H__

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
 *              Podesavanje tipa se vrsi pomocu @ref CFG_EVT_SIZE_TYPE.
 * @note        Podrazumevano podesavanje: 0 (dogadjaji ne koriste atribut o
 *              velicini)
 */
#if !defined(CFG_EVT_USE_SIZE)
# define CFG_EVT_USE_SIZE               1
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
#if !defined(OPT_EVT_USE_GENERATOR)
# define CFG_EVT_USE_GENERATOR          1
#endif

/**
 * @brief       Koristi se vremenski marker dogadjaja.
 * @details     Moguce vrednosti:
 *              - 0 - vrem. marker se ne koristi
 *              - 1 - vrem. marker se koristi
 *
 *              Podesavanje tipa se vrsi pomocu @ref CFG_EVT_TIMESTAMP_TYPE.
 * @note        Podrazumevano podesavanje: 0 (dogadjaji ne koriste atribut o
 *              vremenskom markeru)
 */
#if !defined(OPT_EVT_USE_TIMESTAMP)
# define CFG_EVT_USE_TIMESTAMP          1
#endif

/**
 * @brief       Specification of event identifier type size
 * @details     Tip podatka treba da dovoljne velicine moze da predstavi sve
 *              dogadjaje u sistemu. Na primer, ako u sistemu postoje manje od
 *              255 identifikatora dogadjaja onda ovaj tip moze najmanje biti
 *              uint8_t tipa. Ukoliko sistem poseduje vise od 255 razlicitih
 *              dogadjaja onda ovaj tip treba da bude najmanje uint16_t. Naravno,
 *              treba voditi racuna o zauzecu strukture koja opisuje dogadjaj i
 *              njenom poravnjanju (alignement) u memoriji, gde se moze javiti
 *              potreba za vecim tipom od minimalnog.
 *              Tipovi:
 *              - 0 - 8 bit identifier
 *              - 1 - 16 bit identifier
 *              - 2 - 32 bit identifier
 * @note        Default: 2 (32 bit identifier)
 */
#if !defined(CFG_EVT_ID_TYPE)
# define CFG_EVT_ID_TYPE                2
#endif

/**
 * @brief       Tip podataka za cuvanje velicine prosirenog dogadjaja.
 * @details     - 0 - use standard `size_t` type variable
 *              - 1 - force 8 bit size variable
 *              - 2 - force 16 bit size variable
 *              - 3 - force 32 bit size variable
 * @note        Default: 0 (size_t)
 * @note        Ovo podesavanje ima znacaja samo ako je ukljucena opcija
 *              @ref CFG_EVT_USE_SIZE.
 *
 */
#if !defined(CFG_EVT_SIZE_TYPE)
# define CFG_EVT_SIZE_TYPE              0
#endif

/**
 * @brief       Tip za cuvanje vremenskih atributa dogadjaja.
 * @details     - 0 - use 8 bit timestamp variable
 *              - 1 - use 16 bit timestamp variable
 *              - 2 - use 32 bit timestamp variable
 * @note        Default: 2 (32 bit timestamp)
 * @note        Ovo podesavanje ima znacaja samo ako je ukljucena opcija
 *              @ref CFG_EVT_USE_TIMESTAMP
 */
#if !defined(CFG_EVT_TIMESTAMP_TYPE)
# define CFG_EVT_TIMESTAMP_TYPE         2
#endif

/**@} *//*----------------------------------------------------------------*//**
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
#if !defined(OPT_EVT_GENERATOR_CALLBACK)
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
#if !defined(OPT_EVT_TIMESTAMP_CALLBACK)
# define OPT_EVT_TIMESTAMP_CALLBACK     1U
#endif

/**@} *//*----------------------------------------------------------------*//**
 * @name        Tipovi podataka u zaglavlju dogadjaja
 * @brief       Ovde su navedeni tipovi podataka koji se koriste u strukturi
 *              @ref esEvt_T
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Tip podataka za identifikator dogadjaja
 * @details     Identifikator dogadjaja je jedinstveni broj tipa dogadjaja u
 *              sistemu. Opcijom @ref CFG_EVT_ID_TYPE se vrsi odabir koliko bajtova
 *              se dodeljuje promenljivi koja cuva pomenuti jedinstveni broj.
 *              Ovaj tip podataka se koristi u strukturi @ref esEvt_T i kao
 *              argument funkcije za kreiranje dogadjaja.
 */
#if   (0 == CFG_EVT_ID_TYPE)
typedef uint8_t                         esEvtId_T;
#elif (1 == CFG_EVT_ID_TYPE)
typedef uint16_t                        esEvtId_T;
#elif (2 == CFG_EVT_ID_TYPE)
typedef uint32_t                        esEvtId_T;
#endif

/**
 * @brief       Tip podataka za atribut velicine dogadjaja
 * @details     Ovaj tip podataka cuva informaciju o velicini dogadjaja. Koristi
 *              se u strukturi @ref esEvt_T i od strane funkcija koje kreiraju
 *              dogadjaj.
 * @see         CFG_EVT_SIZE_TYPE
 */
#if (1 == CFG_EVT_USE_SIZE)
# if   (0 == CFG_EVT_SIZE_TYPE)
typedef size_t                          esEvtSize_T;
# elif (1 == CFG_EVT_SIZE_TYPE)
typedef uint8_t                         esEvtSize_T;
# elif (2 == CFG_EVT_SIZE_TYPE)
typedef uint16_t                        esEvtSize_T;
# elif (3 == CFG_EVT_SIZE_TYPE)
typedef uint32_t                        esEvtSize_T;
# endif
#endif

/**
 * @brief       Tip podataka za vremenski marker dogadjaja
 * @details     Ovaj tip podataka treba da cuva informaciju o vremenskom markeru
 *              nastanka dogadjaja. eSolid moze da automatski generise vremenski
 *              marker nastanka dogadjaja, ali korisnik mora da odluci sta ce da
 *              uradi sa tom informacijom.
 * @see         CFG_EVT_TIMESTAMP_TYPE
 * @see         OPT_EVT_TIMESTAMP_CALLBACK
 */
#if (1 == CFG_EVT_USE_TIMESTAMP)
# if (0 == CFG_EVT_TIMESTAMP_TYPE)
typedef uint8_t                         esEvtTime_T;
# elif (1 == CFG_EVT_TIMESTAMP_TYPE)
typedef uint16_t                        esEvtTime_T;
# elif (2 == CFG_EVT_TIMESTAMP_TYPE)
typedef uint32_t                        esEvtTime_T;
# endif
#endif

/**@} *//*----------------------------------------------------------------*//**
 * @name        Event storage settings
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Specifies the event storage method
 * @details     eSolid provides several options for memory management like
 *              memory pools and dynamic memory management.
 *              - 0 - use pools and dynamic memory
 *              - 1 - use only pool memory
 *              - 2 - use only dynamic memory
 *              - 3 - use external dynamic memory
 *
 *              When option `0` is chosen eSolid will try to use pools for
 *              event storage. If an event can not be allocated in event pools
 *              it will call dynamic memory allocators.
 *              Default: 0 (use pools and dynamic memory)
 */
#if !defined(CFG_EVT_STORAGE)
# define CFG_EVT_STORAGE                1
#endif

/**@brief       Specifies the number of event pool used
 * @details     When pool memory is being used for storage this setting can be
 *              used the maximum number of different pools being used.
 */
#if !defined(CFG_EVT_STORAGE_NPOOL)
# define CFG_EVT_STORAGE_NPOOL          4
#endif

/**@} *//*--------------------------------------------------------------------*/

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/

#if (2 < CFG_EVT_ID_TYPE)
# error "eSolid: EVT: Event ID type is not properly set"
#endif

#if (3 < CFG_EVT_SIZE_TYPE) && (1 == CFG_EVT_USE_SIZE)
# error "eSolid: EVT: Event size type is not properly set"
#endif

#if (2 < CFG_EVT_TIMESTAMP_TYPE) && (1 == CFG_EVT_USE_TIMESTAMP),
# error "eSolid: EVT: Event timestamp type is not properly set"
#endif

#if (1 == OPT_EVT_TIMESTAMP_CALLBACK) && (0 == CFG_EVT_USE_TIMESTAMP)
# error "eSolid: EVT: Option OPT_EVT_TIMESTAMP_CALLBACK is enabled while option OPT_EVT_USE_TIMESTAMP is disabled. Enable option OPT_EVT_TIMESTAMP_CALLBACK only if you use timestamp."
#endif

#if (1 == OPT_EVT_GENERATOR_CALLBACK) && (0 == CFG_EVT_USE_GENERATOR)
# error "eSolid: EVT: Option OPT_EVT_GENERATOR_CALLBACK is enabled while option OPT_EVT_USE_GENERATOR is disabled. Enable option OPT_EVT_GENERATOR_CALLBACK only if you use generator."
#endif

/** @endcond *//** @} *//******************************************************
 * END of evt_cfg.h
 *//** @} *//******************************************************************/
#endif /* EVT_CFG_H__ */
