/*************************************************************************************************
 * This file is part of eSolid
 *
 * Copyright (C) 2011 - Nenad Radulovic
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
 * @author      nesa
 *
 * @brief   	Interfejs State Machine Processor (SMP) modula.
 *
 * ------------------------------------------------------------------------------------------------
 *
 * @addtogroup  smp_intf
 *
 ****************************************************************************************//** @{ */


#ifndef SMP_H_
#define SMP_H_


/*************************************************************************************************
 * INCLUDE FILES
 *************************************************************************************************/

/*-----------------------------------------------------------------------------------------------*
 * Konfiguracija i adaptacija modula
 *-----------------------------------------------------------------------------------------------*/

#include "core/cfg/smp_cfg.h"


/*-----------------------------------------------------------------------------------------------*
 * EXTERNS
 *-----------------------------------------------------------------------------------*//** @cond */

#ifdef SMP_H_VAR
#define SMP_H_EXT
#else
#define SMP_H_EXT extern
#endif


/** @endcond*//***********************************************************************************
 * DEFINES
 *************************************************************************************************/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Nabrajanja tipova dogadjaja (SMP sloj).
 */
/*-----------------------------------------------------------------------------------------------*/
enum smp_evtType {
/**
 * @brief       Rezervisani tip dogadjaja.
 */
    EVT_TYPE_RESERVED1 = EOT_TYPE_USR,

/**
 * @brief       Rezervisani tip dogadjaja.
 */
    EVT_TYPE_RESERVED2,

/**
 * @brief       Domen korisnickih tipova dogadjaja.
 */
    SMP_TYPE_USR
};

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Signali koje SMP interno salje state handler funkcijama.
 *
 *              Obicno se opsluzivanje SIG_SUPER signala vrsi u @c default
 *              slucaju @c switch - @c case strukture.
 *
 *              Ostali signali se opsluziju prema zelji korisnika.
 *
 *              Nakon signala SMP_SIG_USR korisnik moze da stavi stavi svoje
 *              signale.
 *
 *              Primer kako definisati svoje signale:
 * @include     smp_usr_signals.c
 */
/*-----------------------------------------------------------------------------------------------*/
enum smp_evtID {
/**
 * @brief       Prazan signal.
 *
 *              Koristi se da se detektuje da li je automat spreman da prihvati
 *              nov dogadjaj.
 */
    SIG_EMPTY,

/**
 * @brief       Zahteva se entry obrada u datom stanju.
 */
    SIG_ENTRY,

/**
 * @brief       Zahteva se exit obrada u datom stanju.
 */
    SIG_EXIT,

/**
 * @brief       Zahteva se inicijalizaciona (init) putanja.
 */
    SIG_INIT,

/**
 * @brief       Zahteva se obrada flowchart-a.
 *
 *              Ovaj signal se salje nakon detektovanja @ref RETN_NOEX.
 */
    SIG_NOEX,

/**
 * @brief       Zahteva se super stanje.
 *
 *              Od funkcije stanja (u aplikaciji) se zahteva koje je njeno
 *              super stanje. Funkcija mora da vrati njeno super stanje u
 *              @c stHandler promenljivoj.
 */
    SIG_SUPER,
/**
    EPA_GET_LIST = 6
    EPA_GET_INFO = 7
    EPA_LIST     = 8
    EPA_INFO     = 9
   @todo Treba prebaciti ove konstante u module koje ih definisu po prirodi
   */
/**
 * @brief       Domen korisnickih signala.
 *
 *              Nakon ovog signala korisnik definise svoje, aplikacione signale.
 */
    SMP_SIG_USR
};


/*************************************************************************************************
 * MACRO's
 *************************************************************************************************/

/*-------------------------------------------------------------------------------------------*//**
 * @name        Makroi za tranziciju stanja
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Vraca dispeceru informaciju da treba da se izvrsi tranzicija.
 *
 *              Makro koristi binarni operator @a zarez (,) koji grupise izraze
 *              sa leva na desno. Vrednost i tip celokupnog izraza je vrednost i
 *              tip desnog izraza.
 *
 * @param       hsm                     Pokazivac na strukturu HSM automata,
 * @param       state                   naredno stanje automata.
 */
/*-----------------------------------------------------------------------------------------------*/
#define SMP_STATE_TRAN(hsm, state)                                              \
    (((smp_exec_T *)(hsm))->pStateHandler = (smp_ptrState_T)(state), RETN_TRAN)

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Vraca dispeceru informaciju o super stanju trenutnog stanja.
 *
 *              Makro koristi binarni operator @a zarez (,) koji grupise izraze
 *              sa leva na desno. Vrednost i tip celokupnog izraza je vrednost i
 *              tip desnog izraza.
 *
 * @param       hsm                     Pokazivac na strukturu HSM automata,
 * @param       state                   super stanje trenutnog stanja.
 *
 * @note        Koristi se samo kod HSM automata.
 */
