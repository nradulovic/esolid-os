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
 * @brief       Interfejs/implementacija za manipulaciju sa povezanim listama
 *              (linked list)
 * @details     Povezane liste su inspirisane kodom koji se nalazi u Linux
 *              jezgru.
 * @addtogroup  list_intf
 ****************************************************************************************//** @{ */


#ifndef LIST_H_
#define LIST_H_


/*============================================================================  INCLUDE FILES  ==*/
#include "hal/hal.h"

/*==================================================================================  DEFINES  ==*/
/*==================================================================================  MACRO's  ==*/

/*-------------------------------------------------------------------------------------------*//**
 * @name        Makroi za Single Linked list with Sentinel, SLS liste
 * @{ *//*---------------------------------------------------------------------------------------*/

/**
 * @brief       Dobavlja pokazivac na strukturu podataka na koju pokazuje @c ptr
 *              u povezanoj listi
 * @param       ptr                     Pokazivac na clan u povezanoj listi,
 * @param       typeOfData              tip strukture podataka,
 * @param       list                    clan liste u strukturi podataka.
 * @api
 */
#define esSlsNodeEntry(typeOfData, list, ptr)                                   \
    ((typeOfData *)((char *)(ptr) - offsetof(typeOfData, list)))

/**
 * @brief       Kreira @c FOR petlju koja prelazi kroz sve clanove liste
 * @param       sentinel                Pokazivac na cuvara liste,
 * @param       currNode                pokazivac na clanove liste koji se
 *                                      koristi kao brojac.
 * @mseffect
 */
#define SLS_FOR_EACH(sentinel, currNode)                                        \
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
 */
#define SLS_FOR_EACH_ENTRY(typeOfData, list, sentinel, currNode)                \
    for (                                                                       \
        currNode = esSlsNodeEntry(typeOfData, list, (sentinel)->next);          \
        &currNode->list != (sentinel);                                          \
        currNode = esSlsNodeEntry(typeOfData, list, currNode->list.next))

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Makroi za Doubly Linked list with Sentinel, DLS liste
 * @{ *//*---------------------------------------------------------------------------------------*/

/**
 * @brief       Dobavlja pokazivac na strukturu podataka na koju pokazuje @c ptr
 *              u povezanoj listi
 * @param       ptr                     Pokazivac na clan u povezanoj listi,
 * @param       typeOfData              tip strukture podataka,
 * @param       list                    clan liste u strukturi podataka.
 * @api
 */
#define esDlsNodeEntry(typeOfData, list, ptr)                                   \
    ((typeOfData *)((char *)(ptr) - offsetof(typeOfData, list)))

/**
 * @brief       Kreira @c FOR petlju koja prelazi kroz sve clanove liste
 * @param       sentinel                Pokazivac na cuvara liste,
 * @param       currNode                pokazivac na clanove liste koji se
 *                                      koristi kao brojac.
 * @mseffect
 */
#define DLS_FOR_EACH(sentinel, currNode)                                        \
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
 */
#define DLS_FOR_EACH_ENTRY(typeOfData, list, sentinel, currNode)                \
    for (                                                                       \
        currNode = esDlsNodeEntry(typeOfData, list, (sentinel)->next);          \
        &currNode->list != (sentinel);                                          \
        currNode = esDlsNodeEntry(typeOfData, list, currNode->list.next))

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*===============================================================================  DATA TYPES  ==*/

/*-------------------------------------------------------------------------------------------*//**
 * @name        Tipovi podataka za Single Linked list with Sentinel, SLS liste
 * @{ *//*---------------------------------------------------------------------------------------*/

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
    struct esSlsList   * next;
} esSlsList_T;

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Tipovi podataka za Doubly Linked list with Sentinel, DLS liste
 * @{ *//*---------------------------------------------------------------------------------------*/

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
    struct esDlsList   * next;

/**
 * @brief       Prethodni clan u povezanoj listi
 */
    struct esDlsList   * prev;
} esDlsList_T;

/** @} *//*--------------------------------------------------------------------------------------*/

/*=========================================================================  GLOBAL VARIABLES  ==*/
/*======================================================================  FUNCTION PROTOTYPES  ==*/

/*-------------------------------------------------------------------------------------------*//**
 * @name        Funkcije za Single Linked list with Sentinel, odnosno, SLS liste
 * @{ *//*---------------------------------------------------------------------------------------*/

