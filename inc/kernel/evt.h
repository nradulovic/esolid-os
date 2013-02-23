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
 * ------------------------------------------------------------------------------------------------
 * @addtogroup  evt_intf
 * @brief       Menadzmet dogadjaja
 ****************************************************************************************//** @{ */


#ifndef EVT_H_
#define EVT_H_

/*============================================================================  INCLUDE FILES  ==*/
/*==================================================================================  DEFINES  ==*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Dinamicki attributi dogadjaja
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Bit maska za definisanje rezervisanog dogadjaja.
 * @details     Ukoliko je ovaj bit postavljen na jedinicu, dati dogadjaj je
 *              rezervisan i sistem ga nece razmatrati kao kandidata za brisanje.
 *              Brojac korisnika dogadjaja se i dalje azurira, tako da kada se
 *              dogadjaj oslobodii rezervacije on moze da bude obrisan ako ima 0
 *              korisnika.
 *//*--------------------------------------------------------------------------------------------*/
#define EVT_RESERVED_MASK               ((evtDynamic_T)(1U << 6))

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Bit maska za definisanje staticnog dogadjaja.
 * @details     Ukoliko je ovaj bit postavljen na jedinicu, dati dogadjaj je
 *              statican (nalazi se u ROM delu memorije) i ne moze biti izbrisan.
 *              Broj korisnika dogadjaja se ne azurira. Dati dogadjaja ne moze
 *              da postane dinamican.
 *//*--------------------------------------------------------------------------------------------*/
#define EVT_STATIC_MASK                 ((evtDynamic_T)(1U << 7))

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Bit maska za brojac korisnika dogadjaja
 *//*--------------------------------------------------------------------------------------------*/
#define EVT_USERS_MASK                  ((evtDynamic_T)0x3F)

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Konstanta za potpis dogadjaja
 * @details     Konstanta se koristi prilikom debag procesa kako bi funkcije
 *              koje prime dogadjaj bile sigurne da je dogadjaj kreiran
 *              funkcijom esEvtCreate i da je i dalje validan. Dogadjaji koji se
 *              obrisu nemaju ovaj potpis.
 *//*--------------------------------------------------------------------------------------------*/
#define EVT_SIGNATURE                   (0xFEED)

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @brief       Tipovi predefinisanih dogadjaja
 *//*--------------------------------------------------------------------------------------------*/
enum esEvtType {
/**
 * @brief       Dogadjaj je sistemskog tipa.
 * @details     Ovaj dogadjaj je inicirao sam sistem kao sredstvo da se izvrsi
 *              obrada nekog dogadjaja.
 */
    EVT_TYPE_SYSTEM,

/**
 * @brief       Tip dogadjaja kada nije specificiran.
 * @details     Ovaj tip dogadjaja se automatski specificira prilikom pozivanja
 *              esEvtCreate() funkcije.
 */
    EVT_TYPE_UNSPEC,

/**
 * @brief       Vremenski tip dogadjaja.
 */
    EVT_TYPE_TIME,

/**
 * @brief       Rezervisani tip dogadjaja.
 */
    EVT_TYPE_RESERVED2,

/**
 * @brief       Domen korisnickih tipova dogadjaja.
 */
    EVT_TYPE_USR
};


/*-------------------------------------------------------------------------------------------*//**
 * @brief       Identifikatori predefinisanih dogadjaja
 *//*--------------------------------------------------------------------------------------------*/
enum esEvtID {
/**
 * @brief       Signalni dogadjaj - prazan signal.
 * @details     Koristi se da se detektuje da li je automat spreman da prihvati
 *              nov dogadjaj.
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
 *              super stanje. Funkcija mora da vrati njeno super stanje.
 */
    SIG_SUPER,

/**
 * @brief       Sistemski dogadjaj - zahteva se lista identifikatora aktivnih
 *              EPA objekata u sistemu.
 * @todo        Ovaj identifikator ne treba da se nalazi ovde, staviti ga u
 *              okviru sistemskog automata
 */
    EPA_GET_LIST,

/**
 * @brief       Sistemski dogadjaj - zahtevaju se informacije o aktivnom EPA
 *              objektu.
 * @todo        Ovaj identifikator ne treba da se nalazi ovde, staviti ga u
 *              okviru sistemskog automata
 */
    EPA_GET_INFO,

/**
 * @brief       Sistemski dogadjaj - lista aktivnih EPA objekata.
 * @details     Dogadjaj sadrzi listu aktivnih EPA objekata u sistemu. Najcesce
 *              se generise kao odgovor na @ref EPA_GET_LIST dogadjaj.
 * @todo        Ovaj identifikator ne treba da se nalazi ovde, staviti ga u
 *              okviru sistemskog automata
 */
    EPA_LIST,

/**
 * @brief       Sistemski dogadjaj - informacije o EPA objektu.
 * @details     Dogadjaj sadrzi informacije o trazenom aktivnom EPA objektu u
 *              @ref EPA_GET_INFO dogadjaju.
 * @todo        Ovaj identifikator ne treba da se nalazi ovde, staviti ga u
 *              okviru sistemskog automata
 */
    EPA_INFO,

/**
 * @brief       Domen korisnickih identifikatora dogadjaja.
 * @details     Nakon ovog identifikatora korisnik definise svoje, aplikacione
 *              identifikatore dogadjaj.
 * @todo        Ovaj identifikator ne treba da se nalazi ovde, staviti ga u
 *              okviru sistemskog automata
 */
    EVT_ID_USR = 15
};

