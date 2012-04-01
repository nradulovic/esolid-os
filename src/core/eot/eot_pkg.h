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
 * @author  	nesa
 *
 * @brief       Privatni objekti EOT podmodula.
 *
 * ------------------------------------------------------------------------------------------------
 *
 * @addtogroup  eot_impl
 *
 ****************************************************************************************//** @{ */

#ifndef EOT_PKG_H_
#define EOT_PKG_H_


/*************************************************************************************************
 * INCLUDE FILES
 *************************************************************************************************/

/*-----------------------------------------------------------------------------------------------*
 * Module dependencies
 *-----------------------------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------------------------*
 * Module configuration and adapter
 *-----------------------------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------------------------*
 * EXTERNS
 *-----------------------------------------------------------------------------------*//** @cond */

#ifdef EOT_PKG_H_VAR
#define EOT_PKG_H_EXT
#else
#define EOT_PKG_H_EXT extern
#endif


/** @endcond*//***********************************************************************************
 * DEFINES
 *************************************************************************************************/

/*-------------------------------------------------------------------------------------------*//**
 * @name        Konstante za definisanje metode cuvanja dogadjaja
 * @{ *//*---------------------------------------------------------------------------------------*/

#define EVT_QUALIF_MASK                                                         \
    ((eot_evtDyn_T)(ES_MASK_MSB(eot_evtDyn_T)))

#define EVT_QUALIF_STATIC                                                       \
    (EVT_QUALIF_MASK)

#define EVT_QUALIF_DYNAMIC                                                      \
    ((eot_evtDyn_T)0U)

/** @} *//*--------------------------------------------------------------------------------------*/


/*************************************************************************************************
 * MACRO's
 *************************************************************************************************/

/*-------------------------------------------------------------------------------------------*//**
 * @name        Pristup internim podacima zaglavlja dogadjaja
 * @{ *//*---------------------------------------------------------------------------------------*/

#define EVT_STORAGE_RD(aEvt)                                                    \
    ((aEvt)->dyn & EVT_QUALIF_MASK)

#define EVT_CNTR_RD(aEvt)                                                       \
    ((aEvt)->dyn & ~EVT_QUALIF_MASK)

#define EVT_CNTR_WR_I(aEvt, cntr)                                               \
    do {                                                                        \
        eot_evtDyn_T tmp;                                                       \
        tmp = (aEvt)->dyn & EVT_QUALIF_MASK;                                    \
        (aEvt)->dyn = tmp | (cntr & ~EVT_QUALIF_MASK);                          \
    } while (0)

/**
 * @mseffect
 */
# define EVT_STORAGE_WR(aEvt, type)                                             \
    do {                                                                        \
        if ((type) == EVT_QUALIF_STATIC) {                                      \
            (aEvt)->dyn |= EVT_QUALIF_MASK;                                     \
        } else {                                                                \
            (aEvt)->dyn &= ~EVT_QUALIF_MASK;                                    \
        }                                                                       \
    } while (0)

#if defined(OPT_EVT_USE_TIME) || defined(__DOXYGEN__)
# define EOT_EVT_WRTIMESTAMP(pEvt, evtTimestamp)                                \
    (pEvt)->timestamp = (evtTimestamp)
#else
# define EOT_EVT_WRTIMESTAMP(pEvt, evtTimestamp)                                \
    (void)0

#endif

#if defined(OPT_EVT_USE_TRANSMITTER) || defined(__DOXYGEN__)
# define EOT_EVT_WRTRANSMITTER(pEvt, evtTransmitter)                            \
    (pEvt)->transmitter = (evtTransmitter)
#else
# define EOT_EVT_WRTRANSMITTER(pEvt, evtTransmitter)                            \
    (void)0

