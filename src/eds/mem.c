/*
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
 *//**
 * @file
 * @author      Nenad Radulovic
 * @brief       Memory Management Implementation
 * @addtogroup  mem_impl
 *********************************************************************//** @{ */

/*=========================================================  INCLUDE FILES  ==*/

#include "eds/mem.h"
#include "eds/common.h"
#include "arch/cpu.h"

/*=========================================================  LOCAL MACRO's  ==*/

/**@brief       Signature for memory classes
 */
#define MEM_CLASS_SIGNATURE             ((portReg_T)0xDEADBEECU)

/**@brief       Signature for static memory manager
 */
#define SMEM_SIGNATURE                  ((portReg_T)0xDEADBEEDU)

/**@brief       Signature for pool memory manager
 */
#define PMEM_SIGNATURE                  ((portReg_T)0xDEADBEEEU)

/**@brief       Signature for dynamic memory manager
 */
#define DMEM_SIGNATURE                  ((portReg_T)0xDEADBEEFU)

/*======================================================  LOCAL DATA TYPES  ==*/

/**@brief       Dynamic allocator memory block header structure
 */
struct PORT_C_ALIGNED(PORT_DATA_ALIGNMENT) dMemBlock {
    size_t              phySize;                                                /**<@brief Block size (including header)                    */
    struct dMemBlock *  phyPrev;                                                /**<@brief Previous block in linked list                    */
    struct dMemBlock *  freeNext;                                               /**<@brief Next free block in linked list                   */
    struct dMemBlock *  freePrev;                                               /**<@brief Previous free block in linked list               */
};

/**@brief       Dynamic allocator memory block header type
 */
typedef struct dMemBlock dMemBlock_T;

/**@brief       Pool allocator header structure
 */
struct pMemBlock {
    struct pMemBlock *  next;                                                   /**<@brief Next free block                                  */
};

/**@brief       Pool allocator header type
 */
typedef struct pMemBlock pMemBlock_T;

struct memMethods {
    void * (* alloc)(void *, size_t);
    void (* dealloc)(void *, void *);
    void (* status)(void *, esMemStatus_T *);
};

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

static void unused_(
    void *              handle,
    void *              mem);

static void * wrapPMemAlloc(void * handle, size_t size);

/*=======================================================  LOCAL VARIABLES  ==*/

DECL_MODULE_INFO("MEM", "Memory management", "Nenad Radulovic");

static const PORT_C_ROM struct memMethods gSMemMethods = {
    (void * (*)(void *, size_t))esSMemAlloc,
    unused_,
    (void (*)(void *, esMemStatus_T*))esSMemUpdateStatus
};

static const PORT_C_ROM struct memMethods gPMemMethods = {
    wrapPMemAlloc,
    (void (*)(void *, void *))esPMemDeAlloc,
    (void (*)(void *, esMemStatus_T*))esPMemUpdateStatus
};

static const PORT_C_ROM struct memMethods gDMemMethods = {
    (void * (*)(void *, size_t))esDMemAlloc,
    (void (*)(void *, void *))esDMemDeAlloc,
    (void (*)(void *, esMemStatus_T*))esDMemUpdateStatus
};

/*======================================================  GLOBAL VARIABLES  ==*/

esSMemHandle_T gDefSMemHandle;
esPMemHandle_T gDefPMemHandle;
esDMemHandle_T gDefDMemHandle;

/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

static void unused_(
    void *              handle,
    void *              mem) {
    (void)handle;
    (void)mem;
}

static void * wrapPMemAlloc(
    void *              handle,
    size_t              size) {

    esPMemHandle_T *    pMemHandle;
    void *              mem;

    pMemHandle = (esPMemHandle_T *)handle;

    ES_DBG_API_REQUIRE(ES_DBG_NOT_ENOUGH_MEM, size <= pMemHandle->blockSize);
    mem = esPMemAlloc(pMemHandle);

    return (mem);
}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

