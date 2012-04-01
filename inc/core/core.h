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
 * @brief       Interfejs jezgra eSolid operativnog sistema.
 *
 * ------------------------------------------------------------------------------------------------
 *
 * @addtogroup  core_intf
 *
 ****************************************************************************************//** @{ */


#ifndef CORE_H_
#define CORE_H_


/*************************************************************************************************
 * INCLUDE FILES
 *************************************************************************************************/

/*-----------------------------------------------------------------------------------------------*
 * Module dependencies
 *-----------------------------------------------------------------------------------------------*/

#include "dbg/dbg.h"
#include "hal/hal.h"
#include "primitive/list.h"
#include "primitive/queue.h"


/*-----------------------------------------------------------------------------------------------*
 * Module configuration
 *-----------------------------------------------------------------------------------------------*/

#include "core/smm.h"
#include "core/eot.h"
#include "core/smp.h"
#include "core/epe.h"
#include "core/epn.h"
#include "core/tem.h"
#include "core/str.h"


/*-----------------------------------------------------------------------------------------------*
 * EXTERNS
 *-----------------------------------------------------------------------------------*//** @cond */

#ifdef CORE_H_VAR
#define CORE_H_EXT
#else
#define CORE_H_EXT extern
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

/**
 * @brief       Struktura podataka za EIR dogadjaj
 */
typedef struct core_epaInfo {
    uint16_t    ID;
    uint8_t     name[8];
    uint8_t     type;
    uint8_t     queueMax;
    uint8_t     queueSize;
    void        * ptr;
    uint8_t     status;
    uint32_t    EPS;
} core_epaInfo_T;

/**
 * @brief       Dogadjaj <c>Request</c>
 *
 *              Sistemski zahtevi su dogadjaji osnovnog sistemskog tipa
 *              @ref smp_evtSystem.
 *
 * @todo        Napisati ROM strukturu
 */
typedef struct core_evtSysReq {
/**
 * @brief       Super struktura sistemskog dogadjaja
 */
    smp_evtSystem_T super;
} core_evtSysReq_T;

/**
 * @brief       Dogadjaj <c>EPA Info Report</c>
 *
 * @todo        Napisati ROM strukturu
 */
typedef struct core_evtSysEIRep {
/**
 * @brief       Super struktura zaglavlja dogadjaja
 */
    eot_evt_T       super;
/**
 * @brief       Struktura podataka koju nosi ovaj dogadjaj
 */
    core_epaInfo_T  info;
} smp_evtSysEIRep_T;

/**
 * @brief       Dogadjaj <c>EPA List Report</c>
 *
 *              Dogadjaj ima promenljivu velicinu i formira se kompletno u RAM
 *              memoriji, za ove dogadjaje se ne koriste podaci iz ROM strukture
 *              osim konstruktora i destruktora.
 *
 * @todo        Da bi ovaj dogadjaj bio moguc potrebno je da konstruktor
 *              funkcija ima mogucnost da izmeni predati pokazivac tek kreiranog
 *              dogadjaja. Pogledati OOC i implementaciju new funkcije. Zbog
 *              toga treba promeniti typedef-ove, vec napisane ROM strukture  i
 *              funkcije koje kreiraju dogadjaj.
 *
 * @todo        Napisati ROM strukturu
 */
typedef struct core_evtSysELRep {
/**
 * @brief       Super struktura zaglavlja dogadjaja
 */
    eot_evt_T   super;
} core_evtSysELRep;


/*************************************************************************************************
 * GLOBAL VARIABLES
 *************************************************************************************************/


/*************************************************************************************************
 * FUNCTION PROTOTYPES
 *************************************************************************************************/


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
 * END of core.h
 *************************************************************************************************/
#endif /* CORE_H_ */
