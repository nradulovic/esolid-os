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
 * @brief   	Interfejs State Machine Processor (SMP) objekata
 * @addtogroup  smp_intf
 *********************************************************************//** @{ */

#ifndef SMP_H_
#define SMP_H_

/*=========================================================  INCLUDE FILES  ==*/
#include "evt.h"

/*===============================================================  DEFINES  ==*/
/*===============================================================  MACRO's  ==*/
/*------------------------------------------------------  C++ extern begin  --*/
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
enum esEvtSignalId {
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
 */
    SIG_ID_USR = 15
};

/**
 * @brief       Status koji state handler funkcije vracaju dispeceru.
 * @details     Ovo je apstraktni tip koji se koristi za podatke koji vracaju
 *              odgovor state handler funkcija dispeceru automata. Preko ovog
 *              podatka state handler funkcije obavestavaju dispecer koje akcije
 *              automat zeli da preduzme, kao na primer, tranzicija ka drugom
 *              stanju.
 * @api
 */
typedef uint_fast8_t esStatus_T;

/**
 * @brief       Tip state handler funkcija.
 * @details     State handler funkcije vracaju esStatus_T , a kao parametar
 *              prihvataju pokazivac (void *) na strukturu podataka i pokazivac
 *              na dogadjaj.
 * @api
 */
typedef esStatus_T (* esState_T) (void *, esEvt_T *);

/**
 * @brief       Objekat konacnog automata
 * @details     Ovo je apstraktni tip koji se koristi za referenciranje SMP
 *              objekata.
 * @api
 */
typedef struct esSm esSm_T;

/**
 * @brief       Definiciona struktura koja opisuje jedan SMP objekat
 * @details     Ova struktura se koristi prilikom kreiranja novog SMP objekta.
 *              Potrebno je popuniti ovu strukturu sa zeljenim vrednostima, a
 *              zatim je predati funkciji esSmCreate(). Na osnovu vrednosti u
 *              ovoj strukturi funkcija ce kreirati automat.
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

/*======================================================  GLOBAL VARIABLES  ==*/

/**
 * @brief       Signalni događaji koji se koriste prilikom izvršavanja automata
 * @details     Identifikatori događaja su navedeni u @ref esEvtSignalId
 */
extern const C_ROM esEvt_T esEvtSignal[];

/*===================================================  FUNCTION PROTOTYPES  ==*/

/*------------------------------------------------------------------------*//**
 * @name Funkcije za rad sa konacnim automatom (State Machine)
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Inicijalizuje SMP modul
 * @details     Pre koriscenja funkcija SMP modula mora se pozvati ova funkcija.
 * @note        Sa obzirom da SMP modul zahteva funkcije MM modula, ova funkcija
 *              automatski poziva inicijalnu funkciju MM modula.
 * @note        Ukoliko se koristi kernel, onda ce ova funkcija biti pozvana od
 *              strane kernel-a u toku inicijalizacije.
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
 * @api
 */
esStatus_T esSmTopState(
    void *          sm,
    esEvt_T *       evt);

/**
 * @brief       Automat @c sm treba da izvrsi tranziciju ka stanju @c state.
 * @param       [out] sm                Pokazivac trenutne radne povrsine
 * @param       [in] state              Pokazivac na sledece stanje automata.
 * @return      Odgovor dispeceru.
 * @api
 */
esStatus_T esRetnTransition(
    void *          sm,
    esState_T       state);

/**
 * @brief       Automat je odbacio dogadjaj.
 * @return      Odgovor dispeceru.
 * @api
 */
esStatus_T esRetnDeferred(
    void);

/**
 * @brief       Automat je zavrsio sa obradom dogadjaja.
 * @return      Odgovor dispeceru.
 */
esStatus_T esRetnHandled(
    void);

/**
 * @brief       Automat je ignorisao dogadjaj. Ne preduzima se nikakva dodatna
 *              akcija.
 * @return      Odgovor dispeceru.
 */
esStatus_T esRetnIgnored(
    void);

/**
 * @brief       Automat vraca informaciju o svom superstanju
 * @param       [out] sm                Pokazivac trenutne radne povrsine.
 * @param       [in] state              Pokazivac na superstanje.
 * @return      Odgovor dispeceru.
 */
esStatus_T esRetnSuper(
    void *          sm,
    esState_T       state);

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
