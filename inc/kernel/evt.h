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
 * @author      Nenad Radulovic
 * @brief       Menadzmet dogadjaja
 * @details     This file is not meant to be included in application code
 *              independently but through the inclusion of "kernel.h" file.
 * @addtogroup  evt_intf
 ****************************************************************************************//** @{ */


#ifndef EVT_H_
#define EVT_H_

/*============================================================================  INCLUDE FILES  ==*/
#include "primitive/queue.h"

/*==================================================================================  DEFINES  ==*/

/*-------------------------------------------------------------------------------------------*//**
 * @name        Dinamicki attributi dogadjaja
 * @{ *//*---------------------------------------------------------------------------------------*/

/**
 * @brief       Bit maska za definisanje rezervisanog dogadjaja.
 * @details     Ukoliko je ovaj bit postavljen na jedinicu, dati dogadjaj je
 *              rezervisan i sistem ga nece razmatrati kao kandidata za brisanje.
 *              Brojac korisnika dogadjaja se i dalje azurira, tako da kada se
 *              dogadjaj oslobodii rezervacije on moze da bude obrisan ako nema
 *              korisnika.
 */
#define EVT_RESERVED_MASK               ((uint_fast8_t)(1U << 6))

/**
 * @brief       Bit maska za definisanje konstantnog dogadjaja.
 * @details     Ukoliko je ovaj bit postavljen na jedinicu, dati dogadjaj je
 *              konstantan (najčešće se nalazi u ROM memoriju) i ne moze biti
 *              izbrisan. Broj korisnika dogadjaja se ne azurira. Dati dogadjaj
 *              ne moze da postane dinamican.
 */
#define EVT_CONST_MASK                  ((uint_fast8_t)(1U << 7))

/**
 * @brief       Bit maska za brojac korisnika dogadjaja
 * @details     Brojac korisnika je 6-bitni, što znači da maksimalan broj
 *              korisnika dogadjaja u jednom trenutku iznosi 63 EPA objekata.
 */
#define EVT_USERS_MASK                  ((uint_fast8_t)0x3F)

/** @} *//*--------------------------------------------------------------------------------------*/

/**
 * @brief       Identifikatori predefinisanih dogadjaja
 * @details     Zadnji predefinisan identifikator je @ref SIG_ID_USR. Nakon ovog
 *              identifikatora korisnik definise svoje, aplikacione
 *              identifikatore dogadjaja.
 */
enum esEvtID {
/**
 * @brief       Signalni dogadjaj - prazan signal.
 * @note        Ne koristi se.
 */
    SIG_EMPTY,

/**
 * @brief       Signalni dogadjaj - zahteva se entry obrada u datom stanju.
 */
    SIG_ENTRY,

/**
 * @brief       Signalni dogadjaj - zahteva se exit obrada u datom stanju.
 */
    SIG_EXIT,

/**
 * @brief       Signalni dogadjaj - zahteva se inicijalizaciona (init) putanja.
 */
    SIG_INIT,

/**
 * @brief       Signalni dogadjaj - zahteva se super stanje.
 * @details     Od funkcije stanja (u aplikaciji) se zahteva koje je njeno
 *              super stanje. Funkcija stanja mora da vrati pokazivac na njeno
 *              super stanje.
 */
    SIG_SUPER,

/**
 * @brief       Domen korisnickih identifikatora dogadjaja.
 * @details     Nakon ovog identifikatora korisnik definise svoje, aplikacione
 *              identifikatore dogadjaja.
 */
    SIG_ID_USR = 15
};

/*==================================================================================  MACRO's  ==*/

/**
 * @brief       Pomocni makro za kreiranje dogadjaja.
 * @param       type                    Tip dogadjaja koji se kreira
 * @param       id                      identifikator dogadjaja.
 * @details     Ovo je samo pomocni makro koji poziva funkciju esEvtCreate().
 *              Glavna namena je da se samo smanji obim kucanja u glavnoj
 *              aplikaciji.
 */
#define ES_EVT_CREATE(type, id)                                                 \
    (type)esEvtCreate(sizeof(type), (esEvtId_T)id)

/*-------------------------------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*===============================================================================  DATA TYPES  ==*/

