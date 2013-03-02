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
 * @brief       Implementacija Memory Management modula.
 * @addtogroup  mm_impl
 ****************************************************************************************//** @{ */


/*============================================================================  INCLUDE FILES  ==*/

#define MM_PKG_H_VAR
#include "kernel_private.h"
#include "primitive/list.h"

/*============================================================================  LOCAL DEFINES  ==*/

/*-------------------------------------------------------------------------------------------*//**
 * @name        Konstante za status blokova
 * @{ *//*---------------------------------------------------------------------------------------*/

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

/** @} *//*--------------------------------------------------------------------------------------*/

/*============================================================================  LOCAL MACRO's  ==*/

/*-------------------------------------------------------------------------------------------*//**
 * @name        Makroi za manipulaciju sa statusom blokova
 * @{ *//*---------------------------------------------------------------------------------------*/

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

/** @} *//*--------------------------------------------------------------------------------------*/

/**
 * @brief       Makroi za manipulaciju sa listama fizickih blokova
 * @details     Koristi se posebna implementacija Single Linked list with
 *              Sentinel gde nije potrebno vrsiti skeniranje liste prilikom
 *              dodavanja ili oduzimanja elemenata
 */
#define PHY_BLK_PREV(currBlk)                                                   \
    ((hmemBlkHdr_T *)((uint8_t *)(currBlk) + currBlk->blk.size + sizeof(hmemBlk_T)))


/*=========================================================================  LOCAL DATA TYPES  ==*/

/**
 * @brief       Struktura jednog bloka memorije.
 */
typedef struct hmemBlk {
/**
 * @brief       Velicina ovog bloka memorije u bajtovima.
 */
    size_t          size;

/**
 * @brief       Lista fizickih blokova memorije.
 */
    esSlsList_T     phyList;
} hmemBlk_T;

/**
 * @extends     hmemBlk_T
 * @brief       Zaglavlje jedne slobodne oblasti memorije.
 * @details     Ovo zaglavlje postoji samo u slobodnim oblastima memorije.
 */
typedef struct hmemBlkHdr {
/**
 * @brief       Lista fizickih blokova i zauzece blokova
 */
    hmemBlk_T       blk;
/**
 * @brief       Lista slobodnih blokova
 */
    esDlsList_T     freeList;
} hmemBlkHdr_T;

/*================================================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*==========================================================================  LOCAL VARIABLES  ==*/

/**
 * @brief       Cuvar liste slobodnih blokova
 */
static hmemBlkHdr_T * heapSentinel;

/*-------------------------------------------------------------------------------------------*//**
 * @name        Promenljive koje se koriste u slucaju da je omoguceno debagovanje MM modula
 * @{ *//*---------------------------------------------------------------------------------------*/

#if defined(OPT_DBG_MM) || defined(__DOXYGEN__)
static void * dbgHeapBegin;
static void * dbgHeapEnd;
#endif
/** @} *//*--------------------------------------------------------------------------------------*/

#if (OPT_MM_MANAGED_SIZE != 0U)
/*
 * Provera da li je OPT_MM_MANAGED_SIZE veca od velicine dva blokovska zaglavlja
 * i da li je OPT_MM_MANAGED_SIZE manja od maksimalne velicine bloka koja se
 * moze predstaviti jednim blokom.
 */
# if (OPT_MM_MANAGED_SIZE > (sizeof(hmemBlkHdr_T) * 2))) && (OPT_MM_MANAGED_SIZE < ES_MASK_MSB(size_t))
static C_ALIGNED(ES_CPU_ATTRIB_ALIGNMENT) uint8_t heap[ES_ALIGN(size, ES_CPU_ATTRIB_ALIGNMENT)]
# else
#  error "KERNEL=>MM: OPT_MM_MANAGED_SIZE has invalid value."
# endif
#endif

/*=========================================================================  GLOBAL VARIABLES  ==*/

/*-------------------------------------------------------------------------------------------*//**
* @name        Klase memorijskog alokatora
* @{ *//*---------------------------------------------------------------------------------------*/

/**
 * @brief       Dinamicki memorijski alokator (heap memory)
 */
