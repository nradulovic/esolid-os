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
 * @defgroup    mm_impl Memory Management Implementation
 * @brief       Implementacija
 *********************************************************************//** @{ */

/*=========================================================  INCLUDE FILES  ==*/
#include "kernel/mem.h"

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
 * @brief       Blok memorije dinamickog alokatora
 */
typedef struct C_ALIGNED(ES_CPU_ATTRIB_ALIGNMENT) dMemBlock {
/** @brief      Velicina bloka memorije                                       */
    size_t          phySize;

/** @brief      Prethodni blok u fizickoj listi                               */
    struct dMemBlock * phyPrev;

/** @brief      Sledeci blok u listi slobodnih blokova                        */
    struct dMemBlock * freeNext;

/** @brief      Prethodni blok u listi slobodnih blokova                      */
    struct dMemBlock * freePrev;
} dMemBlock_T;

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/

/**
 * @brief       Cuvar memorije staticnog alokatora
 */
static struct sMemSentinel gSMemSentinel;

/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

#if defined(OPT_SYS_ENABLE_MEM)
/*----------------------------------------------------------------------------*/
void esSMemInit(
    void) {

#if (OPT_MEM_CORE_SIZE != 0U)

    static unative_T sMemBuffer[ES_DIV_ROUNDUP(OPT_MEM_CORE_SIZE, sizeof(unative_T))];

    gSMemSentinel.begin = &sMemBuffer;
    gSMemSentinel.current = ES_DIMENSION(sMemBuffer);
#else

    extern uint8_t _sheap;
    extern uint8_t _eheap;

    gSMemSentinel.begin = (unative_T *)&_sheap;
    gSMemSentinel.current = (&_eheap - &_sheap) / sizeof(unative_T);
#endif
}

/*----------------------------------------------------------------------------*/
size_t esSMemFreeSpace(
    void) {

    return (gSMemSentinel.current);
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
void esDMemInit(
    esDMemHandle_T *    handle,
    void *          array,
    size_t          bytes) {

    dMemBlock_T * begin;

    bytes = ES_ALIGN(bytes, sizeof(unative_T));
    handle->heapSentinel = (dMemBlock_T *)((uint8_t *)array + bytes) - 1U;        /* HeapSentinel is the last element of the array            */
    begin = (dMemBlock_T *)array;
    begin->phySize = bytes - sizeof(dMemBlock_T);
    begin->phyPrev = handle->heapSentinel;
    begin->freeNext = handle->heapSentinel;
    begin->freePrev = handle->heapSentinel;
    handle->heapSentinel->phySize = 0U;
    handle->heapSentinel->phyPrev = begin;
    handle->heapSentinel->freeNext = begin;
    handle->heapSentinel->freePrev = begin;
}

/*----------------------------------------------------------------------------*/
void * esDMemAllocI(
    esDMemHandle_T *    handle,
    size_t          size) {

    dMemBlock_T * curr;

    size = ES_ALIGN_UP(size, sizeof(unative_T)) + sizeof(dMemBlock_T);
    curr = handle->heapSentinel->freeNext;

    while (curr != handle->heapSentinel) {

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
        curr->freeNext = handle->heapSentinel->freeNext;
        curr->freePrev = handle->heapSentinel;
        curr->freePrev->freeNext = curr;
        curr->freeNext->freePrev = curr;
    }
}

/*----------------------------------------------------------------------------*/
void esDMemStatusI(
    esDMemHandle_T *    handle,
    esDMemStatus_T *    status) {

}

/*----------------------------------------------------------------------------*/
void esPMemInit(
    esPMemHandle_T *    handle,
    void *          array,
    size_t          arraySize,
    size_t          blockSize) {

    size_t blockCnt;
    size_t blocks;
    esPMemBlock_T * block;

    blockSize = ES_ALIGN_UP(blockSize + sizeof(struct esPMemBlock), sizeof(unative_T));
    handle->blockSize = blockSize;
    handle->poolSentinel = (esPMemBlock_T *)array;
    blocks = arraySize / blockSize;
    block = handle->poolSentinel;

    for (blockCnt = 0U; blockCnt < blocks - 1U; blockCnt++) {
        block->next = (uint8_t *)block + handle->blockSize;
        block = block->next;
    }
    block->next = NULL;
}

/*----------------------------------------------------------------------------*/
size_t esPMemCalcArraySize(
    size_t          blocks,
    size_t          blockSize) {

    blockSize = ES_ALIGN_UP(blockSize + sizeof(struct esPMemBlock), sizeof(unative_T));

    return (blocks * blockSize);
}

/*----------------------------------------------------------------------------*/
void * esPMemAllocI(
    esPMemHandle_T *    handle) {

    esPMemBlock_T * block;

    block = handle->poolSentinel;

    if (NULL != block) {
        handle->poolSentinel = block->next;
    }

    return ((void *)(block + 1U));
}

/*----------------------------------------------------------------------------*/
void esPMemDeAllocI(
    esPMemHandle_T *    handle,
    void *          mem) {

    esPMemBlock_T * block;

    block = (esPMemBlock_T *)mem - 1U;
    block->next = handle->poolSentinel;
    handle->poolSentinel = block;
}

#else /* defined(OPT_SYS_ENABLE_MEM) */


#endif /* !defined(OPT_SYS_ENABLE_MEM) */

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of mem.c
 ******************************************************************************/
