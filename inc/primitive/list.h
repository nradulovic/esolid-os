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
 * @brief       Interfejs/implementacija za manipulaciju sa povezanim listama
 *              (linked list)
 * @details     Povezane liste su inspirisane kodom koji se nalazi u Linux
 *              jezgru.
 * @addtogroup  list_intf
 *********************************************************************//** @{ */

#ifndef LIST_H_
#define LIST_H_

/*=========================================================  INCLUDE FILES  ==*/
#include "hal/hal.h"

/*===============================================================  DEFINES  ==*/
/*===============================================================  MACRO's  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Makroi za Single Linked list with Sentinel, SLS liste
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Dobavlja pokazivac na strukturu podataka na koju pokazuje @c ptr
 *              u povezanoj listi
 * @param       ptr                     Pokazivac na clan u povezanoj listi,
 * @param       typeOfData              tip strukture podataka,
 * @param       list                    clan liste u strukturi podataka.
 * @api
 */
#define ES_SLS_NODE_ENTRY(typeOfData, list, ptr)                                \
    ((typeOfData *)((char *)(ptr) - offsetof(typeOfData, list)))

/**
 * @brief       Kreira @c FOR petlju koja prelazi kroz sve clanove liste
 * @param       sentinel                Pokazivac na cuvara liste,
 * @param       currNode                pokazivac na clanove liste koji se
 *                                      koristi kao brojac.
 * @mseffect
 * @api
 */
#define ES_SLS_FOR_EACH(sentinel, currNode)                                     \
    for (                                                                       \
        currNode = (sentinel)->next;                                            \
        currNode != (sentinel);                                                 \
        currNode = (currNode)->next)

/**
 * @brief       Kreira @c FOR petlju koja prelazi kroz sve clanove liste i
 *              izvlaci trenutni clan iz liste radi pristupa
 * @param       typeOfData              Tip strukture podataka koji formiraju
 *                                      listu,
 * @param       list                    clan liste u strukturi podataka,
 * @param       sentinel                pokazivac na cuvara liste,
 * @param       currNode                pokazivac na podatke u povezanoj listi.
 * @details     Pokazivac @c currNode se koristi za pristup zeljenim podacima.
 * @mseffect
 * @api
 */
#define ES_SLS_FOR_EACH_ENTRY(typeOfData, list, sentinel, currNode)             \
    for (                                                                       \
        currNode = ES_SLS_NODE_ENTRY(typeOfData, list, (sentinel)->next);       \
        &currNode->list != (sentinel);                                          \
        currNode = ES_SLS_NODE_ENTRY(typeOfData, list, currNode->list.next))

/** @} *//*-------------------------------------------------------------------*/
/*------------------------------------------------------------------------*//**
 * @name        Makroi za Doubly Linked list with Sentinel, DLS liste
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Dobavlja pokazivac na strukturu podataka na koju pokazuje @c ptr
 *              u povezanoj listi
 * @param       ptr                     Pokazivac na clan u povezanoj listi,
 * @param       typeOfData              tip strukture podataka,
 * @param       list                    clan liste u strukturi podataka.
 * @api
 */
#define ES_DLS_NODE_ENTRY(typeOfData, list, ptr)                                \
    ((typeOfData *)((char *)(ptr) - offsetof(typeOfData, list)))

/**
 * @brief       Kreira @c FOR petlju koja prelazi kroz sve clanove liste
 * @param       sentinel                Pokazivac na cuvara liste,
 * @param       currNode                pokazivac na clanove liste koji se
 *                                      koristi kao brojac.
 * @mseffect
 * @api
 */
#define ES_DLS_FOR_EACH(sentinel, currNode)                                     \
    for (                                                                       \
        currNode = (sentinel)->next;                                            \
        currNode != (sentinel);                                                 \
        currNode = (currNode)->next)

/**
 * @brief       Kreira @c FOR petlju koja prelazi kroz sve clanove liste i
 *              izvlaci trenutni clan iz liste radi pristupa
 * @param       typeOfData              Tip strukture podataka koji formiraju
 *                                      listu,
 * @param       list                    clan liste u strukturi podataka,
 * @param       sentinel                pokazivac na cuvara liste,
 * @param       currNode                pokazivac na podatke u povezanoj listi.
 * @details     Pokazivac @c currNode se koristi za pristup zeljenim podacima.
 * @mseffect
 * @api
 */
#define ES_DLS_FOR_EACH_ENTRY(typeOfData, list, sentinel, currNode)             \
    for (                                                                       \
        currNode = ES_DLS_NODE_ENTRY(typeOfData, list, (sentinel)->next);       \
        &currNode->list != (sentinel);                                          \
        currNode = ES_DLS_NODE_ENTRY(typeOfData, list, currNode->list.next))

