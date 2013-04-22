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
/**
 * @brief       Pocetak bloka memorije
 */
    unative_T *     begin;

/**
 * @brief       Indeks trenutno koriscene memorije
 */
    unative_T       current;
};

/**
 * @brief       Blok memorije dinamickog alokatora
 */
typedef struct C_ALIGNED(ES_CPU_ATTRIB_ALIGNMENT) dMemBlock {
    struct dBlockPhy {
        size_t          size;
        struct dMemBlock * prev;
    }                   phy;
    struct dBlockFree {
        struct dMemBlock * next;
        struct dMemBlock * prev;
    }                   free;
} dMemBlock_T;

/**
 * @brief       Deskriptor dinamickog alokatora
 */
struct esDMemDesc {
/**
 * @brief       Iznos slobodne memorije
 */
    size_t          freeSpace;

/**
 * @brief       Pokazivac na cuvara memorije
 */
    struct dMemBlock * heapSentinel;
};

/**
 * @brief       Deskriptor pool alokatora
 */
struct esPMemDesc {
/**
 * @brief       Iznos slobodne memorije
 */
    size_t          freeSpace;

/**
 * @brief       Pokazivac na cuvara memorije
 */
    struct dMemBlock * heapSentinel;
};

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
void * esSMemAlloc(
    size_t          size) {

    ES_CRITICAL_DECL();
    void * tmp;

    ES_CRITICAL_ENTER(OPT_SYS_INTERRUPT_PRIO_MAX);
    tmp = esSMemAllocI(
        size);
    ES_CRITICAL_EXIT();

    return (tmp);
}

/*----------------------------------------------------------------------------*/
void esDMemInit(
    esDMemDesc_T *  desc,
    void *          array,
    size_t          elements) {

    dMemBlock_T * begin;

    elements = ES_ALIGN(elements, sizeof(unative_T));
    desc->freeSpace = elements - (2U * sizeof(dMemBlock_T));
    desc->heapSentinel = (dMemBlock_T *)((uint8_t *)array + elements) - 1U;        /* HeapSentinel is the last element of the array            */
    begin = (dMemBlock_T *)array;
    begin->phy.size = elements - sizeof(dMemBlock_T);
    begin->phy.prev = desc->heapSentinel;
    begin->free.next = desc->heapSentinel;
    begin->free.prev = desc->heapSentinel;
    desc->heapSentinel->phy.size = 0U;
    desc->heapSentinel->phy.prev = begin;
    desc->heapSentinel->free.next = begin;
    desc->heapSentinel->free.prev = begin;
}

/*----------------------------------------------------------------------------*/
void * esDMemAllocI(
    esDMemDesc_T *  desc,
    size_t          size) {

    dMemBlock_T * curr;

    size = ES_ALIGN_UP(size, sizeof(unative_T)) + sizeof(dMemBlock_T);
    curr = desc->heapSentinel->free.next;

    while (curr != desc->heapSentinel) {

        if (curr->phy.size >= size) {

            if (curr->phy.size > size + sizeof(dMemBlock_T)) {
                dMemBlock_T * tmp;

                tmp = (dMemBlock_T *)((uint8_t *)curr + size);
                tmp->phy.size = curr->phy.size - size;
                tmp->phy.prev = curr;
                tmp->free.next = curr->free.next;
                tmp->free.prev = curr->free.prev;
                tmp->free.next->free.prev = tmp;
                tmp->free.prev->free.next = tmp;
                curr->free.next = NULL;                                         /* Mark block as allocated                                  */

                return ((void *)(curr + 1U));
            } else {
                curr->free.next->free.prev = curr->free.prev;
                curr->free.prev->free.next = curr->free.next;
                curr->free.next = NULL;                                         /* Mark block as allocated                                  */

                return ((void *)(curr + 1U));
            }
        }
        curr = curr->free.next;
    }

    return (NULL);
}

/*----------------------------------------------------------------------------*/
void * esDMemAlloc(
    esDMemDesc_T *  desc,
    size_t          size) {

    ES_CRITICAL_DECL();
    void * mem;

    ES_CRITICAL_ENTER(OPT_SYS_INTERRUPT_PRIO_MAX);
    mem = esDMemAllocI(
        desc,
        size);
    ES_CRITICAL_EXIT();

    return (mem);
}

