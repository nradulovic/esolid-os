/*
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
 * @brief       Interfejs za rad sa redovima za cekanje (queue).
 * @details     Postoje dva skupa funkcija: funkcije za rad sa 8-bitnim redovima
 *              i funkcije za redove sa pokazivacima.
 * @addtogroup  queue_intf
 *********************************************************************//** @{ */

#ifndef QUEUE_H_
#define QUEUE_H_

/*=========================================================  INCLUDE FILES  ==*/
#include "compiler.h"

/*===============================================================  MACRO's  ==*/
/*------------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/**
 * @brief       Struktura zaglavlja indirektnog reda za cekanje
 */
typedef struct esQueuePtr {
/**
 * @brief       Pocetak reda za cekanje
 */
    void **         head;

/**
 * @brief       Kraj reda za cekanje
 */
    void **         tail;

/**
 * @brief       Pocetak bafera reda za cekanje
 */
    void **         begin;

/**
 * @brief       Kraj bafera reda za cekanje + jedna pozicija
 */
    void **         end;
} esQp_T;

/**
 * @brief       Struktura zaglavlja direktnog reda za cekanje
 */
typedef struct esQueue {
/**
 * @brief       Pocetak reda za cekanje
 */
    uint8_t *       head;

/**
 * @brief       Kraj reda za cekanje
 */
    uint8_t *       tail;

/**
 * @brief       Pocetak bafera reda za cekanje
 */
    uint8_t *       begin;

/**
 * @brief       Kraj bafera reda za cekanje + jedna pozicija
 */
    uint8_t *       end;
} esQ_T;

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Manipilacija sa redovima cekanja sa pokazivacima
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Vrsi inicijalizaciju strukture za red za cekanje
 * @param       queue                   Pokazivac na tek kreirani red cekanja,
 * @param       memBuff                 memorijska oblast koja se koristi,
 * @param       size                    velicina potrebnog reda cekanja izrazena
 *                                      u broju elemenata u redu za cekanje.
 * @inline
 */
static PORT_C_INLINE_ALWAYS void esQpInit_(
    esQp_T *        queue,
    void **         memBuff,
    size_t          size) {

    queue->head = memBuff;
    queue->tail = memBuff;
    queue->begin = memBuff;
    queue->end = memBuff + (size - 1U);
}

/**
 * @brief       Destruktor funkcija za red za cekanje
 * @param       queue                   Red za cekanje koji treba da se unisti.
 * @return      Pocetak reda za cekanje.
 * @inline
 */
static PORT_C_INLINE_ALWAYS void * esQpDeInit_(
    esQp_T *        queue) {

    queue->head = (void **)0U;
    queue->tail = (void **)0U;
    queue->begin = (void **)0U;
    queue->end = (void **)0U;

    return ((void *)(queue->begin));
}

/**
 * @brief       Postavlja element @b na @b kraju reda (tail) za cekanje (FIFO
 *              metod)
 * @param       queue                   Red cekanja gde treba postaviti dogadjaj,
 * @param       item                    element koji treba postaviti u red za
 *                                      cekanje.
 * @pre         Red za cekanje je prethodno kreiran init funkcijom esQpInit_().
 * @inline
 */
static PORT_C_INLINE_ALWAYS void esQpPut_(
    esQp_T *        queue,
    void *          item) {

    *(queue->tail) = item;

    if (queue->tail != queue->end) {
        ++queue->tail;
    } else {
        queue->tail = queue->begin;
    }
}

/**
 * @brief       Postavlja dogadjaj @b na pocetku @b reda (head) za cekanje (LIFO
 *              metod)
 * @param       queue                   Red cekanja gde treba postaviti dogadjaj,
 * @param       item                    element koji treba postaviti u red za
 *                                      cekanje.
 * @pre         Red za cekanje je prethodno kreiran init funkcijom esQpInit_().
 * @inline
 */
static PORT_C_INLINE_ALWAYS void esQpPutAhead_(
    esQp_T *        queue,
    void *          item) {

    if (queue->head != queue->begin) {
        --queue->head;
    } else {
        queue->head = queue->end;
    }
    *(queue->head) = item;
}

/**
 * @brief       Dobavlja element sa pocetka reda (head) za cekanje
 * @param       queue                   Red za cekanje odakle treba dobaviti
 *                                      element.
 * @return      Element iz reda za cekanje.
 * @pre         Red za cekanje je prethodno kreiran init funkcijom esQpInit_().
 * @inline
 */
static PORT_C_INLINE_ALWAYS void * esQpGet_(
    esQp_T *        queue) {

    void * tmp;

    tmp = *(queue->head);

    if (queue->head != queue->end) {
        ++queue->head;
    } else {
        queue->head = queue->begin;
    }

    return (tmp);
}

