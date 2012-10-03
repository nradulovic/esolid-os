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
 * @brief   	Interfejs State Machine Processor (SMP) modula.
 * ------------------------------------------------------------------------------------------------
 * @addtogroup  sproc_intf
 * @brief       Interfejs State Machine Processor (SMP) modula.
 ****************************************************************************************//** @{ */


#ifndef SMP_H_
#define SMP_H_

/*============================================================================  INCLUDE FILES  ==*/
/*==================================================================================  DEFINES  ==*/
/*==================================================================================  MACRO's  ==*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Makroi za tranziciju stanja
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Vraca dispeceru informaciju da treba da se izvrsi tranzicija.
 * @param       epa                     Pokazivac na strukturu HSM automata,
 * @param       state                   naredno stanje automata.
 * @details     Makro koristi binarni operator @a zarez (,) koji grupise izraze
 *              sa leva na desno. Vrednost i tip celokupnog izraza je vrednost i
 *              tip desnog izraza.
 *//*--------------------------------------------------------------------------------------------*/
#define SMP_STATE_TRAN(epa, state)                                              \
    (((esEpaHeader_T *)(epa))->pState = (esPtrState_T)(state), RETN_TRAN)

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Vraca dispeceru informaciju o super stanju trenutnog stanja.
 * @param       epa                     Pokazivac na strukturu HSM automata,
 * @param       state                   super stanje trenutnog stanja.
 * @details     Makro koristi binarni operator @a zarez (,) koji grupise izraze
 *              sa leva na desno. Vrednost i tip celokupnog izraza je vrednost i
 *              tip desnog izraza.
 * @note        Koristi se samo kod HSM automata.
 *//*--------------------------------------------------------------------------------------------*/
#define SMP_STATE_SUPER(epa, state)                                             \
    (((esEpaHeader_T *)(epa))->pState = (esPtrState_T)(state), RETN_SUPER)

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Vraca dispeceru informaciju da treba da se izvrsi inicijalna
 *              tranzicija.
 * @param       epa                     Pokazivac na strukturu HSM automata,
 * @param       state                   naredno stanje automata.
 *//*--------------------------------------------------------------------------------------------*/
#define SMP_STATE_INIT(epa, state)                                              \
    (((esEpaHeader_T *)(epa))->pState = (esPtrState_T)(state), RETN_INIT)

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Vraca dispeceru informaciju da automat ulazi u flowchart deo
 *              dijagrama stanja.
 * @details     Ovaj makro se koristi kada se zeli tranzicija u sledece stanje
 *              bez dogadjaja. Naime, kada se ovaj makro koristi, dispecer sam
 *              generise @ref SIG_NOEX dogadjaj tako da automat moze da obradi
 *              flowchart bez spoljasnih dogadjaja. Flowchart se pise u odeljku
 *              koji obradjuje SIG_NOEX, dok se u SIG_ENTRY odeljku samo
 *              koristi ovaj makro.
 *//*--------------------------------------------------------------------------------------------*/
#define SMP_STATE_NOEX()                                                        \
    (RETN_NOEX)

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Vraca dispeceru informaciju da je dogadjaj opsluzen.
 * @details     Ovaj makro samo obavestava dispecer da je dogadjaj opsluzen i ne
 *              treba da se izvrsi promena stanja.
 *//*--------------------------------------------------------------------------------------------*/
#define SMP_STATE_HANDLED()                                                     \
    (RETN_HANDLED)

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Vraca dispeceru informaciju da je dogadjaj ignorisan.
 * @details     Dogadjaj se ignorise i ne dolazi do promene stanja automata.
 *//*--------------------------------------------------------------------------------------------*/
#define SMP_STATE_IGNORED()                                                     \
    (RETN_IGNORED)

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*===============================================================================  DATA TYPES  ==*/
/*-------------------------------------------------------------------------------------------*//**
 * @brief       Nabrajanje odgovora state handler funkcije.
 *
 *              State handler funkcija preko ovih nabrajanja govori SMP
 *              dispeceru da li treba da se preuzme neka akcija kao odgovor na
 *              dogadjaj.
 *//*--------------------------------------------------------------------------------------------*/
typedef enum smp_state {
/**
 * @brief       Vraca se koje je super stanje date state handler funkcije.
 *
 *              Ova vrednost se vraca kada state handler funkcija ne zna da
 *              obradi neki dogadjaj ili je od nje zahtevano da vrati koje je
 *              njeno super stanje.
 */
    RETN_SUPER,

/**
 * @brief       Treba izvrsiti tranziciju ka drugom stanju.
 *
 *              Akcija koja je potrebna za odgovor na dogadjaj je tranzicija ka
 *              drugom stanju.
 */
    RETN_TRAN,

/**
 * @brief       Ne treba izvrsiti nikakve dalje akcije.
 *
 *              Ovo je odgovor state handler funkcije da je potpuno opsluzila
 *              dogadjaj i nikakve dodatne akcije ne treba da se preduzmu.
 */
    RETN_HANDLED,

/**
 * @brief       (NO Events Execution) Treba izvrsiti tranziciju bez dogadjaja.
 *
 *              Odgovor koji se vraca kada izvrsna jedinica zahteva od dispecera
 *              da nastavi sa izvrsavanjem i bez novih dogadjaja.
 */
    RETN_NOEX,

/**
 * @brief       Treba odloziti pristigli dogadjaj.
 *
 *              Sistem ce predati dogadjaj vratiti ponovo u red za cekanje za
 *              dogadjaje.
 */
    RETN_DEFERRED,

/**
 * @brief       Pristigli dogadjaj nije obradjen i ignorisan je.
 *
 *              Obicno se ovakav odgovor u top state-u automata i koristi se u
 *              svrhe debagiranja sistema.
 */
    RETN_IGNORED
} esState_T;

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Tip pokazivaca na state handler funkcije.
 *
 *              Funkcije vracaju esState_T , a kao parametar prihvataju
 *              void pokazivac na strukturu izvrsne jedinice i pokazivac na
 *              dogadjaje ili sam dogadjaj.
 *//*--------------------------------------------------------------------------------------------*/
