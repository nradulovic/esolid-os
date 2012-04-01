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
 *************************************************************************************************/


/*********************************************************************************************//**
 * @file
 *
 * @author  	nenad
 *
 * @brief       Interfejs za manipulaciju redova za cekanje (queue).
 *
 * ------------------------------------------------------------------------------------------------
 *
 * @addtogroup  queue_intf
 *
 ****************************************************************************************//** @{ */


#ifndef QUEUE_H_
#define QUEUE_H_


/*************************************************************************************************
 * INCLUDE FILES
 *************************************************************************************************/

/*-----------------------------------------------------------------------------------------------*
 * Module dependencies
 *-----------------------------------------------------------------------------------------------*/

#include "hal/hal.h"
#include "core/smm.h"


/*-----------------------------------------------------------------------------------------------*
 * Module configuration and adapter
 *-----------------------------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------------------------*
 * EXTERNS
 *-----------------------------------------------------------------------------------*//** @cond */

#ifdef QUEUE_H_VAR
# define QUEUE_H_EXT
#else
# define QUEUE_H_EXT extern
#endif


/** @endcond*//***********************************************************************************
 * DEFINES
 *************************************************************************************************/

/*-------------------------------------------------------------------------------------------*//**
 * @name        Definition group
 *
 * @brief       brief description
 * @{ *//*---------------------------------------------------------------------------------------*/

/** @} *//*--------------------------------------------------------------------------------------*/


/*************************************************************************************************
 * MACRO's
 *************************************************************************************************/

/*-------------------------------------------------------------------------------------------*//**
 * @name        Macro group
 *
 * @brief       brief description
 * @{ *//*---------------------------------------------------------------------------------------*/

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

/*-------------------------------------------------------------------------------------------*//**
 * @name        Data types group
 *
 * @brief       brief description
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Struktura zaglavlja indirektnog reda za cekanje
 */
/*-----------------------------------------------------------------------------------------------*/
typedef struct pq_queuePtr {
/**
 * @brief       Pocetak reda za cekanje
 */
    void            ** head;

/**
 * @brief       Kraj reda za cekanje
 */
    void            ** tail;

/**
 * @brief       Pocetak bafera reda za cekanje
 */
    void            ** begin;

/**
 * @brief       Kraj bafera reda za cekanje + jedna pozicija
 */
    void            ** end;
} pq_queuePtr_T;

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Struktura zaglavlja direktnog reda za cekanje
 */
/*-----------------------------------------------------------------------------------------------*/
typedef struct pq_queue {
/**
 * @brief       Pocetak reda za cekanje
 */
    uint8_t         * head;

/**
 * @brief       Kraj reda za cekanje
 */
    uint8_t         * tail;

/**
 * @brief       Pocetak bafera reda za cekanje
 */
    uint8_t         * begin;

/**
 * @brief       Kraj bafera reda za cekanje + jedna pozicija
 */
    uint8_t         * end;
} pq_queue_T;

/** @} *//*--------------------------------------------------------------------------------------*/


/*************************************************************************************************
 * GLOBAL VARIABLES
 *************************************************************************************************/

/*-------------------------------------------------------------------------------------------*//**
 * @name        Variables group
 *
 * @brief       brief description
 * @{ *//*---------------------------------------------------------------------------------------*/

/** @} *//*--------------------------------------------------------------------------------------*/


/*************************************************************************************************
 * FUNCTION PROTOTYPES
 *************************************************************************************************/

/*-------------------------------------------------------------------------------------------*//**
 * @name        Manipilacija sa redovima cekanja sa pokazivacima
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Vrsi inicijalizaciju strukture za red za cekanje
 *
 * @param       aQueue                  Pokazivac na tek kreirani red cekanja,
 * @param       aMemBuffer              memorijska oblast koja se koristi,
 * @param       aQueueSize              velicina potrebnog reda cekanja izrazena
 *                                      u broju elemenata u redu za cekanje.
 * @api
 */
/*-----------------------------------------------------------------------------------------------*/
C_INLINE void eS_qPtrInit(
    pq_queuePtr_T   * aQueue,
    void            ** aMemBuffer,
    size_t          aQueueSize) {

    aQueue->head = aMemBuffer;
    aQueue->tail = aMemBuffer;
    aQueue->begin = aMemBuffer;
    aQueue->end = aMemBuffer + (aQueueSize - 1U);
}

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Destruktor funkcija za red za cekanje
 *
 *              Sa obzirom da Ctor funkcija nije alocirala memorijski prostor
 *              destruktor samo vraca pokazivac ka memorijskom prostoru.
 *
 * @param       aQueue                  Red za cekanje koji treba da se unisti.
 */
