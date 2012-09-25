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
 * @author      Nenad Radulovic
 * @brief       Interfejs Event Processing Engine podmodula.
 * ------------------------------------------------------------------------------------------------
 * @addtogroup  epe_intf
 ****************************************************************************************//** @{ */

#ifndef KERNEL_H_
#define KERNEL_H_


/*************************************************************************************************
 * INCLUDE FILES
 *************************************************************************************************/

/*-----------------------------------------------------------------------------------------------*
 * Zavisnosti modula
 *-----------------------------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------------------------*
 * Konfiguracija i adaptacija modula
 *-----------------------------------------------------------------------------------------------*/

#include "core/cfg/kernel_cfg.h"


/*-----------------------------------------------------------------------------------------------*
 * EXTERNS
 *-----------------------------------------------------------------------------------*//** @cond */

#ifdef KERNEL_H_VAR
#define KERNEL_H_EXT
#else
#define KERNEL_H_EXT extern
#endif


/** @endcond*//***********************************************************************************
 * DEFINES
 *************************************************************************************************/


/*************************************************************************************************
 * MACRO's
 *************************************************************************************************/


/*-----------------------------------------------------------------------------------------------*
 * C/C++ #ifdef - open
 *-----------------------------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif


/*************************************************************************************************
 * DATA TYPES
 *************************************************************************************************/

#if !defined(OPT_OPTIMIZE_SIZE) && defined(OPT_OPTIMIZE_SPEED)
typedef uint_fast8_t                    esEpaPrio_T;
#endif

#if defined(OPT_OPTIMIZE_SIZE) && !defined(OPT_OPTIMIZE_SPEED)
typedef uint8_t                         esEpaPrio_T;
#endif

#if !defined(OPT_OPTIMIZE_SIZE) && !defined(OPT_OPTIMIZE_SPEED) || defined(__DOXYGEN__)
/*-------------------------------------------------------------------------------------------*//**
 * @brief       Tip podataka za prioritet EPA jedinice
 *//*--------------------------------------------------------------------------------------------*/
typedef uint8_t                         esEpaPrio_T;
#endif

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
    size_t          epaWorkspaceSize;

/**
 * @brief       Inicijalno stanje HSM automata
 */
    esPtrState_T    hsmInitState;

/**
 * @brief       Maksimalna dubina hijerarhije stanja HSM automata.
 */
    uint8_t         hsmStateDepth;

/**
 * @brief       Velicina reda za cekanje za dogadjaje
 */
    size_t          evtQueueDepth;
} esEpaDef_T;

/*-------------------------------------------------------------------------------------------*//**
 * @ingroup     kern_intf
 * @brief       Kontrolna struktura kernel-a
 *//*--------------------------------------------------------------------------------------------*/
typedef struct kernelCtrl {
#if defined(OPT_KERNEL_USE_ROUND_ROBIN) || defined(__DOXYGEN__)
/**
 * @brief       Lista EPA objekata sa istim prioritetom
 */
    esDlsList_T     list;
#endif

/**
 * @brief       Prioritet EPA objekta.
 * @details     Ova promenljiva odredjuje prioritet datog EPA objekta.
 */
    esEpaPrio_T     prio;
} kernelCtrl_T;

/*-------------------------------------------------------------------------------------------*//**
 * @ingroup     reg_intf
 * @brief       Registar EPA objekata.
 *//*--------------------------------------------------------------------------------------------*/
typedef struct regNode {
/**
 * @brief       Lista svih EPA jedinica u sistemu
 */
    esDlsList_T    epaList;

/**
 * @brief       Opis EPA objekta.
 * @details     Pokazivac na opisnu strukturu EPA objekta koja se nalazi u
 *              ROM tipu memorije.
 */
    const C_ROM esEpaDef_T * description;

/**
 * @brief       Process ID - identifikator EPA objekta.
 * @details     Jedinstveni broj koji je vezan za jednu instancu EPA objekta.
 *              Ovaj broj se definise u procesu kreiranja EPA objekta, a
 *              automatski se dodeljuje od strane eSolid-a. Jednom definisan
 *              identifikator ostaje nepromenjen do trenutka gasenja (unistenja)
 *              EPA objekta. Ukoliko se EPA objekat ponovo instacira (kreira),
 *              njemu se dodeljuje nov jedinstven identifikator.
 */
    uint8_t         pid;

/**
 * @brief       Events Per Second
 */
    uint32_t        eps;
} regNode_T;

/*-------------------------------------------------------------------------------------------*//**
 * @ingroup     epe_impl
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
 *              podatka o trenutnom stanju automata.
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


/*************************************************************************************************
 * GLOBAL VARIABLES
 *************************************************************************************************/


