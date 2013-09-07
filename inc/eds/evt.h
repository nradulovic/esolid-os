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
 * @brief       Javni interfejs
 *********************************************************************//** @{ */

#ifndef EVT_H_
#define EVT_H_

/*=========================================================  INCLUDE FILES  ==*/
#include "compiler.h"
#include "../config/sys_config.h"
#include "../config/evt_config.h"

#if (0U == OPT_MEM_POOL_EXTERN)
# include "kernel/mem.h"
#endif

/*===============================================================  MACRO's  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Dogadjaj
 * @brief       Opis dogadjaja i njegovih atributa
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Bit maska za definisanje rezervisanog dogadjaja.
 * @details     Ukoliko je ovaj bit postavljen na jedinicu, dati dogadjaj je
 *              rezervisan i sistem ga nece razmatrati kao kandidata za brisanje.
 *              Brojac korisnika dogadjaja se i dalje azurira, tako da kada se
 *              dogadjaj oslobodii rezervacije on moze da bude obrisan ako nema
 *              korisnika.
 */
#define EVT_RESERVED_MASK               ((uint_fast16_t)(1U << 14))

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
#define ES_EXPAND_EVT_ID(a, b, c, d)                                            \
    a,

/**
 * @brief       Kreira bazu dogadjaja
 * @see         esEvtDBElem_T
 * @api
 */
