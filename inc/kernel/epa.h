/******************************************************************************
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
 *//***********************************************************************//**
 * @file
 * @author      Nenad Radulovic
 * @brief       Interfejs EPA objekata
 * @details     This file is not meant to be included in application code
 *              independently but through the inclusion of "kernel.h" file.
 * @addtogroup  epa_intf
 *********************************************************************//** @{ */


#ifndef EPA_H_
#define EPA_H_

/*=========================================================  INCLUDE FILES  ==*/
#include "kernel/smp.h"

/*===============================================================  DEFINES  ==*/
/*===============================================================  MACRO's  ==*/
/*------------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/**
 * @brief       Definiciona struktura koja opisuje jedan EPA objekat
 * @api
 */
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
 * @brief       Inicijalno stanje automata
 */
    esState_T       smInitState;

/**
 * @brief       Maksimalna dubina hijerarhije stanja automata.
 */
    uint8_t         smLevels;
} esEpaDef_T;

/**
 * @brief       Objekt Event Processing Agent-a
 * @api
 */
typedef struct esEpa esEpa_T;

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Transport dogadjaja
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Salje dogadjaj na kraju reda za cekanje (FIFO metod).
 * @param       epa                     Pokazivac na EPA objekat kome se salje,
 * @param       evt                     pokazivac na dogadjaj koji se salje.
 * @details     Prihvata prethodno kreiran dogadjaj funkcijom esEvtCreate() i
 *              postavlja ga u red cekanja datog EPA objekta.
 * @api
 */
void esEvtPost(
    esEpa_T *       epa,
    esEvt_T *       evt);

/**
 * @brief       Salje dogadjaj na kraju reda za cekanje (FIFO metod).
 * @param       epa                     Pokazivac na EPA objekat kome se salje,
 * @param       evt                     pokazivac na dogadjaj koji se salje.
 * @details     Prihvata prethodno kreiran dogadjaj funkcijom esEvtCreate() i
 *              postavlja ga u red cekanja datog EPA objekta.
 * @iclass
 */
void esEvtPostI(
    esEpa_T *       epa,
    esEvt_T *       evt);

/**
 * @brief       Salje dogadjaj na pocetku reda za cekanje (LIFO metod).
 * @param       epa                     Pokazivac na EPA objekat kome se salje,
 * @param       evt                     pokazivac na dogadjaj koji se salje.
 * @details     Prihvata prethodno kreiran dogadjaj funkcijom esEvtCreate() i
 *              postavlja ga u red cekanja datog EPA objekta. Za razliku od
 *              esEvtPost() funkcije dogadjaj se postavlja na pocetku reda za
 *              cekanje. Najcesce se koristi kada je potrebno da se EPA objektu
 *              hitno posalje neki dogadjaj od znacaja koji treba da obradi.
 * @api
 */
void esEvtPostAhead(
    esEpa_T *       epa,
    esEvt_T *       evt);

/**
 * @brief       Salje dogadjaj na pocetku reda za cekanje (LIFO metod).
 * @param       epa                     Pokazivac na EPA objekat kome se salje,
 * @param       evt                     pokazivac na dogadjaj koji se salje.
 * @details     Prihvata prethodno kreiran dogadjaj funkcijom esEvtCreate() i
 *              postavlja ga u red cekanja datog EPA objekta. Za razliku od
 *              esEvtPost() funkcije dogadjaj se postavlja na pocetku reda za
 *              cekanje. Najcesce se koristi kada je potrebno da se EPA objektu
 *              hitno posalje neki dogadjaj od znacaja koji treba da obradi.
 * @iclass
 */
void esEvtPostAheadI(
    esEpa_T *       epa,
    esEvt_T *       evt);

/** @} *//*-------------------------------------------------------------------*/
/*------------------------------------------------------------------------*//**
 * @name        Osnovne funkcije za menadzment EPA objekata
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Kreira EPA objekat.
 * @param       [in] memClass           Klasa memorije koja se koristi za
 *                                      skladistenje:
 *  @arg        esMemDynClass
 *  @arg        esMemStaticClass
 * @param       [in] definition         Definiciona struktura EPA objekta.
 * @return      Pokazivac na strukturu zaglavlja EPA objekta.
 * @see         esEpaDef_T
 * @details     Nakon dobavljanja odgovarajuceg memorijskog prostora ova
 *              funkcija vrsi inicijalizaciju EPA objekta sa odgovarajucim 
 * 				parametrima.
 * @api
 */
esEpa_T * esEpaCreate(
    const C_ROM esMemClass_T *  memClass,
    const C_ROM esEpaDef_T *    definition);

/**
 * @brief       Unistava EPA objekat.
 * @param       [in] epa                Pokazivac na strukturu EPA objekta.
 * @details     Vrsi se oslobadjanje memorije ukoliko je EPA objekat koristio
 *              dinamicki memorijski alokator.
 * @api
 */
void esEpaDestroy(
    esEpa_T *       epa);

/**
 * @brief       Dobavlja prioritet EPA objekta
 * @param       [in] epa                Pokazivac na EPA objekat
 * @return      Trenutni prioritet EPA objekta.
 * @api
 */
uint8_t esEpaPrioGet(
    const esEpa_T * epa);

/**
 * @brief       Postavlja nov prioritet EPA objekta.
 * @param       [out] epa               Pokazivac na EPA objekat,
 * @param       [in] newPrio            nov prioritet EPA objekta.
 * @warning     Ukoliko je zahtevani prioritet vec zauzet javice se assert
 *              obavestenje.
 * @api
 */
void esEpaPrioSet(
    esEpa_T *       epa,
    uint8_t         newPrio);

/** @} *//*-------------------------------------------------------------------*/
/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of epa.h
 ******************************************************************************/
#endif /* EPA_H_ */