/**
 * @brief       Vrsi inicijalizaciju clana SLS liste
 * @param       aNode                   Pokazivac na clan SLS liste.
 * @details     Ova funkcija se najcesce poziva nakon brisanja datog clana iz
 *              neke liste.
 * @inline
 */
static C_INLINE_ALWAYS void esSlsNodeInit_(
    esSlsList_T    * aNode) {

    aNode->next = aNode;
}

/**
 * @brief       Vrsi inicijalizaciju clana SLS liste
 * @param       aSentinel               Pokazivac na cuvara SLS liste.
 * @details     Ova funkcija se najcesce poziva pre koriscenja SLS liste.
 * @inline
 */
static C_INLINE_ALWAYS void esSlsSentinelInit_(
    esSlsList_T    * aSentinel) {

    esSlsNodeInit_(
        aSentinel);
}

/**
 * @brief       Dodaje @c aNewBlk izmedju @c aPrevBlk i @c aNextBlk
 * @param       aNewNode                Pokazivac na novi clan koji treba dodati
 *                                      u listu,
 * @param       aPrevNode               pokazivac na prethodni clan u listi,
 * @param       aNextNode               pokazivac na sledeci clan u listi.
 * @details     Dodaje clan @c aNew u listu kada su poznati prethodni i sledeci
 *              clanovi.
 * @inline
 */
static C_INLINE_ALWAYS void esSlsNodeAdd_(
    esSlsList_T    * aNewNode,
    esSlsList_T    * aPrevNode,
    esSlsList_T    * aNextNode) {

    aNewNode->next = aNextNode;
    aPrevNode->next = aNewNode;
}

/**
 * @brief       Dodaje @c aNewNode nakon @c aCurrNode clana.
 * @param       aCurrNode               Pokazivac na trenutno koriscenog clana,
 * @param       aNewNode                pokazivac na clan koji se dodaje.
 * @inline
 */
static C_INLINE_ALWAYS void esSlsNodeAddAfter_(
    esSlsList_T    * aCurrNode,
    esSlsList_T    * aNewNode) {

    esSlsNodeAdd_(
        aNewNode,
        aCurrNode,
        aCurrNode->next);
}

/**
 * @brief       Dodaje @c aNewNode na pocetak liste koju cuva @c aSentinel.
 * @param       aSentinel               Pokazivac na cuvara liste,
 * @param       aNewNode                pokazivac na clan koji se dodaje.
 * @inline
 */
static C_INLINE_ALWAYS void esSlsNodeAddHead_(
    esSlsList_T    * aSentinel,
    esSlsList_T    * aNewNode) {

    esSlsNodeAdd_(
        aNewNode,
        aSentinel,
        aSentinel->next);
}

/**
 * @brief       Uklanja @c aOldNode iz povezane lise.
 * @param       aOldNode                Pokazivac na clan koji se uklanja iz
 *                                      liste,
 * @param       aPrevNode               pokazivac na prethodni clan u listi,
 * @param       aNextNode               pokazivac na sledeci clan u listi.
 * @details     Dodaje clan @c aNew u listu kada su poznati prethodni i sledeci
 *              clanovi.
 * @inline
 */
static C_INLINE_ALWAYS void esSlsNodeRemove_(
    esSlsList_T    * aOldNode,
    esSlsList_T    * aPrevNode,
    esSlsList_T    * aNextNode) {

    aPrevNode->next = aNextNode;
    (void)aOldNode;
}

/**
 * @brief       Uklanja clan nakon clana @c aCurrNode iz povezane lise.
 * @param       aCurrNode               Pokazivac na clan u SLS listi nakon
 *                                      kojeg treba ukloniti clan
 * @inline
 */