/*-----------------------------------------------------------------------------------------------*/
C_INLINE void * eS_qPtrDeInit(
    pq_queuePtr_T   * aQueue) {

    aQueue->head = (void **)0U;
    aQueue->tail = (void **)0U;
    aQueue->begin = (void **)0U;
    aQueue->end = (void **)0U;

    return ((void *)(aQueue->begin));
}

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Postavlja element @b na @b kraju reda (tail) za cekanje (FIFO metod)
 *
 * @param       aQueue                  Red cekanja gde treba postaviti dogadjaj,
 * @param       aItem                   element koji treba postaviti u red za
 *                                      cekanje.
 * @pre         Red za cekanje je prethodno kreiran konstruktor funkcijom
 *              eS_qPtrInit().
 * @iclass
 */
/*-----------------------------------------------------------------------------------------------*/
C_INLINE void eS_qPtrPutI(
    pq_queuePtr_T   * aQueue,
    void            * aItem) {

    *(aQueue->tail) = aItem;

    if (aQueue->tail != aQueue->end) {
        ++aQueue->tail;
    } else {
        aQueue->tail = aQueue->begin;
    }
}

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Postavlja dogadjaj @b na @b pocetku reda (head) za cekanje (LIFO metod)
 *
 * @param       aQueue                  Red cekanja gde treba postaviti dogadjaj,
 * @param       aItem                   element koji treba postaviti u red za
 *                                      cekanje.
 * @pre         Red za cekanje je prethodno kreiran konstruktor funkcijom
 *              eS_qPtrInit().
 * @iclass
 */
/*-----------------------------------------------------------------------------------------------*/
C_INLINE void eS_qPtrPushI(
    pq_queuePtr_T   * aQueue,
    void            * aItem) {

    if (aQueue->head != aQueue->begin) {
        --aQueue->head;
    } else {
        aQueue->head = aQueue->end;
    }
    *(aQueue->head) = aItem;
}

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Dobavlja element sa pocetka reda (head) za cekanje
 *
 * @param       aQueue                  Red za cekanje odakle treba dobaviti
 *                                      element.
 * @return      Element iz reda za cekanje.
 * @pre         Red za cekanje je prethodno kreiran konstruktor funkcijom
 *              eS_qPtrInit().
 * @iclass
 */
/*-----------------------------------------------------------------------------------------------*/
C_INLINE void * eS_qPtrGetI(
    pq_queuePtr_T   * aQueue) {

    void * tmpPtr;

    tmpPtr = *(aQueue->head);

    if (aQueue->head != aQueue->end) {
        ++aQueue->head;
    } else {
        aQueue->head = aQueue->begin;
    }

    return (tmpPtr);
}

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Dobavlja koliko maksimalno elemenata moze da stane u red za cekanje
 *
 * @param       aQueue                  Red za cekanje koji se ispituje.
 * @return      Maksimalan broj elemenata koji mogu da stanu u red za cekanje
 * @pre         Red za cekanje je prethodno kreiran konstruktor funkcijom
 *              eS_qPtrInit().
 * @api
 */
/*-----------------------------------------------------------------------------------------------*/
C_INLINE size_t eS_qPtrSize(
    const pq_queuePtr_T * aQueue) {

    return ((size_t)(aQueue->end - aQueue->begin + 1U));
}

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Dobavlja velicinu reda za cekanje
 *
 * @param       aQueue                  Red za cekanje koji se ispituje.
 * @return      Trenutni broj elemenata u redu za cekanje.
 * @pre         Red za cekanje je prethodno kreiran konstruktor funkcijom
 *              eS_qPtrInit().
 * @api
 */
/*-----------------------------------------------------------------------------------------------*/
C_INLINE size_t eS_qPtrOccupied(
    const pq_queuePtr_T * aQueue) {

    if (aQueue->head < aQueue->tail) {

        return ((size_t)(aQueue->tail - aQueue->head));
    } else {

        return ((size_t)(eS_qPtrSize(aQueue) - (aQueue->head - aQueue->tail)));
    }
}

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Dobavlja koliko jos elemenata moze da se smesti u red za cekanje
 *
 * @param       aQueue                  Red za cekanje koji se ispituje
 * @return      Koliko slobodnog prostora je preostalo.
 * @pre         Red za cekanje je prethodno kreiran konstruktor funkcijom
 *              eS_qPtrInit().
 * @api
 */