/*-----------------------------------------------------------------------------------------------*/
#define SMP_STATE_SUPER(hsm, state)                                             \
    (((smp_exec_T *)(hsm))->pStateHandler = (smp_ptrState_T)(state), RETN_SUPER)

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Vraca dispeceru informaciju da treba da se izvrsi inicijalna
 *              tranzicija.
 *
 * @param       hsm                     Pokazivac na strukturu HSM automata,
 * @param       state                   naredno stanje automata.
 */
/*-----------------------------------------------------------------------------------------------*/
#define SMP_STATE_INIT(hsm, state)                                              \
    (((smp_exec_T *)(hsm))->pStateHandler = (smp_ptrState_T)(state), RETN_INIT)

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Vraca dispeceru informaciju da automat ulazi u flowchart deo
 *              dijagrama stanja.
 *
 *              Ovaj makro se koristi kada se zeli tranzicija u sledece stanje
 *              bez dogadjaja. Naime, kada se ovaj makro koristi, dispecer sam
 *              generise @ref SIG_NOEX dogadjaj tako da automat moze da obradi
 *              flowchart bez spoljasnih dogadjaja. Flowchart se pise u odeljku
 *              koji obradjuje SIG_NOEX, dok se u SIG_ENTRY odeljku samo
 *              koristi ovaj makro.
 */
/*-----------------------------------------------------------------------------------------------*/
#define SMP_STATE_NOEX()                                                        \
    (RETN_NOEX)

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Vraca dispeceru informaciju da je dogadjaj opsluzen.
 *
 *              Ovaj makro samo obavestava dispecer da je dogadjaj opsluzen i ne
 *              treba da se izvrsi promena stanja.
 */
/*-----------------------------------------------------------------------------------------------*/
#define SMP_STATE_HANDLED()                                                     \
    (RETN_HANDLED)

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Vraca dispeceru informaciju da je dogadjaj ignorisan.
 *
 *              Dogadjaj se ignorise i ne dolazi do promene stanja automata.
 */
/*-----------------------------------------------------------------------------------------------*/
#define SMP_STATE_IGNORED()                                                     \
    (RETN_IGNORED)

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

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Nabrajanje odgovora state handler funkcije.
 *
 *              State handler funkcija preko ovih nabrajanja govori SMP
 *              dispeceru da li treba da se preuzme neka akcija kao odgovor na
 *              dogadjaj.
 */
/*-----------------------------------------------------------------------------------------------*/
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
} smp_state_T;

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Tip pokazivaca na state handler funkcije.
 *
 *              Funkcije vracaju smp_state_T , a kao parametar prihvataju
 *              void pokazivac na strukturu izvrsne jedinice i pokazivac na
 *              dogadjaje ili sam dogadjaj.
 *
 *              Tip pokazivaca zavisi od reprezentacije dogadjaja koja se bira
 *              pretprocesorskom promenljivom OPT_EOT_EVT_T.
 *
 * @see         eot_evtExt
 */
/*-----------------------------------------------------------------------------------------------*/
typedef smp_state_T (* smp_ptrState_T) (void *, const eot_evt_T *);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Sistemski dogadjaj
 */
/*-----------------------------------------------------------------------------------------------*/
typedef struct smp_evtSystem {
/**
 * @brief       Super struktura zaglavlja dogadjaja
 */
    eot_evt_T   super;
} smp_evtSystem_T;

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Klasa sistemskih dogadjaja
 */
/*-----------------------------------------------------------------------------------------------*/
extern const C_ROM eot_evtClass_T evtClassSystem;

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Struktura HSM izvrsne jedinice
 */
/*-----------------------------------------------------------------------------------------------*/
typedef struct smp_exec {
/**
 * @brief       Pokazivac na state handler funkciju.
 *
 *              Ovaj pokazivac pokazuje na funkciju stanja koja vrsi obradu
 *              dogadjaja.
 */
    smp_ptrState_T  pStateHandler;

/**
 * @brief       Niz za cuvanje izvornih stanja HSM automata
 */
    smp_ptrState_T  * pSrcStates;

/**
 * @brief       Niz za cuvanje odredisnih stanja HSM automata
 */
    smp_ptrState_T  * pDstStates;
} smp_exec_T;


/*************************************************************************************************
 * GLOBAL VARIABLES
 *************************************************************************************************/


/*************************************************************************************************
 * FUNCTION PROTOTYPES
 *************************************************************************************************/

/*-------------------------------------------------------------------------------------------*//**
 * @name Funkcije za rad sa HSM izvrsnim jedinicama
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Kreira HSM automat
 *
 *              Kreira odgovarajuci memorijski prostor za automat i poziva
 *              konstruktor funkciju.
 *
 * @param       aMemClass               Klasa memorije koja se koristi za
 *                                      skladistenje:
 *  @arg        memHeapClass
 *  @arg        memPoolClass
 *  @arg        memStaticClass
 * @param       aHsm                    Pokazivac na strukturu HSM automata,
 * @param       aInitState              pokazivac na inicijalno stanje automata,
 * @param       aStateDepth             maksimalna hijerarhijska dubina stanja
 *                                      automata.
 * @return      Pokazivac na tek kreiran i konstruisan HSM automat.
 * @api
 * @todo        Napisati funkciju
 */
