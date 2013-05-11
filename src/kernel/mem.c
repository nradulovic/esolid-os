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
 * @brief       Memory Management Implementation
 * @addtogroup  mem_impl
 * @brief       Implementacija
 *********************************************************************//** @{ */

/*=========================================================  INCLUDE FILES  ==*/
#include "kernel/mem.h"
#include "../config/log_config.h"
#include "hal/hal_cpu.h"

/*
 * Ako GUARD ili CRITICAL makroi koriste eSolid HAL onda hal_int treba da se ukljuci
 */
#if (0U == OPT_GUARD_EXTERN) || (0U == OPT_CRITICAL_EXTERN)
# include "hal/hal_int.h"
#endif

/*=========================================================  LOCAL DEFINES  ==*/
/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/

/**
 * @brief       Cuvar memorije staticnog alokatora
 */
struct sMemSentinel {
/** @brief      Pocetak bloka memorije                                        */
    unative_T *     begin;

/** @brief      Indeks trenutno koriscene memorije                            */
    unative_T       current;
};

/**
 * @brief       Zaglavlje bloka dinamickog alokatora
 */
typedef struct C_ALIGNED(ES_CPU_ATTRIB_ALIGNMENT) dMemBlock {
/** @brief      Velicina bloka memorije (zajedno sa ovom strukturom)          */
    size_t          phySize;

/** @brief      Prethodni blok u fizickoj listi                               */
    struct dMemBlock *  phyPrev;

/** @brief      Sledeci blok u listi slobodnih blokova                        */
    struct dMemBlock *  freeNext;

/** @brief      Prethodni blok u listi slobodnih blokova                      */
    struct dMemBlock *  freePrev;
} dMemBlock_T;

/**
 * @brief       Zaglavlje bloka pool alokatora
 */
typedef struct pMemBlock {
/** @brief      Pokazivac na sledeci slobodan blok                            */
    struct pMemBlock *  next;
} pMemBlock_T;

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/

/**
 * @brief       Cuvar memorije staticnog alokatora
 */
static struct sMemSentinel gSMemSentinel;

#if (OPT_MEM_SMEM_SIZE != 0U)
static unative_T sMemBuffer[ES_DIV_ROUNDUP(OPT_MEM_SMEM_SIZE, sizeof(unative_T))];
#else
extern uint8_t _sheap;
extern uint8_t _eheap;
#endif

/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

/*----------------------------------------------------------------------------*/
void esSMemInit(
    void) {

#if (OPT_MEM_SMEM_SIZE != 0U)
    gSMemSentinel.begin = &sMemBuffer[0];
    gSMemSentinel.current = ES_DIMENSION(sMemBuffer);
#else
    gSMemSentinel.begin = (unative_T *)&_sheap;
    gSMemSentinel.current = (&_eheap - &_sheap) / sizeof(unative_T);
#endif
}