/*-----------------------------------------------------------------------------------------------*/
C_INLINE size_t eS_qPtrFreeSpace(
    const pq_queuePtr_T * aQueue) {

    if (aQueue->head < aQueue->tail) {

        return ((size_t)(eS_qPtrSize(aQueue) - (aQueue->tail - aQueue->head)));
    } else {

        return ((size_t)(aQueue->head - aQueue->tail));
    }
}

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Vraca da li je red za cekanje pun
 *
 * @param       aQueue                  Red za cekanje koji se ispituje.
 * @return      Da li je red za cekanje pun?
 *  @retval     TRUE - red za cekanje je pun
 *  @retval     FALSE - red za cekanje nije pun
 * @pre         Red za cekanje je prethodno kreiran konstruktor funkcijom
 *              eS_qPtrInit().
 * @api
 */
/*-----------------------------------------------------------------------------------------------*/
C_INLINE bool_T eS_qPtrIsFull (
    const pq_queuePtr_T * aQueue) {

    if (((aQueue->head == aQueue->begin) && (aQueue->tail == aQueue->end)) ||
        (aQueue->tail == (aQueue->head - 1U))) {

        return (TRUE);
    } else {

        return (FALSE);
    }
}

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Vraca da li je red za cekanje prazan
 *
 * @param       aQueue                  Red za cekanje koji se ispituje.
 * @return      Da li je red za cekanje prazan?
 *  @retval     TRUE - red cekanja je prazan
 *  @retval     FALSE - red cekanaj nije prazan
 * @pre         Red za cekanje je prethodno kreiran konstruktor funkcijom
 *              eS_qPtrInit().
 * @api
 */
/*-----------------------------------------------------------------------------------------------*/
C_INLINE bool_T eS_qPtrIsEmpty(
    const pq_queuePtr_T * aQueue) {

    if (aQueue->head == aQueue->tail) {

        return (TRUE);
    } else {

        return (FALSE);
    }
}

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Manipilacija sa redovima cekanja sa podacima
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Vrsi inicijalizaciju strukture za red za cekanje
 *
 * @param       aQueue                  Pokazivac na tek kreirani red cekanja,
 * @param       aMemBuffer              memorijska oblast koja se koristi,
 * @param       aQueueSize              velicina potrebnog reda cekanja izrazena
 *                                      u broju elemenata u redu za cekanje.
 * @api
 */
/*-----------------------------------------------------------------------------------------------*/
C_INLINE void eS_qInit(
    pq_queue_T      * aQueue,
    uint8_t         * aMemBuffer,
    size_t          aQueueSize) {

    aQueue->head = aMemBuffer;
    aQueue->tail = aMemBuffer;
    aQueue->begin = aMemBuffer;
    aQueue->end = aMemBuffer + (aQueueSize - 1U);
}

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Destruktor funkcija za red za cekanje
 *
 *              Sa obzirom da Ctor funkcija nije alocirala memorijski prostor
 *              destruktor samo vraca pokazivac ka memorijskom prostoru.
 *
 * @param       aQueue                  Red za cekanje koji treba da se unisti.
 */
/*-----------------------------------------------------------------------------------------------*/
C_INLINE void * eS_qDeInit(
    pq_queue_T      * aQueue) {

    aQueue->head = (uint8_t *)0U;
    aQueue->tail = (uint8_t *)0U;
    aQueue->begin = (uint8_t *)0U;
    aQueue->end = (uint8_t *)0U;

    return ((void *)(aQueue->begin));
}

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Postavlja element @b na @b kraju reda (tail) za cekanje (FIFO metod)
 *
 * @param       aQueue                  Red cekanja gde treba postaviti dogadjaj,
 * @param       aItem                   element koji treba postaviti u red za
 *                                      cekanje.
 * @pre         Red za cekanje je prethodno kreiran konstruktor funkcijom
 *              eS_qPtrInit().
 * @iclass
 */
/*-----------------------------------------------------------------------------------------------*/
C_INLINE void eS_qPutI(
    pq_queue_T      * aQueue,
    uint8_t         aItem) {

    *(aQueue->tail) = aItem;

    if (aQueue->tail != aQueue->end) {
        ++aQueue->tail;
    } else {
        aQueue->tail = aQueue->begin;
    }
}

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Postavlja dogadjaj @b na @b pocetku reda (head) za cekanje (LIFO metod)
 *
 * @param       aQueue                  Red cekanja gde treba postaviti dogadjaj,
 * @param       aItem                   element koji treba postaviti u red za
 *                                      cekanje.
 * @pre         Red za cekanje je prethodno kreiran konstruktor funkcijom
 *              eS_qPtrInit().
 * @iclass
 */
