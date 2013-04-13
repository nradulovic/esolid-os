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

#define MEM_PKG_H_VAR
#include "mem_pkg.h"
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

#if (OPT_MEM_CORE_SIZE == 0U) || defined(__DOXYGEN__)                         /* Koristi se linker skripta.                               */
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

/**
 * @brief       Cuvar liste slobodnih blokova
 */
#define DMEM_SENTINEL                   ((dmemBlkHdr_T *)HEAP_END - 1U)

/*=========================================================  LOCAL MACRO's  ==*/

/**
 * @brief       Izvlacenje statusa bloka iz @c size clana.
 * @return      stanje da li je blok slobodan.
 */
#define BLOCK_GET_STATUS(currBlk)                                                 \
    ((currBlk)->blk.size & BLOCK_STATUS_MASK)

/**
 * @brief       Vrsi postavljanje statusa bloka da je zauzet.
 */
#define BLOCK_SET_BUSY(currBlk)                                                  \
    (currBlk)->blk.size |= BLOCK_STATUS_MASK

/**
 * @brief       Vrsi postavljanje statusa bloka da je slobodan.
 */
#define BLOCK_SET_FREE(currBlk)                                                  \
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
typedef struct C_ALIGNED(ES_CPU_ATTRIB_ALIGNMENT) dmemBlk {
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
 * @brief       Zaglavlje jedne slobodne oblasti memorije.
 * @details     Ovo zaglavlje postoji samo u slobodnim oblastima memorije.
 */
typedef struct C_ALIGNED(ES_CPU_ATTRIB_ALIGNMENT) dmemBlkHdr {
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
/*=======================================================  LOCAL VARIABLES  ==*/

/**
 * @brief       Cuvar vrednosti slobodne memorije staticnog alokatora
 */
static struct sMemSentinel {
    unative_T *     begin;
    unative_T       current;
} gSMemSentinel;

/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

/**
 * @brief       Inicijalizacija dinamičkog memorijskog menadzera
 * @param       boundary                Početak/granica memorijske oblasti
 *                                      dodeljene za koriscenje dinamickom MM
 */
static void dmemInit(
    uint8_t *       boundary) {

    ((dmemBlkHdr_T *)boundary)->blk.size =
        (size_t)(HEAP_END - boundary - sizeof(dmemBlk_T) - sizeof(dmemBlkHdr_T));
    DMEM_SENTINEL->blk.size = (size_t)0;
    esSlsSentinelInit_(
        &(DMEM_SENTINEL->blk.phyList));
    esSlsNodeAddHead_(
        &(DMEM_SENTINEL->blk.phyList),
        &(((dmemBlkHdr_T *)boundary)->blk.phyList));
    esDlsSentinelInit_(
        &(DMEM_SENTINEL->freeList));
    esDlsNodeAddHead_(
        &(DMEM_SENTINEL->freeList),
        &(((dmemBlkHdr_T *)boundary)->freeList));
    BLOCK_SET_BUSY(DMEM_SENTINEL);
    BLOCK_SET_FREE((dmemBlkHdr_T *)boundary);
}

/**
 * @brief       Prazna funkcija
 * @param       aMemory                 Ignorisan parametar
 * @notapi
 */
static void smemDeAlloc(
    void            * aMemory) {

    (void)aMemory;
    ES_LOG_IF_ERR(&gKernelLog, LOG_FILT_MM, LOG_MM_SDALLOC, ES_USAGE_FAILURE);
}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

/*----------------------------------------------------------------------------*/
void esSmemInit(
    void) {

#if (OPT_MEM_CORE_SIZE != 0U)
    static unative_T sMemBuffer[ES_DIV_ROUNDUP(OPT_MEM_CORE_SIZE, sizeof(unative_T))];

    gSMemSentinel.begin = &sMemBuffer;
    gSMemSentinel.current = ES_DIMENSION(sMemBuffer);
#else

    extern uint8_t _sheap;
    extern uint8_t _eheap;

    gSMemSentinel.begin = (unative_T *)ES_ALIGN_UP(
        &_sheap,
        sizeof(unative_T));
    gSMemSentinel.current = (&_eheap - gSMemSentinel.begin) / sizeof(unative_T);
#endif
}

/*----------------------------------------------------------------------------*/
size_t esSmemFreeSpace(
    void) {

    return (gSMemSentinel.current);
}

/*----------------------------------------------------------------------------*/
void * esSmemAllocI(
    size_t          size) {

    void * mem;

    size = ES_DIV_ROUNDUP(size, sizeof(unative_T));

    if (size <= gSMemSentinel.current) {
        gSMemSentinel.current -= size;
        mem = (void *)&gSMemSentinel.begin[gSMemSentinel.current];
    } else {
        mem = NULL;
    }

    return (mem);
}

/*----------------------------------------------------------------------------*/
void * esSmemAlloc(
    size_t          size) {

    ES_CRITICAL_DECL();
    void * tmp;

    ES_CRITICAL_ENTER(
        OPT_SYS_INTERRUPT_PRIO_MAX);
    tmp = esSmemAllocI(
        size);
    ES_CRITICAL_EXIT();

    return (tmp);
}

/*----------------------------------------------------------------------------*/
void * esDmemAlloc(
    size_t          size) {

    ES_CRITICAL_DECL();
    void * tmpMem;

    ES_CRITICAL_ENTER(
        OPT_SYS_INTERRUPT_PRIO_MAX);
    tmpMem = esDmemAllocI(
        size);
    ES_CRITICAL_EXIT();

    return (tmpMem);
}

/*----------------------------------------------------------------------------*/
void * esDmemAllocI(
    size_t          size) {

#if (OPT_MEM_HEAP_SIZE != ES_MM_STATIC_ONLY)
    dmemBlkHdr_T * prevPhy;
    dmemBlkHdr_T * freeBlk;

    if (0UL == size) {
        size = ES_CPU_ATTRIB_ALIGNMENT;
    } else {

#if !defined(ES_CPU_ATTRIB_UNALIGNED_ACCESS) || defined(OPT_OPTIMIZE_SPEED)
        size = ES_ALIGN_UP(size, ES_CPU_ATTRIB_ALIGNMENT);
#else
        ;
#endif
    }

    ES_DLS_FOR_EACH_ENTRY(
        dmemBlkHdr_T,
        freeList,
        &(DMEM_SENTINEL->freeList),
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
                esDlsNodeRm_(
                    &(freeBlk->freeList));
            }
            BLOCK_SET_BUSY(freeBlk);

            return ((void *)&(freeBlk->freeList));
        }
    }
    ES_LOG_IF_ERR(&gKernelLog, LOG_FILT_MM, LOG_MM_DALLOC, ES_NOT_ENOUGH_MEM);

    return ((void *)0);
#else /* OPT_MEM_HEAP_SIZE != ES_MM_STATIC_ONLY */
    (void)size;
    ES_LOG_IF_ERR(&gKernelLog, LOG_FILT_MM, LOG_MM_DALLOC, ES_USAGE_FAILURE);

    return ((void *)0);
#endif
}