/*==================================================================================  MACRO's  ==*/
/*-------------------------------------------------------------------------------------------*//**
 * @brief       Dobavlja identifikator dogadjaja
 *//*--------------------------------------------------------------------------------------------*/
#define ES_EVT_ID(evt)                                                             \
    ((evt)->internals.id)

/*-------------------------------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*===============================================================================  DATA TYPES  ==*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Interni tipovi podataka
 * @ingroup     evt_impl
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Tip za cuvanje dinamickih atributa dogadjaja.
 *//*--------------------------------------------------------------------------------------------*/
#if defined(OPT_OPTIMIZE_SPEED) || defined(__DOXYGEN__)
typedef uint_fast8_t evtDynamic_T;
#else
typedef uint8_t evtDynamic_T;
#endif

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Interni podaci dogadjaja
 *//*--------------------------------------------------------------------------------------------*/
typedef struct OPT_EVT_STRUCT_ATTRIB evtIntr {
/**
 * @brief       Identifikator dogadjaja
 * @details     Podesavanje tipa se vrsi pomocu: @ref OPT_EVT_ID_T.
 */
    esEvtId_T       id;

/**
 * @brief       Dinamicki atributi dogadjaja
 */
    evtDynamic_T    dynamic;

#if defined(OPT_DBG_EO) && defined(OPT_DBG_USE_CHECK) || defined(__DOXYGEN__)
/**
 * @brief       Potpis koji pokazuje da je ovo zaista dogadjaj.
 */
    uint_fast32_t   signature;
#endif
} evtIntr_T;

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Red cekanja za dogadjaje.
 * @details     Ova struktura opisuje redove cekanja za dogadjaje koji se
 *              koriste za aktivne objekte. Red cekanja zajedno sa
 *              procesorom konacnih automata (SP) cini jedan agent za obradu
 *              dogadjaja, (Event Processing Agent - EPA).
 *
 *              Struktura sadrzi samo podatke za upravljanje reda cekanja i ne
 *              sadrzi memorijski prostor za cuvanje dogadjaja. Memorijski
 *              prostor se mora rezervisati unapred i predati pokazivac na taj
 *              prostor EO-u prilikom inicijalizacije reda cekanja.
 *
 *              U redovima cekanja se cuvaju samo pokazivaci ka dogadjajima, a
 *              ne i same instance dogadjaja.
 *//*--------------------------------------------------------------------------------------------*/
struct evtQueue {
/**
 * @brief       Instanca reda za cekanje opste namene
 */
    esQueuePtr_T    queue;

#if defined(OPT_KERNEL_USE_REGISTRY) || defined(__DOXYGEN__)
/**
 * @brief       Trenutni broj slobodnih mesta za dogadjaje u redu za cekanje.
 */
    size_t          freeCurr;

/**
 * @brief       Minimalni broj slobodnih mesta za dogadjaje u redu za cekanje.
 */
    size_t          freeMin;
#endif
};

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Korisnicki podaci
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------*//**
 * @extends     evtIntr_T
 * @brief       Zaglavlje dogadjaja.
 * @details     Interni podaci dogadjaja su obavezni podaci u zaglavlju
 *              dogadjaja. Svi ostali podaci se mogu ukljuciti/iskljuciti po
 *              zelji.
 *
 *              Ova struktura nudi sledece opisivanje dogadjaja:
 *              - @c transmitter - ko je generisao dogadjaj,
 *              - @c timestamp - vremenska oznaka kada se dogadjaj desio,
 *              - @c size - velicina dogadjaja,
 *              - @c type - enumerator tipa dogadjaja.
 *
 *              Svaka od navedenih clanova strukture se moze nezavisno
 *              ukljuciti/iskljuciti i tipovi clanova se mogu po zelji izabrati.
 * @note        Ukoliko se vrsi razmena dogadjaja izmedju sistema sa razlicitim
 *              procesorima/okruzenjem mora se pokloniti posebna paznja
 *              poravnanju (align) podataka ove strukture. Radi podesavanja
 *              nacina pokovanja strukture koristi se @ref OPT_EVT_STRUCT_ATTRIB.
 *//*--------------------------------------------------------------------------------------------*/
