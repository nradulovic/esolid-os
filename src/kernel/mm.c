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
 * @brief       Implementacija Memory Management modula.
 * @addtogroup  mm_impl
 *********************************************************************//** @{ */


/*=========================================================  INCLUDE FILES  ==*/

#define MM_PKG_H_VAR
#include "kernel_private.h"
#include "primitive/list.h"

/*=========================================================  LOCAL DEFINES  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Konstante za status blokova
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Maska koja se koristi za definisanje statusa bloka
 * @details     U implementaciji se koristi MSB bit za oznacavanje da li je
 *              blok zauzet ili nije. Ukoliko je bit setovan na MSB poziciji
 *              onda je blok zauzet, u suprotnom nije zauzet.
 */
#define BLOCK_STATUS_MASK               (size_t)ES_MASK_MSB(size_t)

/**
 * @brief       Marker koji pokazuje da je blok zauzet
 */
#define BLOCK_IS_BUSY                   (size_t)ES_MASK_MSB(size_t)

/**
 * @brief       Marker koji pokazuje da blok nije zauzet
 */
#define BLOCK_IS_FREE                   (size_t)0U

/** @} *//*-------------------------------------------------------------------*/

#if (OPT_MM_MANAGED_SIZE == 0U) || defined(__DOXYGEN__)                         /* Koristi se linker skripta.                               */
/**
 * @brief       Pocetak heap memorije
 */
# define HEAP_BEGIN                     &_sheap

/**
 * @brief       Kraj heap memorije
 */
# define HEAP_END                       &_eheap

/**
 * @brief       Velicina heap memorije
 */
# define HEAP_SIZE                      (size_t)(&_eheap - &_sheap)

#else
# define HEAP_BEGIN                     (uint8_t *)&heap
# define HEAP_END                       (uint8_t *)(HEAP_BEGIN + sizeof(heap))
# define HEAP_SIZE                      sizeof(heap)
#endif

/*=========================================================  LOCAL MACRO's  ==*/

/**
 * @brief       Izvlacenje statusa bloka iz @c size clana.
 * @return      status da li je blok slobodan.
 */
#define BLK_STAT_QUERY(currBlk)                                                 \
    ((currBlk)->blk.size & BLOCK_STATUS_MASK)

/**
 * @brief       Vrsi postavljanje statusa bloka da je zauzet.
 */
#define BLK_STAT_BUSY(currBlk)                                                  \
    (currBlk)->blk.size |= BLOCK_STATUS_MASK

/**
 * @brief       Vrsi postavljanje statusa bloka da je slobodan.
 */
#define BLK_STAT_FREE(currBlk)                                                  \
    (currBlk)->blk.size &= ~BLOCK_STATUS_MASK

/**
 * @brief       Makroi za manipulaciju sa listama fizickih blokova
 * @details     Koristi se posebna implementacija Single Linked list with
 *              Sentinel gde nije potrebno vrsiti skeniranje liste prilikom
 *              dodavanja ili oduzimanja elemenata
 */
#define PHY_BLK_PREV(currBlk)                                                   \
    ((dmemBlkHdr_T *)((uint8_t *)(currBlk) + currBlk->blk.size + sizeof(dmemBlk_T)))

/*======================================================  LOCAL DATA TYPES  ==*/

/**
 * @brief       Struktura jednog bloka memorije.
 */
typedef struct dmemBlk {
/**
 * @brief       Velicina ovog bloka memorije u bajtovima.
 */
    size_t          size;

/**
 * @brief       Lista fizickih blokova memorije.
 */
    esSlsList_T     phyList;
} dmemBlk_T;

/**
 * @extends     dmemBlk_T
 * @brief       Zaglavlje jedne slobodne oblasti memorije.
 * @details     Ovo zaglavlje postoji samo u slobodnim oblastima memorije.
 */
typedef struct dmemBlkHdr {
/**
 * @brief       Lista fizickih blokova i zauzece blokova
 */
    dmemBlk_T       blk;

/**
 * @brief       Lista slobodnih blokova
 */
    esDlsList_T     freeList;
} dmemBlkHdr_T;

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