/**
 * @brief       Red cekanja za dogadjaje.
 * @details     Ova struktura opisuje redove cekanja za dogadjaje koji se
 *              koriste za aktivne objekte. Red cekanja zajedno sa
 *              procesorom konacnih automata (SMP) cini jedan agent za obradu
 *              dogadjaja, (Event Processing Agent - EPA).
 *
 *              Struktura sadrzi samo podatke za upravljanje reda cekanja i ne
 *              sadrzi memorijski prostor za cuvanje dogadjaja. Memorijski
 *              prostor se mora rezervisati unapred i predati pokazivac na taj
 *              prostor EVT-u prilikom inicijalizacije reda cekanja.
 *
 *              U redovima cekanja se cuvaju samo pokazivaci ka dogadjajima, a
 *              ne i same instance dogadjaja.
 *
 *              Pored navedenog reda za cekanje, struktura moze da sadrzi
 *              brojace zauzeca reda za cekanje. @ref freeCurr pokazuje trenutni
 *              broj praznih lokacija dok @ref freeMin sadrzi najmanji broj
 *              slobodnih lokacija ikada.
 * @notapi
 */
struct evtQueue {
/**
 * @brief       Instanca reda za cekanje opste namene
 */
    esQueuePtr_T    queue;

#if defined(OPT_KERNEL_DBG_EVT) || defined(__DOXYGEN__)
/**
 * @brief       Trenutni broj slobodnih lokacija u redu za cekanje
 */
    uint_fast8_t    freeCurr;

/**
 * @brief       Najmanji broj slobodnih lokacija u redu za cekanje
 */
    uint_fast8_t    freeMin;
#endif
};

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
struct OPT_EVT_STRUCT_ATTRIB esEvtHeader {
/**
 * @brief       Identifikator dogadjaja
 * @details     Podesavanje tipa se vrsi pomocu: @ref OPT_EVT_ID_T.
 */
    esEvtId_T       id;

/**
 * @brief       Dinamicki atributi dogadjaja
 */
    uint_fast8_t    dynamic;

#if defined(OPT_KERNEL_DBG_EVT) && defined(OPT_DBG_USE_CHECK) || defined(__DOXYGEN__)
/**
 * @brief       Potpis koji pokazuje da je ovo zaista dogadjaj.
 */
    uint_fast32_t   signature;
#endif

#if defined(OPT_EVT_USE_GENERATOR) || defined(__DOXYGEN__)
/**
 * @brief       Adresa generatora dogadjaja
 * @details     Ukljucivanje/iskljucivanje ovog podatka se vrsi opcijom
 *              @ref OPT_EVT_USE_GENERATOR.
 */
    esEpaHeader_T   * generator;
#endif

#if defined(OPT_EVT_USE_TIMESTAMP) || defined(__DOXYGEN__)
/**
 * @brief       Vremenska oznaka kada se desio dogadjaj.
 * @details     Podesavanje tipa se vrsi pomocu: @ref OPT_EVT_TIMESTAMP_T.
 *              Ukljucivanje/iskljucivanje ovog podatka se vrsi opcijom
 *              @ref OPT_EVT_USE_TIMESTAMP.
 */
    esEvtTimestamp_T time;
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
};

/*=========================================================================  GLOBAL VARIABLES  ==*/
/*======================================================================  FUNCTION PROTOTYPES  ==*/

/*-------------------------------------------------------------------------------------------*//**
 * @name        Kreiranje/brisanje dogadjaja
 * @{ *//*---------------------------------------------------------------------------------------*/

/**
 * @brief       Kreira dogadjaj.
 * @param       dataSize                Velicina potrebnog memorijskog prostora
 *                                      za podatke dogadjaja,
 * @param       id                      identifikator dogadjaja.
 * @return      Pokazivac na memorijski prostor za podatke dogadjaja.
 * @details     Kreira memorijski prostor koji je rezervisan za navedeni
 *              dogadjaj. U zaglavlje dogadjaja se upisuju podaci dogadjaja, a
 *              nakon toga se korisniku predaje pokazivac na memorijski prostor
 *              rezervisan za dogadjaje. Preko ovog pokazivaca korisnik dalje
 *              upisuje svoje podatke u dogadjaj.
 * @note        Zaglavlje dogadjaja se smatra da je deo korisnickih podataka.
 *              Zbog toga dataSize treba da nosi velicinu strukture zaglavlja
 *              esEvtHeader + velicinu podataka koje dogadjaj obuhvata.
 *
 *              Primer:
 *              U datoj aplikaciji je konfigurisano da je struktura zaglavlja
 *              esEvtHeader velicine 4B, a dogadjaj pri tom obuhvata i dodatni
 *              podatak unsigned long int cija je velicina, takodje, 4 bajta. U
 *              tom slucaju ovoj funkciji se kao prvi parametar predaje 8.
 * @api
 */
esEvtHeader_T * esEvtCreate(
    size_t              dataSize,
    esEvtId_T           id);