/*----------------------------------------------------------------------------*/
void esSMemInit(
    esSMemHandle_T *    handle,
    void *              storage,
    size_t              storageSize) {

    ES_DBG_API_REQUIRE(ES_DBG_POINTER_NULL, NULL != handle);
    ES_DBG_API_REQUIRE(ES_DBG_POINTER_NULL, NULL != storage);
    ES_DBG_API_REQUIRE(ES_DBG_OUT_OF_RANGE, 0U != storageSize);
    handle->begin = storage;
    handle->current = GP_ALIGN(storageSize, sizeof(portReg_T));
    handle->size = GP_ALIGN(storageSize, sizeof(portReg_T));

#if defined(OPT_GUARD_T)
    OPT_GUARD_INIT(handle->guard);
#else
    OPT_GUARD_INIT(0U);
#endif
    ES_DBG_API_OBLIGATION(handle->signature = SMEM_SIGNATURE);
}

/*----------------------------------------------------------------------------*/
void * esSMemAllocI(
    esSMemHandle_T *    handle,
    size_t              size) {

    void * mem;

    ES_DBG_API_REQUIRE(ES_DBG_POINTER_NULL, NULL != handle);
    ES_DBG_API_REQUIRE(ES_DBG_OBJECT_NOT_VALID, SMEM_SIGNATURE == handle->signature);
    ES_DBG_API_REQUIRE(ES_DBG_OUT_OF_RANGE, 0U != size);

    size = GP_DIV_ROUNDUP(size, sizeof(portReg_T));
    handle->current -= size;
    mem = (void *)&handle->begin[handle->current];

    ES_DBG_API_ENSURE(ES_DBG_NOT_ENOUGH_MEM, size <= handle->current);

    return (mem);
}

