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
 * @brief       Interfejs za manipulaciju sa povezanim listama (linked list)
 *
 * ------------------------------------------------------------------------------------------------
 *
 * @addtogroup  list_intf
 *
 ****************************************************************************************//** @{ */


#ifndef LIST_H_
#define LIST_H_


/*************************************************************************************************
 * INCLUDE FILES
 *************************************************************************************************/

/*-----------------------------------------------------------------------------------------------*
 * Module dependencies
 *-----------------------------------------------------------------------------------------------*/

#include "hal/hal.h"


/*-----------------------------------------------------------------------------------------------*
 * Module configuration and adapter
 *-----------------------------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------------------------*
 * EXTERNS
 *-----------------------------------------------------------------------------------*//** @cond */

#ifdef LIST_H_VAR
# define LIST_H_EXT
#else
# define LIST_H_EXT extern
#endif


/** @endcond*//***********************************************************************************
 * DEFINES
 *************************************************************************************************/


/*************************************************************************************************
 * MACRO's
 *************************************************************************************************/

/*-------------------------------------------------------------------------------------------*//**
 * @name        Makroi za Single Linked list with Sentinel, SLS liste
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Dobavlja pokazivac na strukturu podataka na koju pokazuje @c ptr
 *              u povezanoj listi
 *
 * @param       ptr                     Pokazivac na clan u povezanoj listi,
 * @param       typeOfData              tip strukture podataka,
 * @param       list                    clan liste u strukturi podataka.
 * @api
 */
/*-----------------------------------------------------------------------------------------------*/
#define eS_slsNodeEntry(typeOfData, list, ptr)                                  \
    ((typeOfData *)((char *)(ptr) - offsetof(typeOfData, list)))

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Kreira @c FOR petlju koja prelazi kroz sve clanove liste
 *
 * @param       sentinel                Pokazivac na cuvara liste,
 * @param       currNode                pokazivac na clanove liste koji se
 *                                      koristi kao brojac.
 * @mseffect
 */
/*-----------------------------------------------------------------------------------------------*/
#define SLS_FOR_EACH(sentinel, currNode)                                        \
    for (                                                                       \
        currNode = (sentinel)->next;                                            \
        currNode != (sentinel);                                                 \
        currNode = (currNode)->next)

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Kreira @c FOR petlju koja prelazi kroz sve clanove liste i
 *              izvlaci trenutni clan iz liste radi pristupa
 *
 * @param       typeOfData              Tip strukture podataka koji formiraju
 *                                      listu,
 * @param       list                    clan liste u strukturi podataka,
 * @param       sentinel                pokazivac na cuvara liste,
 * @param       currNode                pokazivac na podatke u povezanoj listi.
 *
 *              Pokazivac @c currNode se koristi za pristup zeljenim podacima.
 *
 * @mseffect
 */
/*-----------------------------------------------------------------------------------------------*/
#define SLS_FOR_EACH_ENTRY(typeOfData, list, sentinel, currNode)                \
    for (                                                                       \
        currNode = eS_slsNodeEntry(typeOfData, list, (sentinel)->next);         \
        &currNode->list != (sentinel);                                          \
        currNode = eS_slsNodeEntry(typeOfData, list, currNode->list.next))

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Makroi za Doubly Linked list with Sentinel, DLS liste
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Dobavlja pokazivac na strukturu podataka na koju pokazuje @c ptr
 *              u povezanoj listi
 *
 * @param       ptr                     Pokazivac na clan u povezanoj listi,
 * @param       typeOfData              tip strukture podataka,
 * @param       list                    clan liste u strukturi podataka.
 * @api
 */
/*-----------------------------------------------------------------------------------------------*/
#define eS_dlsNodeEntry(typeOfData, list, ptr)                                  \
    ((typeOfData *)((char *)(ptr) - offsetof(typeOfData, list)))

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Kreira @c FOR petlju koja prelazi kroz sve clanove liste
 *
 * @param       sentinel                Pokazivac na cuvara liste,
 * @param       currNode                pokazivac na clanove liste koji se
 *                                      koristi kao brojac.
 * @mseffect
 */
