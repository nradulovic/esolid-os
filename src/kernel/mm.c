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
 * ------------------------------------------------------------------------------------------------
 * @addtogroup  mm_impl
 ****************************************************************************************//** @{ */


/*************************************************************************************************
 * INCLUDE FILES
 *************************************************************************************************/

#define MM_PKG_H_VAR
#include "kernel_private.h"


/*************************************************************************************************
 * LOCAL DEFINES
 *************************************************************************************************/

/*-----------------------------------------------------------------------------------------------*
 * Local debug defines
 *-----------------------------------------------------------------------------------*//** @cond */

MM_DBG_DEFINE_MODULE(Memory Management);


/** @endcond *//*--------------------------------------------------------------------------------*
 * Local user defines
 *-----------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------*//**
 * @name        Konstante za status blokova
 * @{ *//*---------------------------------------------------------------------------------------*/

/**
 * @brief       Maska koja se koristi za definisanje statusa bloka
 * @details     U implementaciji se koristi MSB bit za oznacavanje da li je
 *              blok zauzet ili nije.
 *
 *              Ukoliko je bit setovan na MSB poziciji onda je blok zauzet, u
 *              suprotnom nije zauzet.
 */
#define BLOCK_STATUS_MASK               (hmemBlkSize_T)ES_MASK_MSB(hmemBlkSize_T)

/**
 * @brief       Marker koji pokazuje da je blok zauzet
 */
#define BLOCK_IS_BUSY                   (hmemBlkSize_T)ES_MASK_MSB(hmemBlkSize_T)

/**
 * @brief       Marker koji pokazuje da blok nije zauzet
 */
#define BLOCK_IS_FREE                   (hmemBlkSize_T)0U

/** @} *//*--------------------------------------------------------------------------------------*/


/*************************************************************************************************
 * LOCAL MACRO's
 *************************************************************************************************/

/*-------------------------------------------------------------------------------------------*//**
 * @name        Makroi za manipulaciju sa statusom blokova
 * @{ *//*---------------------------------------------------------------------------------------*/

/**
 * @brief       Izvlacenje statusa bloka iz @c size clana.
 * @return      status da li je blok slobodan.
 */
#define BLK_STAT_QUERY(currBlk)                                             \
    ((currBlk)->blk.size & BLOCK_STATUS_MASK)

/**
 * @brief       Vrsi postavljanje statusa bloka da je zauzet.
 */
#define BLK_STAT_BUSY(currBlk)                                              \
    (currBlk)->blk.size |= BLOCK_STATUS_MASK

/**
 * @brief       Vrsi postavljanje statusa bloka da je slobodan.
 */
