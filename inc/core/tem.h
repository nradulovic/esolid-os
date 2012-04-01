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
 * @brief       Interfejs Time Event Management (TEM) modula.
 *
 * ------------------------------------------------------------------------------------------------
 *
 * @addtogroup  tem_intf
 *
 ****************************************************************************************//** @{ */


#ifndef TEM_H_
#define TEM_H_


/*************************************************************************************************
 * INCLUDE FILES
 *************************************************************************************************/

/*-----------------------------------------------------------------------------------------------*
 * Module configuration and adapter
 *-----------------------------------------------------------------------------------------------*/

#include "core/cfg/tem_cfg.h"


/*-----------------------------------------------------------------------------------------------*
 * EXTERNS
 *-----------------------------------------------------------------------------------*//** @cond */

#ifdef TEM_H_VAR
#define TEM_H_EXT
#else
#define TEM_H_EXT extern
#endif


/** @endcond*//***********************************************************************************
 * DEFINES
 *************************************************************************************************/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Nabrajanja tipova dogadjaja.
 *
 *              Ova nabrajanja se koriste samo kod prosirenih dogadjaja.
 *
 * @see         smp_evtExt
 */
/*-----------------------------------------------------------------------------------------------*/
enum tem_evtType {
/**
 * @brief       Dogadjaj je vremenskog tipa.
 *
 *              Ovakvi dogadjaji se iniciraju nakon isteka nekog vremenskog
 *              intervala.
 */
    EVT_TYPE_TIME = SMP_TYPE_USR,
/**
 * @brief       Domen korisnickih tipova dogadjaja.
 *
 *              Nakon ovog tipa dogadjaja korisnik moze definisati dodatne
 *              tipove dogadjaja.
 */
    TEM_TYPE_USR
};


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
 * @brief       Struktura jednog vremenskog dogadjaja.
 *
 *              U sustini ovo je jedan dogadjaj sa prosirenim parametrima koji
 *              opisuju vremenski dogadjaj.
 */
/*-----------------------------------------------------------------------------------------------*/
typedef struct tem_evtTime {
/**
 * @brief       Struktura prosirenog dogadjaja.
 */
    eot_evt_T super;
/**
 * @brief       Sledeca struktura u povezanoj listi.
 */
    struct tem_evtTime  * next;
/**
 * @brief       Prethodna struktura u povezanoj listi.
 */
    struct tem_evtTimeT * prev;
/**
 * @brief       Trenutna vrednost brojaca.
 */
    tem_tmrTick_T   curr;
/**
 * @brief       Vrednost ponovnog punjenja.
 */
    tem_tmrTick_T   reload;
    /**
     * @todo napisati strukturu za vremenske dogadjaje
     */
} tem_evtTime_T;


/*************************************************************************************************
 * GLOBAL VARIABLES
 *************************************************************************************************/


/*************************************************************************************************
 * FUNCTION PROTOTYPES
 *************************************************************************************************/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Kreiraj prazan tajmer sa signalom @c aEvtID.
 *
 *              Memorijski prostor se reservise i pokazuje se pokazivacem.
 *
 * @param       aEvtID                  Signal dogadjaja koji se generise kada
 *                                      istekne vremenski period.
 * @return      Pokazivac na novo kreirani tajmer.
 */
/*-----------------------------------------------------------------------------------------------*/
tem_evtTime_T * eS_tmrCreate (
    eot_evtId_T     aEvtID);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Podesi tajmer @c aTmr, za EPA @c aEPA sa vremenom od @c aTicks
 *              tick intervala.
 *
 * @param       aTmr                    Tajmer koji se podesava,
 * @param       aEPA                    kojoj EPA jedinici se salje dogadjaj,
 * @param       aTicks                  vremenski interval [broj tick ciklusa].
 */
/*-----------------------------------------------------------------------------------------------*/
void eS_tmrSet (
    tem_evtTime_T   * aTmr,
    epe_epa_T       * aEPA,
    tem_tmrTick_T   aTicks);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Ponovo podesi @c aTmr tajmer.
 *
 * @param       aTmr                    Koji tajmer ponovo podesiti?
 */
/*-----------------------------------------------------------------------------------------------*/
void eS_tmrReset (
    tem_evtTime_T   * aTmr);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Ukljuci prethodno iskljucen tajmer @c aTmr.
 *
 * @param       aTmr                    Tajmer koji treba ukljuciti.
 */
/*-----------------------------------------------------------------------------------------------*/
void eS_tmrEnable (
    tem_evtTime_T   * aTmr);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Iskljuci tajmer @c aTmr.
 *
 * @param       aTmr                    Tajmer koji treba iskljuciti.
 */
/*-----------------------------------------------------------------------------------------------*/
void eS_tmrDisable (
    tem_evtTime_T   * aTmr);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Unisti tajmer @c aTmr.
 *
 * @param       aTmr                    Tajmer koji treba unistiti.
 */
/*-----------------------------------------------------------------------------------------------*/
void eS_tmrDestroy (
    tem_evtTime_T   * aTmr);


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
 * END of tem.h
 *************************************************************************************************/
#endif /* TEM_H_ */