#if (1U == OPT_EVT_DB_USE_DESC_DATA) && (1U == OPT_EVT_USE_MEM_POOL)            /* Koriste se: deskriptivni podaci i pool alokator          */
# define ES_EXPAND_EVT_DATA(a, b, c, d)                                         \
    [a] = {sizeof(b), c, #a, #b, #d},
#elif (1U == OPT_EVT_DB_USE_DESC_DATA) && (0U == OPT_EVT_USE_MEM_POOL)          /* Koriste se samo deskriptivni podaci                      */
# define ES_EXPAND_EVT_DATA(a, b, c, d)                                         \
    [a] = {sizeof(b), #a, #b, #d},
#elif (0U == OPT_EVT_DB_USE_DESC_DATA) && (1U == OPT_EVT_USE_MEM_POOL)          /* Koristi se samo pool alokator                            */
# define ES_EXPAND_EVT_DATA(a, b, c, d)                                         \
    [a] = {sizeof(b), c},
#else                                                                           /* Ne koriste se deskriptivni podaci i ne koristi se pool   */
# define ES_EXPAND_EVT_DATA(a, b, c, d)                                         \
    [a] = {sizeof(b)},
#endif

/**
 * @brief       Kreira C tipove za dogadjaje
 * @api
 */
#define ES_EXPAND_EVT_TYPEDEF(a, b, c, d)                                       \
    typedef b a##_T;

/** @} *//*-------------------------------------------------------------------*/
/*------------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Transport dogadjaja
 * @brief       Transport dogadjaja izmedju EPA objekata
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Apstraktni tip pokazivaca na EPA objekat
 * @details     U zavisnosti od konfiguracije EPA objekat moze da bude prosta
 *              funkcija, automat ili automat enkapsuliran u RTOS process/task.
 */
typedef struct esEpa esEpa_T;

/** @} *//*-------------------------------------------------------------------*/
/*------------------------------------------------------------------------*//**
 * @name        Dogadjaj
 * @{ *//*--------------------------------------------------------------------*/

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
struct OPT_EVT_STRUCT_ATTRIB esEvt {

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

#if (1U == OPT_EVT_USE_TIMESTAMP) || defined(__DOXYGEN__)
/**
 * @brief       Vremenska oznaka kada se desio dogadjaj.
 * @details     Podesavanje tipa se vrsi pomocu: @ref OPT_EVT_TIMESTAMP_T.
 *              Ukljucivanje/iskljucivanje ovog podatka se vrsi opcijom
 *              @ref OPT_EVT_USE_TIMESTAMP.
 */
    esEvtTimestamp_T timestamp;
#endif

#if (1U == OPT_EVT_USE_GENERATOR) || defined(__DOXYGEN__)
/**
 * @brief       Adresa generatora dogadjaja
 * @details     Ukljucivanje/iskljucivanje ovog podatka se vrsi opcijom
 *              @ref OPT_EVT_USE_GENERATOR.
 */
    esEpa_T *       generator;
#endif

#if (1U == OPT_EVT_USE_SIZE) || defined(__DOXYGEN__)
/**
 * @brief       Velicina podataka dogadjaja.
 * @details     Podesavanje tipa se vrsi pomocu: @ref OPT_EVT_SIZE_T.
 *              Ukljucivanje/iskljucivanje ovog podatka se vrsi opcijom
 *              @ref OPT_EVT_USE_SIZE.
 */
    esEvtSize_T     size;
#endif
};

/**
 * @brief       Tip dogadjaja
 * @api
 */
typedef struct esEvt esEvt_T;

/** @} *//*-------------------------------------------------------------------*/
/*------------------------------------------------------------------------*//**
 * @name        Baza dogadjaja
 * @{ *//*--------------------------------------------------------------------*/
/**
 * @brief       Struktura jednog elementa baze dogadjaja
 * @details     Ova struktura navodi koji se podaci pamte za svaki dogadjaj u
 *              bazi dogadjaja.
 */
struct esEvtDBElem {
/** @brief      Velicina dogadjaja                                            */
    size_t          size;

#if (1U == OPT_EVT_USE_MEM_POOL) || defined(__DOXYGEN__)
/** @brief      Deskriptor Pool alokatora                                     */
    OPT_MEM_POOL_T * pool;
#endif

#if (1U == OPT_EVT_DB_USE_DESC_DATA) || defined(__DOXYGEN__)
/** @brief      Ime dogadjaja                                                 */
    const char *    name;

/** @brief      Tip dogadjaja                                                 */
    const char *    type;

/** @brief      Kratak opis dogadjaja                                         */
    const char *    desc;
#endif
};

/**
 * @brief       Tip elementa baze dogadjaja
 * @api
 */
typedef struct esEvtDBElem esEvtDBElem_T;

/** @} *//*-------------------------------------------------------------------*/
/*======================================================  GLOBAL VARIABLES  ==*/

/**
 * @brief       Deskriptor dinamickog alokatora
 * @details     Ukoliko se koristi dinamicki alokator onda je potrebno kreirati
 *              instancu dinamickog alokatora. Dinamickom alokatoru se predaje
 *              ovaj deskriptor prilikom inicijalizacije.
 * @see         evtc_step_2
 */
extern OPT_MEM_DYN_T gEvtDynStorage;

/*===================================================  FUNCTION PROTOTYPES  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Baza dogadjaja
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Vrsi registraciju baze dogadjaja
 * @param       evtDB                   Pokazivac na formiranu bazu dogadjaja
 * @param       elements                Broj elemenata (dogadjaja) u bazi
 * @details     Nakon sto se formira baza dogadjaja ona mora da se registruje.
 *              U jednom trenutku koristi se samo jedna baza dogadjaja i ona se
 *              mora nalaziti u ROM memoriji.
 * @api
 */
void esEvtDBRegister(
    const PORT_C_ROM esEvtDBElem_T * evtDB[],
    uint16_t        elements);

/**
 * @brief       Dobavlja informaciju o velicini dogadjaja iz baze dogadjaja
 * @param       id                      Identifikator dogadjaja
 * @return      Velicinu trazenog dogadjaja u bajtovima.
 * @pre         Baza dogadjaja pre poziva ove funkcije mora da bude registrovana
 *              funkcijom esEvtDBRegister()
 * @api
 */
size_t esEvtDBQuerySize(
    esEvtId_T       id);

/**
 * @brief       Dobavlja informaciju o imenu dogadjaja iz baze dogadjaja
 * @param       id                      Identifikator dogadjaja
 * @return      Znakovni niz imena dogadjaja
 * @pre         Baza dogadjaja pre poziva ove funkcije mora da bude registrovana
 *              funkcijom esEvtDBRegister()
 * @api
 */
const PORT_C_ROM char * esEvtDBQueryName(
    esEvtId_T       id);

/**
 * @brief       Dobavlja informaciju o tipu dogadjaja iz baze dogadjaja
 * @param       id                      Identifikator dogadjaja
 * @return      Znakovni niz tipa dogadjaja
 * @pre         Baza dogadjaja pre poziva ove funkcije mora da bude registrovana
 *              funkcijom esEvtDBRegister()
 * @api
 */
const PORT_C_ROM char * esEvtDBQueryType(
    esEvtId_T       id);

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

/** @} *//*-------------------------------------------------------------------*/
/*------------------------------------------------------------------------*//**
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

/** @} *//*-------------------------------------------------------------------*/
/*------------------------------------------------------------------------*//**
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

/** @} *//*-------------------------------------------------------------------*/
/*------------------------------------------------------------------------*//**
 * @name        Callback funkcije
 * @brief       Funkcije koje se pozivaju od strane EVT modula u odredjenim
 *              trenucima izvrsenja
 * @details     Ovde se nalaze sve callback funkcije koje EVT modul poziva. Njih
 *              treba da definise korisnik prema svojim potrebama.
 * @{ *//*--------------------------------------------------------------------*/

#if (1U == OPT_EVT_USE_GENERATOR) && (1U == OPT_EVT_GENERATOR_CALLBACK) ||      \
    defined(__DOXYGEN__)
/**
 * @brief       Dobavlja ko kreira dogadjaj
 * @return      Pokazivac na objekat koji kreira dogadjaj
 * @details     Funkcija se poziva u toku kreiranja dogadjaja. Ona treba da
 *              vrati pokazivac na objekat koji upravo kreira dogadjaj.
 * @pre         Funkcija se poziva samo ako su ukljucene opcije:
 *              - @ref OPT_EVT_USE_GENERATOR
 *              - @ref OPT_EVT_GENERATOR_CALLBACK
 */
extern esEpa_T * appEvtGeneratorGet(
    void);
#endif

#if (1U == OPT_EVT_USE_TIMESTAMP) && (1U == OPT_EVT_TIMESTAMP_CALLBACK) ||      \
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
 *              - @ref OPT_EVT_USE_TIMESTAMP
 *              - @ref OPT_EVT_TIMESTAMP_CALLBACK
 */
extern esEvtTimestamp_T appEvtTimestampGet(
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
