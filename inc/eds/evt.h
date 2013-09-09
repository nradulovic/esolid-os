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
 * @author      Nenad Radulovic
 * @brief       Event object API
 * @addtogroup  evt_intf
 * @brief       Event object API
 *********************************************************************//** @{ */

#if !defined(EVT_H_)
#define EVT_H_

/*=========================================================  INCLUDE FILES  ==*/

#include "compiler.h"
#include "eds/dbg.h"
#include "../config/sys_config.h"
#include "../config/evt_config.h"

/*===============================================================  MACRO's  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Event object attributes
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Bit mask which defines a reserved event
 * @details     When the bits defined in this bit mask are set the given event
 *              is marked as reserved. In this case EDS will not try to delete
 *              the event even if there are no EPA units who are using it. When
 *              the event is unmarked as reserved EDS will take it into
 *              consideration to delete it. This bit mask can be used when
 *              defining a static event in ROM address space.
 * @api
 */
#define EVT_RESERVED_MASK               ((uint_fast16_t)(1U << 14))

/**@brief       Bit mask which defines a constant event
 * @details     When the bits defined in this bit mask are set the given event
 *              is marked as constant. In this case EDS will never try to delete
 *              it. Once the event is marked as constant it never can be deleted.
 * @api
 */
#define EVT_CONST_MASK                  ((uint_fast16_t)(1U << 15))

/** @} *//*---------------------------------------------------------------*//**
 * @name        Event database
 * @brief       Event data storage
 * @details     EDS will keep and manage all data about events in a given systen.
 *              Some of the data is valuable to the internal functions of EDS,
 *              but some data can be of use to the API user, too. All data kept
 *              in this database is constant (does not change during execution)
 *              and is stored in ROM address space.
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Create enum table
 * @api
 */
#define ES_EXPAND_EVT_ID(a, b, c, d)                                            \
    a,

/**@brief       Create event database
 * @see         esEvtDBElem_T
 * @api
 */
