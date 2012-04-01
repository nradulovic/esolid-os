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
 * @author      nesa
 *
 * @brief       Privatni objekti EPE modula
 *
 * ------------------------------------------------------------------------------------------------
 *
 * @addtogroup  epe_impl
 *
 ****************************************************************************************//** @{ */


#ifndef EPE_PKG_H_
#define EPE_PKG_H_


/*************************************************************************************************
 * INCLUDE FILES
 *************************************************************************************************/

#include PORT_ARCH_HEADER(core/epe)


/*-----------------------------------------------------------------------------------------------*
 * EXTERNS
 *-----------------------------------------------------------------------------------*//** @cond */

#ifdef EPE_PKG_H_VAR
#define EPE_PKG_H_EXT
#else
#define EPE_PKG_H_EXT extern
#endif


/** @endcond*//***********************************************************************************
 * DEFINES
 *************************************************************************************************/


/*************************************************************************************************
 * MACRO's
 *************************************************************************************************/

/*-------------------------------------------------------------------------------------------*//**
 * @name        Debug podrska
 *
 * @brief       Makroi za debug podrsku. Pogledati @ref dbg_intf.
 *
 * @{ *//*---------------------------------------------------------------------------------------*/

#if defined(OPT_DBG_EPE)
# define EPE_ASSERT                     DBG_ASSERT
# define EPE_ASSERT_ALWAYS              DBG_ASSERT_ALWAYS
# define EPE_COMPILE_ASSERT             DBG_COMPILE_ASSERT
# define EPE_DBG_DECL                   DBG_DECL
# define EPE_DBG_DEFINE_MODULE          DBG_DEFINE_MODULE
# define EPE_DBG_ENTRY                  DBG_ENTRY
# define EPE_DBG_EXIT                   DBG_EXIT
# define EPE_DBG_MACRO                  DBG_MACRO
# define EPE_DBG_CHECK                  DBG_CHECK
#else
# define EPE_ASSERT(expr)               DBG_EMPTY_MACRO()
# define EPE_ASSERT_ALWAYS(expr)        DBG_EMPTY_MACRO()
# define EPE_COMPILE_ASSERT(expr)       DBG_EMPTY_DECL()
# define EPE_DBG_DECL(expr)             DBG_EMPTY_DECL()
# define EPE_DBG_DEFINE_MODULE(expr)    DBG_EMPTY_DECL()
# define EPE_DBG_ENTRY()                DBG_EMPTY_MACRO()
# define EPE_DBG_EXIT()                 DBG_EMPTY_MACRO()
# define EPE_DBG_MACRO(expt)            DBG_EMPTY_MACRO()
# define EPE_DBG_CHECK(expr)            DBG_EMPTY_MACRO()
#endif

/** @} *//*--------------------------------------------------------------------------------------*/

#if (OPT_EPA_PRIO_MAX < HAL_UNATIVE_BITS)
# define PRIO_INDX                      OPT_EPA_PRIO_MAX
# define PRIO_INDX_GROUP                1
#else
# define PRIO_INDX                      HAL_UNATIVE_BITS
# define PRIO_INDX_GROUP                (ES_DIV_ROUNDUP(OPT_EPA_PRIO_MAX, PRIO_INDX))
#endif

#define PRIO_INDX_PWR                   ES_UINT8_LOG2(PRIO_INDX)

/*-----------------------------------------------------------------------------------------------*
 * C/C++ #ifdef - open
 *-----------------------------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif


/*************************************************************************************************
 * DATA TYPES
 *************************************************************************************************/

#if !defined(ES_OPTIMIZE_SIZE) && defined(ES_OPTIMIZE_SPEED)
typedef uint_fast8_t                    epe_epaPid_T;
typedef uint_fast8_t                    epe_epaPrio_T;
#endif

#if defined(ES_OPTIMIZE_SIZE) && !defined(ES_OPTIMIZE_SPEED)
typedef uint8_t                         epe_epaPid_T;
typedef uint8_t                         epe_epaPrio_T;
#endif

