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
 *************************************************************************************************/


/*********************************************************************************************//**
 * @file
 *
 * @author      Nenad
 *
 * @brief       Interfejs Event Object Transportation (EOT) modula.
 *
 * ------------------------------------------------------------------------------------------------
 *
 * @addtogroup  eot_intf
 *
 ****************************************************************************************//** @{ */


#ifndef EOT_H_
#define EOT_H_


/*************************************************************************************************
 * INCLUDE FILES
 *************************************************************************************************/

/*-----------------------------------------------------------------------------------------------*
 * Konfiguracija i adaptacija modula
 *-----------------------------------------------------------------------------------------------*/

#include "core/cfg/eot_cfg.h"


/*-----------------------------------------------------------------------------------------------*
 * EXTERNS
 *-----------------------------------------------------------------------------------*//** @cond */

#ifdef EOT_H_VAR
# define EOT_H_EXT
#else
# define EOT_H_EXT extern
#endif


/** @endcond*//***********************************************************************************
 * DEFINES
 *************************************************************************************************/


/*************************************************************************************************
 * MACRO's
 *************************************************************************************************/

/*-------------------------------------------------------------------------------------------*//**
 * @name        Pristup podacima zaglavlja u dogadjajima sa prostom i prosirenom reprezentacijom
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Dobavlja identifikator dogadjaja.
 *
 *              Koristi se u @c switch - @c case strukturi kako bi se odredio
 *              koji dogadjaj se obradjuje.
 *
 *              Radi sa:
 *              - prostom reprezentacijom,
 *              - prosirenom reprezentacijom.
 *
 * @param       pEvt                    Pokazivac na strukturu dogadjaja.
 * @return      identifikator dogadjaja.
 *
 *              Primer koriscenja makroa:
 * @include     eot_evt_id.c
 */
/*-----------------------------------------------------------------------------------------------*/
#define EOT_EVT_ID(pEvt)                                                        \
    (((const eot_evt_T *)(pEvt))->pid)

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Dobavlja vremenski marker dogadjaja.
 *
 *              Ovim makroom se vrsi pristup vremenskom markeru dogadjaja.
 *              Vremenski marker se upisuje u toku slanja dogadjaja i omogucava
 *              aplikaciji da sazna tacan trenutak kada je dogadjaj poslat.
 *
 *              Radi sa:
 *              - prostom reprezentacijom,
 *              - prosirenom reprezentacijom.
 *
 * @param       pEvt                    Pokazivac na strukturu dogadjaja.
 * @return      vremenski marker dogadjaja.
 * @retval      0U - kada se ne koriste vremenski dogadjaji.
 * @note        Ovaj makro je validan samo ukoliko se koriste vremenski markeri,
 *              videti @ref OPT_EVT_USE_TIME.
 */
/*-----------------------------------------------------------------------------------------------*/
#if defined(OPT_EVT_USE_TIME) || defined(__DOXYGEN__)
# define EOT_EVT_TIMESTAMP(pEvt)                                                \
    (((const eot_evt_T *)(pEvt))->timestamp)
#else
# define EOT_EVT_TIMESTAMP(pEvt)                                                \
    ((eot_evtTime_T)0U)

#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Dobavlja adresu generatora dogadjaja.
 *
 *              Radi sa:
 *              - prostom reprezentacijom,
 *              - Prosirenom reprezentacijom.
 *
 * @param       pEvt                    Pokazivac na strukturu dogadjaja.
 * @return      adresa generatora dogadjaja.
 * @retval      Svi bitovi na jedinici, "1" - kada se ne koriste pokazivaci na
 *              generatore dogadjaja.
 * @note        Ovaj makro je validan samo ukoliko se koristi adresa kreatora
 *              dogadjaja, videti @ref OPT_EVT_USE_TRANSMITTER.
 */
/*-----------------------------------------------------------------------------------------------*/
#if defined(OPT_EVT_USE_TRANSMITTER) || defined(__DOXYGEN__)
# define EOT_EVT_TRANSMITTER(pEvt)                                              \
    (((const eot_evt_T *)(pEvt))->transmitter)
#else
# define EOT_EVT_TRANSMITTER(pEvt)                                              \
    ((void *)(~0U))