#if (1U == CFG_EVT_DB_USE_DESC_DATA) && (1U == OPT_EVT_USE_MEM_POOL)            /* Using descriptive data and pool memory                   */
# define ES_EXPAND_EVT_DATA(a, b, c, d)                                         \
    [a] = {sizeof(b), c, #a, #b, #d},
#elif (1U == CFG_EVT_DB_USE_DESC_DATA) && (0U == OPT_EVT_USE_MEM_POOL)          /* Using only descriptive data                              */
# define ES_EXPAND_EVT_DATA(a, b, c, d)                                         \
    [a] = {sizeof(b), #a, #b, #d},
#elif (0U == CFG_EVT_DB_USE_DESC_DATA) && (1U == OPT_EVT_USE_MEM_POOL)          /* Using only pool memory                                   */
# define ES_EXPAND_EVT_DATA(a, b, c, d)                                         \
    [a] = {sizeof(b), c},
#else                                                                           /* Not using descriptive data and pool memory               */
# define ES_EXPAND_EVT_DATA(a, b, c, d)                                         \
    [a] = {sizeof(b)},
#endif

/**@brief       Create C variable types
 * @api
 */
#define ES_EXPAND_EVT_TYPEDEF(a, b, c, d)                                       \
    typedef b a##_T;

/** @} *//*---------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Event transport
 * @brief       Event transport between EPA objects
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Abstract type of EPA object
 * @details     At this stage we do not know what an EPA object is (FSM, HSM or
 *              a thread). We are using this type just as a placeholder for now.
 */
typedef struct esEpa esEpa_T;

/** @} *//*---------------------------------------------------------------*//**
 * @name        Event object
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Event header structure
 * @details     This structure defines mandatory event data. Other data fields
 *              can be enabled/disabled using configuration provided in
 *              evt_config.h file.
 * @note        In use
 * @note        Ukoliko se vrsi razmena dogadjaja izmedju sistema sa razlicitim
 *              procesorima/okruzenjem mora se pokloniti posebna paznja
 *              poravnanju (align) podataka ove strukture. Radi podesavanja
 *              nacina pakovanja strukture koristi se @ref CFG_EVT_STRUCT_ATTRIB
 *				opcija.
 * @api
 */
struct CFG_EVT_STRUCT_ATTRIB esEvt {
    esEvtId_T      	    id;                                                     /**<@brief Event ID, see @ref OPT_EVT_ID_T                  */
    uint16_t      	    attrib;                                                 /**<@brief Event dynamic attributes                         */

#if (1U == CFG_EVT_USE_TIMESTAMP) || defined(__DOXYGEN__)
    esEvtTimestamp_T    timestamp;                                              /**<@brief Event create time-stamp, see
                                                                                           @ref CFG_EVT_TIMESTAMP_T                         */
#endif
#if (1U == CFG_EVT_USE_GENERATOR) || defined(__DOXYGEN__)
    esEpa_T *           generator;                                              /**<@brief Event generator address                          */
#endif
#if (1U == OPT_EVT_USE_SIZE) || defined(__DOXYGEN__)
    esEvtSize_T         size;                                                   /**<@brief Event size in bytes, see @ref CFG_EVT_SIZE_T     */
#endif
#if (1U == CFG_DBG_API_VALIDATION)
    portReg_T           signature;                                              /**<@brief Structure signature, used during development only*/
#endif
};

/**@brief       Event header type
 * @api
 */
typedef struct esEvt esEvt_T;

/** @} *//*---------------------------------------------------------------*//**
 * @name        Event database
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Database element structure
 * @api
 */
struct esEvtDBElem {
    size_t              size;                                                   /**<@brief Event size in bytes                              */

#if (1U == OPT_EVT_USE_MEM_POOL) || defined(__DOXYGEN__)
    OPT_MEM_POOL_T *    pool;                                                   /**<@brief Pool memory handle                               */
#endif
#if (1U == CFG_EVT_DB_USE_DESC_DATA) || defined(__DOXYGEN__)
    const PORT_C_ROM char * name;                                               /**<@brief Event name                                       */
    const PORT_C_ROM char * type;                                               /**<@brief Event type                                       */
    const PORT_C_ROM char * desc;                                               /**<@brief Short event description                          */
#endif
};

/**@brief       Database element type
 * @api
 */
typedef struct esEvtDBElem esEvtDBElem_T;

/** @} *//*-------------------------------------------------------------------*/

/*======================================================  GLOBAL VARIABLES  ==*/

extern OPT_MEM_DYN_T gEvtDynStorage;

/*===================================================  FUNCTION PROTOTYPES  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Event database
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Register a database
 * @param       evtDB
 *              Event database : pointer to formed database table.
 * @param       elements
 *              Elements : number of elements in database.
 * @details     Before any use a database must be registered to system. Only one
 *              database can be registerd at the time. Database must be stored
 *              in ROM address space.
 * @api
 */
void esEvtDBRegister(
    const PORT_C_ROM esEvtDBElem_T * evtDB[],
    uint16_t            elements);

/**
 * @brief       Dobavlja informaciju o velicini dogadjaja iz baze dogadjaja
 * @param       id                      Identifikator dogadjaja
 * @return      Velicinu trazenog dogadjaja u bajtovima.
 * @pre         Baza dogadjaja pre poziva ove funkcije mora da bude registrovana
 *              funkcijom esEvtDBRegister()
 * @api
 */
size_t esEvtDBQuerySize(
    esEvtId_T           id);

/**
 * @brief       Dobavlja informaciju o imenu dogadjaja iz baze dogadjaja
 * @param       id                      Identifikator dogadjaja
 * @return      Znakovni niz imena dogadjaja
 * @pre         Baza dogadjaja pre poziva ove funkcije mora da bude registrovana
 *              funkcijom esEvtDBRegister()
 * @api
 */
const PORT_C_ROM char * esEvtDBQueryName(
    esEvtId_T           id);

/**
 * @brief       Dobavlja informaciju o tipu dogadjaja iz baze dogadjaja
 * @param       id                      Identifikator dogadjaja
 * @return      Znakovni niz tipa dogadjaja
 * @pre         Baza dogadjaja pre poziva ove funkcije mora da bude registrovana
 *              funkcijom esEvtDBRegister()
 * @api
 */
const PORT_C_ROM char * esEvtDBQueryType(
    esEvtId_T           id);

/**
 * @brief       Dobavlja informaciju o opisu dogadjaja iz baze dogadjaja
 * @param       id                      Identifikator dogadjaja
 * @return      Znakovni niz kratkog opisa dogadjaja
 * @pre         Baza dogadjaja pre poziva ove funkcije mora da bude registrovana
 *              funkcijom esEvtDBRegister()
 * @api
 */
const PORT_C_ROM char * esEvtDBQueryDesc(
    esEvtId_T       id);

/** @} *//*---------------------------------------------------------------*//**
 * @name        Kreiranje/brisanje/rezervacija dogadjaja
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Kreira dogadjaj.
 * @param       id                      Identifikator dogadjaja.
 * @return      Pokazivac na kreiranu instancu dogadjaja. Dogadjaj se preko ovog
 *              pokazivaca popunjava parametrima koje on nosi.
 * @details     Kreira memorijski prostor koji je rezervisan za navedeni
 *              dogadjaj. U zaglavlje dogadjaja se upisuju podaci dogadjaja, a
 *              nakon toga se korisniku predaje pokazivac na memorijski prostor
 *              rezervisan za dogadjaje. Preko ovog pokazivaca korisnik dalje
 *              upisuje svoje podatke u dogadjaj.
 * @note        Zaglavlje dogadjaja se smatra da je deo korisnickih podataka.
 *              Zbog toga size treba da nosi velicinu strukture zaglavlja
 *              esEvt + velicinu podataka koje dogadjaj obuhvata.
 * @p			Primer:
 *              U datoj aplikaciji je konfigurisano da je struktura zaglavlja
 *              esEvt velicine 4B, a dogadjaj pri tom obuhvata i dodatni
 *              podatak unsigned long int cija je velicina, takodje, 4 bajta. U
 *              tom slucaju ovoj funkciji se kao prvi parametar predaje 8.
 * @pre         Baza dogadjaja pre poziva ove funkcije mora da bude registrovana
 *              funkcijom esEvtDBRegister()
 * @api
 */
esEvt_T * esEvtCreate(
    esEvtId_T       id);

/**
 * @brief       Kreira dogadjaj.
 * @param       id                      Identifikator dogadjaja.
 * @return      Pokazivac na kreiranu instancu dogadjaja. Dogadjaj se preko ovog
 *              pokazivaca popunjava parametrima koje on nosi.
 * @details     Kreira memorijski prostor koji je rezervisan za navedeni
 *              dogadjaj. U zaglavlje dogadjaja se upisuju podaci dogadjaja, a
 *              nakon toga se korisniku predaje pokazivac na memorijski prostor
 *              rezervisan za dogadjaje. Preko ovog pokazivaca korisnik dalje
 *              upisuje svoje podatke u dogadjaj.
 * @pre         Baza dogadjaja pre poziva ove funkcije mora da bude registrovana
 *              funkcijom esEvtDBRegister()
 * @iclass
 */
esEvt_T * esEvtCreateI(
    esEvtId_T       id);

/**
 * @brief       Unistava dogadjaj.
 * @param       evt                     Pokazivac na dogadjaj koji treba da se
 *                                      unisti.
 * @details     Dogadjaj ce biti unisten ukoliko dati @c evt dogadjaj nema vise
 *              ni jednog korisnika, u suprotnom, dogadjaj nastavlja da postoji.
 * @api
 */
void esEvtDestroy(
    esEvt_T *       evt);

/**
 * @brief       Unistava dogadjaj.
 * @param       evt                     Pokazivac na dogadjaj koji treba da se
 *                                      unisti.
 * @details     Dogadjaj ce biti unisten ukoliko dati @c evt dogadjaj nema vise
 *              ni jednog korisnika, u suprotnom, dogadjaj nastavlja da postoji.
 * @iclass
 */
void esEvtDestroyI(
    esEvt_T *       evt);

/**
 * @brief       Rezervise dogadjaj @c evt.
 * @param       evt                     Pokazivac na dogadjaj koji se rezervise.
 * @pre         Dogadjaj mora da bude kreiran funkcijom esEvtCreate().
 * @details     Rezervise @c evt dogadjaj cime se onemogucava eSolid-u da
 *              izvrsi recikliranje memorijskog prostora ovog dogadjaja.
 *              Uglavnom se rezervacija dogadjaja vrsi kada je potrebno iz neke
 *              prekidne rutine brzo poslati dogadjaj nekom aktivnom EPA objektu.
 *              Prethodnim kreiranjem dogadjaja i njegovom rezervacijom se vrsi
 *              alociranje memorije, tako da u kriticnim trenucima nije potrebno
 *              ponovo izvrsiti alociranje memorije, vec se spreman dogadjaj
 *              odmah koristi.
 * @api
 */
void esEvtReserve(
    esEvt_T *       evt);

/**
 * @brief       Oslobadja prethodno rezervisan dogadjaj.
 * @param       evt                     Pokazivac na dogadjaj koji se oslobadja.
 * @api
 */
void esEvtUnReserve(
    esEvt_T *       evt);

/** @} *//*---------------------------------------------------------------*//**
 * @name        Transport dogadjaja
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Salje dogadjaj na kraju reda za cekanje (FIFO metod).
 * @param       epa                     Pokazivac na EPA objekat kome se salje,
 * @param       evt                     pokazivac na dogadjaj koji se salje.
 * @details     Prihvata prethodno kreiran dogadjaj funkcijom esEvtCreate() i
 *              postavlja ga u red cekanja datog EPA objekta.
 * @api
 */
void esEvtPost(
    esEpa_T *       epa,
    esEvt_T *       evt);

/**
 * @brief       Salje dogadjaj na kraju reda za cekanje (FIFO metod).
 * @param       epa                     Pokazivac na EPA objekat kome se salje,
 * @param       evt                     pokazivac na dogadjaj koji se salje.
 * @details     Prihvata prethodno kreiran dogadjaj funkcijom esEvtCreate() i
 *              postavlja ga u red cekanja datog EPA objekta.
 * @iclass
 */
void esEvtPostI(
    esEpa_T *       epa,
    esEvt_T *       evt);

/**
 * @brief       Salje dogadjaj na pocetku reda za cekanje (LIFO metod).
 * @param       epa                     Pokazivac na EPA objekat kome se salje,
 * @param       evt                     pokazivac na dogadjaj koji se salje.
 * @details     Prihvata prethodno kreiran dogadjaj funkcijom esEvtCreate() i
 *              postavlja ga u red cekanja datog EPA objekta. Za razliku od
 *              esEvtPost() funkcije dogadjaj se postavlja na pocetku reda za
 *              cekanje. Najcesce se koristi kada je potrebno da se EPA objektu
 *              hitno posalje neki dogadjaj od znacaja koji treba da obradi.
 * @api
 */
void esEvtPostAhead(
    esEpa_T *       epa,
    esEvt_T *       evt);

/**
 * @brief       Salje dogadjaj na pocetku reda za cekanje (LIFO metod).
 * @param       epa                     Pokazivac na EPA objekat kome se salje,
 * @param       evt                     pokazivac na dogadjaj koji se salje.
 * @details     Prihvata prethodno kreiran dogadjaj funkcijom esEvtCreate() i
 *              postavlja ga u red cekanja datog EPA objekta. Za razliku od
 *              esEvtPost() funkcije dogadjaj se postavlja na pocetku reda za
 *              cekanje. Najcesce se koristi kada je potrebno da se EPA objektu
 *              hitno posalje neki dogadjaj od znacaja koji treba da obradi.
 * @iclass
 */
void esEvtPostAheadI(
    esEpa_T *       epa,
    esEvt_T *       evt);

/** @} *//*---------------------------------------------------------------*//**
 * @name        Callback funkcije
 * @brief       Funkcije koje se pozivaju od strane EVT modula u odredjenim
 *              trenucima izvrsenja
 * @details     Ovde se nalaze sve callback funkcije koje EVT modul poziva. Njih
 *              treba da definise korisnik prema svojim potrebama.
 * @{ *//*--------------------------------------------------------------------*/

#if (1U == CFG_EVT_USE_GENERATOR) && (1U == OPT_EVT_GENERATOR_CALLBACK) ||      \
    defined(__DOXYGEN__)
/**
 * @brief       Dobavlja ko kreira dogadjaj
 * @return      Pokazivac na objekat koji kreira dogadjaj
 * @details     Funkcija se poziva u toku kreiranja dogadjaja. Ona treba da
 *              vrati pokazivac na objekat koji upravo kreira dogadjaj.
 * @pre         Funkcija se poziva samo ako su ukljucene opcije:
 *              - @ref CFG_EVT_USE_GENERATOR
 *              - @ref OPT_EVT_GENERATOR_CALLBACK
 */
extern esEpa_T * appEvtGeneratorGet(
    void);
#endif

#if (1U == CFG_EVT_USE_TIMESTAMP) && (1U == OPT_EVT_TIMESTAMP_CALLBACK) ||      \
    defined(__DOXYGEN__)
/**
 * @brief       Dobavlja trenutni vremenski aspekt
 * @return      Vremenski aspekt koji postaje vremenski marker dogadjaja
 * @details     Funkcija se poziva u toku kreiranja dogadjaja. Ona treba da
 *              vrati trenutnu vrednost vremenskog aspekta koja se postavlja u
 *              dogadjaj i postaje vremenski marker kreiranja dogadjaja.
 *              Tipicna primena ove funkcije bi bila da vrati trenutnu vrednost
 *              nekog tajmera koji je stalno aktivan, na primer, @c systick.
 * @pre         Funkcija se poziva samo ako su ukljucene opcije:
 *              - @ref CFG_EVT_USE_TIMESTAMP
 *              - @ref OPT_EVT_TIMESTAMP_CALLBACK
 */
extern esEvtTimestamp_T appEvtTimestampGet(
    void);
#endif

/** @} *//*-----------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of evt.h
 ******************************************************************************/
#endif /* EVT_H_ */