struct OPT_EVT_STRUCT_ATTRIB esEvtHeader {
/**
 * @brief       Interni podaci dogadjaja.
 */
    evtIntr_T       internals;

#if defined(OPT_EVT_USE_TRANSMITTER) || defined(__DOXYGEN__)
/**
 * @brief       Adresa generatora dogadjaja
 *
 *              Ukljucivanje/iskljucivanje ovog podatka se vrsi opcijom
 *              @ref OPT_EVT_USE_TRANSMITTER.
 *
 */
    esEpaHeader_T   * transmitter;
#endif

#if defined(OPT_EVT_USE_TIME) || defined(__DOXYGEN__)
/**
 * @brief       Vremenska oznaka kada se desio dogadjaj.
 *
 *              Podesavanje tipa se vrsi pomocu: @ref OPT_EVT_TIME_T.
 *
 *              Ukljucivanje/iskljucivanje ovog podatka se vrsi opcijom
 *              @ref OPT_EVT_USE_TIME.
 */
    esEvtTime_T     time;
#endif

#if defined(OPT_EVT_USE_SIZE) || defined(__DOXYGEN__)
/**
 * @brief       Velicina podataka dogadjaja.
 *
 *              Podesavanje tipa se vrsi pomocu: @ref OPT_EVT_SIZE_T.
 *
 *              Ukljucivanje/iskljucivanje ovog podatka se vrsi opcijom
 *              @ref OPT_EVT_USE_SIZE.
 */
    esEvtSize_T     size;
#endif

#if defined(OPT_EVT_USE_TYPE) || defined(__DOXYGEN__)
/**
 * @brief       Velicina podataka dogadjaja.
 *
 *              Podesavanje tipa se vrsi pomocu: @ref OPT_EVT_TYPE_T.
 *
 *              Ukljucivanje/iskljucivanje ovog podatka se vrsi opcijom
 *              @ref OPT_EVT_USE_TYPE.
 */
    esEvtType_T     type;
#endif
};

/** @} *//*--------------------------------------------------------------------------------------*/
/*=========================================================================  GLOBAL VARIABLES  ==*/
/*======================================================================  FUNCTION PROTOTYPES  ==*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Kreiranje/brisanje dogadjaja
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------*//**
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
 *//*--------------------------------------------------------------------------------------------*/
esEvtHeader_T * esEvtCreate(
    size_t              dataSize,
    esEvtId_T           id);

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Kreira dogadjaj.
 * @param       dataSize                Velicina potrebnog memorijskog prostora
 *                                      za podatke dogadjaja,
 * @param       id                      identifikator dogadjaja.
 * @return      Pokazivac na memorijski prostor za podatke dogadjaja.
 * @details     Ova funkcija je potpuno identicna esEvtCreate() funkciji.
 * @iclass
 *//*--------------------------------------------------------------------------------------------*/
esEvtHeader_T * esEvtCreateI(
    size_t              dataSize,
    esEvtId_T           id);

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Rad sa staticnim dogadjajima
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Rezervise dogadjaj @c aEvt.
 * @param       aEvt                    Pokazivac na dogadjaj koji se rezervise.
 * @pre         Dogadjaj mora da bude kreiran funkcijom esEvtCreate().
 *
 *              Rezervise @c aEvt dogadjaj cime se onemogucava eSolid-u da
 *              izvrsi recikliranje memorijskog prostora ovog dogadjaja.
 *              Uglavnom se rezervacija dogadjaja vrsi kada je potrebno iz neke
 *              prekidne rutine brzo poslati dogadjaj nekom aktivnom EPA objektu.
 *              Prethodnim kreiranjem dogadjaja i njegovom rezervacijom se vrsi
 *              alociranje memorije, tako da u kriticnim trenucima nije potrebno
 *              ponovo izvrsiti alociranje memorije, vec se spreman dogadjaj
 *              odmah koristi.
 * @api
 *//*--------------------------------------------------------------------------------------------*/
void esEvtReserve(
    esEvtHeader_T       * aEvt);

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Oslobadja prethodno rezervisan dogadjaj.
 * @param       aEvt                    Pokazivac na dogadjaj koji se oslobadja.
 * @api
 *//*--------------------------------------------------------------------------------------------*/
void esEvtUnReserve(
    esEvtHeader_T       * aEvt);

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Transport dogadjaja
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Salje dogadjaj na kraju reda za cekanje (FIFO metod).
 * @param       aEpa                    Pokazivac na EPA objekat kome se salje,
 * @param       aEvt                    pokazivac na dogadjaj koji se salje.
 * @details     Prihvata prethodno kreiran dogadjaj funkcijom esEvtCreate() i
 *              postavlja ga u red cekanja datog EPA objekta.
 * @api
 *//*--------------------------------------------------------------------------------------------*/
void esEvtPost(
    esEpaHeader_T       * aEpa,
    esEvtHeader_T       * aEvt);

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Salje dogadjaj na pocetku reda za cekanje (LIFO metod).
 * @param       aEpa                    Pokazivac na EPA objekat kome se salje,
 * @param       aEvt                    pokazivac na dogadjaj koji se salje.
 * @details     Prihvata prethodno kreiran dogadjaj funkcijom esEvtCreate() i
 *              postavlja ga u red cekanja datog EPA objekta. Za razliku od
 *              esEvtPost() funkcije dogadjaj se postavlja na pocetku reda za
 *              cekanje. Najcesce se koristi kada je potrebno da se EPA objektu
 *              hitno posalje neki dogadjaj od znacaja koji treba da obradi.
 * @api
 *//*--------------------------------------------------------------------------------------------*/
void esEvtPostAhead(
    esEpaHeader_T       * aEpa,
    esEvtHeader_T       * aEvt);

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
