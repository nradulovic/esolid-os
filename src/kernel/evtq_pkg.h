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
 * @author  	Nenad Radulovic
 * @brief       Privatni interfejs Event Queue modula.
 * @addtogroup  evtq_impl
 *********************************************************************//** @{ */

#ifndef EVTQ_PKG_H_
#define EVTQ_PKG_H_

/*=========================================================  INCLUDE FILES  ==*/
#include "primitive/queue.h"

/*===============================================================  DEFINES  ==*/
/*===============================================================  MACRO's  ==*/

/**
 * @brief       Proverava da li je velicina reda za cekanje validna.
 */
#define EVTQ_RANGE_SIZE(size)                                                   \
    ((size) >= 1)

/*------------------------------------------------------  C++ extern begin  --*/
#if defined(__cplusplus)
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/**
 * @brief       Red cekanja za dogadjaje.
 * @details     Ova struktura opisuje redove cekanja za dogadjaje koji se
 *              koriste za aktivne objekte. Red cekanja zajedno sa
 *              procesorom konacnih automata (SMP) cini jedan agent za obradu
 *              dogadjaja, (Event Processing Agent - EPA).
 *
 *              Struktura sadrzi samo podatke za upravljanje reda cekanja i ne
 *              sadrzi memorijski prostor za cuvanje dogadjaja. Memorijski
 *              prostor se mora rezervisati unapred i predati pokazivac na taj
 *              prostor EVT-u prilikom inicijalizacije reda cekanja.
 *
 *              U redovima cekanja se cuvaju samo pokazivaci ka dogadjajima, a
 *              ne i same instance dogadjaja.
 *
 *              Pored navedenog reda za cekanje, struktura moze da sadrzi
 *              brojace zauzeca reda za cekanje. @ref free pokazuje trenutni
 *              broj praznih lokacija dok @ref freeMin sadrzi najmanji broj
 *              slobodnih lokacija ikada.
 * @notapi
 */
typedef struct evtQueue {
/**
 * @brief       Instanca reda za cekanje opste namene
 */
    esQueuePtr_T    queue;

#if defined(OPT_KERNEL_DBG_EVT) || defined(__DOXYGEN__)
/**
 * @brief       Trenutni broj slobodnih lokacija u redu za cekanje
 */
    uint_fast8_t    free;

/**
 * @brief       Najmanji broj slobodnih lokacija u redu za cekanje
 */
    uint_fast8_t    freeMin;
#endif
} evtQueue_T;


/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

/**
 * @brief       Da li je red cekanja prazan?
 * @param       [in] evtQ               Pokazivac na red cekanja.
 * @return      Status popunjenosti reda za cekanje dogadjaja.
 */
static C_INLINE_ALWAYS bool_T evtQIsEmptyI_(
    evtQueue_T *    evtQ) {

    return (esQpIsEmpty_(&evtQ->queue));
}

/**
 * @brief       Da li je red cekanja pun?
 * @param       [in] evtQ               Pokazivac na red cekanja.
 * @return      Status popunjenosti reda za cekanje dogadjaja.
 */
static C_INLINE_ALWAYS bool_T evtQIsFullI_(
    evtQueue_T *    evtQ) {

    return (esQpIsFull_(&evtQ->queue));
}

/**
 * @brief       Dobavlja jedan dogadjaj iz reda za cekanje
 * @param       [in] evtQ               Pokazivac na red cekanja.
 * @return      Dogadjaj sa pocetka reda za cekanje.
 */
static C_INLINE_ALWAYS esEvt_T * evtQGetI_(
    evtQueue_T *    evtQ) {

    esEvt_T * evt;

    evt = esQpGet_(
        &evtQ->queue);

#if defined(OPT_KERNEL_DBG_EVT)
    ++(epa->evtQueue.free);
#endif

    return (evt);
}

/**
 * @brief       Postavlja dogadjaj u red za cekanje na pocetku reda.
 * @param       [out] evtQ              Pokazivac na red cekanja.
 * @param       [in] evt                Pokazivac na dogadjaj koji treba da se
 *                                      ubaci u red za cekanje.
 */
static C_INLINE_ALWAYS void evtQPutAheadI_(
    evtQueue_T *    evtQ,
    esEvt_T *       evt) {

    esQpPutAhead_(
        &evtQ->queue,
        evt);

#if defined(OPT_KERNEL_DBG_EVT)
    --(evtQ->free);
    if (evtQ->free < evtQ->freeMin) {
        evtQ->freeMin = evtQ->free;
    }
#endif
}

/**
 * @brief       Postavlja dogadjaj u red za cekanje na kraju reda.
 * @param       [out] evtQ              Pokazivac na red cekanja.
 * @param       [in] evt                Pokazivac na dogadjaj koji treba da se
 *                                      ubaci u red za cekanje.
 */
static C_INLINE_ALWAYS void evtQPutI_(
    evtQueue_T *    evtQ,
    esEvt_T *       evt) {

    esQpPut_(
        &evtQ->queue,
        evt);

#if defined(OPT_KERNEL_DBG_EVT)
    --(evtQ->free);
    if (evtQ->free < evtQ->freeMin) {
        evtQ->freeMin = evtQ->free;
    }
#endif
}

/**
 * @brief       Inicijalizije red za cekanje
 * @param       [out] evtQ              Pokazivac na red za cekanje
 * @param       [in] evtQBuff           Bafer reda za cekanje
 * @param       [in] size               Velicina bafera
 */
void evtQInit(
    evtQueue_T *    evtQ,
    esEvt_T **      evtQBuff,
    uint8_t         size);

/**
 * @brief       Deinicijalizuje red za cekanje
 * @param       evtQ
 */
void evtQDeInit(
    evtQueue_T *    evtQ);

/**
 * @brief       Kolika je potrebna velicina bafera za cuvanje @c levels
 *              dogadjaja?
 * @param       [in] levels             Koliko dogadjaja treba da se smesti u
 *                                      bafer?
 * @return      Velicinu bafera u bajtovima.
 */
size_t evtQReqSize(
    uint8_t         levels);

/*--------------------------------------------------------  C++ extern end  --*/
#if defined(__cplusplus)
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of evtq_pkg.h
 ******************************************************************************/
#endif /* EVTQ_PKG_H_ */