/** @} *//*-------------------------------------------------------------------*/
/*------------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Tipovi podataka za Single linked list With Sentinel, SLS liste
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Cuvar/clan SLS liste
 * @details     Ova struktura mora da postoji u strukturi podataka
 *              koji formiraju povezanu listu. Dovoljno je ukljuciti ovu
 *              strukturu (pozicija u strukturi je nebitna) i mogu se koristiti
 *              funkcije za rad sa povezanim listama.
 * @note        Bolje performanse se mogu postici postavljanjem ove strukture
 *              na prvom mestu u strukturi podataka koji formiraju povezanu
 *              listu.
 */
typedef struct esSlsList {
/**
 *  @brief      Sledeci clan u povezanoj listi
 */
    struct esSlsList * next;
} esSlsList_T;

/** @} *//*-------------------------------------------------------------------*/
/*------------------------------------------------------------------------*//**
 * @name        Tipovi podataka za Doubly linked list With Sentinel, DLS liste
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Cuvar/clan DLS liste
 * @details     Ova struktura mora da postoji u strukturi podataka
 *              koji formiraju povezanu listu. Dovoljno je ukljuciti ovu
 *              strukturu (pozicija u strukturi je nebitna) i mogu se koristiti
 *              funkcije za rad sa povezanim listama.
 * @note        Bolje performanse se mogu postici postavljanjem ove strukture
 *              na prvom mestu u strukturi podataka koji formiraju povezanu
 *              listu.
 */
typedef struct esDlsList {
/**
 * @brief       Sledeci clan u povezanoj listi
 */
    struct esDlsList * next;

/**
 * @brief       Prethodni clan u povezanoj listi
 */
    struct esDlsList * prev;
} esDlsList_T;

/** @} *//*-------------------------------------------------------------------*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Funkcije za Single Linked list with Sentinel, odnosno, SLS liste
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Vrsi inicijalizaciju clana SLS liste
 * @param       node                    Pokazivac na clan SLS liste.
 * @details     Vrsi inicijalizaciju clana da pokazuje na samog sebe.
 * @inline
 */
static C_INLINE_ALWAYS void esSlsNodeInit_(
    esSlsList_T *   node) {

    node->next = node;
}

/**
 * @brief       Vrsi inicijalizaciju clana SLS liste
 * @param       sentinel                Pokazivac na cuvara SLS liste.
 * @details     Cuvar liste se mora inicijalizovati pre koriscenja liste.
 * @inline
 */
static C_INLINE_ALWAYS void esSlsSentinelInit_(
    esSlsList_T *   sentinel) {

    esSlsNodeInit_(
        sentinel);
}

/**
 * @brief       Dodaje @c newNode izmedju @c prevNode i @c nextNode
 * @param       newNode                 Pokazivac na novi clan koji treba dodati
 *                                      u listu,
 * @param       prevNode                pokazivac na prethodni clan u listi,
 * @param       nextNode                pokazivac na sledeci clan u listi.
 * @details     Dodaje clan @c newNode u listu kada su poznati prethodni i
 *              sledeci clanovi. Clan @c newNode mora prethodno da bude
 *              inicijalizovan funkcijom esSlsNodeInit_().
 * @inline
 */
static C_INLINE_ALWAYS void esSlsNodeAdd_(
    esSlsList_T *   newNode,
    esSlsList_T *   prevNode,
    esSlsList_T *   nextNode) {

    newNode->next = nextNode;
    prevNode->next = newNode;
}

/**
 * @brief       Dodaje @c newNode nakon @c currNode clana.
 * @param       currNode                Pokazivac na trenutno koriscenog clana,
 * @param       newNode                 pokazivac na clan koji se dodaje.
 * @inline
 */
static C_INLINE_ALWAYS void esSlsNodeAddAfter_(
    esSlsList_T *   currNode,
    esSlsList_T *   newNode) {

    esSlsNodeAdd_(
        newNode,
        currNode,
        currNode->next);
}

/**
 * @brief       Dodaje @c newNode na pocetak liste koju cuva @c sentinel.
 * @param       sentinel                Pokazivac na cuvara liste,
 * @param       newNode                 pokazivac na clan koji se dodaje.
 * @inline
 */
static C_INLINE_ALWAYS void esSlsNodeAddHead_(
    esSlsList_T *   sentinel,
    esSlsList_T *   newNode) {

    esSlsNodeAdd_(
        newNode,
        sentinel,
        sentinel->next);
}

/**
 * @brief       Uklanja @c oldNode iz povezane lise.
 * @param       oldNode                 Pokazivac na clan koji se uklanja iz
 *                                      liste,
 * @param       prevNode                pokazivac na prethodni clan u listi,
 * @param       nextNode                pokazivac na sledeci clan u listi.
 * @details     Uklanja clan @c oldNode u listu kada su poznati prethodni i
 *              sledeci clanovi liste.
 * @inline
 */
static C_INLINE_ALWAYS void esSlsNodeRm_(
    esSlsList_T *   oldNode,
    esSlsList_T *   prevNode,
    esSlsList_T *   nextNode) {

    prevNode->next = nextNode;
    (void)oldNode;
}