/*-----------------------------------------------------------------------------------------------*/
#define DLS_FOR_EACH(sentinel, currNode)                                        \
    for (                                                                       \
        currNode = (sentinel)->next;                                            \
        currNode != (sentinel);                                                 \
        currNode = (currNode)->next)

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Kreira @c FOR petlju koja prelazi kroz sve clanove liste i
 *              izvlaci trenutni clan iz liste radi pristupa
 *
 * @param       typeOfData              Tip strukture podataka koji formiraju
 *                                      listu,
 * @param       list                    clan liste u strukturi podataka,
 * @param       sentinel                pokazivac na cuvara liste,
 * @param       currNode                pokazivac na podatke u povezanoj listi.
 *
 *              Pokazivac @c currNode se koristi za pristup zeljenim podacima.
 *
 * @mseffect
 */
/*-----------------------------------------------------------------------------------------------*/
#define DLS_FOR_EACH_ENTRY(typeOfData, list, sentinel, currNode)                \
    for (                                                                       \
        currNode = eS_dlsNodeEntry(typeOfData, list, (sentinel)->next);         \
        &currNode->list != (sentinel);                                          \
        currNode = eS_dlsNodeEntry(typeOfData, list, currNode->list.next))

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
 * @name        Tipovi podataka za Single Linked list with Sentinel, SLS liste
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Cuvar/clan SLS liste
 *
 *              Ova struktura mora da postoji u strukturi podataka
 *              koji formiraju povezanu listu. Dovoljno je ukljuciti ovu
 *              strukturu (pozicija u strukturi je nebitna) i mogu se koristiti
 *              funkcije za rad sa povezanim listama.
 *
 * @note        Bolje performanse se mogu postici postavljanjem ove strukture
 *              na prvom mestu u strukturi podataka koji formiraju povezanu
 *              listu.
 */
/*-----------------------------------------------------------------------------------------------*/
typedef struct pl_slsList {
/**
 *  @brief      Sledeci clan u povezanoj listi
 */
    struct pl_slsList   * next;
} pl_slsList_T;

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Tipovi podataka za Doubly Linked list with Sentinel, DLS liste
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Cuvar/clan DLS liste
 *
 *              Ova struktura mora da postoji u strukturi podataka
 *              koji formiraju povezanu listu. Dovoljno je ukljuciti ovu
 *              strukturu (pozicija u strukturi je nebitna) i mogu se koristiti
 *              funkcije za rad sa povezanim listama.
 *
 * @note        Bolje performanse se mogu postici postavljanjem ove strukture
 *              na prvom mestu u strukturi podataka koji formiraju povezanu
 *              listu.
 */
/*-----------------------------------------------------------------------------------------------*/
typedef struct pl_dlsList {
/**
 * @brief       Sledeci clan u povezanoj listi
 */
    struct pl_dlsList   * next;

/**
 * @brief       Prethodni clan u povezanoj listi
 */
    struct pl_dlsList   * prev;
} pl_dlsList_T;

/** @} *//*--------------------------------------------------------------------------------------*/


/*************************************************************************************************
 * GLOBAL VARIABLES
 *************************************************************************************************/


/*************************************************************************************************
 * FUNCTION PROTOTYPES
 *************************************************************************************************/

/*-------------------------------------------------------------------------------------------*//**
 * @name        Funkcije za Single Linked list with Sentinel, odnosno, SLS liste
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Vrsi inicijalizaciju clana SLS liste
 *
 *              Ova funkcija se najcesce poziva nakon brisanja datog clana iz
 *              neke liste.
 *
 * @param       aNode                   Pokazivac na clan SLS liste.
 * @iclass
 */
/*-----------------------------------------------------------------------------------------------*/
C_INLINE void eS_slsNodeInitI(
    pl_slsList_T    * aNode) {

    aNode->next = aNode;
}

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Vrsi inicijalizaciju clana SLS liste
 *
 *              Ova funkcija se najcesce poziva pre koriscenja SLS liste.
 *
 * @param       aSentinel               Pokazivac na cuvara SLS liste.
 * @iclass
 */