/*----------------------------------------------------------------------------*/
void * esSMemAlloc(
    esSMemHandle_T *    handle,
    size_t              size) {

    OPT_GUARD_DECL;
    void * mem;

#if defined(OPT_GUARD_T)
    OPT_GUARD_LOCK(handle->guard);
#else
    OPT_GUARD_LOCK(0U);
#endif
    mem = esSMemAllocI(
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
void esSMemUpdateStatusI(
    esSMemHandle_T *    handle,
    esMemStatus_T *     status) {

    ES_DBG_API_REQUIRE(ES_DBG_POINTER_NULL, NULL != handle);
    ES_DBG_API_REQUIRE(ES_DBG_OBJECT_NOT_VALID, SMEM_SIGNATURE == handle->signature);
    ES_DBG_API_REQUIRE(ES_DBG_POINTER_NULL, NULL != status);

    status->size = handle->size;
    status->freeSpaceContiguous = handle->current * sizeof(portReg_T);
    status->freeSpaceTotal = status->freeSpaceContiguous;
}

/*----------------------------------------------------------------------------*/
void esSMemUpdateStatus(
    esSMemHandle_T *    handle,
    esMemStatus_T *     status) {

    OPT_GUARD_DECL;

#if defined(OPT_GUARD_T)
    OPT_GUARD_LOCK(handle->guard);
#else
    OPT_GUARD_LOCK(0U);
#endif
    esSMemUpdateStatusI(
        handle,
        status);
#if defined(OPT_GUARD_T)
    OPT_GUARD_UNLOCK(handle->guard);
#else
    OPT_GUARD_UNLOCK(0U);
#endif
}

/*----------------------------------------------------------------------------*/
void esPMemInit(
    esPMemHandle_T *    handle,
    void *              array,
    size_t              arraySize,
    size_t              blockSize) {

    size_t blockCnt;
    size_t blocks;
    pMemBlock_T * block;

    blockSize = GP_ALIGN_UP(blockSize, sizeof(portReg_T));

    ES_DBG_API_REQUIRE(ES_DBG_POINTER_NULL, NULL != handle);
    ES_DBG_API_REQUIRE(ES_DBG_POINTER_NULL, NULL != array);
    ES_DBG_API_REQUIRE(ES_DBG_OUT_OF_RANGE, 0U != blockSize);
    ES_DBG_API_REQUIRE(ES_DBG_OUT_OF_RANGE, blockSize <= arraySize);

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
    ES_DBG_API_OBLIGATION(handle->signature = PMEM_SIGNATURE);
}

/*----------------------------------------------------------------------------*/
size_t esPMemCalcPoolSize(
    size_t              blocks,
    size_t              blockSize) {

    ES_DBG_API_REQUIRE(ES_DBG_OUT_OF_RANGE, 0U != blocks);
    ES_DBG_API_REQUIRE(ES_DBG_OUT_OF_RANGE, 0U != blockSize);

    blockSize = GP_ALIGN_UP(blockSize, sizeof(portReg_T));

    return (blocks * blockSize);
}

/*----------------------------------------------------------------------------*/
void * esPMemAllocI(
    esPMemHandle_T *    handle) {

    pMemBlock_T * block;

    ES_DBG_API_REQUIRE(ES_DBG_POINTER_NULL, NULL != handle);
    ES_DBG_API_REQUIRE(ES_DBG_OBJECT_NOT_VALID, PMEM_SIGNATURE == handle->signature);

    block = handle->sentinel;
    ES_DBG_API_ENSURE(ES_DBG_NOT_ENOUGH_MEM, NULL != block);
    handle->sentinel = block->next;

    return ((void *)block);
}
/*----------------------------------------------------------------------------*/
void * esPMemAlloc(
    esPMemHandle_T *    handle) {

    OPT_GUARD_DECL;
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
    void *              mem) {

    pMemBlock_T * block;

    ES_DBG_API_REQUIRE(ES_DBG_POINTER_NULL, NULL != handle);
    ES_DBG_API_REQUIRE(ES_DBG_OBJECT_NOT_VALID, PMEM_SIGNATURE == handle->signature);
    ES_DBG_API_REQUIRE(ES_DBG_POINTER_NULL, NULL != mem);

    block = (pMemBlock_T *)mem;
    block->next = handle->sentinel;
    handle->sentinel = block;
}

/*----------------------------------------------------------------------------*/
void esPMemDeAlloc(
    esPMemHandle_T *    handle,
    void *              mem) {

    OPT_GUARD_DECL;

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

    ES_DBG_API_REQUIRE(ES_DBG_POINTER_NULL, NULL != handle);
    ES_DBG_API_REQUIRE(ES_DBG_OBJECT_NOT_VALID, PMEM_SIGNATURE == handle->signature);
    ES_DBG_API_REQUIRE(ES_DBG_POINTER_NULL, NULL != status);

    freeTotal = 0U;
    block = handle->sentinel;

    while (NULL != block) {
        freeTotal += handle->blockSize;
        block = block->next;
    }
    status->size = handle->size;
    status->freeSpaceTotal = freeTotal;
    status->freeSpaceContiguous = handle->blockSize;
}

/*----------------------------------------------------------------------------*/
void esPMemUpdateStatus(
    esPMemHandle_T *    handle,
    esMemStatus_T *     status) {

    OPT_GUARD_DECL;

#if defined(OPT_GUARD_T)
    OPT_GUARD_LOCK(handle->guard);
#else
    OPT_GUARD_LOCK(0U);
#endif
    esPMemUpdateStatusI(
        handle,
        status);
#if defined(OPT_GUARD_T)
    OPT_GUARD_UNLOCK(handle->guard);
#else
    OPT_GUARD_UNLOCK(0U);
#endif
}

/*----------------------------------------------------------------------------*/
void esDMemInit(
    esDMemHandle_T *    handle,
    void *              storage,
    size_t              storageSize) {

    dMemBlock_T * begin;

    ES_DBG_API_REQUIRE(ES_DBG_POINTER_NULL, NULL != handle);
    ES_DBG_API_REQUIRE(ES_DBG_POINTER_NULL, NULL != storage);
    ES_DBG_API_REQUIRE(ES_DBG_OUT_OF_RANGE, 0U != storageSize);

    storageSize = GP_ALIGN(storageSize, sizeof(portReg_T));
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
    ES_DBG_API_OBLIGATION(handle->signature = DMEM_SIGNATURE);
}

/*----------------------------------------------------------------------------*/
void * esDMemAllocI(
    esDMemHandle_T *    handle,
    size_t              size) {

    dMemBlock_T * curr;

    ES_DBG_API_REQUIRE(ES_DBG_POINTER_NULL, NULL != handle);
    ES_DBG_API_REQUIRE(ES_DBG_OBJECT_NOT_VALID, DMEM_SIGNATURE == handle->signature);
    ES_DBG_API_REQUIRE(ES_DBG_OUT_OF_RANGE, 0U != size);

    size = GP_ALIGN_UP(size, sizeof(portReg_T)) + sizeof(dMemBlock_T);
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
                curr->freeNext = NULL;                                          /* Mark block as allocated                                  */

                return ((void *)(curr + 1U));
            } else {
                curr->freeNext->freePrev = curr->freePrev;
                curr->freePrev->freeNext = curr->freeNext;
                curr->freeNext = NULL;                                          /* Mark block as allocated                                  */

                return ((void *)(curr + 1U));
            }
        }
        curr = curr->freeNext;
    }
    curr = NULL;
    ES_DBG_API_ENSURE(ES_DBG_NOT_ENOUGH_MEM, NULL != curr);

    return ((void *)curr);
}