/**
 * @brief       Kreira dogadjaj.
 * @param       dataSize                Velicina potrebnog memorijskog prostora
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
esEvtHeader_T * esEvtCreateI(
    size_t              dataSize,
    esEvtId_T           id);

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Rad sa staticnim dogadjajima
 * @{ *//*---------------------------------------------------------------------------------------*/

/**
 * @brief       Rezervise dogadjaj @c aEvt.
 * @param       aEvt                    Pokazivac na dogadjaj koji se rezervise.
 * @pre         Dogadjaj mora da bude kreiran funkcijom esEvtCreate().
 * @details     Rezervise @c aEvt dogadjaj cime se onemogucava eSolid-u da
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
    esEvtHeader_T       * aEvt);

/**
 * @brief       Oslobadja prethodno rezervisan dogadjaj.
 * @param       aEvt                    Pokazivac na dogadjaj koji se oslobadja.
 * @api
 */
void esEvtUnReserve(
    esEvtHeader_T       * aEvt);

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Transport dogadjaja
 * @{ *//*---------------------------------------------------------------------------------------*/

/**
 * @brief       Salje dogadjaj na kraju reda za cekanje (FIFO metod).
 * @param       aEpa                    Pokazivac na EPA objekat kome se salje,
 * @param       aEvt                    pokazivac na dogadjaj koji se salje.
 * @details     Prihvata prethodno kreiran dogadjaj funkcijom esEvtCreate() i
 *              postavlja ga u red cekanja datog EPA objekta.
 * @api
 */
void esEvtPost(
    esEpaHeader_T       * aEpa,
    esEvtHeader_T       * aEvt);

/**
 * @brief       Salje dogadjaj na kraju reda za cekanje (FIFO metod).
 * @param       aEpa                    Pokazivac na EPA objekat kome se salje,
 * @param       aEvt                    pokazivac na dogadjaj koji se salje.
 * @details     Prihvata prethodno kreiran dogadjaj funkcijom esEvtCreate() i
 *              postavlja ga u red cekanja datog EPA objekta.
 * @iclass
 */
void esEvtPostI(
    esEpaHeader_T       * aEpa,
    esEvtHeader_T       * aEvt);

/**
 * @brief       Salje dogadjaj na pocetku reda za cekanje (LIFO metod).
 * @param       aEpa                    Pokazivac na EPA objekat kome se salje,
 * @param       aEvt                    pokazivac na dogadjaj koji se salje.
 * @details     Prihvata prethodno kreiran dogadjaj funkcijom esEvtCreate() i
 *              postavlja ga u red cekanja datog EPA objekta. Za razliku od
 *              esEvtPost() funkcije dogadjaj se postavlja na pocetku reda za
 *              cekanje. Najcesce se koristi kada je potrebno da se EPA objektu
 *              hitno posalje neki dogadjaj od znacaja koji treba da obradi.
 * @api
 */
void esEvtPostAhead(
    esEpaHeader_T       * aEpa,
    esEvtHeader_T       * aEvt);

/**
 * @brief       Salje dogadjaj na pocetku reda za cekanje (LIFO metod).
 * @param       aEpa                    Pokazivac na EPA objekat kome se salje,
 * @param       aEvt                    pokazivac na dogadjaj koji se salje.
 * @details     Prihvata prethodno kreiran dogadjaj funkcijom esEvtCreate() i
 *              postavlja ga u red cekanja datog EPA objekta. Za razliku od
 *              esEvtPost() funkcije dogadjaj se postavlja na pocetku reda za
 *              cekanje. Najcesce se koristi kada je potrebno da se EPA objektu
 *              hitno posalje neki dogadjaj od znacaja koji treba da obradi.
 * @iclass
 */
void esEvtPostAheadI(
    esEpaHeader_T       * aEpa,
    esEvtHeader_T       * aEvt);

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Korisnicke callback funkcije
 * @{ *//*---------------------------------------------------------------------------------------*/

#if defined(OPT_EVT_USE_TIMESTAMP) || defined(__DOXYGEN__)
/**
 * @brief       Korisnika callback funkcija: generise timestamp prilikom
 *              slanja dogadjaja.
 * @return      timestamp informacija koja se ugradjuje u dogadjaj.
 * @details     Poziva se prilikom slanja dogadjaja odredišnom EPA objektu (
 *              funkcije esEvtPost() i esEvtPostAhead()).
 */
extern esEvtTimestamp_T uTimestampGet(
    void);
#endif

/** @} *//*--------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*===================================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/

/** @endcond *//** @} *//*************************************************************************
 * END of evt.h
 *************************************************************************************************/
#endif /* EVT_H_ */