/**
 * @brief       Dobavlja koliko maksimalno elemenata moze da stane u red za
 *              cekanje
 * @param       queue                   Red za cekanje koji se ispituje.
 * @return      Maksimalan broj elemenata koji mogu da stanu u red za cekanje
 * @pre         Red za cekanje je prethodno kreiran init funkcijom esQpInit_().
 * @inline
 */
static PORT_C_INLINE_ALWAYS size_t esQpSize_(
    const esQp_T *  queue) {

    return ((size_t)(queue->end - queue->begin + 1U));
}

/**
 * @brief       Dobavlja velicinu reda za cekanje
 * @param       queue                   Red za cekanje koji se ispituje.
 * @return      Trenutni broj elemenata u redu za cekanje.
 * @pre         Red za cekanje je prethodno kreiran init funkcijom esQpInit_().
 * @inline
 */
static PORT_C_INLINE_ALWAYS size_t esQpOccupied_(
    const esQp_T *  queue) {

    if (queue->head < queue->tail) {

        return ((size_t)(queue->tail - queue->head));
    } else {

        return ((size_t)(esQpSize_(queue) - (queue->head - queue->tail)));
    }
}

/**
 * @brief       Dobavlja koliko jos elemenata moze da se smesti u red za cekanje
 * @param       queue                   Red za cekanje koji se ispituje
 * @return      Koliko slobodnog prostora je preostalo.
 * @pre         Red za cekanje je prethodno kreiran init funkcijom esQpInit_().
 * @inline
 */
static PORT_C_INLINE_ALWAYS size_t esQpFreeSpace_(
    const esQp_T *  queue) {

    if (queue->head <= queue->tail) {

        return ((size_t)(esQpSize_(queue) - (queue->tail - queue->head)));
    } else {

        return ((size_t)(queue->head - queue->tail));
    }
}

/**
 * @brief       Vraca da li je red za cekanje pun
 * @param       queue                   Red za cekanje koji se ispituje.
 * @return      Da li je red za cekanje pun?
 *  @retval     TRUE - red za cekanje je pun
 *  @retval     FALSE - red za cekanje nije pun
 * @pre         Red za cekanje je prethodno kreiran init funkcijom esQpInit_().
 * @inline
 */
static PORT_C_INLINE_ALWAYS bool_T esQpIsFull_ (
    const esQp_T *  queue) {

    if (((queue->head == queue->begin) && (queue->tail == queue->end)) ||
        (queue->tail == (queue->head - 1U))) {

        return (TRUE);
    } else {

        return (FALSE);
    }
}

/**
 * @brief       Vraca da li je red za cekanje prazan
 * @param       queue                   Red za cekanje koji se ispituje.
 * @return      Da li je red za cekanje prazan?
 *  @retval     TRUE - red cekanja je prazan
 *  @retval     FALSE - red cekanaj nije prazan
 * @pre         Red za cekanje je prethodno kreiran init funkcijom esQpInit_().
 * @inline
 */
static PORT_C_INLINE_ALWAYS bool_T esQpIsEmpty_(
    const esQp_T *  queue) {

    if (queue->head == queue->tail) {

        return (TRUE);
    } else {

        return (FALSE);
    }
}

/** @} *//*-------------------------------------------------------------------*/
/*------------------------------------------------------------------------*//**
 * @name        Manipilacija sa redovima cekanja sa podacima
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Vrsi inicijalizaciju strukture za red za cekanje
 * @param       queue                   Pokazivac na tek kreirani red cekanja,
 * @param       memBuff                 memorijska oblast koja se koristi,
 * @param       size                    velicina potrebnog reda cekanja izrazena
 *                                      u broju elemenata u redu za cekanje.
 * @inline
 */
static PORT_C_INLINE_ALWAYS void esQInit_(
    esQ_T *         queue,
    uint8_t *       memBuff,
    size_t          size) {

    queue->head = memBuff;
    queue->tail = memBuff;
    queue->begin = memBuff;
    queue->end = memBuff + (size - 1U);
}

/**
 * @brief       Destruktor funkcija za red za cekanje
 * @param       queue                   Red za cekanje koji treba da se unisti.
 * @inline
 */
static PORT_C_INLINE_ALWAYS void * esQDeInit_(
    esQ_T *         queue) {

    queue->head = (uint8_t *)0U;
    queue->tail = (uint8_t *)0U;
    queue->begin = (uint8_t *)0U;
    queue->end = (uint8_t *)0U;

    return ((void *)(queue->begin));
}

/**
 * @brief       Postavlja element @b na @b kraju reda (tail) za cekanje (FIFO
 *              metod)
 * @param       queue                   Red cekanja gde treba postaviti dogadjaj,
 * @param       item                    element koji treba postaviti u red za
 *                                      cekanje.
 * @pre         Red za cekanje je prethodno kreiran init funkcijom esQInit_().
 * @inline
 */