#if !defined(ES_OPTIMIZE_SIZE) && !defined(ES_OPTIMIZE_SPEED) || defined(__DOXYGEN__)

/**
 * @brief       Tip podataka za identifikator EPA jedinice
 */
typedef uint16_t                        epe_epaPid_T;

/**
 * @brief       Tip podataka za prioritet EPA jedinice
 */
typedef uint8_t                         epe_epaPrio_T;
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Registar struktura EPA objekata.
 */
/*-----------------------------------------------------------------------------------------------*/
typedef struct epe_epaRegNode {
/**(sizeof(unative_T) * 8)
 * @brief       Lista svih EPA jedinica u sistemu
 */
    pl_dlsList_T    epaList;

/**
 * @brief       Opis EPA objekta.
 *
 *              Pokazivac na opisnu strukturu EPA objekta koja se nalazi u
 *              ROM tipu memorije.
 */
    const C_ROM epe_epaDef_T * description;

/**
 * @brief       Identifikator EPA objekta.
 *
 *              Jedinstveni broj koji je vezan za jednu instancu EPA objekta.
 *              Ovaj broj se definise u procesu kreiranja EPA objekta, a
 *              automatski se dodeljuje od strane eSolid-a. Jednom definisan
 *              identifikator ostaje nepromenjen do trenutka gasenja (unistenja)
 *              EPA objekta. Ukoliko se EPA objekat ponovo instacira (kreira),
 *              njemu se dodeljuje nov jedinstven identifikator.
 */
    epe_epaPid_T     pid;
} epe_epaRegNode_T;

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Kontrolna struktura kernel-a
 */
/*-----------------------------------------------------------------------------------------------*/
typedef struct epe_kernCtrl {
/**
 * @brief       Prioritet EPA objekta.
 *
 *              Ova promenljiva odredjuje prioritet datog EPA objekta.
 */
    epe_epaPrio_T   prio;

#if defined(OPT_EPA_USE_MUTEX) || defined(__DOXYGEN__)
/**
 * @brief       Prioritet za vreme nasledjivanja
 */
    epe_epaPrio_T   prioBase;
#endif
} epe_kernCtrl_T;

/*-----------------------------------------------------------------------------------------------*/
/**
 * @extends     smp_exec_T
 * @brief       Osnovna struktura EPA objekta.
 *
 *              Agent za obradu dogadjaj, odnosno, EPA objekat se, u sustini,
 *              sastoji od strukture izvrsne jedinice smp_exec i reda cekanja za
 *              dogadjaje. Pored ova dva elementa definisu se i ostali podaci
 *              koji cuvaju informaciju o prioritetu objekta, o njegovom statusu,
 *              identifikaciji i slicno.
 *
 *              Struktura se sastoji iz tri celine:
 *              - EPA control information
 *              - EPA state information
 *              - EPA identification information
 *
 *              <b> EPA control information </b>
 *
 *              Zadatak ove celine je da se opise struktura EPA objekta. U ovoj
 *              celini se definise:
 *              - dispecer ove izvrsne jedinice,
 *              - informacije o redu cekanja za dogadjaje,
 *              - prioritet EPA objekta.
 *
 *              <b> EPA state information </b>
 *
 * @todo        Opsirniji opis celine
 *
 *              <b> EPA identification information </b>
 *
 *              Ova celina aplikacionom programeru pruza osnovni interfejs za
 *              spregu sa EPA objektima. Preko identifikacionog broja programer
 *              moze da manipulise sa EPA objektom. Pored identifikacionog broja
 *              svaki EPA objekat se opisuje i imenom. Identifikacioni broj je
 *              jedinstven za svaku instancu EPA objekta. Imena se koriste samo
 *              u svhre lakse identifikacije EPA objekata i ne moraju da budu
 *              jedinstvena za jedan EPA objekat.
 *
 *              Primer listinga aktivnih EPA objekata sa nazivom "drvUART":
 *              @code
 *              pid:     Name:
 *              12      drvUART
 *              16      drvUART
 *              22      drvUART
 *              @endcode
 */
