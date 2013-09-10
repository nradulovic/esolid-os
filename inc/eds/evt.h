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
 * @author      Nenad Radulovic
 * @brief       Event object API
 * @addtogroup  evt_intf
 * @brief       Event object API
 *********************************************************************//** @{ */

#if !defined(EVT_H_)
#define EVT_H_

/*=========================================================  INCLUDE FILES  ==*/

#include "arch/compiler.h"
#include "arch/cpu.h"
#include "dbg/dbg.h"
#include "../config/sys_cfg.h"
#include "../config/evt_cfg.h"

/*===============================================================  DEFINES  ==*/

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

/** @} *//*-------------------------------------------------------------------*/

/*===============================================================  MACRO's  ==*/

/**@brief       Pomocni makro za kreiranje dogadjaja.
 * @param       type
 *              Tip dogadjaja koji se kreira.
 * @param       id
 *              Identifikator dogadjaja.
 * @details     Ovo je samo pomocni makro koji poziva funkciju esEvtCreate().
 *              Glavna namena je da se samo smanji obim kucanja u glavnoj
 *              aplikaciji.
 * @api
 */
#define ES_EVT_CREATE(type, id)                                                 \
    (type)esEvtCreate(sizeof(type), (esEvtId_T)id)

/*------------------------------------------------------  C++ extern begin  --*/
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
 * @note        Ukoliko se vrsi razmena dogadjaja izmedju sistema sa razlicitim
 *              procesorima/okruzenjem mora se pokloniti posebna paznja
 *              poravnanju (align) podataka ove strukture. Radi podesavanja
 *              nacina pakovanja strukture koristi se @ref CFG_EVT_STRUCT_ATTRIB
 *              opcija.
 * @api
 */
CFG_EVT_STRUCT_ATTRIB struct esEvt {
    esEvtId_T           id;                                                     /**<@brief Event ID, see @ref OPT_EVT_ID_T                  */
    uint16_t            attrib;                                                 /**<@brief Event dynamic attributes                         */

#if (1U == CFG_EVT_USE_TIMESTAMP) || defined(__DOXYGEN__)
    esTime_T            timestamp;                                              /**<@brief Event create time-stamp, see
                                                                                           @ref CFG_EVT_TIMESTAMP_T                         */
#endif
#if (1U == CFG_EVT_USE_GENERATOR) || defined(__DOXYGEN__)
    esEpa_T *           generator;                                              /**<@brief Event generator address                          */
#endif
#if (1U == OPT_EVT_USE_SIZE) || defined(__DOXYGEN__)
    esEvtSize_T         size;                                                   /**<@brief Event size in bytes, see @ref CFG_EVT_SIZE_T     */
#endif
#if (1U == CFG_DBG_API_VALIDATION) || defined(__DOXYGEN__)
    portReg_T           signature;                                              /**<@brief Structure signature, used during development only*/
#endif
};

/**@brief       Event header type
 * @api
 */
typedef struct esEvt esEvt_T;

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Kreiranje/brisanje dogadjaja
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Kreira dogadjaj.
 * @param       size
 *              Velicina potrebnog memorijskog prostora za podatke dogadjaja.
 * @param       id
 *              Identifikator dogadjaja.
 * @return      Pokazivac na memorijski prostor za podatke dogadjaja.
 * @details     Kreira memorijski prostor koji je rezervisan za navedeni
 *              dogadjaj. U zaglavlje dogadjaja se upisuju podaci dogadjaja, a
 *              nakon toga se korisniku predaje pokazivac na memorijski prostor
 *              rezervisan za dogadjaje. Preko ovog pokazivaca korisnik dalje
 *              upisuje svoje podatke u dogadjaj.
 * @note        Zaglavlje dogadjaja se smatra da je deo korisnickih podataka.
 *              Zbog toga size treba da nosi velicinu strukture zaglavlja
 *              esEvt + velicinu podataka koje dogadjaj obuhvata.
 *
 *              Primer:
 *              U datoj aplikaciji je konfigurisano da je struktura zaglavlja
 *              esEvt velicine 4B, a dogadjaj pri tom obuhvata i dodatni
 *              podatak unsigned long int cija je velicina, takodje, 4 bajta. U
 *              tom slucaju ovoj funkciji se kao prvi parametar predaje 8.
 * @api
 */