/*-----------------------------------------------------------------------------------------------*/
void eS_hsmCreate(
    smm_memClass_T  * aMemClass,
    smp_exec_T      * aHsm,
    smp_ptrState_T  aInitState,
    uint8_t         aStateDepth);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Brise HSM automat
 *
 * @param       aHsm                    Pokazivac na HSM automat koji treba da
 *                                      se izbrise.
 * @api
 * @todo        Napisati funkciju
 */
/*-----------------------------------------------------------------------------------------------*/
void eS_hsmDestroy(
    smp_exec_T      * aHsm);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Pokrece dati HSM automat.
 *
 *              Ovu funkcija se pokrece nakon zakljucivanja da je dati
 *              automat spreman za rad. Dispecer pokrece stateHandler funkcije i
 *              ispituje njihovu povratnu vrednost. U zavisnosti od povratne
 *              vrednosti funkcije stanja on preduzima dodatne akcije. Kada je
 *              zavrsena obrada dogadjaja, dispecer postavlja prazan signal
 *              (SIG_EMPTY) u pokazivac dogadjaja cime se govori da je zavrsena
 *              obrada prethodnog dogadjaja i da je automat spreman da prihvati
 *              nov dogadjaj.
 *
 * @param       aHsm                    Pokazivac na strukturu HSM automata,
 * @param       aEvt                    podatak/pokazivac na podatak dogadjaja.
 * @return      U slucaju da automat ne moze trenutno da obradi dogadjaj ili
 *              treba da obradi jos neki dogadjaj, dispecer vraca pokazivac na
 *              taj dogadjaj kako bi se stavio u red cekanja.
 *  @retval     NULL - automat je zavrsio obradu dogadjaja,
 *  @retval     !NULL - automat nije zavrsio obradu dogadjaja, pozvace
 *              se opet prilikom sledece iteracije.
 * @note        Ukoliko funkcija stanja, u koju se najskorije uslo prilikom
 *              obrade tranzicije, umesto @ref RETN_HANDLED vrati
 *              @ref RETN_NOEX onda se ne izvrsava inicijalizacija vec se vrsi
 *              kreiranje @ref SIG_NOEX dogadjaja. Drugim recima, obrada
 *              flowchart-a ima prioritet u odnosu na inicijalizaciju.
 *
 * @todo        Prvu petlju prebaciti u do...while() i izbaciti iz nje poredjenje.
 * @todo        Indeksi trebaju da budu 32-bitni!
 * @todo        Iz svih while() petlja izbaci poredjenje.
 * @api
 */
/*-----------------------------------------------------------------------------------------------*/
eot_evt_T * eS_hsmDispatch(
    smp_exec_T      * aHsm,
    const eot_evt_T * aEvt);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Da li je automat aHsm u datom aState stanju?
 *
 *              Vraca da li je @c aHsm u navedenom stanju ili podstanju
 *              @c aState. Funkcija za vreme ispitivanja automata @c aHsm
 *              zabranjuje prekide, da se ne bi istovremeno pokrenuo automat u
 *              toku ispitivanja. Kada se zavrsi sa ispitivanjem automata,
 *              funkcija vraca automat u stanje koje je zatekla, istovremeno,
 *              omogucavajuci prekide.
 *
 * @param       aHsm                    Pokazivac na strukturu HSM automata,
 * @param       aState                  stanje automata koje se ispituje.
 *
 * @return                              Da li je automat u tom stanju ili
 *                                      podstanju.
 * @retval      TRUE                    Automat je u tom stanju ili podstanju.
 * @retval      FALSE                   Automat nije u tom stanju.
 * @api
 */
/*-----------------------------------------------------------------------------------------------*/
bool_T eS_hsmIsInState(
    smp_exec_T      * aHsm,
    smp_ptrState_T  aState);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Najvisi nivo u hijerarhiji HSM automata.
 *
 *              Uvek vraca odgovor IGNORED, jer ona ne prihvata nikakav
 *              dogadjaj.
 *
 * @param       aHsm                    Pokazivac na strukturu HSM automata,
 * @param       aEvt                    pokazivac/podatak na strukturu dogadjaja.
 * @return      smp_state_T             status funkcije.
 * @api
 */
/*-----------------------------------------------------------------------------------------------*/
smp_state_T eS_hsmTopState(
    smp_exec_T      * aHsm,
    const eot_evt_T * aEvt);

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
 * END of smp.h
 *************************************************************************************************/
#endif /* SMP_H_ */