C_UNUSED_FUNC static void smemInit(
    void);

C_UNUSED_FUNC static void dmemInit(
    uint8_t     * dbegin,
    uint8_t     * dend);

C_UNUSED_FUNC static void dummyDeAlloc(
    void            * aMemory);

/*=======================================================  LOCAL VARIABLES  ==*/

/**
 * @brief       Cuvar liste slobodnih blokova
 */
static dmemBlkHdr_T * gDmemSentinel;

/**
 * @brief       Cuvar vrednosti slobodne memorije staticnog alokatora
 */
static uint8_t * gSmemSentinel;

#if (OPT_MM_MANAGED_SIZE != 0U)
/*
 * Provera da li je OPT_MM_MANAGED_SIZE veca od velicine dva blokovska zaglavlja
 * i da li je OPT_MM_MANAGED_SIZE manja od maksimalne velicine bloka koja se
 * moze predstaviti jednim blokom.
 */
static C_ALIGNED(ES_CPU_ATTRIB_ALIGNMENT) uint8_t heap[ES_ALIGN(size, ES_CPU_ATTRIB_ALIGNMENT)]
#endif

/*======================================================  GLOBAL VARIABLES  ==*/

/*------------------------------------------------------------------------*//**
* @name        Klase memorijskog alokatora
* @{ *//*---------------------------------------------------------------------*/

/**
 * @brief       Dinamicki memorijski alokator
 */
const C_ROM esMemClass_T esMemDynClass = {
#if (OPT_MM_DISTRIBUTION == ES_MM_STATIC_ONLY)
    &esSmemAlloc,
    &dummyDeAlloc,
#else
    &esDmemAlloc,
    &esDmemDeAlloc,
#endif
};

/**
 * @brief       Staticki memorijski alokator
 */
const C_ROM esMemClass_T esMemStaticClass = {
#if (OPT_MM_DISTRIBUTION == ES_MM_DYNAMIC_ONLY)
    &esDmemAlloc,
    &esDmemDeAlloc,
#else
    &esSmemAlloc,
    &dummyDeAlloc,
#endif
};

/** @} *//*-------------------------------------------------------------------*/

/**
 * @brief       Pocetak heap memorije, definicija je u linker skripti.
 */
extern uint8_t _sheap;

/**
 * @brief       Kraj heap memorije, definicija je u linker skripti.
 */
extern uint8_t _eheap;

/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

/**
 * @brief       Inicijalizacija statičnog memorijskog menadzera
 */
static void smemInit(
    void) {

    gSmemSentinel = HEAP_BEGIN;
}

/**
 * @brief       Inicijalizacija dinamičkog memorijskog menadzera
 * @param       dbegin                  Početak memorijske oblasti dodeljene za
 *                                      koriscenje
 * @param       dend                    kraj memorijske oblasti
 */
static void dmemInit(
    uint8_t *       dbegin,
    uint8_t *       dend) {

    ((dmemBlkHdr_T *)dbegin)->blk.size =
        (size_t)(dend - dbegin - sizeof(dmemBlk_T) - sizeof(dmemBlkHdr_T));
    gDmemSentinel = ((dmemBlkHdr_T *)dend - 1U);
    gDmemSentinel->blk.size = (size_t)0;
    esSlsSentinelInit_(
        &(gDmemSentinel->blk.phyList));
    esSlsNodeAddHead_(
        &(gDmemSentinel->blk.phyList),
        &(((dmemBlkHdr_T *)dbegin)->blk.phyList));
    esDlsSentinelInit_(
        &(gDmemSentinel->freeList));
    esDlsNodeAddHead_(
        &(gDmemSentinel->freeList),
        &(((dmemBlkHdr_T *)dbegin)->freeList));
    BLK_STAT_BUSY(gDmemSentinel);
    BLK_STAT_FREE((dmemBlkHdr_T *)dbegin);
}

/**
 * @brief       Prazna funkcija
 * @param       aMemory                 Ignorisan parametar
 */
static void dummyDeAlloc(
    void            * aMemory) {

    (void)aMemory;
}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