esEvt_T * esEvtCreate(
    size_t          size,
    esEvtId_T       id);

/**@brief       Kreira dogadjaj.
 * @param       size
 *              Velicina potrebnog memorijskog prostora za podatke dogadjaja.
 * @param       id
 *              Identifikator dogadjaja.
 * @return      Pokazivac na memorijski prostor za podatke dogadjaja.
 * @details     Kreira memorijski prostor koji je rezervisan za navedeni
 *              dogadjaj. U zaglavlje dogadjaja se upisuju podaci dogadjaja, a
 *              nakon toga se korisniku predaje pokazivac na memorijski prostor
 *              rezervisan za dogadjaje. Preko ovog pokazivaca korisnik dalje
 *              upisuje svoje podatke u dogadjaj.
 * @iclass
 */
esEvt_T * esEvtCreateI(
    size_t          size,
    esEvtId_T       id);

/**@brief       Unistava dogadjaj.
 * @param       evt
 *              Pokazivac na dogadjaj koji treba da se unisti.
 * @details     Ukoliko dati @c evt dogadjaj nema vise ni jednog korisnika,
 *              onda ce memorijski prostor koji on zauzima biti recikliran, u
 *              suprotnom, dogadjaj nastavlja da postoji.
 * @api
 */
void esEvtDestroy(
    esEvt_T *       evt);

/**@brief       Unistava dogadjaj.
 * @param       evt
 *              Pokazivac na dogadjaj koji treba da se unisti.
 * @details     Ukoliko dati @c evt dogadjaj nema vise ni jednog korisnika,
 *              onda ce memorijski prostor koji on zauzima biti recikliran, u
 *              suprotnom, dogadjaj nastavlja da postoji.
 * @iclass
 */
void esEvtDestroyI(
    esEvt_T *       evt);

/** @} *//*---------------------------------------------------------------*//**
 * @name        Rad sa staticnim dogadjajima
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Rezervise dogadjaj @c evt.
 * @param       evt
 *              Pokazivac na dogadjaj koji se rezervise.
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

/**@brief       Oslobadja prethodno rezervisan dogadjaj.
 * @param       evt
 *              Pokazivac na dogadjaj koji se oslobadja.
 * @api
 */
void esEvtUnReserve(
    esEvt_T *       evt);

/** @} *//*---------------------------------------------------------------*//**
 * @name        Korisnicke callback funkcije
 * @{ *//*--------------------------------------------------------------------*/

#if (1U == CFG_EVT_USE_TIMESTAMP) || defined(__DOXYGEN__)
/**@brief       Korisnicka callback funkcija: generise timestamp prilikom slanja
 *              dogadjaja.
 * @return      timestamp informacija koja se ugradjuje u dogadjaj.
 * @details     Poziva se prilikom slanja dogadjaja odredišnom EPA objektu (
 *              funkcije esEvtPost() i esEvtPostAhead()).
 * @see         @ref OPT_EVT_USE_TIMESTAMP
 * @see         @ref OPT_EVT_TIMESTAMP_T
 */
extern esTime_T uTimestampGet(
    void);
#endif

#if (1U == CFG_EVT_USE_GENERATOR) || defined(__DOXYGEN__)
/**@brief       Korisnicka callback funkcija: generise pokazivac na generatora
 *              dogadjaja.
 * @return      Pokazivac ka generatoru dogadjaja.
 * @details     Poziva se u toku kreiranja dogadjaja. Ukoliko je API kernel nivo
 *              jednak 2 (ukljucen je ceo kernel API) onda sistem sam generise
 *              pokazivac na dogadjaj i ova funkcija se ne poziva.
 * @see         @ref OPT_EVT_USE_GENERATOR
 */
extern esEpa_T * uGeneratorGet(
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