static C_INLINE_ALWAYS void esSlsNodeRemoveAfter_(
    esSlsList_T    * aCurrNode) {

    esSlsNodeRemove_(
        aCurrNode->next,
        aCurrNode,
        aCurrNode->next->next);
}

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Funkcije za Doubly Linked list with Sentinel, odnosno, DLS liste
 * @{ *//*---------------------------------------------------------------------------------------*/

/**
 * @brief       Vrsi inicijalizaciju clana DLS liste
 * @param       aNode                   Pokazivac na clan DLS liste.
 * @inline
 */
static C_INLINE_ALWAYS void esDlsNodeInit_(
    esDlsList_T    * aNode) {

    aNode->next = aNode;
    aNode->prev = aNode;
}

/**
 * @brief       Vrsi inicijalizaciju cuvara DLS liste
 * @param       aSentinel               Pokazivac na clan tipa esDlsList_T
 *                                      u strukturi podataka.
 * @inline
 */
static C_INLINE_ALWAYS void esDlsSentinelInit_(
    esDlsList_T    * aSentinel) {

    esDlsNodeInit_(
        aSentinel);
}

/**
 * @brief       Dodaje @c aNewBlk izmedju @c aPrevBlk i @c aNextBlk
 * @param       aNew                    Pokazivac na novi clan koji treba dodati
 *                                      u listu,
 * @param       aPrev                   pokazivac na prethodni clan u listi,
 * @param       aNext                   pokazivac na sledeci clan u listi.
 * @details     Dodaje clan @c aNew u listu kada su poznati prethodni i sledeci
 *              clanovi.
 * @inline
 */
static C_INLINE_ALWAYS void esDlsNodeAdd_(
    esDlsList_T    * aNew,
    esDlsList_T    * aPrev,
    esDlsList_T    * aNext) {

    aNew->next = aNext;
    aNew->prev = aPrev;
    aPrev->next = aNew;
    aNext->prev = aNew;
}

/**
 * @brief       Dodaje @c aNewNode na pocetak liste koju cuva @c aSentinel.
 * @param       aSentinel               Pokazivac na cuvara liste,
 * @param       aNewNode                pokazivac na clan koji se dodaje.
 * @inline
 */
static C_INLINE_ALWAYS void esDlsNodeAddHead_(
    esDlsList_T    * aSentinel,
    esDlsList_T    * aNewNode) {

    esDlsNodeAdd_(
        aNewNode,
        aSentinel,
        aSentinel->next);
}

/**
 * @brief       Dodaje @c aNewNode na kraj liste koju cuva @c aSentinel.
 * @param       aSentinel               Pokazivac na cuvara liste,
 * @param       aNewNode                pokazivac na clan koji se dodaje.
 * @inline
 */
static C_INLINE_ALWAYS void esDlsNodeAddTail_(
    esDlsList_T    * aSentinel,
    esDlsList_T    * aNewNode) {

    esDlsNodeAdd_(
        aNewNode,
        aSentinel->prev,
        aSentinel);
}

/**
 * @brief       Dodaje @c aNewNode pre @c aCurrNode clana.
 * @param       aCurrNode               Pokazivac na trenutno koriscenog clana,
 * @param       aNewNode                pokazivac na clan koji se dodaje.
 * @inline
 */
static C_INLINE_ALWAYS void esDlsNodeAddBefore_(
    esDlsList_T    * aCurrNode,
    esDlsList_T    * aNewNode) {

    esDlsNodeAdd_(
        aNewNode,
        aCurrNode->prev,
        aCurrNode);
}

/**
 * @brief       Dodaje @c aNewNode nakon @c aCurrNode clana.
 * @param       aCurrNode               Pokazivac na trenutno koriscenog clana,
 * @param       aNewNode                pokazivac na clan koji se dodaje.
 * @inline
 */
static C_INLINE_ALWAYS void esDlsNodeAddAfter_(
    esDlsList_T    * aCurrNode,
    esDlsList_T    * aNewNode) {

    esDlsNodeAdd_(
        aNewNode,
        aCurrNode,
        aCurrNode->next);
}

/**
 * @brief       Uklanja @c aOldNode iz povezane lise.
 * @param       aOldNode                Pokazivac na clan koji se uklanja.
 * @inline
 */
static C_INLINE_ALWAYS void esDlsNodeRemove_(
    esDlsList_T    * aOldNode) {

    aOldNode->next->prev = aOldNode->prev;
    aOldNode->prev->next = aOldNode->next;
}

/**
 * @brief       Ispituje da li je lista prazna.
 * @param       aSentinel               Pokazivac na cuvara liste.
 * @return      Stanje liste
 *  @retval     TRUE - lista je prazna
 *  @retval     FALSE - lista nije prazna
 * @inline
 */
static C_INLINE_ALWAYS bool_T esDlsIsEmpty_(
    esDlsList_T     * aSentinel) {

    if (aSentinel->next != aSentinel) {

        return (FALSE);
    } else {

        return (TRUE);
    }
}

/** @} *//*--------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*===================================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/

/** @endcond *//** @} *//*************************************************************************
 * END of list.h
 *************************************************************************************************/
#endif /* LIST_H_ */