/*-----------------------------------------------------------------------------------------------*/
struct epe_epa {
/*-----------------------------------------------------------------------------------------------*
 *
 * EPA memory information
 *
 *-----------------------------------------------------------------------------------------------*/
#if defined(OPT_EPA_USE_DYNAMIC) || defined(__DOXYGEN__)
/**
 * @brief       Memorijska klasa EPA objekta
 */
    const C_ROM smm_memClass_T  * memClass;
#endif

/*-----------------------------------------------------------------------------------------------*
 *
 * EPA execution and dispatch
 *
 *-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Struktura izvrsne jedinice.
 *
 *              Strukturu izvrsne jedinice koju definise SMP modul i pristup
 *              podacima ove strukture je zabranjen drugim modulima.
 */
    smp_exec_T      execUnit;

/*-----------------------------------------------------------------------------------------------*
 *
 * EPA event domain
 *
 *-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Red cekanja za dogadjaje.
 *
 *              Svi agenti za obradu dogadjaja imaju redove cekanja za dogadjaje.
 *              Ova struktura se definise u EOT modulu.
 *
 * @see         eot_evtQueue_T
 */
    eot_evtQueue_T  evtQueue;

/*-----------------------------------------------------------------------------------------------*
 *
 * EPA control information
 *
 *-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Kontrolna struktura kernel-a
 */
    epe_kernCtrl_T  kernCtrl;

/*-----------------------------------------------------------------------------------------------*
 *
 * EPA identification information
 *
 *-----------------------------------------------------------------------------------------------*/
#if defined(OPT_EPA_USE_REGISTRY) || defined(__DOXYGEN__)
/**
 * @brief       Registar struktura EPA objekata
 *
 *              Registar omogucava dodatnu prezentaciju informacija o EPA
 *              objektima koji se nalaze u sistemu.
 */
    epe_epaRegNode_T registry;
#endif
};

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Struktura cuvara registra
 */
/*-----------------------------------------------------------------------------------------------*/
typedef struct epe_epaRegSentinel {
/**
 * @brief       Lista svih aktivnih EPA jedinica u sistemu
 */
    pl_dlsList_T    activeList;

/**
 * @brief       Broj aktivnih jedinica u EPA sistemu
 */
    epe_epaPid_T    activeCnt;

/**
 * @brief       Zadnja vrednost PID broja
 */
    epe_epaPid_T    lastPid;
} epe_epaRegSentinel_T;

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Struktura bitmape spremnih EPA objekata
 */
/*-----------------------------------------------------------------------------------------------*/
typedef struct epe_rdyBitmap {
/**
 * @brief       Grupa prioriteta EPA objekata
 *
 *              Prilikom trazenja sledeceg aktivnog EPA objekta prvo se
 *              pretrazuje ovaj clan.
 */
    unative_T       group;

/**
 * @brief       Prioriteti EPA objekata
 *
 *              Kad je pretragom group utvrdjeno da se ovde nalazi spreman
 *              EPA objekat, onda se pretraga nastavlja ovde.
 */
    unative_T       bits[PRIO_INDX_GROUP];
} epe_rdyBitmap_T;

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Struktura internih podataka schedulera
 */
/*-----------------------------------------------------------------------------------------------*/
typedef struct epe_currCtrl {
/**
 * @brief       Trenutni EPA objekat koji se izvrsava
 */
    epe_epa_T       * epa;

/**
 * @brief       Prioritet trenutnog EPA objekata koji se izvrsava
 */
    epe_epaPrio_T   prio;
} epe_currCtrl_T;


/*************************************************************************************************
 * GLOBAL VARIABLES
 *************************************************************************************************/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Cuvar registra
 */