/*-----------------------------------------------------------------------------------------------*/
C_INLINE void eS_slsSentinelInit(
    pl_slsList_T    * aSentinel) {

    eS_slsNodeInitI(
        aSentinel);
}

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Dodaje @c aNewBlk izmedju @c aPrevBlk i @c aNextBlk
 *
 *              Dodaje clan @c aNew u listu kada su poznati prethodni i sledeci
 *              clanovi.
 *
 * @param       aNewNode                Pokazivac na novi clan koji treba dodati
 *                                      u listu,
 * @param       aPrevNode               pokazivac na prethodni clan u listi,
 * @param       aNextNode               pokazivac na sledeci clan u listi.
 * @iclass
 */
/*-----------------------------------------------------------------------------------------------*/
C_INLINE void eS_slsNodeAddI(
    pl_slsList_T    * aNewNode,
    pl_slsList_T    * aPrevNode,
    pl_slsList_T    * aNextNode) {

    aNewNode->next = aNextNode;
    aPrevNode->next = aNewNode;
}

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Dodaje @c aNewNode nakon @c aCurrNode clana.
 *
 * @param       aCurrNode               Pokazivac na trenutno koriscenog clana,
 * @param       aNewNode                pokazivac na clan koji se dodaje.
 * @iclass
 */
/*-----------------------------------------------------------------------------------------------*/
C_INLINE void eS_slsNodeAddAfterI(
    pl_slsList_T    * aCurrNode,
    pl_slsList_T    * aNewNode) {

    eS_slsNodeAddI(
        aNewNode,
        aCurrNode,
        aCurrNode->next);
}

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Dodaje @c aNewNode na pocetak liste koju cuva @c aSentinel.
 *
 * @param       aSentinel               Pokazivac na cuvara liste,
 * @param       aNewNode                pokazivac na clan koji se dodaje.
 * @iclass
 */
/*-----------------------------------------------------------------------------------------------*/
C_INLINE void eS_slsNodeAddHeadI(
    pl_slsList_T    * aSentinel,
    pl_slsList_T    * aNewNode) {

    eS_slsNodeAddI(
        aNewNode,
        aSentinel,
        aSentinel->next);
}

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Uklanja @c aOldNode iz povezane lise.
 *
 *              Funkcija se koristi kada su poznati prethodni i sledeci clan u
 *              listi.
 *
 * @param       aOldNode                Pokazivac na clan koji se uklanja iz
 *                                      liste,
 * @param       aPrevNode               pokazivac na prethodni clan u listi,
 * @param       aNextNode               pokazivac na sledeci clan u listi.
 * @iclass
 */
/*-----------------------------------------------------------------------------------------------*/
C_INLINE void eS_slsNodeRemoveI(
    pl_slsList_T    * aOldNode,
    pl_slsList_T    * aPrevNode,
    pl_slsList_T    * aNextNode) {

    aPrevNode->next = aNextNode;
    (void)aOldNode;
}

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Uklanja clan nakon clana @c aCurrNode iz povezane lise.
 *
 * @param       aCurrNode               Pokazivac na clan u SLS listi nakon
 *                                      kojeg treba ukloniti clan
 * @iclass
 */
/*-----------------------------------------------------------------------------------------------*/
C_INLINE void eS_slsNodeRemoveAfterI(
    pl_slsList_T    * aCurrNode) {

    eS_slsNodeRemoveI(
        aCurrNode->next,
        aCurrNode,
        aCurrNode->next->next);
}

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Funkcije za Doubly Linked list with Sentinel, odnosno, DLS liste
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Vrsi inicijalizaciju clana DLS liste
 *
 * @param       aNode                   Pokazivac na clan DLS liste.
 * @iclass
 */
/*-----------------------------------------------------------------------------------------------*/
C_INLINE void eS_dlsNodeInitI(
    pl_dlsList_T    * aNode) {

    aNode->next = aNode;
    aNode->prev = aNode;
}

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Vrsi inicijalizaciju cuvara DLS liste
 *
 * @param       aSentinel               Pokazivac na clan tipa pl_dlsList_T
 *                                      u strukturi podataka.
 * @iclass
 */