/*----------------------------------------------------------------------------*/
void * esDMemAlloc(
    esDMemHandle_T *    handle,
    size_t              size) {

    OPT_GUARD_DECL;
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
    void *              mem) {

    dMemBlock_T * curr;
    dMemBlock_T * tmp;

    ES_DBG_API_REQUIRE(ES_DBG_POINTER_NULL, NULL != handle);
    ES_DBG_API_REQUIRE(ES_DBG_OBJECT_NOT_VALID, DMEM_SIGNATURE == handle->signature);
    ES_DBG_API_REQUIRE(ES_DBG_POINTER_NULL, NULL != mem);

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
    void *              mem) {

    OPT_GUARD_DECL;

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

    ES_DBG_API_REQUIRE(ES_DBG_POINTER_NULL, NULL != handle);
    ES_DBG_API_REQUIRE(ES_DBG_OBJECT_NOT_VALID, DMEM_SIGNATURE == handle->signature);
    ES_DBG_API_REQUIRE(ES_DBG_POINTER_NULL, NULL != status);

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
    status->freeSpaceContiguous = freeAvailable;
}

/*----------------------------------------------------------------------------*/
void esDMemUpdateStatus(
    esDMemHandle_T *    handle,
    esMemStatus_T *     status) {

    OPT_GUARD_DECL;

#if defined(OPT_GUARD_T)
    OPT_GUARD_LOCK(handle->guard);
#else
    OPT_GUARD_LOCK(0U);
#endif
    esDMemUpdateStatusI(
        handle,
        status);
#if defined(OPT_GUARD_T)
    OPT_GUARD_UNLOCK(handle->guard);
#else
    OPT_GUARD_UNLOCK(0U);
#endif
}

/*----------------------------------------------------------------------------*/
void esSMemClassInit(
    esMemClass_T *      class,
    esSMemHandle_T *    handle) {

    ES_DBG_API_REQUIRE(ES_DBG_OBJECT_NOT_VALID, class->signature != MEM_CLASS_SIGNATURE);
    class->handle = (void *)handle;
    class->methods = &gSMemMethods;
    ES_DBG_API_OBLIGATION(class->signature = MEM_CLASS_SIGNATURE);
}

/*----------------------------------------------------------------------------*/
void esPMemClassInit(
    esMemClass_T *      class,
    esPMemHandle_T *    handle) {

    ES_DBG_API_REQUIRE(ES_DBG_OBJECT_NOT_VALID, class->signature != MEM_CLASS_SIGNATURE);
    class->handle = (void *)handle;
    class->methods = &gPMemMethods;
    ES_DBG_API_OBLIGATION(class->signature = MEM_CLASS_SIGNATURE);
}

/*----------------------------------------------------------------------------*/
void esDMemClassInit(
    esMemClass_T *      class,
    esDMemHandle_T *    handle) {

    ES_DBG_API_REQUIRE(ES_DBG_OBJECT_NOT_VALID, class->signature != MEM_CLASS_SIGNATURE);
    class->handle = (void *)handle;
    class->methods = &gDMemMethods;
    ES_DBG_API_OBLIGATION(class->signature = MEM_CLASS_SIGNATURE);
}
/*----------------------------------------------------------------------------*/
void esMemClassTerm(
    esMemClass_T *  class) {

    ES_DBG_API_REQUIRE(ES_DBG_OBJECT_NOT_VALID, class->signature == MEM_CLASS_SIGNATURE);
    class->handle = NULL;
    class->methods = NULL;
    ES_DBG_API_OBLIGATION(class->signature = ~MEM_CLASS_SIGNATURE);
}

void * esMemClassAlloc(
    esMemClass_T *      class,
    size_t              size) {

    void * mem;

    mem = class->methods->alloc(class->handle, size);

    return (mem);
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of mem.c
 ******************************************************************************/