/*-----------------------------------------------------------------------------------------------*/
C_INLINE void eS_qPushI(
    pq_queue_T   * aQueue,
    uint8_t      aItem) {

    if (aQueue->head != aQueue->begin) {
        --aQueue->head;
    } else {
        aQueue->head = aQueue->end;
    }
    *(aQueue->head) = aItem;
}

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Dobavlja element sa pocetka reda (head) za cekanje
 *
 * @param       aQueue                  Red za cekanje odakle treba dobaviti
 *                                      element.
 * @return      Element iz reda za cekanje.
 * @pre         Red za cekanje je prethodno kreiran konstruktor funkcijom
 *              eS_qPtrInit().
 * @iclass
 */
/*-----------------------------------------------------------------------------------------------*/
C_INLINE uint8_t eS_qGetI(
    pq_queue_T   * aQueue) {

    uint8_t tmp;

    tmp = *(aQueue->head);

    if (aQueue->head != aQueue->end) {
        ++aQueue->head;
    } else {
        aQueue->head = aQueue->begin;
    }

    return (tmp);
}

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Dobavlja koliko maksimalno elemenata moze da stane u red za cekanje
 *
 * @param       aQueue                  Red za cekanje koji se ispituje.
 * @return      Maksimalan broj elemenata koji mogu da stanu u red za cekanje
 * @pre         Red za cekanje je prethodno kreiran konstruktor funkcijom
 *              eS_qPtrInit().
 * @api
 */
/*-----------------------------------------------------------------------------------------------*/
C_INLINE size_t eS_qSize(
    const pq_queue_T    * aQueue) {

    return ((size_t)(aQueue->end - aQueue->begin + 1U));
}

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Dobavlja velicinu reda za cekanje
 *
 * @param       aQueue                  Red za cekanje koji se ispituje.
 * @return      Trenutni broj elemenata u redu za cekanje.
 * @pre         Red za cekanje je prethodno kreiran konstruktor funkcijom
 *              eS_qPtrInit().
 * @api
 */
/*-----------------------------------------------------------------------------------------------*/
C_INLINE size_t eS_qOccupied(
    const pq_queue_T    * aQueue) {

    if (aQueue->head < aQueue->tail) {

        return ((size_t)(aQueue->tail - aQueue->head));
    } else {

        return ((size_t)(eS_qSize(aQueue) - (aQueue->head - aQueue->tail)));
    }
}

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Dobavlja koliko jos elemenata moze da se smesti u red za cekanje
 *
 * @param       aQueue                  Red za cekanje koji se ispituje
 * @return      Koliko slobodnog prostora je preostalo.
 * @pre         Red za cekanje je prethodno kreiran konstruktor funkcijom
 *              eS_qPtrInit().
 * @api
 */
/*-----------------------------------------------------------------------------------------------*/
C_INLINE size_t eS_qFreeSpace(
    const pq_queue_T    * aQueue) {

    if (aQueue->head < aQueue->tail) {

        return ((size_t)(eS_qSize(aQueue) - (aQueue->tail - aQueue->head)));
    } else {

        return ((size_t)(aQueue->head - aQueue->tail));
    }
}

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Vraca da li je red za cekanje pun
 *
 * @param       aQueue                  Red za cekanje koji se ispituje.
 * @return      Da li je red za cekanje pun?
 *  @retval     TRUE - red za cekanje je pun
 *  @retval     FALSE - red za cekanje nije pun
 * @pre         Red za cekanje je prethodno kreiran konstruktor funkcijom
 *              eS_qCtor().
 * @api
 */
/*-----------------------------------------------------------------------------------------------*/
C_INLINE bool_T eS_qIsFull (
    const pq_queue_T    * aQueue) {

    if (((aQueue->head == aQueue->begin) && (aQueue->tail == aQueue->end)) ||
        (aQueue->tail == (aQueue->head - 1U))) {

        return (TRUE);
    } else {

        return (FALSE);
    }
}

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Vraca da li je red za cekanje prazan
 *
 * @param       aQueue                  Red za cekanje koji se ispituje.
 * @return      Da li je red za cekanje prazan?
 *  @retval     TRUE - red cekanja je prazan
 *  @retval     FALSE - red cekanaj nije prazan
 * @pre         Red za cekanje je prethodno kreiran konstruktor funkcijom
 *              eS_qCtor().
 * @api
 */
/*-----------------------------------------------------------------------------------------------*/
C_INLINE bool_T eS_qIsEmpty(
    const pq_queue_T    * aQueue) {

    if (aQueue->head == aQueue->tail) {

        return (TRUE);
    } else {

        return (FALSE);
    }
}

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
 * END of queue.h
 *************************************************************************************************/
#endif /* QUEUE_H_ */