#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @def         EOT_EVT_TYPE
 * @brief       Dobavlja tip dogadjaja.
 *
 *              Radi sa:
 *              - prostom reprezentacijom,
 *              - Prosirenom reprezentacijom.
 *
 *              Ovim makroom se vrsi pristup podatku koji vrsi citanje tipa
 *              dogadjaja.
 *
 * @param       pEvt                    Pokazivac na strukturu dogadjaja.
 * @return      tip dogadjaja.
 * @note        Ovaj makro je validan samo ukoliko se koriste tipovi dogadjaja,
 *              videti @ref OPT_EVT_USE_TYPE.
 */
/*-----------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @def         EOT_EVT_SIZE
 * @brief       Dobavlja velicinu dogadjaja izrazena u bajtovima.
 *
 *              Makro vraca podatak tipa koji se definise pomocu
 *              @ref OPT_EVT_SIZE_T.
 *
 *              Radi sa:
 *              - prostom reprezentacijom,
 *              - Prosirenom reprezentacijom.
 *
 * @param       pEvt                    Pokazivac na strukturu dogadjaja.
 * @return      velicina dogadjaja.
 */
/*-----------------------------------------------------------------------------------------------*/

#if !defined(OPT_EVT_USE_EXT) || defined(__DOXYGEN__)
# define EOT_EVT_TYPE(pEvt)                                                     \
    (((const eot_evt_T *)(pEvt))->type)

# define EOT_EVT_SIZE(pEvt)                                                     \
    (((const eot_evt_T *)(pEvt))->size)

#else
# define EOT_EVT_TYPE(pEvt)                                                     \
    (((const eot_evt_T *)(pEvt))->evtClass->type)

# define EOT_EVT_SIZE(pEvt)                                                     \
    (((const eot_evt_T *)(pEvt))->evtClass->size)