/*-----------------------------------------------------------------------------------------------*/
EPE_PKG_H_EXT epe_epaRegSentinel_T epe_regSentinel;

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Bitmape spremnih EPA objekata
 */
/*-----------------------------------------------------------------------------------------------*/
EPE_PKG_H_EXT epe_rdyBitmap_T epe_rdyBitmap;

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Pokazivaci na EPA objekte
 */
/*-----------------------------------------------------------------------------------------------*/
EPE_PKG_H_EXT epe_epa_T       * epe_rdyList[(PRIO_INDX_GROUP * HAL_UNATIVE_BITS)];

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Interni podaci schedulera
 */
/*-----------------------------------------------------------------------------------------------*/
EPE_PKG_H_EXT epe_currCtrl_T epe_curr;


/*************************************************************************************************
 * FUNCTION PROTOTYPES
 *************************************************************************************************/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Prijavljuje EPA objekat redu za cekanje
 */
/*-----------------------------------------------------------------------------------------------*/
C_INLINE void epe_schedRegisterI(
    const epe_epa_T     * aEpa) {

    EPE_ASSERT((epe_epa_T *)0U == epe_rdyList[aEpa->kernCtrl.prio]);            /* Provera: da nije ovaj prioritet vec zauzet?              */
    epe_rdyList[aEpa->kernCtrl.prio] = (epe_epa_T *)aEpa;
}

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Odjavljue EPA objekat iz reda za cekanje
 */
/*-----------------------------------------------------------------------------------------------*/
C_INLINE void epe_schedUnRegisterI(
    const epe_epa_T     * aEpa) {

    epe_schedRdyRemoveI(
        aEpa);
    epe_rdyList[aEpa->kernCtrl.prio] = (epe_epa_T *)0U;
}

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Ubacuje EPA objekat u red za cekanje
 */
/*-----------------------------------------------------------------------------------------------*/
C_INLINE void epe_schedRdyInsertI(
    const epe_epa_T     * aEpa) {

    unative_T indxGroup;
    unative_T indx;

    EPE_ASSERT((epe_epa_T *)0U != epe_rdyBitmap[aEpa->kernCtrl.prio]);          /* Provera: da li je dati EPA prijavljen u sistem?          */
    indx = aEpa->kernCtrl.prio & (~((unative_T)0U) >> (HAL_UNATIVE_BITS - PRIO_INDX_PWR));

#if (OPT_EPA_PRIO_MAX < HAL_UNATIVE_BITS)
    indxGroup = (unative_T)0U;
#else
    indxGroup = aEpa->kernCtrl.prio >> PRIO_INDX_PWR;
#endif
    epe_rdyBitmap.group |= (unative_T)1U << indxGroup;
    epe_rdyBitmap.bits[indxGroup] |= (unative_T)1U << indx;
}

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Izbacuje EPA objekat iz liste cekanja
 */
/*-----------------------------------------------------------------------------------------------*/
C_INLINE void epe_schedRdyRemoveI(
    const epe_epa_T     * aEpa) {

    unative_T indxGroup;
    unative_T indx;

    indx = aEpa->kernCtrl.prio & (~((unative_T)0U) >> (HAL_UNATIVE_BITS - PRIO_INDX_PWR));

#if (OPT_EPA_PRIO_MAX < HAL_UNATIVE_BITS)
    indxGroup = (unative_T)0U;
#else
    indxGroup = aEpa->kernCtrl.prio >> PRIO_INDX_PWR;
#endif
    epe_rdyBitmap.bits[indxGroup] &= ~((unative_T)1U << indx);

    if ((unative_T)0U == epe_rdyBitmap.bits[indxGroup]) {
        epe_rdyBitmap.group &= ~((unative_T)1U << indxGroup);
    }
}

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Ispituje da li je EPA objekat u listi reda za cekanje
 */