const C_ROM esMemClass_T esMemHeapClass = {
   &esHmemAlloc,
   &esHmemDeAlloc,
   &esHmemBlockSize,
};

/**
 * @brief       Staticki memorijski alokator (static memory)
 * @todo        Napisati funkcije za staticki alokator, sada samo koristiom heap.
 */
const C_ROM esMemClass_T esMemStaticClass = {
   &esHmemAlloc,
   &esHmemDeAlloc,
   &esHmemBlockSize,
};

/** @} *//*--------------------------------------------------------------------------------------*/

#if (OPT_MM_MANAGED_SIZE == 0U)
/**
 * @brief       Pocetak heap memorije, definicija je u linker skripti.
 */
extern uint8_t _sheap;

/**
 * @brief       Kraj heap memorije, definicija je u linker skripti.
 */
extern uint8_t _eheap;
#endif

/*===============================================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*======================================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/

void hmemInit(
    void) {

    hmemBlkHdr_T    * freeMemory;

#if (OPT_MM_MANAGED_SIZE != 0U)
    MM_DBG_CHECK((void *)0 != aHeap);
    MM_DBG_CHECK(aSize > (sizeof(hmemBlkHdr_T) * 2));
    MM_DBG_CHECK(aSize < (size_t)BLOCK_STATUS_MASK);
    MM_DBG_CHECK((size_t)0U == (aSize & (C_DATA_ALIGNMENT - 1U)));
    MM_DBG_CHECK((size_t)0U == ((size_t)aHeap & (C_DATA_ALIGNMENT - 1U)));
    ES_TRACE(
        STP_FILT_MEM_0,
        txtMemHeapInit,
        heap,
        OPT_MM_MANAGED_SIZE);
    hmemBlkHdr_T    * freeMemory;

    freeMemory = (hmemBlkHdr_T *)aHeap;
    freeMemory->blk.size = aSize - sizeof(hmemBlk_T) - sizeof(hmemBlkHdr_T);
    heapSentinel = (hmemBlkHdr_T *)((uint8_t *)freeMemory + freeMemory->blk.size + sizeof(hmemBlk_T));
    heapSentinel->blk.size = (size_t)0;
    esSlsSentinelInit_(
        &(heapSentinel->blk.phyList));
    esSlsNodeAddHead_(
        &(heapSentinel->blk.phyList),
        &(freeMemory->blk.phyList));
    esDlsSentinelInit_(
        &(heapSentinel->freeList));
    esDlsNodeAddHead_(
        &(heapSentinel->freeList),
        &(freeMemory->freeList));
    BLK_STAT_BUSY(heapSentinel);
    BLK_STAT_FREE(freeMemory);
    ES_TRACE(
        STP_FILT_MEM_0,
        txtMemHeapFree,
        esHmemFreeSpace());
#else
    freeMemory = (hmemBlkHdr_T *)&_sheap;
    freeMemory->blk.size = (size_t)((uint8_t *)&_eheap - (uint8_t *)&_sheap - sizeof(hmemBlk_T) - sizeof(hmemBlkHdr_T));
    heapSentinel = (hmemBlkHdr_T *)((uint8_t *)&_eheap - sizeof(hmemBlkHdr_T));
    heapSentinel->blk.size = (size_t)0;
    esSlsSentinelInit_(
        &(heapSentinel->blk.phyList));
    esSlsNodeAddHead_(
        &(heapSentinel->blk.phyList),
        &(freeMemory->blk.phyList));
    esDlsSentinelInit_(
        &(heapSentinel->freeList));
    esDlsNodeAddHead_(
        &(heapSentinel->freeList),
        &(freeMemory->freeList));
    BLK_STAT_BUSY(heapSentinel);
    BLK_STAT_FREE(freeMemory);

# if defined(OPT_DBG_MM)
    dbgHeapBegin = (void *)&_sheap;
    dbgHeapEnd = (void *)heapSentinel;
# endif
#endif

}

/*=======================================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

void * esHmemAlloc(
    size_t  aSize) {

    void * tmpMem;
    ES_CRITICAL_DECL();

    ES_CRITICAL_ENTER(OPT_KERNEL_INTERRUPT_PRIO_MAX);
    tmpMem = esHmemAllocI(
        aSize);
    ES_CRITICAL_EXIT();

    return (tmpMem);
}

/*-----------------------------------------------------------------------------------------------*/
void * esHmemAllocI(
    size_t  aSize) {

    hmemBlkHdr_T * prevPhy;
    hmemBlkHdr_T * freeBlk;

    if (aSize < (sizeof(hmemBlkHdr_T) - sizeof(hmemBlk_T))) {
        aSize = sizeof(hmemBlkHdr_T) - sizeof(hmemBlk_T);
    }

#if (PORT_SUPP_UNALIGNED_ACCESS != PORT_TRUE) || defined(OPT_OPTIMIZE_SPEED)
    aSize = ES_ALIGN(aSize, C_DATA_ALIGNMENT);
#endif
    DLS_FOR_EACH_ENTRY(
        hmemBlkHdr_T,
        freeList,
        &(heapSentinel->freeList),
        freeBlk) {

        if (freeBlk->blk.size >= aSize) {

            if (freeBlk->blk.size >= (aSize + sizeof(hmemBlkHdr_T))) {
                freeBlk->blk.size -= (aSize + sizeof(hmemBlk_T));
                freeBlk = PHY_BLK_PREV(freeBlk);
                freeBlk->blk.size = aSize;
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

    return((void *)0);
}

/*-----------------------------------------------------------------------------------------------*/
size_t esHmemBlockSize(
    void        * aMemory) {

    hmemBlkHdr_T * currBlk;

    currBlk = C_CONTAINER_OF((esDlsList_T *)aMemory, hmemBlkHdr_T, freeList);

    return ((size_t)(currBlk->blk.size & ~BLOCK_STATUS_MASK));
}

/*-----------------------------------------------------------------------------------------------*/
void esHmemDeAlloc(
    void        * aMemory) {

    ES_CRITICAL_DECL();

    ES_CRITICAL_ENTER(OPT_KERNEL_INTERRUPT_PRIO_MAX);
    esHmemDeAllocI(
        aMemory);
    ES_CRITICAL_EXIT();
}

/*-----------------------------------------------------------------------------------------------*/
void esHmemDeAllocI(
    void        * aMemory) {

    size_t blkStat;
    hmemBlkHdr_T * freeBlk;
    hmemBlkHdr_T * currPhy;
    hmemBlkHdr_T * tmpPhy;

    freeBlk = C_CONTAINER_OF((esDlsList_T *)aMemory, hmemBlkHdr_T, freeList);
    currPhy = esSlsNodeEntry(
        hmemBlkHdr_T,
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
        currPhy->blk.size += freeBlk->blk.size + sizeof(hmemBlk_T);
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
        freeBlk->blk.size += currPhy->blk.size + sizeof(hmemBlk_T);
    }
    esDlsNodeAddHead_(
        &(heapSentinel->freeList),
        &(freeBlk->freeList));
}

/*-----------------------------------------------------------------------------------------------*/
size_t esHmemFreeSpace(
    void) {

    size_t tmpSize;
    ES_CRITICAL_DECL();

    ES_CRITICAL_ENTER(OPT_KERNEL_INTERRUPT_PRIO_MAX);
    tmpSize = esHmemFreeSpaceI();
    ES_CRITICAL_EXIT();

    return (tmpSize);
}

/*-----------------------------------------------------------------------------------------------*/
size_t esHmemFreeSpaceI(
    void) {

    size_t  free;
    hmemBlkHdr_T   * currBlk;

    free = (size_t)0;

    DLS_FOR_EACH_ENTRY(
        hmemBlkHdr_T,
        freeList,
        &(heapSentinel->freeList),
        currBlk) {
        free += currBlk->blk.size;
    }

    return ((size_t)(free & ~BLOCK_STATUS_MASK));                               /* U slucaju da je samo sentinel prividno dostupan.         */
}

/*===================================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//*************************************************************************
 * END of mem.c
 *************************************************************************************************/
