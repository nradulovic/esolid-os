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
 * @brief       Privatni interfejs Event Object podmodula.
 * @addtogroup  evt_impl
 ****************************************************************************************//** @{ */

#ifndef EVT_PKG_H_
#define EVT_PKG_H_

/*============================================================================  INCLUDE FILES  ==*/
/*==================================================================================  DEFINES  ==*/
/*==================================================================================  MACRO's  ==*/
/*-------------------------------------------------------------------------  C++ extern begin  --*/
#if defined(__cplusplus)
extern "C" {
#endif

/*===============================================================================  DATA TYPES  ==*/
/*=========================================================================  GLOBAL VARIABLES  ==*/

extern const C_ROM esEvtHeader_T evtSignal[];

/*======================================================================  FUNCTION PROTOTYPES  ==*/

/**
 * @brief       Unistava dogadjaj.
 * @param       aEvt                    Pokazivac na dogadjaj koji treba da se
 *                                      unisti.
 * @details     Ukoliko dati @c aEvt dogadjaj nema vise ni jednog korisnika,
 *              onda ce memorijski prostor koji on zauzima biti recikliran, u
 *              suprotnom, dogadjaj nastavlja da postoji.
 * @notapi
 * @inline
 */
C_INLINE_ALWAYS void evtDestroyI_(
    esEvtHeader_T       * aEvt) {

    if ((uint_fast8_t)0U == aEvt->dynamic) {

#if defined(OPT_KERNEL_DBG_EVT) && defined(OPT_DBG_USE_CHECK)
    aEvt->signature = ~EVT_SIGNATURE;                                           /* Postavljanje lošeg potpisa                               */
#endif
        esHmemDeAllocI((void *)aEvt);
    }
}

/**
 * @brief       Vraca kolika je potrebna velicina memorijskog prostora za
 *              cuvanje bafera dogadjaja.
 * @param       aQueueSize              Maksimalan broj dogadjaja u baferu.
 * @return      Potreban memorijski prostor u bajtovima.
 * @notapi
 * @inline
 */
C_INLINE_ALWAYS size_t evtQReqSize_(
    size_t              aQueueSize) {

    return (aQueueSize * sizeof(void *));
}

/**
 * @brief       Konstruise red cekanja za dogadjaje.
 * @param       aEpa                    Pokazivac na postojeci EPA objekat
 * @param       aStorage                memorijski prostor za red cekanja,
 * @param       aQueueSize              velicina potrebnog reda cekanja.
 * @details     Inicijalizuje strukturu i rezervise memorijski prostor za red
 *              cekanja za dati @c aEvtQueue red cekanja.
 * @notapi
 */
void evtQInitI(
    esEpaHeader_T       * aEpa,
    esEvtHeader_T       ** aStorage,
    size_t              aQueueSize);

/**
 * @brief       Dekonstruise red cekanja za dogadjaje
 * @param       aEpa                    Pokazivac na red cekanja koji se
 *                                      dekonstruise.
 * @details     Svi dogadjaji koji su u redu cekanja ce se prikupiti i obrisati.
 * @notapi
 */
void evtQDeInitI(
    esEpaHeader_T       * aEpa);

/**
 * @brief       Dobavlja dogadjaj iz reda za cekanje @c aEvtQueue
 * @param       aEpa                    Pokazivac na red za cekanje.
 * @return      Dogadjaj iz reda cekanja.
 * @notapi
 */
esEvtHeader_T * evtQGetI(
    esEpaHeader_T       * aEpa);

/**
 * @brief       Salje dogadjaj na pocetku reda za cekanje (LIFO metod).
 * @param       aEpa                    Pokazivac na red za cekanje,
 * @param       aEvt                    pokazivac na dogadjaj koji se salje.
 * @iclass
 */
void evtQPutAheadI(
    esEpaHeader_T       * aEpa,
    esEvtHeader_T       * aEvt);

/**
 * @brief       Salje dogadjaj na pocetku reda za cekanje (LIFO metod).
 * @param       aEpa                    Pokazivac na red za cekanje,
 * @param       aEvt                    pokazivac na dogadjaj koji se salje.
 */
void evtQPutAhead(
    esEpaHeader_T       * aEpa,
    esEvtHeader_T       * aEvt);

/**
 * @brief       Salje dogadjaj na kraju reda za cekanje (FIFO metod).
 * @param       aEpa                    Pokazivac na red za cekanje,
 * @param       aEvt                    pokazivac na dogadjaj koji se salje.
 * @iclass
 */
void evtQPutI(
    esEpaHeader_T       * aEpa,
    esEvtHeader_T       * aEvt);

/**
 * @brief       Salje dogadjaj na kraju reda za cekanje (FIFO metod).
 * @param       aEpa                    Pokazivac na red za cekanje,
 * @param       aEvt                    pokazivac na dogadjaj koji se salje.
 */
void evtQPut(
    esEpaHeader_T       * aEpa,
    esEvtHeader_T       * aEvt);

/*---------------------------------------------------------------------------  C++ extern end  --*/
#if defined(__cplusplus)
}
#endif

/*===================================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/

/** @endcond *//** @} *//*************************************************************************
 * END of evt_pkg.h
 *************************************************************************************************/
#endif /* EVT_PKG_H_ */
