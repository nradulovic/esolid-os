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
 * @brief       Interfejs eSolid operativnog sistema.
 * ------------------------------------------------------------------------------------------------
 * @addtogroup  kernel_intf
 ****************************************************************************************//** @{ */


#ifndef KERNEL_H_
#define KERNEL_H_

/*============================================================================  INCLUDE FILES  ==*/
#include "../config/kernel_config.h"
#include "hal/hal.h"
#include "port/kernel_profiles.h"
#include "primitive/list.h"
#include "primitive/queue.h"

/*==================================================================================  DEFINES  ==*/
/*==================================================================================  MACRO's  ==*/
/*-------------------------------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*===============================================================================  DATA TYPES  ==*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Deklaracije unapred
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Memorijska klasa alokatora
 *//*--------------------------------------------------------------------------------------------*/
typedef struct esMemClass esMemClass_T;

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Zaglavlje dogadjaja
 *//*--------------------------------------------------------------------------------------------*/
typedef struct esEvtHeader esEvtHeader_T;

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Red cekanja za dogadjaje
 *//*--------------------------------------------------------------------------------------------*/
typedef struct evtQueue evtQueue_T;

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Event Processing Agent
 *//*--------------------------------------------------------------------------------------------*/
typedef struct esEpaHeader esEpaHeader_T;
/** @} *//*--------------------------------------------------------------------------------------*/

#include "kernel/mm.h"
#include "kernel/evt.h"
#include "kernel/sproc.h"
#include "kernel/core.h"
#include "kernel/epa.h"

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Struktura podataka za EIR dogadjaj
 *//*--------------------------------------------------------------------------------------------*/
typedef struct esEpaInfo {
/**
 * @brief       Identifikator EPA objekta
 */
    uint16_t        id;

/**
 * @brief       Ime EPA objekta
 */
    uint8_t         name[8];

/**
 * @brief       Tip EPA objekta
 */
    uint8_t         type;

/**
 * @brief       Maksimalno zauzece reda za cekanje
 */
    uint8_t         queueMax;

/**
 * @brief       Velicina reda za cekanje
 */
    uint8_t         queueSize;

/**
 * @brief       Pokazivac na EPA objekat
 */
    esEpaHeader_T   * ptr;

/**
 * @brief       Status izvrsavanja EPA objekta
 */
    uint8_t         status;

/**
 * @brief       Events Per Second faktor
 */
    uint32_t        EPS;
} esEpaInfo_T;

/*-------------------------------------------------------------------------------------------*//**
 * @extends     esEvtHeader_T
 * @brief       Dogadjaj <c>Request</c>
 *//*--------------------------------------------------------------------------------------------*/
typedef struct evtSysReq {
/**
 * @brief       Super struktura sistemskog dogadjaja
 */
    esEvtHeader_T   super;
} evtSysReq_T;

/*-------------------------------------------------------------------------------------------*//**
 * @extends     esEvtHeader_T
 * @brief       Dogadjaj <c>EPA Info Report</c>
 *//*--------------------------------------------------------------------------------------------*/
typedef struct evtSysEIRep {
/**
 * @brief       Super struktura zaglavlja dogadjaja
 */
    esEvtHeader_T   super;
/**
 * @brief       Struktura podataka koju nosi ovaj dogadjaj
 */
    esEpaInfo_T     info;
} evtSysEIRep_T;

/*-------------------------------------------------------------------------------------------*//**
 * @extends     esEvtHeader_T
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
 *//*--------------------------------------------------------------------------------------------*/
typedef struct evtSysELRep {
/**
 * @brief       Super struktura zaglavlja dogadjaja
 */
    esEvtHeader_T   super;
} evtSysELRep_T;

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Domen korisnickih signala
 * @todo        Pogledati gde smestiti ovaj define
 *//*--------------------------------------------------------------------------------------------*/
#define EVT_SIGNAL_USER                 15

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Domen tipova korisnickih signala
 * @todo        Pogledati gde smestiti ovaj define
 *//*--------------------------------------------------------------------------------------------*/
#define EVT_TYPE_USER                   4

/*=========================================================================  GLOBAL VARIABLES  ==*/
/*======================================================================  FUNCTION PROTOTYPES  ==*/
/*---------------------------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*===================================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/

/** @endcond *//** @} *//*************************************************************************
 * END of kernel.h
 *************************************************************************************************/
#endif /* KERNEL_H_ */