/*----------------------------------------------------------------------------*/
void esMemInit(
    void) {

#if (OPT_MM_DISTRIBUTION == ES_MM_DYNAMIC_ONLY)
    dmemInit(
        HEAP_BEGIN,
        HEAP_END);
#elif (OPT_MM_DISTRIBUTION == ES_MM_STATIC_ONLY)
    smemInit();
#else
    void * tmp;

    smemInit();
    tmp = esSmemAlloc(
        OPT_MM_DISTRIBUTION);
    dmemInit(
        tmp, (tmp + (size_t)OPT_MM_DISTRIBUTION));
#endif
}

/*----------------------------------------------------------------------------*/
size_t esSmemFreeSpace(
    void) {

#if (OPT_MM_DISTRIBUTION != ES_MM_DYNAMIC_ONLY)
    size_t freeSpace;

    freeSpace = HEAP_END - gSmemSentinel;

    return (freeSpace);
#else

    return (0U);
#endif
}

/*----------------------------------------------------------------------------*/
void * esSmemAllocI(
    size_t          size) {

#if (OPT_MM_DISTRIBUTION != ES_MM_DYNAMIC_ONLY)
    void * tmp;

    if (size <= (HEAP_END - gSmemSentinel)) {
        tmp = gSmemSentinel;
        gSmemSentinel += size;
    } else {
        tmp = (void *)0;
    }

    return (tmp);
#else
    (void)size;

    return ((void *)0);
#endif
}

/*----------------------------------------------------------------------------*/
void * esSmemAlloc(
    size_t          size) {

#if (OPT_MM_DISTRIBUTION != ES_MM_DYNAMIC_ONLY)
    ES_CRITICAL_DECL();
    void * tmp;

    ES_CRITICAL_ENTER(
        OPT_KERNEL_INTERRUPT_PRIO_MAX);
    tmp = esSmemAllocI(size);
    ES_CRITICAL_EXIT();

    return (tmp);
#else
    (void)size;

    return ((void *)0);
#endif
}

/*----------------------------------------------------------------------------*/
void * esDmemAlloc(
    size_t          size) {

#if (OPT_MM_DISTRIBUTION != ES_MM_STATIC_ONLY)
    ES_CRITICAL_DECL();
    void * tmpMem;

    ES_CRITICAL_ENTER(OPT_KERNEL_INTERRUPT_PRIO_MAX);
    tmpMem = esDmemAllocI(
        size);
    ES_CRITICAL_EXIT();

    return (tmpMem);
#else
    (void)size;

    return ((void *)0);
#endif
}

/*----------------------------------------------------------------------------*/
void * esDmemAllocI(
    size_t          size) {

#if (OPT_MM_DISTRIBUTION != ES_MM_STATIC_ONLY)
    dmemBlkHdr_T * prevPhy;
    dmemBlkHdr_T * freeBlk;

    if (size < (sizeof(dmemBlkHdr_T) - sizeof(dmemBlk_T))) {
        size = sizeof(dmemBlkHdr_T) - sizeof(dmemBlk_T);
    }

#if !defined(PORT_SUPP_UNALIGNED_ACCESS) || defined(OPT_OPTIMIZE_SPEED)
    size = ES_ALIGN(size, ES_CPU_ATTRIB_ALIGNMENT);
#endif
    DLS_FOR_EACH_ENTRY(
        dmemBlkHdr_T,
        freeList,
        &(gDmemSentinel->freeList),
        freeBlk) {

        if (freeBlk->blk.size >= size) {

            if (freeBlk->blk.size >= (size + sizeof(dmemBlkHdr_T))) {
                freeBlk->blk.size -= (size + sizeof(dmemBlk_T));
                freeBlk = PHY_BLK_PREV(freeBlk);
                freeBlk->blk.size = size;
                prevPhy = PHY_BLK_PREV(freeBlk);
                esSlsNodeAddAfter_(
                    &(prevPhy->blk.phyList),
                    &(freeBlk->blk.phyList));
            } else {
                esDlsNodeRemove_(
                    &(freeBlk->freeList));
            }
            BLK_STAT_BUSY(freeBlk);

            return ((void *)&(freeBlk->freeList));
        }
    }

    return ((void *)0);
#else
    (void)size;

    return ((void *)0);
#endif
}

