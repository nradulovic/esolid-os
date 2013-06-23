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
 * @brief   	Interfejs State Machine Processor (SMP) modula.
 * @details     This file is not meant to be included in application code
 *              independently but through the inclusion of "kernel.h" file.
 * @addtogroup  smp_intf
 *********************************************************************//** @{ */

#ifndef SMP_H_
#define SMP_H_

/*=========================================================  INCLUDE FILES  ==*/
#include "kernel/evt.h"

/*===============================================================  DEFINES  ==*/
/*===============================================================  MACRO's  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Makroi za tranziciju stanja
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Vraca dispeceru informaciju da treba da se izvrsi tranzicija.
 * @param       sm                      Pokazivac na strukturu automata,
 * @param       stateHandler            naredno stanje automata.
 * @details     Makro koristi binarni operator @a zarez (,) koji grupise izraze
 *              sa leva na desno. Vrednost i tip celokupnog izraza je vrednost i
 *              tip desnog izraza.
 */
#define ES_STATE_TRAN(sm, stateHandler)                                         \
    (((esSm_T *)(sm))->state = (esState_T)(stateHandler), RETN_TRAN)

/**
 * @brief       Vraca dispeceru informaciju o super stanju trenutnog stanja.
 * @param       sm                     Pokazivac na strukturu automata,
 * @param       stateHandler           super stanje trenutnog stanja.
 * @details     Makro koristi binarni operator @a zarez (,) koji grupise izraze
 *              sa leva na desno. Vrednost i tip celokupnog izraza je vrednost i
 *              tip desnog izraza.
 * @note        Koristi se samo kod HSM automata.
 */
#define ES_STATE_SUPER(sm, stateHandler)                                             \
    (((esSm_T *)(sm))->state = (esState_T)(stateHandler), RETN_SUPER)

/**
 * @brief       Vraca dispeceru informaciju da je dogadjaj opsluzen.
 * @details     Ovaj makro samo obavestava dispecer da je dogadjaj opsluzen i ne
 *              treba da se izvrsi promena stanja.
 */
#define ES_STATE_HANDLED()                                                     \
    (RETN_HANDLED)

/**
 * @brief       Vraca dispeceru informaciju da je dogadjaj ignorisan.
 * @details     Dogadjaj se ignorise i ne dolazi do promene stanja automata.
 */
#define ES_STATE_IGNORED()                                                     \
    (RETN_IGNORED)

/** @} *//*---------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/**
 * @brief       Identifikatori predefinisanih dogadjaja
 * @details     Zadnji predefinisan identifikator je @ref SIG_ID_USR. Nakon ovog
 *              identifikatora korisnik definise svoje, aplikacione
 *              identifikatore dogadjaja.
 */