/*-----------------------------------------------------------------------------------------------*/
C_INLINE void eS_dlsSentinelInit(
    pl_dlsList_T    * aSentinel) {

    eS_dlsNodeInitI(
        aSentinel);
}

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Dodaje @c aNewBlk izmedju @c aPrevBlk i @c aNextBlk
 *
 *              Dodaje clan @c aNew u listu kada su poznati prethodni i sledeci
 *              clanovi.
 *
 * @param       aNew                    Pokazivac na novi clan koji treba dodati
 *                                      u listu,
 * @param       aPrev                   pokazivac na prethodni clan u listi,
 * @param       aNext                   pokazivac na sledeci clan u listi.
 * @iclass
 */
/*-----------------------------------------------------------------------------------------------*/
C_INLINE void eS_dlsNodeAddI(
    pl_dlsList_T    * aNew,
    pl_dlsList_T    * aPrev,
    pl_dlsList_T    * aNext) {

    aNew->next = aNext;
    aNew->prev = aPrev;
    aPrev->next = aNew;
    aNext->prev = aNew;
}

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Dodaje @c aNewNode na pocetak liste koju cuva @c aSentinel.
 *
 * @param       aSentinel               Pokazivac na cuvara liste,
 * @param       aNewNode                pokazivac na clan koji se dodaje.
 * @iclass
 */
/*-----------------------------------------------------------------------------------------------*/
C_INLINE void eS_dlsNodeAddHeadI(
    pl_dlsList_T    * aSentinel,
    pl_dlsList_T    * aNewNode) {

    eS_dlsNodeAddI(
        aNewNode,
        aSentinel,
        aSentinel->next);
}

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Dodaje @c aNewNode na kraj liste koju cuva @c aSentinel.
 *
 * @param       aSentinel               Pokazivac na cuvara liste,
 * @param       aNewNode                pokazivac na clan koji se dodaje.
 * @iclass
 */
/*-----------------------------------------------------------------------------------------------*/
C_INLINE void eS_dlsNodeAddTailI(
    pl_dlsList_T    * aSentinel,
    pl_dlsList_T    * aNewNode) {

    eS_dlsNodeAddI(
        aNewNode,
        aSentinel->prev,
        aSentinel);
}

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Dodaje @c aNewNode pre @c aCurrNode clana.
 *
 * @param       aCurrNode               Pokazivac na trenutno koriscenog clana,
 * @param       aNewNode                pokazivac na clan koji se dodaje.
 * @iclass
 */
/*-----------------------------------------------------------------------------------------------*/
C_INLINE void eS_dlsNodeAddBeforeI(
    pl_dlsList_T    * aCurrNode,
    pl_dlsList_T    * aNewNode) {

    eS_dlsNodeAddI(
        aNewNode,
        aCurrNode->prev,
        aCurrNode);
}

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Dodaje @c aNewNode nakon @c aCurrNode clana.
 *
 * @param       aCurrNode               Pokazivac na trenutno koriscenog clana,
 * @param       aNewNode                pokazivac na clan koji se dodaje.
 * @iclass
 */
/*-----------------------------------------------------------------------------------------------*/
C_INLINE void eS_dlsNodeAddAfterI(
    pl_dlsList_T    * aCurrNode,
    pl_dlsList_T    * aNewNode) {

    eS_dlsNodeAddI(
        aNewNode,
        aCurrNode,
        aCurrNode->next);
}

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Uklanja @c aOldNode iz povezane lise.
 *
 * @param       aOldNode                Pokazivac na clan koji se uklanja.
 * @iclass
 */
/*-----------------------------------------------------------------------------------------------*/
C_INLINE void eS_dlsNodeRemoveI(
    pl_dlsList_T    * aOldNode) {

    aOldNode->next->prev = aOldNode->prev;
    aOldNode->prev->next = aOldNode->next;
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
 * END of list.h
 *************************************************************************************************/
#endif /* LIST_H_ */
