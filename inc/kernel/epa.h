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
 * @author  	Nenad Radulovic
 * @brief       EPA menadzment
 * ------------------------------------------------------------------------------------------------
 * @addtogroup  epa_intf
 * @brief       EPA menadzment
 ****************************************************************************************//** @{ */


#ifndef EPA_H_
#define EPA_H_

/*============================================================================  INCLUDE FILES  ==*/
/*==================================================================================  DEFINES  ==*/
/*==================================================================================  MACRO's  ==*/
/*-------------------------------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*===============================================================================  DATA TYPES  ==*/
/*-------------------------------------------------------------------------------------------*//**
 * @brief       Struktura koja opisuje jedan EPA objekat
 *//*--------------------------------------------------------------------------------------------*/
typedef struct esEpaDef {
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
    size_t          epaMemory;

/**
 * @brief       Velicina reda za cekanje za dogadjaje
 */
    size_t          evtQueueSize;

/**
 * @brief       Inicijalno stanje HSM automata
 */
    esPtrState_T    hsmInitState;

/**
 * @brief       Maksimalna dubina hijerarhije stanja HSM automata.
 */
    uint8_t         hsmStateDepth;
} esEpaDef_T;

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Interni podaci EPA objekta
 *//*--------------------------------------------------------------------------------------------*/
typedef struct epaIntr {
#if defined(OPT_KERNEL_USE_DYNAMIC) || defined(__DOXYGEN__)
/**
 * @brief       Memorijska klasa EPA objekta
 */
    const C_ROM esMemClass_T  * memClass;
#endif

/**
 * @brief       Struktura izvrsne jedinice.
 * @details     Strukturu izvrsne jedinice koju definise SMP modul i pristup
 *              podacima ove strukture je zabranjen drugim modulima.
 */
    hsmExec_T       exec;

/**
 * @brief       Red cekanja za dogadjaje.
 */
    evtQueue_T      evtQueue;

#if defined(OPT_KERNEL_ENABLE) || defined(__DOXYGEN__)
/**
 * @brief       Kontrolna struktura kernel-a
 */
    kernelCtrl_T    kernCtrl;

# if defined(OPT_KERNEL_USE_REGISTRY) || defined(__DOXYGEN__)
/**
 * @brief       Registar struktura EPA objekata
 * @details     Registar omogucava dodatnu prezentaciju informacija o EPA
 *              objektima koji se nalaze u sistemu.
 */
    regNode_T       registry;
# endif
#endif
} epaIntr_T;

/*-------------------------------------------------------------------------------------------*//**
 * @extends     epaIntr_T
 * @brief       Zaglavlje Event Processing Agent objekta
 * @details     EPA objekat se sastoji od internih podataka (epaIntr_T) i
 *              podataka o trenutnom stanju automata.
 *//*--------------------------------------------------------------------------------------------*/
struct esEpaHeader {
/**
 * @brief       Interni podaci EPA objekta
 */
    epaIntr_T       internals;

/**
 * @brief       Pokazivac na state handler funkciju.
 * @details     Ovaj pokazivac pokazuje na funkciju stanja koja vrsi obradu
 *              dogadjaja.
 */
    esPtrState_T    pState;
};

/*=========================================================================  GLOBAL VARIABLES  ==*/
/*======================================================================  FUNCTION PROTOTYPES  ==*/
/*-------------------------------------------------------------------------------------------*//**
 * @brief       Kreira EPA objekat.
 * @param       aMemClass               Klasa memorije koja se koristi za
 *                                      skladistenje:
 *  @arg        memHeapClass
 *  @arg        memPoolClass
 *  @arg        memStaticClass
 * @param       aDescription            pokazivac na opis EPA objekta.
 * @return      Pokazivac na strukturu EPA objekta.
 * @see         esEpaDef_T
 * @details     Nakon dobavljanja odgovarajuceg memorijskog prostora ova
 *              funkcija poziva esEpaInit() sa odgovarajucim parametrima.
 * @api
 *//*--------------------------------------------------------------------------------------------*/
esEpaHeader_T * esEpaCreate(
    const C_ROM esMemClass_T * aMemClass,
    const C_ROM esEpaDef_T * aDescription);

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Inicijalizuje EPA objekat
 * @param       aEpa                    Pokazivac na strukturu EPA objekta,
 * @param       aStateBuff              memorija za cuvanje stanja HSM automata,
 * @param       aEvtBuff                memorija za cuvanje reda za cekanje,
 * @param       aDescription            pokazivac na opisnu strukturu EPA
 *                                      objekta.
 * @details     Ova funkcija se poziva od strane esEpaCreate, a ovde je
 *              stavljena na raspolaganju naprednijim korisnicima koji zele vecu
 *              kontrolu nad rasporedjivanjem memorijskog prostora.
 * @api
 *//*--------------------------------------------------------------------------------------------*/
void esEpaInit(
    esEpaHeader_T       * aEpa,
    esPtrState_T        * aStateBuff,
    esEvtHeader_T       ** aEvtBuff,
    const C_ROM esEpaDef_T * aDescription);

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Unistava EPA objekat.
 * @param       aEpa                    Pokazivac na strukturu EPA objekta.
 * @api
 *//*--------------------------------------------------------------------------------------------*/
void esEpaDestroy(
    esEpaHeader_T       * aEpa);

void esEpaDeInit(
    esEpaHeader_T       * aEpa);

/** @} *//*--------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Vraca Id pokazivac EPA objekta.
 * @return      Id pokazivac trenutnog EPA objekta koji se izvrsava.
 * @pre         Da bi se koristila kernel funkcija mora se definisati
 *              @ref OPT_KERNEL_ENABLE.
 * @api
 *//*--------------------------------------------------------------------------------------------*/
esEpaHeader_T * esEpaGetId(
    void);

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Dobavlja prioritet EPA objekta
 * @param       aEpa                    Pokazivac na EPA objekat
 * @return      Trenutni prioritet EPA objekta.
 * @pre         Da bi se koristila kernel funkcija mora se definisati
 *              @ref OPT_KERNEL_ENABLE.
 * @api
 *//*--------------------------------------------------------------------------------------------*/
esEpaPrio_T esEpaPrioGet(
    const esEpaHeader_T * aEpa);

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Postavlja nov prioritet EPA objekta.
 * @param       aEpa                    Pokazivac na EPA objekat,
 * @param       aNewPrio                nov prioritet EPA objekta.
 * @pre         Da bi se koristila kernel funkcija mora se definisati
 *              @ref OPT_KERNEL_ENABLE.
 * @warning     Ukoliko se ne koristi @ref OPT_KERNEL_USE_ROUND_ROBIN, a
 *              zahtevani prioritet je vec zauzet javice se assert obavestenje.
 * @api
 *//*--------------------------------------------------------------------------------------------*/
void esEpaPrioSet(
    esEpaHeader_T       * aEpa,
    esEpaPrio_T         aNewPrio);

/*---------------------------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*===================================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/

/** @endcond *//** @} *//*************************************************************************
 * END of epa.h
 *************************************************************************************************/
#endif /* EPA_H_ */
