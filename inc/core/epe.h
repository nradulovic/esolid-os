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
 * @brief       Interfejs Event Processing Engine (EPE) modula.
 *
 * ------------------------------------------------------------------------------------------------
 *
 * @addtogroup  epe_intf
 *
 ****************************************************************************************//** @{ */

#ifndef EPE_H_
#define EPE_H_


/*************************************************************************************************
 * INCLUDE FILES
 *************************************************************************************************/

/*-----------------------------------------------------------------------------------------------*
 * Zavisnosti modula
 *-----------------------------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------------------------*
 * Konfiguracija i adaptacija modula
 *-----------------------------------------------------------------------------------------------*/

#include "core/cfg/epe_cfg.h"


/*-----------------------------------------------------------------------------------------------*
 * EXTERNS
 *-----------------------------------------------------------------------------------*//** @cond */

#ifdef EPE_H_VAR
#define EPE_H_EXT
#else
#define EPE_H_EXT extern
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

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Struktura koja opisuje jedan EPA objekat
 */
/*-----------------------------------------------------------------------------------------------*/
typedef struct epe_epaDef {
/**
 * @brief       Deskriptivno ime automata
 *
 *              Pokazivac na znakovni niz koji cuva ime jedinice za obradu
 *              dogadjaja. Ime jedinice za obradu dogadjaja se moze cuvati samo
 *              u flash/ROM memoriji. Ime pruza samo deskriptivnu informaciju o
 *              EPA objektu i u druge svrhe se ne koristi. Vise EPA objekata
 *              mogu imati isto ime.
 */
    const C_ROM char    * epaName;

/**
 * @brief       Prioritet automata
 */
    uint8_t             epaPrio;

/**
 * @brief       Velicina reda za cekanje dogadjaja
 */
    uint8_t             evtQueueSize;

/**
 * @brief       Inicijalno stanje HSM automata
 */
    smp_ptrState_T      hsmInitState;

/**
 * @brief       Maksimalna dubina hijerarhije stanja HSM automata.
 */
    uint8_t             hsmStateDepth;
} epe_epaDef_T;


/*************************************************************************************************
 * GLOBAL VARIABLES
 *************************************************************************************************/


/*************************************************************************************************
 * FUNCTION PROTOTYPES
 *************************************************************************************************/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Kreira EPA objekat.
 *
 * @param       aMemClass               Klasa memorije koja se koristi za
 *                                      skladistenje:
 *  @arg        memHeapClass
 *  @arg        memPoolClass
 *  @arg        memStaticClass
 * @param       aDescription            pokazivac na opis EPA objekta.
 * @return      Pokazivac na strukturu EPA objekta.
 * @see         epe_epaDef_T
 * @api
 */
/*-----------------------------------------------------------------------------------------------*/
epe_epa_T * eS_epaCreate(
    const C_ROM smm_memClass_T  * aMemClass,
    const C_ROM epe_epaDef_T    * aDescription);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Unistava EPA objekat.
 *
 * @param       aEpa                    Pokazivac na strukturu EPA objekta.
 * @api
 */
/*-----------------------------------------------------------------------------------------------*/
void eS_epaDestroy(
    epe_epa_T       * aEpa);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Vraca Id pokazivac EPA objekta.
 *
 * @return      Id pokazivac trenutnog EPA objekta koji se izvrsava.
 * @api
 */
/*-----------------------------------------------------------------------------------------------*/
epe_epa_T * eS_epaGetId(
    void);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Pronalazi EPA objekat po @c aId broju
 *
 * @param       aPid                    Pid broj zeljenog automata
 * @return      Pokazivac na EPA objekat ciji pid odgovara trazenom @c aPid
 *              broju.
 *  @retval     NULL ako nije pronadjen EPA objekat sa trazenim @c aPid.
 * @pre         Da bi se koristila registar funkcija mora se definisati
 *              @ref OPT_EPA_USE_REGISTRY.
 * @note        Funkcija uvek vraca NULL ako NIJE definisana promenljiva
 *              @ref OPT_EPA_USE_REGISTRY.
 * @api
 */
/*-----------------------------------------------------------------------------------------------*/
epe_epa_T * eS_regFindByPid(
    uint8_t     aPid);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Pronalazi EPA objekat po imenu dato u @c aName nizu.
 *
 * @param       aName                   Ime EPA objekta ciji se pokazivac trazi.
 * @return      Pokazivac na EPA objekat cije ime odgovara trazenom imenu @c
 *              aName.
 *  @retval     NULL ako nije pronadjen EPA objekat sa trazenim @c aNeme imenom.
 * @pre         Da bi se koristila registar funkcija mora se definisati
 *              @ref OPT_EPA_USE_REGISTRY.
 * @note        Funkcija uvek vraca NULL ako NIJE definisana promenljiva
 *              @ref OPT_EPA_USE_REGISTRY.
 * @api
 */
/*-----------------------------------------------------------------------------------------------*/
epe_epa_T * eS_regFindByName(
    const char      * aName);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Vraca pid broj EPA objekta sa @c aEpa pokazivacem na strukturu.
 *
 * @param       aEpa                    Pokazivac na strukturu EPA objekta.
 *  @arg        NULL - vraca pid trenutnog procesa.
 * @return      Pid broj trazenog @c aEpa objekta.
 * @pre         Da bi se koristila registar funkcija mora se definisati
 *              @ref OPT_EPA_USE_REGISTRY.
 * @api
 */
/*-----------------------------------------------------------------------------------------------*/
uint8_t eS_regGetPid(
    epe_epa_T       * aEpa);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Dobavlja prioritet EPA objekta
 *
 * @param       aEpa                    Pokazivac na EPA objekat
 * @return      Trenutni prioritet EPA objekta.
 * @api
 */
/*-----------------------------------------------------------------------------------------------*/
uint8_t eS_epaPrioGet(
    epe_epa_T       * aEpa);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Postavlja nov prioritet EPA objekta
 *
 * @param       aEpa                    Pokazivac na EPA objekat,
 * @param       aNewPrio                nov prioritet EPA objekta.
 * @api
 */
/*-----------------------------------------------------------------------------------------------*/
void eS_epaPrioSet(
    epe_epa_T       * aEpa,
    uint8_t         aNewPrio);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Pokrece izvrsavanje jezgra
 * @api
 */
/*-----------------------------------------------------------------------------------------------*/
void eS_kernStart(
    void);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Vraca odgovor da li trenutno radi kernel
 *
 * @return      Status izvrsavanja kernel-a.
 *  @retval     TRUE - kernel se izvrsava
 *  @retval     FALSE - kernel se ne izvrsava
 * @api
 */
/*-----------------------------------------------------------------------------------------------*/
bool_T eS_kernIsRunning(
    void);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Vraca znakovni niz koji identifikuje eSolid kernel
 *
 * @return      Verzija kernel-a u obliku znakovnog niza.
 * @api
 */
/*-----------------------------------------------------------------------------------------------*/
const C_ROM char * eS_kernSysVer(
    void);


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
 * END of epe.h
 *************************************************************************************************/
#endif /* EPE_H_ */