/*----------------------------------------------------------------------------*/
void * esSMemAllocI(
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
void * esSMemAlloc(
    size_t          size) {

    OPT_CRITICAL_DECL();
    void * mem;

    OPT_CRITICAL_LOCK();
    mem = esSMemAllocI(
        size);
    OPT_CRITICAL_UNLOCK();

    return (mem);
}

/*----------------------------------------------------------------------------*/
void esSMemUpdateStatusI(
    esMemStatus_T *     status) {

#if (OPT_MEM_SMEM_SIZE != 0U)
    status->size = sizeof(sMemBuffer);
#else
    status->size = (size_t)(&_eheap - &_sheap);
#endif
    status->freeSpaceAvailable = gSMemSentinel.current * sizeof(unative_T);
    status->freeSpaceTotal = status->freeSpaceAvailable;
}

/*----------------------------------------------------------------------------*/
void esPMemInit(
    esPMemHandle_T *    handle,
    void *          array,
    size_t          arraySize,
    size_t          blockSize) {

    size_t blockCnt;
    size_t blocks;
    pMemBlock_T * block;

    blocks = arraySize / blockSize;
    handle->size = arraySize;
    handle->blockSize = blockSize;
    handle->sentinel = (pMemBlock_T *)array;
    block = handle->sentinel;

    for (blockCnt = 0U; blockCnt < blocks - 1U; blockCnt++) {
        block->next = (struct pMemBlock *)((uint8_t *)block + handle->blockSize);
        block = block->next;
    }
    block->next = NULL;

#if defined(OPT_GUARD_T)
    OPT_GUARD_INIT(handle->guard);
#else
    OPT_GUARD_INIT(0U);
#endif
}

/*----------------------------------------------------------------------------*/
size_t esPMemCalcPoolSize(
    size_t          blocks,
    size_t          blockSize) {

    blockSize = ES_ALIGN_UP(blockSize, sizeof(unative_T));

    return (blocks * blockSize);
}

/*----------------------------------------------------------------------------*/
void * esPMemAllocI(
    esPMemHandle_T * handle) {

    pMemBlock_T * block;

    block = handle->sentinel;

    if (NULL != block) {
        handle->sentinel = block->next;
    }

    return ((void *)block);
}
/*----------------------------------------------------------------------------*/
void * esPMemAlloc(
    esPMemHandle_T * handle) {

    OPT_GUARD_DECL();
    void * mem;

#if defined(OPT_GUARD_T)
    OPT_GUARD_LOCK(handle->guard);
#else
    OPT_GUARD_LOCK(0U);
#endif
    mem = esPMemAllocI(
        handle);

#if defined(OPT_GUARD_T)
    OPT_GUARD_UNLOCK(handle->guard);
#else
    OPT_GUARD_UNLOCK(0U);
#endif

    return (mem);
}

/*----------------------------------------------------------------------------*/
void esPMemDeAllocI(
    esPMemHandle_T *    handle,
    void *          mem) {

    pMemBlock_T * block;

    block = (pMemBlock_T *)mem;
    block->next = handle->sentinel;
    handle->sentinel = block;
}

/*----------------------------------------------------------------------------*/
void esPMemDeAlloc(
    esPMemHandle_T *    handle,
    void *          mem) {

    OPT_GUARD_DECL();

#if defined(OPT_GUARD_T)
    OPT_GUARD_LOCK(handle->guard);
#else
    OPT_GUARD_LOCK(0U);
#endif
    esPMemDeAllocI(
        handle,
        mem);

#if defined(OPT_GUARD_T)
    OPT_GUARD_UNLOCK(handle->guard);
#else
    OPT_GUARD_UNLOCK(0U);
#endif
}

/*----------------------------------------------------------------------------*/
void esPMemUpdateStatusI(
    esPMemHandle_T *    handle,
    esMemStatus_T *     status) {

    size_t freeTotal;
    pMemBlock_T * block;

    freeTotal = 0U;
    block = handle->sentinel;

    while (NULL != block) {
        freeTotal += handle->blockSize;
        block = block->next;
    }
    status->size = handle->size;
    status->freeSpaceTotal = freeTotal;
    status->freeSpaceAvailable = handle->blockSize;
}

/*----------------------------------------------------------------------------*/
void esDMemInit(
    esDMemHandle_T *    handle,
    void *          storage,
    size_t          storageSize) {

    dMemBlock_T * begin;

    storageSize = ES_ALIGN(storageSize, sizeof(unative_T));
    handle->sentinel = (dMemBlock_T *)((uint8_t *)storage + storageSize) - 1U;  /* Sentinel is the last element of the storage              */
    begin = (dMemBlock_T *)storage;
    begin->phySize = storageSize - sizeof(dMemBlock_T);
    begin->phyPrev = handle->sentinel;
    begin->freeNext = handle->sentinel;
    begin->freePrev = handle->sentinel;
    handle->sentinel->phySize = 0U;
    handle->sentinel->phyPrev = begin;
    handle->sentinel->freeNext = begin;
    handle->sentinel->freePrev = begin;

#if defined(OPT_GUARD_T)
    OPT_GUARD_INIT(handle->guard);
#else
    OPT_GUARD_INIT(0U);
#endif
}

/*----------------------------------------------------------------------------*/
void * esDMemAllocI(
    esDMemHandle_T *    handle,
    size_t          size) {

    dMemBlock_T * curr;

    size = ES_ALIGN_UP(size, sizeof(unative_T)) + sizeof(dMemBlock_T);
    curr = handle->sentinel->freeNext;

    while (curr != handle->sentinel) {

        if (curr->phySize >= size) {

            if (curr->phySize > size + sizeof(dMemBlock_T)) {
                dMemBlock_T * tmp;

                tmp = (dMemBlock_T *)((uint8_t *)curr + size);
                tmp->phySize = curr->phySize - size;
                tmp->phyPrev = curr;
                tmp->freeNext = curr->freeNext;
                tmp->freePrev = curr->freePrev;
                tmp->freeNext->freePrev = tmp;
                tmp->freePrev->freeNext = tmp;
                curr->freeNext = NULL;                                         /* Mark block as allocated                                  */

                return ((void *)(curr + 1U));
            } else {
                curr->freeNext->freePrev = curr->freePrev;
                curr->freePrev->freeNext = curr->freeNext;
                curr->freeNext = NULL;                                         /* Mark block as allocated                                  */

                return ((void *)(curr + 1U));
            }
        }
        curr = curr->freeNext;
    }

    return (NULL);
}

/*----------------------------------------------------------------------------*/
void * esDMemAlloc(
    esDMemHandle_T * handle,
    size_t          size) {

    OPT_GUARD_DECL();
    void * mem;

#if defined(OPT_GUARD_T)
    OPT_GUARD_LOCK(handle->guard);
#else
    OPT_GUARD_LOCK(0U);
#endif
    mem = esDMemAllocI(
        handle,
        size);

#if defined(OPT_GUARD_T)
    OPT_GUARD_UNLOCK(handle->guard);
#else
    OPT_GUARD_UNLOCK(0U);
#endif

    return (mem);
}

/*----------------------------------------------------------------------------*/
void esDMemDeAllocI(
    esDMemHandle_T *    handle,
    void *          mem) {

    dMemBlock_T * curr;
    dMemBlock_T * tmp;

    curr = (dMemBlock_T *)mem - 1U;
    tmp = (dMemBlock_T *)((uint8_t *)curr + curr->phySize);

    if ((NULL != curr->phyPrev->freeNext) && (NULL == tmp->freeNext)) {         /* Previous block is free                                   */
        curr->phyPrev->phySize += curr->phySize;
        tmp->phyPrev = curr->phyPrev;
    } else if ((NULL == curr->phyPrev->freeNext) && (NULL != tmp->freeNext)) {  /* Next block is free                                     */
        curr->freeNext = tmp->freeNext;
        curr->freePrev = tmp->freePrev;
        curr->freePrev->freeNext = curr;
        curr->freeNext->freePrev = curr;
        curr->phySize += tmp->phySize;
        tmp = (dMemBlock_T *)((uint8_t *)curr + curr->phySize);
        tmp->phyPrev = curr;
    } else if ((NULL != curr->phyPrev->freeNext) && (NULL != tmp->freeNext)) {  /* Previous and next blocks are free                      */
        tmp->freePrev->freeNext = tmp->freeNext;
        tmp->freeNext->freePrev = tmp->freePrev;
        curr->phyPrev->phySize += curr->phySize + tmp->phySize;
        tmp = (dMemBlock_T *)((uint8_t *)curr->phyPrev + curr->phyPrev->phySize);
        tmp->phyPrev = curr->phyPrev;
    } else {                                                                    /* Previous and next blocks are allocated                   */
        curr->freeNext = handle->sentinel->freeNext;
        curr->freePrev = handle->sentinel;
        curr->freePrev->freeNext = curr;
        curr->freeNext->freePrev = curr;
    }
}

/*----------------------------------------------------------------------------*/
void esDMemDeAlloc(
    esDMemHandle_T *    handle,
    void *          mem) {

    OPT_GUARD_DECL();

#if defined(OPT_GUARD_T)
    OPT_GUARD_LOCK(handle->guard);
#else
    OPT_GUARD_LOCK(0U);
#endif
    esDMemDeAllocI(
        handle,
        mem);

#if defined(OPT_GUARD_T)
    OPT_GUARD_UNLOCK(handle->guard);
#else
    OPT_GUARD_UNLOCK(0U);
#endif
}

/*----------------------------------------------------------------------------*/
void esDMemUpdateStatusI(
    esDMemHandle_T *    handle,
    esMemStatus_T *     status) {

    size_t size;
    size_t freeTotal;
    size_t freeAvailable;
    dMemBlock_T * curr;

    size = 0U;
    freeTotal = 0U;
    freeAvailable = 0U;
    curr = handle->sentinel->phyPrev;

    while (curr != handle->sentinel) {
        size += curr->phySize;

        if (NULL != curr->freeNext) {
            size_t freeSize;

            freeSize = curr->phySize - sizeof(dMemBlock_T);
            freeTotal += freeSize;

            if (freeSize > freeAvailable) {
                freeAvailable = freeSize;
            }
        }
        curr = curr->phyPrev;
    }
    status->size = size;
    status->freeSpaceTotal = freeTotal;
    status->freeSpaceAvailable = freeAvailable;
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of mem.c
 ******************************************************************************/