/*----------------------------------------------------------------------------*/
size_t esDmemBlockSize(
    void *          mem) {

#if (OPT_MM_DISTRIBUTION != ES_MM_STATIC_ONLY)
    dmemBlkHdr_T * currBlk;

    currBlk = C_CONTAINER_OF((esDlsList_T *)mem, dmemBlkHdr_T, freeList);

    return ((size_t)(currBlk->blk.size & ~BLOCK_STATUS_MASK));
#else
    (void)mem;

    return (0);
#endif
}

/*----------------------------------------------------------------------------*/
void esDmemDeAlloc(
    void *          mem) {

#if (OPT_MM_DISTRIBUTION != ES_MM_STATIC_ONLY)
    ES_CRITICAL_DECL();

    ES_CRITICAL_ENTER(OPT_KERNEL_INTERRUPT_PRIO_MAX);
    esDmemDeAllocI(
        mem);
    ES_CRITICAL_EXIT();
#else
    (void)mem;
#endif
}

/*----------------------------------------------------------------------------*/
void esDmemDeAllocI(
    void *          mem) {

#if (OPT_MM_DISTRIBUTION != ES_MM_STATIC_ONLY)
    size_t blkStat;
    dmemBlkHdr_T * freeBlk;
    dmemBlkHdr_T * currPhy;
    dmemBlkHdr_T * tmpPhy;

    freeBlk = C_CONTAINER_OF((esDlsList_T *)mem, dmemBlkHdr_T, freeList);
    currPhy = esSlsNodeEntry(
        dmemBlkHdr_T,
        blk.phyList,
        freeBlk->blk.phyList.next);
    blkStat = BLK_STAT_QUERY(currPhy);
    BLK_STAT_FREE(freeBlk);

    if (BLOCK_IS_FREE == blkStat) {
        esDlsNodeRemove_(
            &(currPhy->freeList));
        tmpPhy = PHY_BLK_PREV(freeBlk);
        esSlsNodeRemoveAfter_(
            &(tmpPhy->blk.phyList));
        currPhy->blk.size += freeBlk->blk.size + sizeof(dmemBlk_T);
        freeBlk = currPhy;
    }
    currPhy = PHY_BLK_PREV(freeBlk);
    blkStat = BLK_STAT_QUERY(currPhy);

    if (BLOCK_IS_FREE == blkStat) {
        esDlsNodeRemove_(
            &(currPhy->freeList));
        tmpPhy = PHY_BLK_PREV(currPhy);
        esSlsNodeRemoveAfter_(
            &(tmpPhy->blk.phyList));
        freeBlk->blk.size += currPhy->blk.size + sizeof(dmemBlk_T);
    }
    esDlsNodeAddHead_(
        &(gDmemSentinel->freeList),
        &(freeBlk->freeList));
#else
    (void)mem;
#endif
}

/*----------------------------------------------------------------------------*/
size_t esDmemFreeSpace(
    void) {

#if (OPT_MM_DISTRIBUTION != ES_MM_STATIC_ONLY)
    ES_CRITICAL_DECL();
    size_t tmpSize;

    ES_CRITICAL_ENTER(OPT_KERNEL_INTERRUPT_PRIO_MAX);
    tmpSize = esDmemFreeSpaceI();
    ES_CRITICAL_EXIT();

    return (tmpSize);
#else
    return (0);
#endif
}

/*----------------------------------------------------------------------------*/
size_t esDmemFreeSpaceI(
    void) {

#if (OPT_MM_DISTRIBUTION != ES_MM_STATIC_ONLY)
    size_t free;
    dmemBlkHdr_T * currBlk;

    free = (size_t)0;

    DLS_FOR_EACH_ENTRY(
        dmemBlkHdr_T,
        freeList,
        &(gDmemSentinel->freeList),
        currBlk) {
        free += currBlk->blk.size;
    }

    return ((size_t)(free & ~BLOCK_STATUS_MASK));                               /* U slucaju da je samo sentinel prividno dostupan.         */
#else
    return (0);
#endif
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of mem.c
 ******************************************************************************/