/**
 * @brief       Uklanja clan nakon clana @c currNode iz povezane lise.
 * @param       currNode                Pokazivac na clan u SLS listi nakon
 *                                      kojeg treba ukloniti clan
 * @inline
 */
static C_INLINE_ALWAYS void esSlsNodeRmAfter_(
    esSlsList_T *   currNode) {

    esSlsNodeRm_(
        currNode->next,
        currNode,
        currNode->next->next);
}

/** @} *//*-------------------------------------------------------------------*/
/*------------------------------------------------------------------------*//**
 * @name        Funkcije za Doubly Linked list with Sentinel, odnosno, DLS liste
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Vrsi inicijalizaciju clana DLS liste
 * @param       node                    Pokazivac na clan DLS liste.
 * @inline
 */
static C_INLINE_ALWAYS void esDlsNodeInit_(
    esDlsList_T *   node) {

    node->next = node;
    node->prev = node;
}

/**
 * @brief       Vrsi inicijalizaciju cuvara DLS liste
 * @param       sentinel                Pokazivac na clan tipa esDlsList_T
 *                                      u strukturi podataka.
 * @inline
 */
static C_INLINE_ALWAYS void esDlsSentinelInit_(
    esDlsList_T *   sentinel) {

    esDlsNodeInit_(
        sentinel);
}

/**
 * @brief       Dodaje @c newNode izmedju @c prevNode i @c nextNode
 * @param       newNode                 Pokazivac na novi clan koji treba dodati
 *                                      u listu,
 * @param       prevNode                pokazivac na prethodni clan u listi,
 * @param       nextNode                pokazivac na sledeci clan u listi.
 * @details     Dodaje clan @c aNew u listu kada su poznati prethodni i sledeci
 *              clanovi.
 * @inline
 */
static C_INLINE_ALWAYS void esDlsNodeAdd_(
    esDlsList_T *   newNode,
    esDlsList_T *   prevNode,
    esDlsList_T *   nextNode) {

    newNode->next = nextNode;
    newNode->prev = prevNode;
    prevNode->next = newNode;
    nextNode->prev = newNode;
}

/**
 * @brief       Dodaje @c newNode na pocetak liste koju cuva @c sentinel.
 * @param       sentinel                Pokazivac na cuvara liste,
 * @param       newNode                 pokazivac na clan koji se dodaje.
 * @inline
 */
static C_INLINE_ALWAYS void esDlsNodeAddHead_(
    esDlsList_T *   sentinel,
    esDlsList_T *   newNode) {

    esDlsNodeAdd_(
        newNode,
        sentinel,
        sentinel->next);
}

/**
 * @brief       Dodaje @c newNode na kraj liste koju cuva @c sentinel.
 * @param       sentinel                Pokazivac na cuvara liste,
 * @param       newNode                 pokazivac na clan koji se dodaje.
 * @inline
 */
static C_INLINE_ALWAYS void esDlsNodeAddTail_(
    esDlsList_T *   sentinel,
    esDlsList_T *   newNode) {

    esDlsNodeAdd_(
        newNode,
        sentinel->prev,
        sentinel);
}

/**
 * @brief       Dodaje @c newNode pre @c currNode clana.
 * @param       currNode                Pokazivac na trenutno koriscenog clana,
 * @param       newNode                 pokazivac na clan koji se dodaje.
 * @inline
 */
static C_INLINE_ALWAYS void esDlsNodeAddBefore_(
    esDlsList_T *   currNode,
    esDlsList_T *   newNode) {

    esDlsNodeAdd_(
        newNode,
        currNode->prev,
        currNode);
}

/**
 * @brief       Dodaje @c newNode nakon @c currNode clana.
 * @param       currNode                Pokazivac na trenutno koriscenog clana,
 * @param       newNode                 pokazivac na clan koji se dodaje.
 * @inline
 */
static C_INLINE_ALWAYS void esDlsNodeAddAfter_(
    esDlsList_T *   currNode,
    esDlsList_T *   newNode) {

    esDlsNodeAdd_(
        newNode,
        currNode,
        currNode->next);
}

/**
 * @brief       Uklanja @c oldNode iz povezane lise.
 * @param       oldNode                 Pokazivac na clan koji se uklanja.
 * @inline
 */
static C_INLINE_ALWAYS void esDlsNodeRm_(
    esDlsList_T *   oldNode) {

    oldNode->next->prev = oldNode->prev;
    oldNode->prev->next = oldNode->next;
}

/**
 * @brief       Ispituje da li je lista prazna.
 * @param       sentinel                Pokazivac na cuvara liste.
 * @return      Stanje liste
 *  @retval     TRUE - lista je prazna
 *  @retval     FALSE - lista nije prazna
 * @inline
 */
static C_INLINE_ALWAYS bool_T esDlsIsEmpty_(
    esDlsList_T *   sentinel) {

    if (sentinel->next != sentinel) {

        return (FALSE);
    } else {

        return (TRUE);
    }
}

/** @} *//*-------------------------------------------------------------------*/
/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of list.h
 ******************************************************************************/
#endif /* LIST_H_ */