#endif

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Debug podrska
 *
 * @brief       Makroi za debug podrsku. Pogledati @ref dbg_intf.
 * @{ *//*---------------------------------------------------------------------------------------*/

#if defined(OPT_DBG_EOT) || defined(__DOXYGEN__)
# define EOT_ASSERT                     DBG_ASSERT
# define EOT_ASSERT_ALWAYS              DBG_ASSERT_ALWAYS
# define EOT_COMPILE_ASSERT             DBG_COMPILE_ASSERT
# define EOT_DBG_DECL                   DBG_DECL
# define EOT_DBG_DEFINE_MODULE          DBG_DEFINE_MODULE
# define EOT_DBG_ENTRY                  DBG_ENTRY
# define EOT_DBG_EXIT                   DBG_EXIT
# define EOT_DBG_MACRO                  DBG_MACRO
# define EOT_DBG_CHECK                  DBG_CHECK
#else
# define EOT_ASSERT(expr)               DBG_EMPTY_MACRO()
# define EOT_ASSERT_ALWAYS(expr)        DBG_EMPTY_MACRO()
# define EOT_COMPILE_ASSERT(expr)       DBG_EMPTY_DECL()
# define EOT_DBG_DECL(expr)             DBG_EMPTY_DECL()
# define EOT_DBG_DEFINE_MODULE(expr)    DBG_EMPTY_DECL()
# define EOT_DBG_ENTRY()                DBG_EMPTY_MACRO()
# define EOT_DBG_EXIT()                 DBG_EMPTY_MACRO()
# define EOT_DBG_MACRO(expr)            DBG_EMPTY_MACRO()
# define EOT_DBG_CHECK(expr)            DBG_EMPTY_MACRO()
#endif

/** @} *//*--------------------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------------------------*
 * C/C++ #ifdef - open
 *-----------------------------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif


/*************************************************************************************************
 * DATA TYPES
 *************************************************************************************************/

/*
 * Bez optimizacija
 */
#if !defined(ES_OPTIMIZE_SIZE) && !defined(ES_OPTIMIZE_SPEED) || defined(__DOXYGEN__)

/**
 * @brief       Tip podataka za brojace unutar redova cekanja za dogadjaje
 */
typedef uint8_t                         eot_evtQIndx_T;
#endif

/*
 * Optimizacija po brzini
 */
#if !defined(ES_OPTIMIZE_SIZE) && defined(ES_OPTIMIZE_SPEED)
typedef uint_fast8_t                    eot_evtQIndx_T;
#endif

/*
 * Optimizacija po zauzecu
 */
#if defined(ES_OPTIMIZE_SIZE) && !defined(ES_OPTIMIZE_SPEED)
typedef uint8_t                         eot_evtQIndx_T;
#endif

/*-------------------------------------------------------------------------------------------*//**
 * @name        Tipovi podataka za redove cekanja
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Red cekanja za dogadjaje.
 *
 *              Ova struktura opisuje redove cekanja za dogadjaje koji se
 *              koriste za aktivne objekte. Red cekanja zajedno sa
 *              procesorom konacnih automata (SMP) cini jedan agent za obradu
 *              dogadjaja, (Event Processing Agent - EPA).
 *
 *              Struktura sadrzi samo podatke za upravljanje reda cekanja i ne
 *              sadrzi memorijski prostor za cuvanje dogadjaja. Memorijski
 *              prostor se mora rezervisati unapred i predati pokazivac na taj
 *              prostor EOT-u prilikom inicijalizacije reda cekanja.
 *
 *              U redovima cekanja se cuvaju samo pokazivaci ka dogadjajima, a
 *              ne i same instance dogadjaja.
 *
 * @see         eot_evtPln
 * @see         eot_evtExt
 */
/*-----------------------------------------------------------------------------------------------*/
struct eot_evtQueue {
/**
 * @brief       Instanca reda za cekanje opste namene
 */
    pq_queuePtr_T   queue;

#if defined(OPT_EPA_USE_REGISTRY) || defined(OPT_DBG_EOT) || defined(__DOXYGEN__)
/**
 * @brief       Trenutni broj slobodnih mesta za dogadjaje u redu za cekanje.
 */
    eot_evtQIndx_T  freeCurr;

/**
 * @brief       Minimalni broj slobodnih mesta za dogadjaje u redu za cekanje.
 */
    eot_evtQIndx_T  freeMin;
#endif
};

/** @} *//*--------------------------------------------------------------------------------------*/


/*************************************************************************************************
 * GLOBAL VARIABLES
 *************************************************************************************************/


/*************************************************************************************************
 * FUNCTION PROTOTYPES
 *************************************************************************************************/

#if defined(OPT_EPA_USE_REGISTRY) || defined(OPT_DBG_EOT) || defined(__DOXYGEN__)
/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Statistika reda za cekanje - dodavanje dogadjaja
 *
 * @param       aEvtQueue               Pokazivac na strukturu reda za cekanje.
 */
/*-----------------------------------------------------------------------------------------------*/
C_INLINE void eot_evtQStatEvtAdded(
    eot_evtQueue_T * aEvtQueue) {

    --aEvtQueue->freeCurr;

    if (aEvtQueue->freeCurr < aEvtQueue->freeMin) {
        aEvtQueue->freeMin = aEvtQueue->freeCurr;
    }
}

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Statistika reda za cekanje - uklanjanje dogadjaja
 *
 * @param       aEvtQueue               Pokazivac na strukturu reda za cekanje.
 */
/*-----------------------------------------------------------------------------------------------*/
C_INLINE void eot_evtQStatEvtRemoved(
    eot_evtQueue_T * aEvtQueue) {

    ++aEvtQueue->freeCurr;
}

#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Dobavi dogadjaj iz reda cekanja za @c aEpa.
 *
 * @param       aEpa                    Pokazivac na agenta za obradu dogadjaj.
 * @return      Dogadjaj koji treba da se obradi od strane @c aEPA.
 */
/*-----------------------------------------------------------------------------------------------*/
eot_evt_T * eot_evtFetchI(
    epe_epa_T       * aEpa);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Konstruise red cekanja za dogadjaje
 *
 *              Inicijalizuje strukturu i rezervise memorijski prostor za red
 *              cekanja za dati @c aEvtQueue red cekanja.
 *
 * @param       aMemClass               Klasa memorije koja se koristi za
 *                                      skladistenje,
 *  @arg        memHeapClass
 *  @arg        memPoolClass
 *  @arg        memStaticClass
 * @param       aEvtQueue               pokazivac na tek kreirani objekat red
 *                                      cekanja,
 * @param       aQueueSize              velicina potrebnog reda cekanja.
 * @note        Ne vrsi se verifikacija aMemClass argumenta.
 */
/*-----------------------------------------------------------------------------------------------*/
void eot_evtQCtor(
    const C_ROM smm_memClass_T  * aMemClass,
    eot_evtQueue_T  * aEvtQueue,
    eot_evtQIndx_T  aQueueSize);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Dekonstruise red cekanja za dogadjaje
 *
 *              Svi dogadjaji koji su u redu cekanja ce se prikupiti i obrisati.
 *
 * @param       aMemClass               Klasa memorije koja se koristi za
 *                                      skladistenje,
 *  @arg        memHeapClass
 *  @arg        memPoolClass
 *  @arg        memStaticClass
 * @param       aEvtQueue               Pokazivac na red cekanja koji se
 *                                      dekonstruise.
 * @return      Pokazivac na memorijski prostor koji treba da se dealocira
 * @note        Ne vrsi se verifikacija aMemClass argumenta.
 */
/*-----------------------------------------------------------------------------------------------*/
void eot_evtQDtor(
    const C_ROM smm_memClass_T  * aMemClass,
    eot_evtQueue_T  * aEvtQueue);


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
 * END of eot_pkg.h
 *************************************************************************************************/
#endif /* EOT_PKG_H_ */