/*----------------------------------------------------------------------------*/
void esDMemDeAllocI(
    esDMemDesc_T *  desc,
    void *          mem) {

    dMemBlock_T * curr;
    dMemBlock_T * tmp;

    curr = (dMemBlock_T *)mem - 1U;
    tmp = (dMemBlock_T *)((uint8_t *)curr + curr->phy.size);

    if ((NULL != curr->phy.prev->free.next) && (NULL == tmp->free.next)) {      /* Previous block is free                                   */
        curr->phy.prev->phy.size += curr->phy.size;
        tmp->phy.prev = curr->phy.prev;
    } else if ((NULL == curr->phy.prev->free.next) && (NULL != tmp->free.next)) { /* Next block is free                                     */
        curr->free.next = tmp->free.next;
        curr->free.prev = tmp->free.prev;
        curr->free.prev->free.next = curr;
        curr->free.next->free.prev = curr;
        curr->phy.size += tmp->phy.size;
        tmp = (dMemBlock_T *)((uint8_t *)curr + curr->phy.size);
        tmp->phy.prev = curr;
    } else if ((NULL != curr->phy.prev->free.next) && (NULL != tmp->free.next)) { /* Previous and next blocks are free                      */
        tmp->free.prev->free.next = tmp->free.next;
        tmp->free.next->free.prev = tmp->free.prev;
        curr->phy.prev->phy.size += curr->phy.size + tmp->phy.size;
        tmp = (dMemBlock_T *)((uint8_t *)curr->phy.prev + curr->phy.prev->phy.size);
        tmp->phy.prev = curr->phy.prev;
    } else {                                                                    /* Previous and next blocks are allocated                   */
        curr->free.next = desc->heapSentinel->free.next;
        curr->free.prev = desc->heapSentinel;
        curr->free.prev->free.next = curr;
        curr->free.next->free.prev = curr;
    }
}

/*----------------------------------------------------------------------------*/
void esDMemDeAlloc(
    esDMemDesc_T *  desc,
    void *          mem) {

    ES_CRITICAL_DECL();

    ES_CRITICAL_ENTER(OPT_SYS_INTERRUPT_PRIO_MAX);
    esDMemDeAllocI(
        desc,
        mem);
    ES_CRITICAL_EXIT();
}

#else /* defined(OPT_SYS_ENABLE_MEM) */

/*----------------------------------------------------------------------------*/
void esSMemInit(
    void) {

    ;
}

/*----------------------------------------------------------------------------*/
size_t esSMemFreeSpace(
    void) {

    return (0U);
}

/*----------------------------------------------------------------------------*/
void * esSMemAllocI(
    size_t          size) {

    void * mem;

    mem = OPT_MEM_ALLOC(size);

    return (mem);
}

/*----------------------------------------------------------------------------*/
void * esSMemAlloc(
    size_t          size) {

    ES_CRITICAL_DECL();
    void * tmp;

    ES_CRITICAL_ENTER(OPT_SYS_INTERRUPT_PRIO_MAX);
    tmp = OPT_MEM_ALLOC(size);
    ES_CRITICAL_EXIT();

    return (tmp);
}

/*----------------------------------------------------------------------------*/
void esDMemInit(
    esDMemDesc_T *  desc,
    void *          array,
    size_t          elements) {

    (void)desc;
    (void)array;
    (void)elements;
}

/*----------------------------------------------------------------------------*/
void * esDMemAllocI(
    esDMemDesc_T *  desc,
    size_t          size) {

    void * mem;

    (void)desc;

    mem = OPT_MEM_ALLOC(size);

    return (mem);
}

/*----------------------------------------------------------------------------*/
void * esDMemAlloc(
    esDMemDesc_T *  desc,
    size_t          size) {

    ES_CRITICAL_DECL();
    void * mem;
    (void)desc;

    ES_CRITICAL_ENTER(OPT_SYS_INTERRUPT_PRIO_MAX);
    mem = OPT_MEM_ALLOC(size);
    ES_CRITICAL_EXIT();

    return (mem);
}

/*----------------------------------------------------------------------------*/
void esDMemDeAllocI(
    esDMemDesc_T *  desc,
    void *          mem) {

    (void)desc;

    OPT_MEM_FREE(mem);
}

/*----------------------------------------------------------------------------*/
void esDMemDeAlloc(
    esDMemDesc_T *  desc,
    void *          mem) {

    ES_CRITICAL_DECL();
    (void)desc;

    ES_CRITICAL_ENTER(OPT_SYS_INTERRUPT_PRIO_MAX);
    OPT_MEM_FREE(mem);
    ES_CRITICAL_EXIT();
}
#endif /* !defined(OPT_SYS_ENABLE_MEM) */

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of mem.c
 ******************************************************************************/