enum esEvtId {
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


/**
 * @brief       Nabrajanje odgovora state handler funkcije.
 * @details     State handler funkcija preko ovih nabrajanja govori SMP
 *              dispeceru da li treba da se preuzme neka akcija kao odgovor na
 *              dogadjaj.
 */
typedef enum esStatus {
/**
 * @brief       Treba izvrsiti tranziciju ka drugom stanju.
 * @details     Akcija koja je potrebna za odgovor na dogadjaj je tranzicija ka
 *              drugom stanju.
 */
    RETN_TRAN,

/**
 * @brief       Treba odloziti pristigli dogadjaj.
 * @details     Sistem ce predati dogadjaj vratiti ponovo u red za cekanje za
 *              dogadjaje i poslati ga prilikom sledeceg ciklusa.
 */
    RETN_DEFERRED,

/**
 * @brief       Ne treba izvrsiti nikakve dalje akcije.
 * @details     Ovo je odgovor state handler funkcije da je potpuno opsluzila
 *              dogadjaj i nikakve dodatne akcije ne treba da se preduzmu.
 */
    RETN_HANDLED,

/**
 * @brief       Pristigli dogadjaj nije obradjen i ignorisan je.
 * @details     Obicno se ovakav odgovor u top state-u automata i koristi se u
 *              svrhe debagiranja sistema. Dogadjaj se brise iz sistema ako nema
 *              jos korisnika.
 */
    RETN_IGNORED,

/**
 * @brief       Vraca se koje je super stanje date state handler funkcije.
 * @details     Ova vrednost se vraca kada state handler funkcija ne zna da
 *              obradi neki dogadjaj ili je od nje zahtevano da vrati koje je
 *              njeno super stanje.
 */
    RETN_SUPER

} esStatus_T;

/**
 * @brief       Tip pokazivaca na state handler funkcije.
 * @details     Funkcije vracaju esStatus_T , a kao parametar prihvataju
 *              pokazivac na strukturu izvrsne jedinice i pokazivac na
 *              dogadjaj.
 */
typedef esStatus_T (* esState_T) (void *, esEvt_T *);

/**
 * @brief       Definiciona struktura koja opisuje jedan SM objekat
 * @api
 */
typedef struct esSmDef {
/**
 * @brief       Potrebna memorija radnog okruzenja za SM objekat
 */
    size_t          smWorkspaceSize;

/**
 * @brief       Inicijalno stanje automata
 */
    esState_T       smInitState;

/**
 * @brief       Maksimalna dubina hijerarhije stanja automata.
 */
    uint8_t         smLevels;
} esSmDef_T;

/**
 * @brief       Struktura automata
 */
typedef struct esSm {

#if (OPT_MM_DISTRIBUTION != ES_MM_DYNAMIC_ONLY)                              \
    && (OPT_MM_DISTRIBUTION != ES_MM_STATIC_ONLY)                               \
    || defined(__DOXYGEN__)
/**
 * @brief       Pokazivac na klasu memorijskog alokatora
 */
    const C_ROM struct esMemClass * memClass;
#endif

/**
 * @brief       Pokazivac na state handler funkciju.
 * @details     Ovaj pokazivac pokazuje na funkciju stanja koja vrsi obradu
 *              dogadjaja.
 */
    esState_T       state;

#if (OPT_SMP_SM_TYPES == ES_SMP_FSM_AND_HSM) || defined(__DOXYGEN__)
/**
 * @brief       Pokazivac na dispecer funkciju datog automata
 * @details     Ovaj clan strukture se koristi samo ukoliko se istovremeno
 *              koriste FSM i HSM automati.
 */
    esStatus_T (* dispatch)(struct esSm *, const esEvt_T *);
#endif

#if (OPT_SMP_SM_TYPES != ES_SMP_FSM_ONLY) || defined(__DOXYGEN__)
/**
 * @brief       Niz za cuvanje izvornih stanja HSM automata
 * @details     Ovaj clan se koristi samo ukoliko se koriste HSM automati.
 */
    esState_T *     stateQBegin;

/**
 * @brief       Niz za cuvanje odredisnih stanja HSM automata
 * @details     Ovaj clan se koristi samo ukoliko se koriste HSM automati.
 */
    esState_T *     stateQEnd;
#endif
} esSm_T;

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

/*------------------------------------------------------------------------*//**
 * @name Funkcije za rad sa konacnim automatom (State Machine)
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Inicijalizuje SMP modul
 * @details     Pre koriscenja funkcija SMP modula mora se pozvati ova funkcija.
 * @note        Funkcija ce pozvati inicijalizator funkciju MM modula automatski.
 * @api
 */
void esSmpInit(
    void);

/**
 * @brief       Kreira automat
 * @param       [in] memClass           Memorijska klasa alokatora
 *  @arg        esMemDynClass           dinamicki alokator
 *  @arg        esMemStaticClass        staticki alokator
 * @param       [in] definition         Definiciona struktura SM automata.
 * @return      Pokazivac na kreirani automat.
 * @see         esSmDef_T
 * @api
 */
esSm_T * esSmCreate(
    const C_ROM esMemClass_T *  memClass,
    const C_ROM esSmDef_T *     definition);

/**
 * @brief       Unistava automat
 * @param       [out] sm                Pokazivac na postojeci automat.
 * @api
 */
void esSmDestroy(
    esSm_T *        sm);

/**
 * @brief       Pokrece dati HSM automat.
 * @param       [in] sm                 Pokazivac na strukturu automata,
 * @param       [in] evt                pokazivac na dogadjaj.
 * @return      Status obrade dogadjaja.
 * @details     Ovu funkcija se pokrece nakon zakljucivanja da je dati
 *              automat spreman za rad. Dispecer pokrece stateHandler funkcije i
 *              ispituje njihovu povratnu vrednost. U zavisnosti od povratne
 *              vrednosti funkcije stanja on preduzima dodatne akcije.
 * @api
 */
esStatus_T esSmDispatch(
    esSm_T *        sm,
    const esEvt_T * evt);

/**
 * @brief       Najvisi nivo u hijerarhiji HSM automata.
 * @param       [in] sm                 Pokazivac na strukturu HSM automata,
 * @param       [in] evt                pokazivac/podatak na strukturu dogadjaja.
 * @return      esStatus_T              status funkcije.
 * @note        Uvek vraca odgovor IGNORED, jer ona ne prihvata nikakav
 *              dogadjaj.
 * @api
 */
esStatus_T esSmTopState(
    void *          sm,
    esEvt_T *       evt);

/** @} *//*-------------------------------------------------------------------*/
/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of smp.h
 ******************************************************************************/
#endif /* SMP_H_ */
