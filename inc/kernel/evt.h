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
 * @brief       Interfejs Event (EVT) objekata
 * @addtogroup  evt_intf
 * @brief       Javni interfejs
 *********************************************************************//** @{ */

#ifndef EVT_H_
#define EVT_H_

/*=========================================================  INCLUDE FILES  ==*/
#include "hal/hal.h"
#include "../config/evt_config.h"

/*===============================================================  MACRO's  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Dinamicki attributi dogadjaja
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Bit maska za definisanje rezervisanog dogadjaja.
 * @details     Ukoliko je ovaj bit postavljen na jedinicu, dati dogadjaj je
 *              rezervisan i sistem ga nece razmatrati kao kandidata za brisanje.
 *              Brojac korisnika dogadjaja se i dalje azurira, tako da kada se
 *              dogadjaj oslobodii rezervacij))

/**
 * @brief       Bit maska za definisanje konstantnog dogadjaja.
 * @details     Ukoliko je ovaj bit postavljen na jedinicu, dati dogadjaj je
 *              konstantan (najčešće se nalazi u ROM memoriju) i ne moze biti
 *              izbrisan. Broj korisnika dogadjaja se ne azurira. Dati dogadjaj
 *              ne moze da postane dinamican.
 */
#define EVT_CONST_MASK                  ((uint_fast16_t)(1U << 15))

/** @} *//*-------------------------------------------------------------------*/
/*------------------------------------------------------------------------*//**
 * @name        Baza dogadjaja
 * @brief       Cuvanje podataka o svim dogadjajima u sistemu
 * @details     eSolid cuva podatke o svim dogadjajima u sistemu na jedinstvenom
 *              mestu. Neki podaci se koriste od strane eSolid-a, a veliki deo
 *              podataka se koriste za prikaz detalja o dogadjaju prilikom
 *              debagovanja sistema. Svi podaci su konstantni.
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Kreira tabelu enumeratora
 * @api
 */
#define ES_EXPAND_EVT_ID(a, b, c)       a,

/**
 * @brief       Kreira bazu dogadjaja
 * @api
 */
#define ES_EXPAND_EVT_DATA(a, b, c)     [a] = {sizeof(b), #a, #b, c},

/**
 * @brief       Kreira C tipove za dogadjaje
 * @api
 */
#define ES_EXPAND_EVT_TYPEDEF(a, b, c)  typedef b a##_T;


/** @} *//*-------------------------------------------------------------------*/
/*------------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/**
 * @brief       Zaglavlje dogadjaja.
 * @details     Interni podaci dogadjaja su obavezni podaci u zaglavlju
 *              dogadjaja. Svi ostali podaci se mogu ukljuciti/iskljuciti po
 *              zelji.
 * @p			Ova struktura nudi sledece opisivanje dogadjaja:
 *              - @c generator - ko je generisao dogadjaj,
 *              - @c timestamp - vremenska oznaka kada se dogadjaj desio,
 *              - @c size - velicina dogadjaja,
 * @p			Svaka od navedenih clanova strukture se moze nezavisno
 *              ukljuciti/iskljuciti i tipovi clanova se mogu po zelji izabrati.
 * @p			Dinamicki atributi pokazuju da li je dogadjaj konstantan (nalazi
 *              se u ROM memoriji), da li je rezervisan i koliko korisnika dati
 *              dogadjaj ima.
 * @note        Ukoliko se vrsi razmena dogadjaja izmedju sistema sa razlicitim
 *              procesorima/okruzenjem mora se pokloniti posebna paznja
 *              poravnanju (align) podataka ove strukture. Radi podesavanja
 *              nacina pakovanja strukture koristi se @ref OPT_EVT_STRUCT_ATTRIB
 *				opcija.
 * @api
 */
typedef struct OPT_EVT_STRUCT_ATTRIB esEvt {

#if (OPT_LOG_LEVEL <= LOG_DBG) || defined(__DOXYGEN__)
/**
 * @brief       Potpis koji pokazuje da je ovo zaista dogadjaj.
 */
    uint16_t        signature;
#endif

/**
 * @brief       Identifikator dogadjaja
 * @details     Podesavanje tipa se vrsi pomocu opcije @ref OPT_EVT_ID_T. Ovo
 *              polje je obavezno.
 */
    esEvtId_T      	id;

/**
 * @brief       Dinamicki atributi dogadjaja
 * @details     Dinamicki atributi dogadjaja pokazuju koliko korisnika ima dati
 *              dogadjaj i da li je rezervisan ili konstantan.
 * @see         EVT_RESERVED_MASK
 * @see         EVT_CONST_MASK
 */
    uint16_t      	attrib;

#if defined(OPT_EVT_USE_GENERATOR) || defined(__DOXYGEN__)
/**
 * @brief       Adresa generatora dogadjaja
 * @details     Ukljucivanje/iskljucivanje ovog podatka se vrsi opcijom
 *              @ref OPT_EVT_USE_GENERATOR.
 */
    void *         	generator;
#endif

#if defined(OPT_EVT_USE_TIMESTAMP) || defined(__DOXYGEN__)
/**
 * @brief       Vremenska oznaka kada se desio dogadjaj.
 * @details     Podesavanje tipa se vrsi pomocu: @ref OPT_EVT_TIMESTAMP_T.
 *              Ukljucivanje/iskljucivanje ovog podatka se vrsi opcijom
 *              @ref OPT_EVT_USE_TIMESTAMP.
 */
    esEvtTimestamp_T timestamp;
#endif

#if defined(OPT_EVT_USE_SIZE) || defined(__DOXYGEN__)
/**
 * @brief       Velicina podataka dogadjaja.
 * @details     Podesavanje tipa se vrsi pomocu: @ref OPT_EVT_SIZE_T.
 *              Ukljucivanje/iskljucivanje ovog podatka se vrsi opcijom
 *              @ref OPT_EVT_USE_SIZE.
 */
    esEvtSize_T     size;
#endif
} esEvt_T;

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Kreiranje/brisanje/rezervacija dogadjaja
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Kreira dogadjaj.
 * @param       id                      Identifikator dogadjaja.
 * @return      Pokazivac na memorijski prostor za podatke dogadjaja.
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
 * @api
 */
esEvt_T * esEvtCreate(
    esEvtId_T       id);

/**
 * @brief       Kreira dogadjaj.
 * @param       id                      Identifikator dogadjaja.
 * @return      Pokazivac na memorijski prostor za podatke dogadjaja.
 * @details     Kreira memorijski prostor koji je rezervisan za navedeni
 *              dogadjaj. U zaglavlje dogadjaja se upisuju podaci dogadjaja, a
 *              nakon toga se korisniku predaje pokazivac na memorijski prostor
 *              rezervisan za dogadjaje. Preko ovog pokazivaca korisnik dalje
 *              upisuje svoje podatke u dogadjaj.
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

/** @} *//*-------------------------------------------------------------------*/
/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of evt.h
 ******************************************************************************/
#endif /* EVT_H_ */