/*-----------------------------------------------------------------------------------------------*/
C_INLINE bool_T epe_schedEpaIsRdy(
    const epe_epa_T     * aEpa) {

    unative_T indxGroup;
    unative_T indx;

    EPE_ASSERT((epe_epa_T *)0U != epe_rdyBitmap[aEpa->kernCtrl.prio]);          /* Provera: da li je dati EPA prijavljen u sistem?          */
    indx = aEpa->kernCtrl.prio & (~((unative_T)0U) >> (HAL_UNATIVE_BITS - PRIO_INDX_PWR));

#if (OPT_EPA_PRIO_MAX < HAL_UNATIVE_BITS)
    indxGroup = (unative_T)0U;
#else
    indxGroup = aEpa->kernCtrl.prio >> PRIO_INDX_PWR;
#endif

    if (epe_rdyBitmap.bits[indxGroup] & ((unative_T)1U << indx)) {

        return (TRUE);
    } else {

        return (FALSE);
    }
}

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Dobavlja pokazivac na EPA objekat koji se trenutno izvrsava.
 */
/*-----------------------------------------------------------------------------------------------*/
C_INLINE epe_epa_T * epe_kernCurrEpaGet(
    void) {

    return (epe_curr.epa);
}

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Scheduler
 */
/*-----------------------------------------------------------------------------------------------*/
void epe_schedScheduleI(
    void);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Konstruktor funkcija EPA objekta
 *
 *              Inicijalizuje @c aEpa strukturu i rezervise dodatni memorijski
 *              prostor za podatke.
 *
 * @param       aMemClass               Klasa memorije koja se koristi za
 *                                      skladistenje:
 *  @arg        memHeapClass
 *  @arg        memPoolClass
 *  @arg        memStaticClass
 * @param       aEpa                    pokazivac na strukturu EPA objekta,
 * @param       aDescription            pokazivac na opis EPA objekta.
 */
/*-----------------------------------------------------------------------------------------------*/
void epe_epaCtor(
    const C_ROM smm_memClass_T * aMemClass,
    epe_epa_T       * aEpa,
    const C_ROM epe_epaDef_T * aDescription);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Destruktor funkcija EPA objekta
 *
 *              Reciklira memorijski prostor koji se koristio za podatke.
 *
 * @param       aMemClass               Klasa memorije koja se koristi za
 *                                      skladistenje:
 *  @arg        memHeapClass
 *  @arg        memPoolClass
 *  @arg        memStaticClass
 * @param       aEpa                    Pokazivac na strukturu EPA objekta.
 */
/*-----------------------------------------------------------------------------------------------*/
void epe_epaDtor(
    const C_ROM smm_memClass_T * aMemClass,
    epe_epa_T       * aEpa);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Konstruktor funkcija za registar
 *
 * @param       aMemClass               Klasa memorije koja se koristi za
 *                                      skladistenje:
 *  @arg        memHeapClass
 *  @arg        memPoolClass
 *  @arg        memStaticClass
 */
/*-----------------------------------------------------------------------------------------------*/
void epe_regCtor(
    const C_ROM smm_memClass_T * aMemClass);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Prijavljuje EPA objekat sistemu.
 *
 * @param       aEpa                    Pokazivac na strukturu EPA objekta,
 * @param       aDescription            opis EPA objekta.
 * @pre         Da bi se koristila registar funkcija mora se definisati
 *              @ref OPT_EPA_USE_REGISTRY.
 */
/*-----------------------------------------------------------------------------------------------*/
void epe_regRegister(
    epe_epa_T       * aEpa,
    const C_ROM epe_epaDef_T * aDescription);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Odjavljuje EPA objekat iz sistema.
 *
 * @param       aEpa                    Pokazivac na strukturu EPA objekta.
 * @pre         Da bi se koristila registar funkcija mora se definisati
 *              @ref OPT_EPA_USE_REGISTRY.
 */
/*-----------------------------------------------------------------------------------------------*/
void epe_regUnRegister(
    epe_epa_T       * aEpa);


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
 * END of epe_pkg.h
 *************************************************************************************************/
#endif /* EPE_PKG_H_ */