static PORT_C_INLINE_ALWAYS void esQPut_(
    esQ_T      * queue,
    uint8_t         item) {

    *(queue->tail) = item;

    if (queue->tail != queue->end) {
        ++queue->tail;
    } else {
        queue->tail = queue->begin;
    }
}

/**
 * @brief       Postavlja dogadjaj @b na @b pocetku reda (head) za cekanje (LIFO
 *              metod)
 * @param       queue                   Red cekanja gde treba postaviti dogadjaj,
 * @param       item                    element koji treba postaviti u red za
 *                                      cekanje.
 * @pre         Red za cekanje je prethodno kreiran init funkcijom esQInit_().
 * @inline
 */
static PORT_C_INLINE_ALWAYS void esQPutAhead_(
    esQ_T   * queue,
    uint8_t      item) {

    if (queue->head != queue->begin) {
        --queue->head;
    } else {
        queue->head = queue->end;
    }
    *(queue->head) = item;
}

/**
 * @brief       Dobavlja element sa pocetka reda (head) za cekanje
 * @param       queue                   Red za cekanje odakle treba dobaviti
 *                                      element.
 * @return      Element iz reda za cekanje.
 * @pre         Red za cekanje je prethodno kreiran init funkcijom esQInit_().
 * @inline
 */
static PORT_C_INLINE_ALWAYS uint8_t esQGet_(
    esQ_T   * queue) {

    uint8_t tmp;

    tmp = *(queue->head);

    if (queue->head != queue->end) {
        ++queue->head;
    } else {
        queue->head = queue->begin;
    }

    return (tmp);
}

/**
 * @brief       Dobavlja koliko maksimalno elemenata moze da stane u red za
 *              cekanje
 * @param       queue                   Red za cekanje koji se ispituje.
 * @return      Maksimalan broj elemenata koji mogu da stanu u red za cekanje
 * @pre         Red za cekanje je prethodno kreiran init funkcijom esQInit_().
 * @inline
 */
static PORT_C_INLINE_ALWAYS size_t esQSize_(
    const esQ_T    * queue) {

    return ((size_t)(queue->end - queue->begin + 1U));
}

/**
 * @brief       Dobavlja velicinu reda za cekanje
 * @param       queue                   Red za cekanje koji se ispituje.
 * @return      Trenutni broj elemenata u redu za cekanje.
 * @pre         Red za cekanje je prethodno kreiran init funkcijom esQInit_().
 * @inline
 */
static PORT_C_INLINE_ALWAYS size_t esQOccupied_(
    const esQ_T    * queue) {

    if (queue->head < queue->tail) {

        return ((size_t)(queue->tail - queue->head));
    } else {

        return ((size_t)(esQSize_(queue) - (queue->head - queue->tail)));
    }
}

/**
 * @brief       Dobavlja koliko jos elemenata moze da se smesti u red za cekanje
 * @param       queue                   Red za cekanje koji se ispituje
 * @return      Koliko slobodnog prostora je preostalo.
 * @pre         Red za cekanje je prethodno kreiran init funkcijom
 *              esQInit_().
 * @inline
 */
static PORT_C_INLINE_ALWAYS size_t esQFreeSpace_(
    const esQ_T    * queue) {

    if (queue->head < queue->tail) {

        return ((size_t)(esQSize_(queue) - (queue->tail - queue->head)));
    } else {

        return ((size_t)(queue->head - queue->tail));
    }
}

/**
 * @brief       Vraca da li je red za cekanje pun
 * @param       queue                   Red za cekanje koji se ispituje.
 * @return      Da li je red za cekanje pun?
 *  @retval     TRUE - red za cekanje je pun
 *  @retval     FALSE - red za cekanje nije pun
 * @pre         Red za cekanje je prethodno kreiran init funkcijom
 *              esQInit_().
 * @inline
 */
static PORT_C_INLINE_ALWAYS bool_T esQIsFull_(
    const esQ_T *   queue) {

    if (((queue->head == queue->begin) && (queue->tail == queue->end)) ||
        (queue->tail == (queue->head - 1U))) {

        return (TRUE);
    } else {

        return (FALSE);
    }
}

/**
 * @brief       Vraca da li je red za cekanje prazan
 * @param       queue                   Red za cekanje koji se ispituje.
 * @return      Da li je red za cekanje prazan?
 *  @retval     TRUE - red cekanja je prazan
 *  @retval     FALSE - red cekanaj nije prazan
 * @pre         Red za cekanje je prethodno kreiran init funkcijom esQInit_().
 * @inline
 */
static PORT_C_INLINE_ALWAYS bool_T esQIsEmpty_(
    const esQ_T *   queue) {

    if (queue->head == queue->tail) {

        return (TRUE);
    } else {

        return (FALSE);
    }
}

/** @} *//*-------------------------------------------------------------------*/
/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of queue.h
 ******************************************************************************/
#endif /* QUEUE_H_ */