/*************************************************************************************************
 * FUNCTION PROTOTYPES
 *************************************************************************************************/

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
 * @ingroup     kern_intf
 * @name        Upravljanje kernelom
 * @{ *//*---------------------------------------------------------------------------------------*/
#if defined(OPT_KERNEL_ENABLE) || defined(__DOXYGEN__)

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

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Inicijalizacija kernel-a
 * @param       aMemory                 Pokazivac na memorijski prostor
 *                                      rezervisan za rad eSolid kernel-a,
 * @param       aMemorySize             velicina rezervisanog memorijskog
 *                                      prostora,
 * @param       aHeapSize               deo rezervisanog memorijskog prostora
 *                                      koji treba da se odvoji za dinamicki
 *                                      alokator.
 * @pre         Da bi se koristila kernel funkcija mora se definisati
 *              @ref OPT_KERNEL_ENABLE.
 * @api
 *//*--------------------------------------------------------------------------------------------*/
void esKernelInit(
    void                * aMemory,
    size_t              aMemorySize,
    size_t              aHeapSize);

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Pokrece izvrsavanje jezgra
 * @pre         Da bi se koristila kernel funkcija mora se definisati
 *              @ref OPT_KERNEL_ENABLE.
 * @api
 *//*--------------------------------------------------------------------------------------------*/
void esKernelStart(
    void);

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Vraca odgovor da li trenutno radi kernel
 * @return      Status izvrsavanja kernel-a.
 *  @retval     KERNEL_STOPPED - kernel se ne izvrsava,
 *  @retval     KERNEL_RUNNING - kernel se izvrsava,
 * @pre         Da bi se koristila kernel funkcija mora se definisati
 *              @ref OPT_KERNEL_ENABLE.
 * @api
 *//*--------------------------------------------------------------------------------------------*/
esKernelStatus_T esKernelStatus(
    void);

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Vraca znakovni niz koji identifikuje eSolid kernel
 * @return      Verzija kernel-a u obliku znakovnog niza.
 * @pre         Da bi se koristila kernel funkcija mora se definisati
 *              @ref OPT_KERNEL_ENABLE.
 * @api
 *//*--------------------------------------------------------------------------------------------*/
const C_ROM char * esKernelSysVer(
    void);

#endif /* OPT_KERNEL_ENABLE */
/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @ingroup     reg_intf
 * @name        Upravljanje registar bazom
 * @note        Funkcije su dostupne samo ako su kernel i registar omoguceni.
 * @{ *//*---------------------------------------------------------------------------------------*/
# if (defined(OPT_KERNEL_ENABLE) && defined(OPT_KERNEL_USE_REGISTRY)) || defined(__DOXYGEN__)

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Pronalazi EPA objekat po @c aId broju
 * @param       aPid                    Pid broj zeljenog automata
 * @return      Pokazivac na EPA objekat ciji pid odgovara trazenom @c aPid
 *              broju.
 *  @retval     NULL ako nije pronadjen EPA objekat sa trazenim @c aPid.
 * @pre         Da bi se koristila registar funkcija mora se definisati
 *              @ref OPT_KERNEL_ENABLE i @ref OPT_KERNEL_USE_REGISTRY.
 * @todo        Napisati funkciju.
 * @api
 *//*--------------------------------------------------------------------------------------------*/
esEpaHeader_T * esRegFindByPid(
    uint8_t          aPid);

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Pronalazi EPA objekat po imenu dato u @c aName nizu.
 * @param       aName                   Ime EPA objekta ciji se pokazivac trazi.
 * @return      Pokazivac na EPA objekat cije ime odgovara trazenom imenu @c
 *              aName.
 *  @retval     NULL ako nije pronadjen EPA objekat sa trazenim @c aNeme imenom.
 * @pre         Da bi se koristila registar funkcija mora se definisati
 *              @ref OPT_KERNEL_ENABLE i @ref OPT_KERNEL_USE_REGISTRY.
 * @todo        Napisati funkciju.
 * @api
 *//*--------------------------------------------------------------------------------------------*/
esEpaHeader_T * esRegFindByName(
    const char          * aName);

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Vraca pid broj EPA objekta sa @c aEpa pokazivacem na strukturu.
 * @param       aEpa                    Pokazivac na strukturu EPA objekta.
 *  @arg        NULL - vraca pid trenutnog procesa.
 * @return      Pid broj trazenog @c aEpa objekta.
 * @pre         Da bi se koristila registar funkcija mora se definisati
 *              @ref OPT_KERNEL_ENABLE i @ref OPT_KERNEL_USE_REGISTRY.
 * @todo        Napisati funkciju.
 * @api
 *//*--------------------------------------------------------------------------------------------*/
uint8_t esRegGetPid(
    esEpaHeader_T       * aEpa);

#endif /* OPT_KERNEL_ENABLE && OPT_KERNEL_USE_REGISTRY */
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
 * END of kernel.h
 *************************************************************************************************/
#endif /* KERNEL_H_ */