#endif

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Makroi za dogadjaje sa iskljucivo prostom reprezentacijom
 * @{ *//*---------------------------------------------------------------------------------------*/

#if !defined(OPT_EVT_USE_EXT) || defined(__DOXYGEN__)

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Kreira dogadjaj proste reprezentacije tipa @c evtType sa
 *              identifikatorom @c evtID.
 *
 *              Radi sa:
 *              - prostom reprezentacijom.
 *
 *              Primer kreiranja dogadjaja tipa @c paramExchange sa
 *              identifikatorom @c SIG_GET_DATA.
 *
 *              @code
 *              paramExchange_T * pEvt;
 *              pEvt = EOT_PLN_CREATE(paramExchange_T, SIG_GET_DATA);
 *              @endcode
 *
 * @param       evtType                 Tip prosirenog dogadjaja,
 * @param       evtID                   identifikator prosirenog dogadjaja.
 */
/*-----------------------------------------------------------------------------------------------*/
# define EOT_PLN_CREATE(evtType, evtID)                                         \
    (evtType *)eS_evtPlnCreate(sizeof(evtType), (evtID))

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Upisuje tip dogadjaja @c evtType u dogadjaj @c pEvt.
 *
 *              Radi sa:
 *              - prostom reprezentacijom.
 *
 *              Ovim makroom se vrsi pristup podatku koji vrsi opis tipa
 *              dogadjaja.
 *
 * @param       pEvt                    Pokazivac na strukturu dogadjaja,
 * @param       evtType                 tip dogadjaja koji treba upisati.
 */
/*-----------------------------------------------------------------------------------------------*/
# define EOT_EVT_WRTYPE(pEvt, evtType)                                          \
	do {																		\
		(pEvt)->type = (evtType);								                \
	} while (0)

#endif /* !defined(OPT_EVT_USE_EXT) */

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name 		Makroi za dogadjaje sa iskljucivo prosirenom reprezentacijom
 * @{ *//*---------------------------------------------------------------------------------------*/

#if defined(OPT_EVT_USE_EXT)

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Kreira dogadjaja prosirene reprezentacije sa strukturom
 * 				@c evtStruct, klase @c pEvtClass i sa identifikatorom @c evtID.
 *
 *              Primer kreiranja dogadjaja tipa @c paramExchange, klase
 *              @c pParamExchange sa identifikatorom @c SIG_GET_DATA.
 *
 *              @code
 *              paramExchange_T * pEvtExt;
 *              pEvtExt = EOT_EXT_CREATE(paramExchange_T, pParamExchange, SIG_GET_DATA);
 *              @endcode
 *
 * @param       evtStruct               Struktura prosirenog dogadjaja,
 * @param       pEvtClass               pokazivac na klasu dogadjaja,
 * @param       evtID                   identifikator prosirenog dogadjaja.
 */
/*-----------------------------------------------------------------------------------------------*/
#define EOT_EXT_CREATE(evtStruct, pEvtClass, evtID)                             \
    (evtStruct *)eS_evtExtCreate((pEvtClass), (evtID))

#endif /* defined(OPT_EVT_USE_EXT) */

/** @} *//*--------------------------------------------------------------------------------------*/



/*-----------------------------------------------------------------------------------------------*
 * C/C++ #ifdef - open
 *-----------------------------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif


/*************************************************************************************************
 * DATA TYPES
 *************************************************************************************************/

/*-------------------------------------------------------------------------------------------*//**
 * @name        Odabir tipa podataka za brojac korisnika dogadjaja i memorijski kvalifikator.
 * @{ *//*---------------------------------------------------------------------------------------*/

/*
 * Bez optimizacija
 */
#if !defined(ES_OPTIMIZE_SIZE) && !defined(ES_OPTIMIZE_SPEED) || defined(__DOXYGEN__)
typedef uint8_t                         eot_evtDyn_T;
#endif

/*
 * Optimizacija po brzini
 */
#if !defined(ES_OPTIMIZE_SIZE) && defined(ES_OPTIMIZE_SPEED)
typedef uint_fast8_t                    eot_evtDyn_T;
#endif

/*
 * Optimizacija po zauzecu
 */
#if defined(ES_OPTIMIZE_SIZE) && !defined(ES_OPTIMIZE_SPEED)
typedef uint8_t                         eot_evtDyn_T;
#endif

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Deklaracije unapred
 * @{ *//*---------------------------------------------------------------------------------------*/
typedef struct epe_epa epe_epa_T;
typedef struct eot_evtQueue eot_evtQueue_T;

/** @} *//*--------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Nabrajanja tipova dogadjaja (EOT sloj).
 */
/*-----------------------------------------------------------------------------------------------*/
enum eot_evtType {
/**
 * @brief       Dogadjaj je sistemskog tipa.
 *
 *              Ovaj dogadjaj je inicirao sam sistem kao sredstvo da se izvrsi
 *              obrada nekog dogadjaja. U ove signale spadaju dogadjaji kontrole
 *              automata.
 */
    EVT_TYPE_SYSTEM,

/**
 * @brief       Tip dogadjaja kada nije specificiran.
 */
    EVT_TYPE_UNSPEC,

/**
 * @brief       Domen korisnickih tipova dogadjaja.
 */
    EOT_TYPE_USR
};

/*-------------------------------------------------------------------------------------------*//**
 * @name 		Opis zaglavlja dogadjaja sa prostom reprezentacijom
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Zaglavlje proste reprezentacije dogadjaja.
 *
 *              @ref eot_evt_ext
 *
 *              Ova struktura nudi sledece opisivanje dogadjaja:
 *              - @c type - tip dogadjaja,
 *              - @c size - velicinu dogadjaja u bajtovima,
 *              - @c timestamp - vremenska oznaka kada se dogadjaj desio,
 *              - @c transmitter - ko je generisao dogadjaj.
 *
 * @note        Ukoliko se vrsi razmena dogadjaja izmedju sistema sa razlicitim
 *              procesorima/okruzenjem mora se pokloniti posebna paznja
 *              poravnanju (align) podataka ove strukture.
 */
/*-----------------------------------------------------------------------------------------------*/
typedef struct OPT_EVT_STRUCT_ATTRIB eot_evtPln {
/**
 * @brief       Velicina dogadjaja u bajtovima.
 *
 *              Ovaj podatak se koristi kada se ne koriste klase dogadjaja. On
 *              govori koliko bajtova zauzima cela struktura dogadjaja,
 *              ukljucujuci i zaglavlje dogadjaja.
 */
    eot_evtSize_T   size;

#if defined(OPT_EVT_USE_TYPE) || defined(__DOXYGEN__)
/**
 * @brief       Podatak koji ukazuje na tip dogadjaja.
 */
    eot_evtType_T   type;
#endif

#if defined(OPT_EVT_USE_TRANSMITTER) || defined(__DOXYGEN__)
/**
 * @brief       Pokazivac na objekat koji je poslao dogadjaj.
 */
    epe_epa_T       * transmitter;
#endif

#if defined(OPT_EVT_USE_TIME) || defined(__DOXYGEN__)
/**
 * @brief       Vremenska oznaka kada se desio dogadjaj.
 *
 *              Tip podataka se konfigurise pomocu @ref OPT_EVT_TIME_T.
 */
    eot_evtTime_T   timestamp;
#endif

/**
 * @brief       Brojac korisnika dogadjaja i memorijski kvalifikator
 *
 *              Brojac za poslate/obradjene dogadjaje se koristi prilikom
 *              donosenja odluke da li treba obrisati dogadjaj. On u sustini
 *              pokazuje koliko razlicitim objektima je jedan dogadjaj poslat.
 *
 *              MSB bit ove promenljive se koristi za definisanje memorijskog
 *              kvalifikatora, a ostali bitovi se koriste kao brojac korisnika
 *              datog dogadjaja.
 *              - Ukoliko je MSB bit ove promenljive postavljen na jedinicu, "1",
 *              onda je dogadjaj alociran upotrebom statickog metoda alokacije
 *              memorije i onda se memorijski prostor koji dogadjaj zauzima ne
 *              moze reciklirati.
 *              - Ukoliko je MSB bit ove promenljice postavljen na nulu, "0" i
 *              ukoliko je broj korisnika dogadjaja jednak nuli, onda se
 *              memorijski prostor koji dogadjaj zauzima moze reciklirati.
 *
 * @note        Ovaj podatak se koristi iskljucivo interno.
 */
    eot_evtDyn_T    dyn;

/**
 * @brief       Identifikator dogadjaja.
 *
 *              Tip podataka se konfigurise pomocu @ref OPT_EVT_ID_T.
 */
    eot_evtId_T     ID;
} eot_evtPln_T;

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name 		Opis zaglavlja dogadjaja sa prosirenom reprezentacijom
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Pokazivac na konstruktor funkciju u klasi.
 */
/*-----------------------------------------------------------------------------------------------*/
typedef void (* eot_ptrEvtCtor_T) (void *);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Pokazivac na destruktor funkciju u klasi.
 */
/*-----------------------------------------------------------------------------------------------*/
typedef void (* eot_ptrEvtDtor_T) (void *);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Struktura koja opisuje klase dogadjaja.
 *
 *              Ova struktura se koristi samo kada se koriste prosireni
 *              dogadjaji. Ona nudi potpuno opisivanje dogadjaja u sistemu i
 *              nudi metode koji se mogu primeniti nad tipom dogadjaja koje
 *              struktura opisuje.
 *
 *              Struktura klase je namenjena da se definise za vreme
 *              kompajliranja i cuva se u ROM/FLASH memoriji. Svi dogadjaji koji
 *              pripadaju istoj klasi, koju ova struktura opisuje, sadrze samo
 *              pokazivac na ovu strukturu. Posto se svi podaci koji opisuju
 *              jedan tip/klasu dogadjaja nalaze na jednom mestu u ROM/FLASH
 *              memoriji onda se upotreba RAM memorije moze znatno redukovati.
 *
 *              Za opis dogadjaja korisnik je duzan da izvrsi popunjavanje
 *              elemenata ove strukture, i to:
 *              - size - velicina celog dogadjaja,
 *              - type - enumerator tipa dogadjaja,
 * @note        Ukoliko se vrsi razmena dogadjaja izmedju sistema sa razlicitim
 *              procesorima/okruzenjem mora se pokloniti posebna paznja
 *              poravnanju (align) podataka ove strukture.
 */
/*-----------------------------------------------------------------------------------------------*/
typedef struct eot_evtClass {
/**
 * @brief       Velicina celog dogadjaja (zaglavlje + korisnicki podaci).
 */
    eot_evtSize_T   size;

#if defined(OPT_EVT_USE_TYPE) || defined(__DOXYGEN__)
/**
 * @brief       Enumerator koji ukazuje na tip dogadjaja.
 */
    eot_evtType_T   type;
#endif

/**
 * @brief       Konstruktor funkcija podataka.
 *
 * @param       aMe                     Pokazivac na memorijski prostor
 *                                      rezervisan za podatke.
 */
    eot_ptrEvtCtor_T ctor;

/**
 * @brief       Destruktor funkcija podataka.
 *
 * @param       aMe                     Pokazivac na memorijski prostor
 *                                      rezervisan za podatke.
 */
    eot_ptrEvtDtor_T dtor;
} eot_evtClass_T;

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Zaglavlje prosirene reprezentacije dogadjaja.
 *
 *              @ref eot_evt_ext
 *
 *              Ova struktura nudi sledece opisivanje dogadjaja:
 *              - @c klasa - klasa kojoj dogadjaja pripada,
 *              - @c timestamp - vremenska oznaka kada se dogadjaj desio,
 *              - @c transmitter - ko je generisao dogadjaj.
 *
 * @note        Ukoliko se vrsi razmena dogadjaja izmedju sistema sa razlicitim
 *              procesorima/okruzenjem mora se pokloniti posebna paznja
 *              poravnanju (align) podataka ove strukture.
 */
/*-----------------------------------------------------------------------------------------------*/
typedef struct OPT_EVT_STRUCT_ATTRIB eot_evtExt {

/**
 * @brief       Klasa dogadjaja koju korisnik definise.
 *
 *              Klasa dogadjaja opisuje njegovu velicinu u bajtovima i potrebne
 *              konstruktor/destruktor funkcije.
 */
    const C_ROM eot_evtClass_T  * evtClass;

#if defined(OPT_EVT_USE_TRANSMITTER) || defined(__DOXYGEN__)
/**
 * @brief       Pokazivac na EPA objekat koji je poslao dogadjaj.
 *
 *              Ovaj podatak se konfigurise pomocu @ref OPT_EVT_USE_TRANSMITTER.
 */
    epe_epa_T       * transmitter;
#endif

#if defined(OPT_EVT_USE_TIME) || defined(__DOXYGEN__)
/**
 * @brief       Vremenska oznaka kada se desio dogadjaj.
 *
 *              Tip podataka se konfigurise pomocu @ref OPT_EVT_USE_TIME, a tip
 *              se konfigurise pomocu @ref OPT_EVT_TIME_T.
 */
    eot_evtTime_T   timestamp;
#endif

/**
 * @brief       Brojac korisnika dogadjaja i memorijski kvalifikator
 *
 *              Brojac za poslate/obradjene dogadjaje se koristi prilikom
 *              donosenja odluke da li treba obrisati dogadjaj. On u sustini
 *              pokazuje koliko razlicitim objektima je jedan dogadjaj poslat.
 *
 *              MSB bit ove promenljive se koristi za definisanje memorijskog
 *              kvalifikatora, a ostali bitovi se koriste kao brojac korisnika
 *              datog dogadjaja.
 *              - Ukoliko je MSB bit ove promenljive postavljen na jedinicu, "1",
 *              onda je dogadjaj alociran upotrebom statickog metoda alokacije
 *              memorije i onda se memorijski prostor koji dogadjaj zauzima ne
 *              moze reciklirati.
 *              - Ukoliko je MSB bit ove promenljive postavljen na nulu, "0" i
 *              ukoliko je broj korisnika dogadjaja jednak nuli, onda se
 *              memorijski prostor koji dogadjaj zauzima moze reciklirati.
 */
    eot_evtDyn_T    dyn;
/**
 * @brief       Identifikator dogadjaja.
 *
 *              Tip podataka se konfigurise pomocu @ref OPT_EVT_ID_T.
 */
    eot_evtId_T     ID;
} eot_evtExt_T;

/** @} *//*--------------------------------------------------------------------------------------*/


/*************************************************************************************************
 * GLOBAL VARIABLES
 *************************************************************************************************/


/*************************************************************************************************
 * FUNCTION PROTOTYPES
 *************************************************************************************************/

/*-------------------------------------------------------------------------------------------*//**
 * @name        Rad sa dinamickim dogadjajima
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Kreira dogadjaj.
 *
 *              Kreira memorijski prostor koji je rezervisan za navedeni
 *              dogadjaj. U zaglavlje dogadjaja se upisuje identifikator i klasa.
 *              Nakon toga, funkcija vraca pokazivac na memorijski prostor koji
 *              je dostupan korisniku za dalje popunjavanje podacima koje treba
 *              preneti.
 *
 *              Prilikom rezervacije memorijskog prostora vrsi se poziv
 *              konstuktor funkcije ukoliko je ona navedena u klasi dogadjaja.
 *
 * @param       aEvtClass               Pokazivac na klasu dogadjaja,
 * @param       aEvtID                  vrednost identifikatora dogadjaja.
 * @return      Pokazivac na memorijski prostor koji je rezervisan za dogadjaj.
 * @note        Funkcija je validna samo ako je definisana promenljiva
 *              @ref OPT_EVT_USE_EXT.
 * @todo        Preimenovati u "object"New format.
 * @api
 */
/*-----------------------------------------------------------------------------------------------*/
#if defined(OPT_EVT_USE_EXT) || defined(__DOXYGEN__)
void * eS_evtExtCreate(
    const C_ROM eot_evtClass_T * aEvtClass,
    eot_evtId_T     aEvtID);
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Kreira dogadjaj sa prostom reprezentacijom.
 *
 *              Kreira memorijski prostor koji je rezervisan za navedeni
 *              dogadjaj. U zaglavlje dogadjaja se upisuje identifikator i klasa.
 *              Nakon toga, funkcija vraca pokazivac na memorijski prostor koji
 *              je dostupan korisniku za dalje popunjavanje podacima koje treba
 *              preneti.
 *
 * @param       aEvtSize                Velicina dogadjaja u bajtovima,
 * @param       aEvtID                  vrednost identifikatora dogadjaja.
 * @note        Funkcija je validna samo ako nije definisana promenljiva
 *              @ref OPT_EVT_USE_EXT.
 * @todo        Preimenovati u "object"New format.
 * @api
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(OPT_EVT_USE_EXT) || defined(__DOXYGEN__)
void * eS_evtPlnCreate(
    size_t          aEvtSize,
    eot_evtId_T     aEvtID);
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Unistava dogadjaj.
 *
 * @param       aEvt                    Pokazivac na dogadjaj koji treba da se
 *                                      unisti.
 * @api
 */
/*-----------------------------------------------------------------------------------------------*/
void eS_evtDestroy(
    eot_evt_T       * aEvt);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Unistava dogadjaj.
 *
 * @param       aEvt                    Pokazivac na dogadjaj koji treba da se
 *                                      unisti.
 * @iclass
 */
/*-----------------------------------------------------------------------------------------------*/
void eS_evtDestroyI(
    eot_evt_T       * aEvt);

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Rad sa staticnim dogadjajima
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Rezervise dogadjaj @c aEvt.
 *
 * @param       aEvt                    Pokazivac na dogadjaj koji se rezervise.
 * @pre         Dogadjaj mora da bude kreiran funkcijom eS_evtExtCreate() ili
 *              eS_evtPlnCreate().
 * @api
 */
/*-----------------------------------------------------------------------------------------------*/
void eS_evtReserve(
    eot_evt_T       * aEvt);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Oslobadja prethodno rezervisan dogadjaj.
 *
 * @param       aEvt                    Pokazivac na dogadjaj koji se oslobadja.
 * @api
 */
/*-----------------------------------------------------------------------------------------------*/
void eS_evtUnReserve(
    eot_evt_T       * aEvt);

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Transport dogadjaja kada se koriste SMP i EPE moduli
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Salje dogadjaj na kraju reda za cekanje (FIFO metod).
 *
 *              Prihvata unapred definisan dogadjaj i prosledjuje eSolid-u za
 *              dalji transport dogadjaja.
 *
 * @param       aEpa                    Pokazivac na agenta za obradu dogadjaja
 *                                      kome se salje dogadjaj,
 * @param       aEvt                    pokazivac na dogadjaj koji se salje.
 * @api
 */
/*-----------------------------------------------------------------------------------------------*/
void eS_evtPost(
    epe_epa_T       * aEpa,
    eot_evt_T       * aEvt);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Salje dogadjaj na pocetku reda za cekanje (LIFO metod).
 *
 *              Prihvata unapred definisan dogadjaj i prosledjuje eSolid-u za
 *              dalji transport dogadjaja ka EPA objektu.
 *
 *              Najcesce se koristi kada je potrebno da se EPA objektu posalje
 *              neki hitan dogadjaj koji treba da obradi.
 *
 * @param       aEpa                    Pokazivac na agenta za obradu dogadjaja
 *                                      kome se salje dogadjaj,
 * @param       aEvt                    pokazivac na dogadjaj koji se salje.
 * @api
 */
/*-----------------------------------------------------------------------------------------------*/
void eS_evtPostAhead(
    epe_epa_T       * aEpa,
    eot_evt_T       * aEvt);

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Transport dogadjaja kada se koristi samo SMP modul
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Dobavlja dogadjaj iz reda za cekanje @c aEvtQueue
 *
 * @param       aEvtQueue               Pokazivac na red za cekanje.
 * @return      Dogadjaj iz reda cekanja.
 * @api
 */
/*-----------------------------------------------------------------------------------------------*/
eot_evt_T * eS_evtQGet(
    eot_evtQueue_T  * aEvtQueue);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Dobavlja dogadjaj iz reda za cekanje @c aEvtQueue
 *
 * @param       aEvtQueue               Pokazivac na red za cekanje.
 * @return      Dogadjaj iz reda cekanja.
 * @iclass
 */
/*-----------------------------------------------------------------------------------------------*/
eot_evt_T * eS_evtQGetI(
    eot_evtQueue_T  * aEvtQueue);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Salje dogadjaj na pocetku reda za cekanje (LIFO metod).
 *
 * @param       aEvtQueue               Pokazivac na red za cekanje,
 * @param       aEvt                    pokazivac na dogadjaj koji se salje.
 * @api
 */
/*-----------------------------------------------------------------------------------------------*/
void eS_evtQPush(
    eot_evtQueue_T  * aEvtQueue,
    eot_evt_T       * aEvt);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Salje dogadjaj na pocetku reda za cekanje (LIFO metod).
 *
 * @param       aEvtQueue               Pokazivac na red za cekanje,
 * @param       aEvt                    pokazivac na dogadjaj koji se salje.
 * @iclass
 */
/*-----------------------------------------------------------------------------------------------*/
void eS_evtQPushI(
    eot_evtQueue_T  * aEvtQueue,
    eot_evt_T       * aEvt);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Salje dogadjaj na kraju reda za cekanje (FIFO metod).
 *
 * @param       aEvtQueue               Pokazivac na red za cekanje,
 * @param       aEvt                    pokazivac na dogadjaj koji se salje.
 * @api
 */
/*-----------------------------------------------------------------------------------------------*/
void eS_evtQPut(
    eot_evtQueue_T  * aEvtQueue,
    eot_evt_T       * aEvt);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Salje dogadjaj na kraju reda za cekanje (FIFO metod).
 *
 * @param       aEvtQueue               Pokazivac na red za cekanje,
 * @param       aEvt                    pokazivac na dogadjaj koji se salje.
 * @iclass
 */
/*-----------------------------------------------------------------------------------------------*/
void eS_evtQPutI(
    eot_evtQueue_T  * aEvtQueue,
    eot_evt_T       * aEvt);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Kreira red cekanja za dogadjaje.
 *
 * @param       aMemClass               Klasa memorije koja se koristi za
 *                                      skladistenje,
 *  @arg        memHeapClass
 *  @arg        memPoolClass
 *  @arg        memStaticClass
 * @param       aQueueSize              velicina reda za cekanje.
 * @return      Pokazivac na strukturu opisa reda za cekanje.
 * @api
 * @todo        Napisati f-ju
 */
/*-----------------------------------------------------------------------------------------------*/
eot_evtQueue_T * eS_evtQCreate(
    smm_memClass_T  * aMemClass,
    uint8_t         aQueueSize);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Unistava red cekanja za dogadjaje.
 *
 * @param       aEvtQueue               Pokazivac na strukturu reda za cekanje.
 * @pre         Red za cekanje mora prethodno da bude kreiran funkcijom
 *              eS_evtQCreate().
 * @api
 * @todo        Napisati f-ju
 */
/*-----------------------------------------------------------------------------------------------*/
void eS_evtQDestroy(
    eot_evtQueue_T  * aEvtQueue);

/** @} *//*--------------------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------------------------*
 * C/C++ #endif - close
 *-----------------------------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif


/*************************************************************************************************
 * CONFIGURATION ERRORS
 *************************************************************************************//** @cond */


/** @endcond *//** @} *//*************************************************************************
 * END of eot.h
 *************************************************************************************************/
#endif /* EOT_H_ */