/*----------------------------------------------------------------------------*/
size_t esDmemBlockSize(
    void *          mem) {

#if (OPT_MEM_HEAP_SIZE != ES_MM_STATIC_ONLY)
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

    ES_CRITICAL_DECL();

    ES_CRITICAL_ENTER(
        OPT_SYS_INTERRUPT_PRIO_MAX);
    esDmemDeAllocI(
        mem);
    ES_CRITICAL_EXIT();
}

/*----------------------------------------------------------------------------*/
void esDmemDeAllocI(
    void *          mem) {

#if (OPT_MEM_HEAP_SIZE != ES_MM_STATIC_ONLY)
    size_t blkStat;
    dmemBlkHdr_T * freeBlk;
    dmemBlkHdr_T * currPhy;
    dmemBlkHdr_T * tmpPhy;

    if (ES_LOG_IS_DBG(&gKernelLog, LOG_FILT_MM)) {
        ES_LOG_DBG_IF_INVALID(&gKernelLog, (mem >= (void *)HEAP_BEGIN) && (mem <= (void *)HEAP_END), LOG_MM_DDALLOC, ES_ARG_OUT_OF_RANGE);
    }

    freeBlk = C_CONTAINER_OF((esDlsList_T *)mem, dmemBlkHdr_T, freeList);
    currPhy = ES_SLS_NODE_ENTRY(
        dmemBlkHdr_T,
        blk.phyList,
        freeBlk->blk.phyList.next);
    blkStat = BLOCK_GET_STATUS(currPhy);
    BLOCK_SET_FREE(freeBlk);

    if (BLOCK_IS_FREE == blkStat) {
        esDlsNodeRm_(
            &(currPhy->freeList));
        tmpPhy = PHY_BLK_PREV(freeBlk);
        esSlsNodeRmAfter_(
            &(tmpPhy->blk.phyList));
        currPhy->blk.size += freeBlk->blk.size + sizeof(dmemBlk_T);
        freeBlk = currPhy;
    }
    currPhy = PHY_BLK_PREV(freeBlk);
    blkStat = BLOCK_GET_STATUS(currPhy);

    if (BLOCK_IS_FREE == blkStat) {
        esDlsNodeRm_(
            &(currPhy->freeList));
        tmpPhy = PHY_BLK_PREV(currPhy);
        esSlsNodeRmAfter_(
            &(tmpPhy->blk.phyList));
        freeBlk->blk.size += currPhy->blk.size + sizeof(dmemBlk_T);
    }
    esDlsNodeAddHead_(
        &(DMEM_SENTINEL->freeList),
        &(freeBlk->freeList));
#else /* OPT_MEM_HEAP_SIZE != ES_MM_STATIC_ONLY */
    (void)mem;
    ES_LOG_IF_ERR(&gKernelLog, LOG_FILT_MM, LOG_MM_DDALLOC, ES_USAGE_FAILURE);

#endif
}

/*----------------------------------------------------------------------------*/
size_t esDmemFreeSpace(
    void) {

    ES_CRITICAL_DECL();
    size_t tmpSize;

    ES_CRITICAL_ENTER(
        OPT_SYS_INTERRUPT_PRIO_MAX);
    tmpSize = esDmemFreeSpaceI();
    ES_CRITICAL_EXIT();

    return (tmpSize);
}

/*----------------------------------------------------------------------------*/
size_t esDmemFreeSpaceI(
    void) {

#if (OPT_MEM_HEAP_SIZE != ES_MM_STATIC_ONLY)
    size_t free;
    dmemBlkHdr_T * currBlk;

    free = (size_t)0;

    ES_DLS_FOR_EACH_ENTRY(
        dmemBlkHdr_T,
        freeList,
        &(DMEM_SENTINEL->freeList),
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