#define BLK_STAT_FREE(currBlk)                                              \
    (currBlk)->blk.size &= ~BLOCK_STATUS_MASK

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Makroi za manipulaciju sa listama fizickih blokova
 * @brief       Koristi se posebna implementacija Single Linked list with
 *              Sentinel gde nije potrebno vrsiti skeniranje liste prilikom
 *              dodavanja ili oduzimanja elemenata
 * @{ *//*---------------------------------------------------------------------------------------*/

#define PHY_BLK_PREV(currBlk)                                               \
    ((hmemBlkHdr_T *)((uint8_t *)(currBlk) + currBlk->blk.size + sizeof(hmemBlk_T)))

/** @} *//*--------------------------------------------------------------------------------------*/


/*************************************************************************************************
 * LOCAL CONSTANTS
 *************************************************************************************************/


/*************************************************************************************************
 * LOCAL DATA TYPES
 *************************************************************************************************/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Struktura jednog bloka memorije.
 */
/*-----------------------------------------------------------------------------------------------*/
typedef struct hmemBlk {
/**
 * @brief       Velicina ovog bloka memorije u bajtovima.
 */
    hmemBlkSize_T  size;

/**
 * @brief       Lista fizickih blokova memorije.
 */
    esSlsList_T    phyList;
} hmemBlk_T;

/*-----------------------------------------------------------------------------------------------*/
/**
 * @extends     hmemBlk_T
 * @brief       Zaglavlje jedne slobodne oblasti memorije.
 *
 *              Ovo zaglavlje postoji samo u slobodnim oblastima memorije.
 */
/*-----------------------------------------------------------------------------------------------*/
typedef struct hmemBlkHdr {
/**
 * @brief       Lista fizickih blokova i zauzece blokova
 */
    hmemBlk_T       blk;
/**
 * @brief       Lista slobodnih blokova
 */
    esDlsList_T    freeList;
} hmemBlkHdr_T;


/*************************************************************************************************
 * LOCAL TABLES
 *************************************************************************************************/


/*************************************************************************************************
 * SHARED GLOBAL VARIABLES
 *************************************************************************************************/

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

#if (OPT_MM_MANAGED_SIZE != 0U)
/*
 * Provera da li je OPT_MM_MANAGED_SIZE veca od velicine dva blokovska zaglavlja
 * i da li je OPT_MM_MANAGED_SIZE manja od maksimalne velicine bloka koja se
 * moze predstaviti jednim blokom.
 */
# if (OPT_MM_MANAGED_SIZE > (sizeof(hmemBlkHdr_T) * 2))) && (OPT_MM_MANAGED_SIZE < ES_MASK_MSB(hmemBlkSize_T))
static C_ALIGNED(ES_CPU_ATTRIB_ALIGNMENT) uint8_t heap[ES_ALIGN(size, ES_CPU_ATTRIB_ALIGNMENT)]
# else
#  error "KERNEL=>MM: OPT_MM_MANAGED_SIZE has invalid value."
# endif
#else
extern uint8_t _sheap;
extern uint8_t _eheap;
#endif

/*************************************************************************************************
 * LOCAL GLOBAL VARIABLES
 *************************************************************************************************/

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


/*************************************************************************************************
 * LOCAL FUNCTION PROTOTYPES
 *************************************************************************************************/


/*************************************************************************************************
 ***                                I M P L E M E N T A T I O N                                ***
 *************************************************************************************************/



/*************************************************************************************************
 * GLOBAL FUNCTION DEFINITIONS
 *************************************************************************************************/

/*-------------------------------------------------------------------------------------------*//**
 * @ingroup         mem_intf
 * @{ *//*---------------------------------------------------------------------------------------*/

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
    heapSentinel->blk.size = (hmemBlkSize_T)0;
    esSlsSentinelInit(
        &(heapSentinel->blk.phyList));
    esSlsNodeAddHeadI(
        &(heapSentinel->blk.phyList),
        &(freeMemory->blk.phyList));
    esDlsSentinelInit(
        &(heapSentinel->freeList));
    esDlsNodeAddHeadI(
        &(heapSentinel->freeList),
        &(freeMemory->freeList));
    BLK_STAT_BUSY(heapSentinel);
    BLK_STAT_FREE(freeMemory);
    ES_TRACE(
        STP_FILT_MEM_0,
        txtMemHeapFree,
        esHmemFreeSpace());
#else
    ES_TRACE(
        STP_FILT_MEM_0,
        txtMemHeapInit,
        &_sheap,
        OPT_MM_MANAGED_SIZE);
    freeMemory = (hmemBlkHdr_T *)&_sheap;
    freeMemory->blk.size = (hmemBlkSize_T)((uint8_t *)&_eheap - (uint8_t *)&_sheap - sizeof(hmemBlk_T) - sizeof(hmemBlkHdr_T));
    heapSentinel = (hmemBlkHdr_T *)((uint8_t *)&_eheap - sizeof(hmemBlkHdr_T));
    heapSentinel->blk.size = (hmemBlkSize_T)0;
    esSlsSentinelInit(
        &(heapSentinel->blk.phyList));
    esSlsNodeAddHeadI(
        &(heapSentinel->blk.phyList),
        &(freeMemory->blk.phyList));
    esDlsSentinelInit(
        &(heapSentinel->freeList));
    esDlsNodeAddHeadI(
        &(heapSentinel->freeList),
        &(freeMemory->freeList));
    BLK_STAT_BUSY(heapSentinel);
    BLK_STAT_FREE(freeMemory);
    ES_TRACE(
        STP_FILT_MEM_0,
        txtMemHeapFree,
        esHmemFreeSpace());

# if defined(OPT_DBG_MM)
    dbgHeapBegin = (void *)&_sheap;
    dbgHeapEnd = (void *)heapSentinel;
# endif
#endif

}

/*-----------------------------------------------------------------------------------------------*/
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

    MM_DBG_CHECK(aSize < (size_t)BLOCK_STATUS_MASK);
    MM_ASSERT((hmemBlkHdr_T *)0 != heapSentinel);
    ES_TRACE(
        STP_FILT_MEM_0,
        txtMemHeapAlloc,
        aSize);
    ES_TRACE(
        STP_FILT_MEM_0,
        txtMemHeapFree,
        esHmemFreeSpace());

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
                esSlsNodeAddAfterI(
                    &(prevPhy->blk.phyList),
                    &(freeBlk->blk.phyList));
            } else {
                esDlsNodeRemoveI(
                    &(freeBlk->freeList));
            }
            BLK_STAT_BUSY(freeBlk);
            ES_TRACE(
                STP_FILT_MEM_0,
                txtMemHeapAllocated,
                &(freeBlk->freeList));
            ES_TRACE(
                STP_FILT_MEM_0,
                txtMemHeapFree,
                esHmemFreeSpace());

            return ((void *)&(freeBlk->freeList));
        }
    }
    MM_ASSERT_ALWAYS("No free space to allocate.");

    return((void *)0);
}

