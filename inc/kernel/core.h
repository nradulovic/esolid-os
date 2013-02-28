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
 * @brief       Osnovne funkcije kernel-a
 * @addtogroup  core_intf
 ****************************************************************************************//** @{ */


#ifndef CORE_H_
#define CORE_H_

/*============================================================================  INCLUDE FILES  ==*/
/*==================================================================================  DEFINES  ==*/
/*==================================================================================  MACRO's  ==*/

/*-------------------------------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*===============================================================================  DATA TYPES  ==*/

/**
 * @brief       Definiciona struktura koja opisuje jedan EPA objekat
 * @api
 */
struct esEpaDef {
/**
 * @brief       Deskriptivno ime EPA objekta
 * @details     Pokazivac na znakovni niz koji cuva ime jedinice za obradu
 *              dogadjaja. Ime jedinice za obradu dogadjaja se moze cuvati samo
 *              u flash/ROM memoriji. Ime pruza samo deskriptivnu informaciju o
 *              EPA objektu i u druge svrhe se ne koristi. Vise EPA objekata
 *              mogu imati isto ime.
 */
    const C_ROM char * epaName;

/**
 * @brief       Prioritet EPA objekta
 */
    uint8_t         epaPrio;

/**
 * @brief       Potrebna memorija radnog okruzenja za EPA objekat
 */
    size_t          epaWorkspaceSize;

/**
 * @brief       Velicina reda za cekanje za dogadjaje
 */
    size_t          evtQueueDepth;

/**
 * @brief       Inicijalno stanje HSM automata
 */
    esPtrState_T    hsmInitState;

/**
 * @brief       Maksimalna dubina hijerarhije stanja HSM automata.
 */
    uint8_t         hsmStateDepth;
};

/**
 * @extends     smExec
 * @brief       Zaglavlje Event Processing Agent objekta
 * @details     EPA objekat se sastoji od internih podataka koji se nalaze u
 *              ovoj strukturi i korisničkih podataka koji se dodaju na ovu
 *              strukturu.
 * @api
 */
struct esEpaHeader {
/**
 * @brief       Struktura izvrsne jedinice.
 * @details     Strukturu izvrsne jedinice koju definise SMP modul i pristup
 *              podacima ove strukture je zabranjen drugim modulima.
 */
    struct smExec   exec;

/**
 * @brief       Red cekanja za dogadjaje.
 */
    struct evtQueue evtQueue;

#if defined(OPT_KERNEL_USE_DYNAMIC) || defined(__DOXYGEN__)
/**
 * @brief       Memorijska klasa EPA objekta
 */
    const C_ROM struct esMemClass * memClass;
#endif

/**
 * @brief       Prioritet EPA objekta.
 * @details     Ova promenljiva odredjuje prioritet datog EPA objekta.
 */
    uint_fast8_t    prio;

/**
 * @brief       Ime EPA objekta
 */
    const C_ROM char * name;
};

/*=========================================================================  GLOBAL VARIABLES  ==*/
/*======================================================================  FUNCTION PROTOTYPES  ==*/

/*-------------------------------------------------------------------------------------------*//**
 * @name        Osnovne funkcije za menadzment EPA objekata
 * @{ *//*---------------------------------------------------------------------------------------*/

/**
 * @brief       Kreira EPA objekat.
 * @param       [in] aMemClass          Klasa memorije koja se koristi za
 *                                      skladistenje:
 *  @arg        esMemHeapClass
 *  @arg        esMemStaticClass
 * @param       [in] aDescription       pokazivac na opisnu strukturu EPA objekta.
 * @return      Pokazivac na strukturu zaglavlja EPA objekta.
 * @see         esEpaDef_T
 * @details     Nakon dobavljanja odgovarajuceg memorijskog prostora ova
 *              funkcija poziva esEpaInit() sa odgovarajucim parametrima.
 * @api
 */
esEpaHeader_T * esEpaCreate(
    const C_ROM esMemClass_T * aMemClass,
    const C_ROM esEpaDef_T * aDescription);

/**
 * @brief       Inicijalizuje EPA objekat
 * @param       [out] aEpa              Pokazivac na strukturu EPA objekta,
 * @param       [in] aStateBuff         memorija za cuvanje stanja HSM automata,
 * @param       [in] aEvtBuff           memorija za cuvanje reda za cekanje,
 * @param       [in] aDescription       pokazivac na opisnu strukturu EPA
 *                                      objekta.
 * @details     Ova funkcija se poziva od strane esEpaCreate, a ovde je
 *              stavljena na raspolaganju naprednijim korisnicima koji zele vecu
 *              kontrolu nad rasporedjivanjem memorijskog prostora.
 * @api
 */
void esEpaInit(
    esEpaHeader_T       * aEpa,
    esPtrState_T        * aStateBuff,
    esEvtHeader_T       ** aEvtBuff,
    const C_ROM esEpaDef_T * aDescription);

/**
 * @brief       Unistava EPA objekat.
 * @param       [in] aEpa               Pokazivac na strukturu EPA objekta.
 * @details     Vrsi se oslobadjanje memorije ukoliko je EPA objekat koristio
 *              dinamicki memorijski alokator.
 * @api
 */
void esEpaDestroy(
    esEpaHeader_T       * aEpa);

/**
 * @brief       Vrsi deinicijalizaciju koriscenih struktura
 * @param       [out] aEpa              Pokazivac na strukturu EPA objekta.
 * @api
 */
void esEpaDeInit(
    esEpaHeader_T       * aEpa);

/**
 * @brief       Vraca Id pokazivac EPA objekta.
 * @return      Id pokazivac trenutnog EPA objekta koji se izvrsava.
 * @api
 */
esEpaHeader_T * esEpaHeaderGet(
    void);

/**
 * @brief       Dobavlja prioritet EPA objekta
 * @param       [in] aEpa               Pokazivac na EPA objekat
 * @return      Trenutni prioritet EPA objekta.
 * @api
 */
uint8_t esEpaPrioGet(
    const esEpaHeader_T * aEpa);

/**
 * @brief       Postavlja nov prioritet EPA objekta.
 * @param       [out] aEpa              Pokazivac na EPA objekat,
 * @param       [in] aNewPrio           nov prioritet EPA objekta.
 * @warning     Ukoliko je zahtevani prioritet vec zauzet javice se assert
 *              obavestenje.
 * @api
 */
void esEpaPrioSet(
    esEpaHeader_T       * aEpa,
    uint8_t             aNewPrio);

/** @} *//*--------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*===================================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/

/** @endcond *//** @} *//*************************************************************************
 * END of core.h
 *************************************************************************************************/
#endif /* CORE_H_ */