typedef esState_T (* esPtrState_T) (esEpaHeader_T *, esEvtHeader_T *);

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Struktura HSM izvrsne jedinice
 *//*--------------------------------------------------------------------------------------------*/
typedef struct hsmExec {
/**
 * @brief       Niz za cuvanje izvornih stanja HSM automata
 */
    esPtrState_T        * pSrcStates;

/**
 * @brief       Niz za cuvanje odredisnih stanja HSM automata
 */
    esPtrState_T        * pDstStates;
} hsmExec_T;

/*=========================================================================  GLOBAL VARIABLES  ==*/
/*======================================================================  FUNCTION PROTOTYPES  ==*/
/*-------------------------------------------------------------------------------------------*//**
 * @name Funkcije za rad sa HSM izvrsnom jedinicom
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Pokrece dati HSM automat.
 * @param       aEpa                    Pokazivac na strukturu HSM automata,
 * @param       aEvt                    podatak/pokazivac na podatak dogadjaja.
 * @details     Ovu funkcija se pokrece nakon zakljucivanja da je dati
 *              automat spreman za rad. Dispecer pokrece stateHandler funkcije i
 *              ispituje njihovu povratnu vrednost. U zavisnosti od povratne
 *              vrednosti funkcije stanja on preduzima dodatne akcije. Kada je
 *              zavrsena obrada dogadjaja, dispecer postavlja prazan signal
 *              (SIG_EMPTY) u pokazivac dogadjaja cime se govori da je zavrsena
 *              obrada prethodnog dogadjaja i da je automat spreman da prihvati
 *              nov dogadjaj.
 * @note        Ukoliko funkcija stanja, u koju se najskorije uslo prilikom
 *              obrade tranzicije, umesto @ref RETN_HANDLED vrati
 *              @ref RETN_NOEX onda se ne izvrsava inicijalizacija vec se vrsi
 *              kreiranje @ref SIG_NOEX dogadjaja. Drugim recima, obrada
 *              flowchart-a ima prioritet u odnosu na inicijalizaciju.
 * @todo        Prvu petlju prebaciti u do...while() i izbaciti iz nje poredjenje.
 * @todo        Iz svih while() petlja izbaci poredjenje.
 * @api
 *//*--------------------------------------------------------------------------------------------*/
void hsmDispatch(
    esEpaHeader_T       * aEpa,
    const esEvtHeader_T * aEvt);

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Da li je automat aEpa u datom aState stanju?
 * @param       aEpa                    Pokazivac na strukturu HSM automata,
 * @param       aState                  stanje automata koje se ispituje.
 * @return                              Da li je automat u tom stanju ili
 *                                      podstanju.
 * @retval      TRUE                    Automat je u tom stanju ili podstanju.
 * @retval      FALSE                   Automat nije u tom stanju.
 * @details     Vraca da li je @c aEpa u navedenom stanju ili podstanju
 *              @c aState. Funkcija za vreme ispitivanja automata @c aEpa
 *              zabranjuje prekide, da se ne bi istovremeno pokrenuo automat u
 *              toku ispitivanja. Kada se zavrsi sa ispitivanjem automata,
 *              funkcija vraca automat u stanje koje je zatekla, istovremeno,
 *              omogucavajuci prekide.
 * @api
 *//*--------------------------------------------------------------------------------------------*/
bool_T esHsmIsInState(
    esEpaHeader_T       * aEpa,
    esPtrState_T        aState);

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Najvisi nivo u hijerarhiji HSM automata.
 * @param       aEpa                    Pokazivac na strukturu HSM automata,
 * @param       aEvt                    pokazivac/podatak na strukturu dogadjaja.
 * @return      esState_T             status funkcije.
 * @note        Uvek vraca odgovor IGNORED, jer ona ne prihvata nikakav
 *              dogadjaj.
 * @api
 *//*--------------------------------------------------------------------------------------------*/
esState_T esHsmTopState(
    esEpaHeader_T       * aEpa,
    esEvtHeader_T       * aEvt);

/** @} *//*--------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*===================================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/

/** @endcond *//** @} *//*************************************************************************
 * END of sproc.h
 *************************************************************************************************/
#endif /* SPROC_H_ */