/*-----------------------------------------------------------------------------------------------*/
size_t esHmemBlockSize(
    void        * aMemory) {

    hmemBlkHdr_T * currBlk;

    MM_DBG_CHECK((dbgHeapBegin < aMemory) && (aMemory < dbgHeapEnd));
    /* currBlk = (hmemBlk_T *)aMemory; */
    /* --currBlk; */
    currBlk = C_CONTAINER_OF((esDlsList_T *)aMemory, hmemBlkHdr_T, freeList);
    MM_DBG_CHECK(BLOCK_IS_BUSY == BLK_STAT_QUERY(currBlk));

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

    hmemBlkSize_T blkStat;
    hmemBlkHdr_T * freeBlk;
    hmemBlkHdr_T * currPhy;
    hmemBlkHdr_T * tmpPhy;

    MM_ASSERT((hmemBlkHdr_T *)0 != heapSentinel);
    MM_DBG_CHECK((dbgHeapBegin < aMemory) && (aMemory < dbgHeapEnd));
    /* freeBlk = (hmemBlkHdr_T *)((hmemBlk_T *)aMemory - 1U); */
    freeBlk = C_CONTAINER_OF((esDlsList_T *)aMemory, hmemBlkHdr_T, freeList);
    MM_DBG_CHECK(BLOCK_IS_BUSY == BLK_STAT_QUERY(freeBlk));
    ES_TRACE(
        STP_FILT_MEM_0,
        txtMemHeapDeAlloc,
        aMemory);
    currPhy = esSlsNodeEntry(
        hmemBlkHdr_T,
        blk.phyList,
        freeBlk->blk.phyList.next);
    blkStat = BLK_STAT_QUERY(currPhy);
    BLK_STAT_FREE(freeBlk);

    if (BLOCK_IS_FREE == blkStat) {
        esDlsNodeRemoveI(
            &(currPhy->freeList));
        tmpPhy = PHY_BLK_PREV(freeBlk);
        esSlsNodeRemoveAfterI(
            &(tmpPhy->blk.phyList));
        currPhy->blk.size += freeBlk->blk.size + sizeof(hmemBlk_T);
        freeBlk = currPhy;
    }
    currPhy = PHY_BLK_PREV(freeBlk);
    blkStat = BLK_STAT_QUERY(currPhy);

    if (BLOCK_IS_FREE == blkStat) {
        esDlsNodeRemoveI(
            &(currPhy->freeList));
        tmpPhy = PHY_BLK_PREV(currPhy);
        esSlsNodeRemoveAfterI(
            &(tmpPhy->blk.phyList));
        freeBlk->blk.size += currPhy->blk.size + sizeof(hmemBlk_T);
    }
    esDlsNodeAddHeadI(
        &(heapSentinel->freeList),
        &(freeBlk->freeList));
    ES_TRACE(
        STP_FILT_MEM_0,
        txtMemHeapFree,
        esHmemFreeSpace());
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

    hmemBlkSize_T  free;
    hmemBlkHdr_T   * currBlk;

    MM_ASSERT((hmemBlkHdr_T *)0 != heapSentinel);
    free = (hmemBlkSize_T)0;

    DLS_FOR_EACH_ENTRY(
        hmemBlkHdr_T,
        freeList,
        &(heapSentinel->freeList),
        currBlk) {
        free += currBlk->blk.size;
    }

    return ((size_t)(free & ~BLOCK_STATUS_MASK));                               /* U slucaju da je samo sentinel prividno dostupan.         */
}

/*-----------------------------------------------------------------------------------------------*/
void * esMemCopy(
    void        * C_RESTRICT aDst,
    const void  * C_RESTRICT aSrc,
    size_t      aLength) {

#if !defined(ES_OPTIMIZE_SPEED)
    uint8_t * dst;
    uint8_t * src;

    MM_DBG_CHECK((void *)0 != aDst);
    MM_DBG_CHECK((void *)0 != aSrc);
    MM_DBG_CHECK(aDst != aSrc);
    dst = (uint8_t *)aDst;
    src = (uint8_t *)aSrc;

    while ((size_t)0 != aLength--) {
        *dst++ = *src++;
    }

    return (aDst);
#else

    /*
     * todo Napisati funkciju optimizovanu za brzinu (kopiranje 4-32 bajta u jednom ciklusu)
     */
#endif
}

/*-----------------------------------------------------------------------------------------------*/
void * esMemMove(
    void        * aDst,
    const void  * aSrc,
    size_t      aLength) {

#if !defined(ES_OPTIMIZE_SPEED)
    uint8_t * dst;
    uint8_t * src;

    MM_DBG_CHECK((void *)0 != aDst);
    MM_DBG_CHECK((void *)0 != aSrc);
    MM_DBG_CHECK(aDst != aSrc);
    dst = (uint8_t *)aDst;
    src = (uint8_t *)aSrc;

    while ((size_t)0 != aLength--) {
        *dst++ = *src++;
    }

    return (aDst);
#else

    /*
     * todo Napisati funkciju optimizovanu za brzinu (kopiranje 4-32 bajta u jednom ciklusu)
     */
#endif
}

/*-----------------------------------------------------------------------------------------------*/

/** @} *//*--------------------------------------------------------------------------------------*/


/*************************************************************************************************
 * LOCAL FUNCTION DEFINITIONS
 *************************************************************************************************/


/*************************************************************************************************
 * CONFIGURATION ERRORS
 *************************************************************************************//** @cond */


/** @endcond *//** @} *//*************************************************************************
 * END of mem.c
 *************************************************************************************************/
