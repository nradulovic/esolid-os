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
#include "mem/mem.h"
#include "primitive/list.h"

/*=========================================================  LOCAL DEFINES  ==*/

#if (OPT_MEM_CORE_SIZE == 0U)                                                   /* Koristi se linker skripta.                               */
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
/*======================================================  LOCAL DATA TYPES  ==*/

typedef uint16_t blockElement;

typedef struct C_ALIGNED(ES_CPU_ATTRIB_ALIGNMENT) dBlock {
    unative_T       size;
    struct dBlock * freeNext;
    struct dBlock * phyPrev;
    struct dBlock * phyNext;
} dBlock_T;

struct esDmemDesc {
    size_t          freeSpace;
    struct dBlock * heapSentinel;
};

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
void esDmemInit(
    esDmemDesc_T *  desc,
    size_t          size) {

    dBlock_T * heap;
    uint32_t numOfBlocks;

    size = ES_ALIGN_UP(size, sizeof(dBlock_T));
    numOfBlocks = size / sizeof(dBlock_T) + 2U;
    heap = esSmemAlloc(
        numOfBlocks * sizeof(dBlock_T));
    heap[numOfBlocks - 1U].size = 0U;
    heap[numOfBlocks - 1U].freeNext = 0U;
    heap[numOfBlocks - 1U].phyPrev = 0U;
    heap[numOfBlocks - 1U].phyNext = 0U;
    desc->freeSpace = size;
    desc->heapSentinel = &heap[numOfBlocks - 1U];
    heap[0].size = numOfBlocks - 2U;
    heap[0].freeNext = numOfBlocks - 1U;
    heap[0].phyNext = numOfBlocks - 1U;
    heap[0].phyPrev = numOfBlocks - 1U;
}

/*----------------------------------------------------------------------------*/
void * esDmemAllocI(
    esDmemDesc_T *  desc,
    size_t          size) {

    dBlock_T * block;

    size = ES_DIV_ROUNDUP(size, ES_CPU_ATTRIB_ALIGNMENT);

    block = desc->heapSentinel;

    while (block->freeNext != desc->heapSentinel) {

    }
}
/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of mem.c
 ******************************************************************************/
