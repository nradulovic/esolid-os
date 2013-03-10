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
 * @brief       Menadzmet dogadjaja
 * @details     This file is not meant to be included in application code
 *              independently but through the inclusion of "kernel.h" file.
 * @addtogroup  evt_intf
 *********************************************************************//** @{ */

#ifndef EVT_H_
#define EVT_H_

/*=========================================================  INCLUDE FILES  ==*/
#include "kernel/mm.h"

/*===============================================================  DEFINES  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Dinamicki attributi dogadjaja
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Bit maska za definisanje rezervisanog dogadjaja.
 * @details     Ukoliko je ovaj bit postavljen na jedinicu, dati dogadjaj je
 *              rezervisan i sistem ga nece razmatrati kao kandidata za brisanje.
 *              Brojac korisnika dogadjaja se i dalje azurira, tako da kada se
 *              dogadjaj oslobodii rezervacije on moze da bude obrisan ako nema
 *              korisnika.
 */
#define EVT_RESERVED_MASK                ((uint_fast8_t)(1U << 6))

/**
 * @brief       Bit maska za definisanje konstantnog dogadjaja.
 * @details     Ukoliko je ovaj bit postavljen na jedinicu, dati dogadjaj je
 *              konstantan (najčešće se nalazi u ROM memoriju) i ne moze biti
 *              izbrisan. Broj korisnika dogadjaja se ne azurira. Dati dogadjaj
 *              ne moze da postane dinamican.
 */
#define EVT_CONST_MASK                  ((uint_fast8_t)(1U << 7))

/** @} *//*-------------------------------------------------------------------*/
/*===============================================================  MACRO's  ==*/

/**
 * @brief       Pomocni makro za kreiranje dogadjaja.
 * @param       type                    Tip dogadjaja koji se kreira
 * @param       id                      identifikator dogadjaja.
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

/**
 * @brief       Zaglavlje dogadjaja.
 * @details     Interni podaci dogadjaja su obavezni podaci u zaglavlju
 *              dogadjaja. Svi ostali podaci se mogu ukljuciti/iskljuciti po
 *              zelji.
 *
 *              Ova struktura nudi sledece opisivanje dogadjaja:
 *              - @c generator - ko je generisao dogadjaj,
 *              - @c timestamp - vremenska oznaka kada se dogadjaj desio,
 *              - @c size - velicina dogadjaja,
 *
 *              Svaka od navedenih clanova strukture se moze nezavisno
 *              ukljuciti/iskljuciti i tipovi clanova se mogu po zelji izabrati.
 *
 *              Dinamicki atributi pokazuju da li je dogadjaj konstantan (nalazi
 *              se u ROM memoriji), da li je rezervisan i koliko korisnika dati
 *              dogadjaj ima.
 * @note        Ukoliko se vrsi razmena dogadjaja izmedju sistema sa razlicitim
 *              procesorima/okruzenjem mora se pokloniti posebna paznja
 *              poravnanju (align) podataka ove strukture. Radi podesavanja
 *              nacina pokovanja strukture koristi se @ref OPT_EVT_STRUCT_ATTRIB.
 * @api
 */
typedef struct OPT_EVT_STRUCT_ATTRIB esEvt {
/**
 * @brief       Identifikator dogadjaja
 * @details     Podesavanje tipa se vrsi pomocu: @ref OPT_EVT_ID_T.
 */
    esEvtId_T       id;

/**
 * @brief       Dinamicki atributi dogadjaja
 */
    uint_fast8_t    dynamic;

#if defined(OPT_KERNEL_DBG_EVT) && defined(OPT_DBG_USE_CHECK)                   \
    || defined(__DOXYGEN__)
/**
 * @brief       Potpis koji pokazuje da je ovo zaista dogadjaj.
 */
    uint32_t        signature;
#endif

#if defined(OPT_EVT_USE_GENERATOR) || defined(__DOXYGEN__)
/**
 * @brief       Adresa generatora dogadjaja
 * @details     Ukljucivanje/iskljucivanje ovog podatka se vrsi opcijom
 *              @ref OPT_EVT_USE_GENERATOR.
 */
    void *          generator;
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
 * @name        Kreiranje/brisanje dogadjaja
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Kreira dogadjaj.
 * @param       size                    Velicina potrebnog memorijskog prostora
 *                                      za podatke dogadjaja,
 * @param       id                      identifikator dogadjaja.
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

/**
 * @brief       Kreira dogadjaj.
 * @param       size                    Velicina potrebnog memorijskog prostora
 *                                      za podatke dogadjaja,
 * @param       id                      identifikator dogadjaja.
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

/**
 * @brief       Unistava dogadjaj.
 * @param       evt                     Pokazivac na dogadjaj koji treba da se
 *                                      unisti.
 * @details     Ukoliko dati @c evt dogadjaj nema vise ni jednog korisnika,
 *              onda ce memorijski prostor koji on zauzima biti recikliran, u
 *              suprotnom, dogadjaj nastavlja da postoji.
 * @api
 */
void esEvtDestroy(
    esEvt_T *       evt);

/**
 * @brief       Unistava dogadjaj.
 * @param       evt                     Pokazivac na dogadjaj koji treba da se
 *                                      unisti.
 * @details     Ukoliko dati @c evt dogadjaj nema vise ni jednog korisnika,
 *              onda ce memorijski prostor koji on zauzima biti recikliran, u
 *              suprotnom, dogadjaj nastavlja da postoji.
 * @iclass
 */
void esEvtDestroyI(
    esEvt_T *       evt);

/** @} *//*-------------------------------------------------------------------*/
/*------------------------------------------------------------------------*//**
 * @name        Rad sa staticnim dogadjajima
 * @{ *//*--------------------------------------------------------------------*/

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
/*------------------------------------------------------------------------*//**
 * @name        Korisnicke callback funkcije
 * @{ *//*--------------------------------------------------------------------*/

#if defined(OPT_EVT_USE_TIMESTAMP) || defined(__DOXYGEN__)
/**
 * @brief       Korisnicka callback funkcija: generise timestamp prilikom slanja
 *              dogadjaja.
 * @return      timestamp informacija koja se ugradjuje u dogadjaj.
 * @details     Poziva se prilikom slanja dogadjaja odredišnom EPA objektu (
 *              funkcije esEvtPost() i esEvtPostAhead()).
 * @see         @ref OPT_EVT_USE_TIMESTAMP
 * @see         @ref OPT_EVT_TIMESTAMP_T
 */
extern esEvtTimestamp_T uTimestampGet(
    void);
#endif

#if defined(OPT_EVT_USE_GENERATOR) && (OPT_KERNEL_API_LEVEL < 2)                \
    || defined(__DOXYGEN__)
/**
 * @brief       Korisnicka callback funkcija: generise pokazivac na generatora
 *              dogadjaja.
 * @return      Pokazivac ka generatoru dogadjaja.
 * @details     Poziva se u toku kreiranja dogadjaja. Ukoliko je API kernel nivo
 *              jednak 2 (ukljucen je ceo kernel API) onda sistem sam generise
 *              pokazivac na dogadjaj i ova funkcija se ne poziva.
 * @see         @ref OPT_EVT_USE_GENERATOR
 */
extern void * uGeneratorGet(
    void);
#endif

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
