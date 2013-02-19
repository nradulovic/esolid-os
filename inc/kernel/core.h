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
 * ------------------------------------------------------------------------------------------------
 * @addtogroup  kernel_intf
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
/*-------------------------------------------------------------------------------------------*//**
 * @brief       Stanje kernel-a
 *//*--------------------------------------------------------------------------------------------*/
typedef enum esKernelStatus {
/**
 * @brief       Kernel se ne izvrsava
 */
    KERNEL_STOPPED,

/**
 * @brief       Kernel se izvrsava
 */
    KERNEL_RUNNING
} esKernelStatus_T;

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Definiciona struktura koja opisuje jedan EPA objekat
 * @api
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
} esEpaDef_T;

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Kontrolna struktura kernel-a
 * @notapi
 *//*--------------------------------------------------------------------------------------------*/
struct kernelCtrl {
#if defined(OPT_KERNEL_SCHEDULER_PREEMPTIVE) || defined(__DOXYGEN__)
/**
 * @brief       Lista EPA objekata sa istim prioritetom
 */
    esDlsList_T     list;
#endif

/**
 * @brief       Prioritet EPA objekta.
 * @details     Ova promenljiva odredjuje prioritet datog EPA objekta.
 */
    uint_fast8_t    prio;
};

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Interni podaci EPA objekta
 * @notapi
 *//*--------------------------------------------------------------------------------------------*/
struct epaIntr {
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

/**
 * @brief       Kontrolna struktura kernel-a
 */
    struct kernelCtrl kernCtrl;
};

/*-------------------------------------------------------------------------------------------*//**
 * @extends     epaIntr
 * @brief       Zaglavlje Event Processing Agent objekta
 * @details     EPA objekat se sastoji od internih podataka (epaIntr) i
 *              podataka o trenutnom stanju automata.
 * @api
 *//*--------------------------------------------------------------------------------------------*/
struct esEpaHeader {
/**
 * @brief       Interni podaci EPA objekta
 */
    struct epaIntr  internals;

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
 * @name        Osnovne funkcije kernal-a
 * @brief       Ove funkcije se koriste za upravljanjem kernel-om
 * @{ *//*---------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @brief       Inicijalizacija kernel-a
 * @param       aMemory                 Pokazivac na memorijski prostor
 *                                      rezervisan za rad eSolid kernel-a,
 * @param       aMemorySize             velicina rezervisanog memorijskog
 *                                      prostora,
 * @param       aHeapSize               deo rezervisanog memorijskog prostora
 *                                      koji treba da se odvoji za dinamicki
 *                                      alokator.
 * @api
 *//*--------------------------------------------------------------------------------------------*/
void esKernelInit(
    void                * aMemory,
    size_t              aMemorySize,
    size_t              aHeapSize);

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Pokrece izvrsavanje jezgra
 * @pre         Da bi se koristila kernel funkcija mora se definisati
 * @api
 *//*--------------------------------------------------------------------------------------------*/
void esKernelStart(
    void);

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Vraca odgovor da li trenutno radi kernel
 * @return      Status izvrsavanja kernel-a.
 *  @retval     KERNEL_STOPPED - kernel se ne izvrsava,
 *  @retval     KERNEL_RUNNING - kernel se izvrsava,
 * @api
 *//*--------------------------------------------------------------------------------------------*/
esKernelStatus_T esKernelStatus(
    void);

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Vraca znakovni niz koji identifikuje eSolid kernel
 * @return      Verzija kernel-a u obliku znakovnog niza.
 * @api
 *//*--------------------------------------------------------------------------------------------*/
const C_ROM char * esKernelSysVer(
    void);

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Osnovne funkcije za menadzment EPA objekata
 * @{ *//*---------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @brief       Kreira EPA objekat.
 * @param       aMemClass               Klasa memorije koja se koristi za
 *                                      skladistenje:
 *  @arg        esMemHeapClass
 *  @arg        esMemPoolClass
 *  @arg        esMemStaticClass
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
uint8_t esEpaPrioGet(
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
