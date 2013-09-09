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
 *//***********************************************************************//**
 * @file
 * @author      Nenad Radulovic
 * @brief       Memory Management API
 * @addtogroup  mem_intf
 * @brief       Memory Management API
 * @details     This module implements three classes of memory managers:
 *              - dynamic
 *              - pool
 *              - static
 *              For more details see @ref mem.
 *********************************************************************//** @{ */

#if !defined(MEM_H_)
#define MEM_H_

/*=========================================================  INCLUDE FILES  ==*/

#include "arch/compiler.h"
#include "arch/cpu.h"
#include "dbg/dbg.h"
#include "../config/sys_cfg.h"

/*===============================================================  MACRO's  ==*/

/*------------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/**@brief       Memory status structure
 * @details     This structure is used to get the status of a memory instance.
 *              Memory instance can be of type @c static, @c dynamic and @c pool.
 * @see         esSMemUpdateStatusI(), esPMemUpdateStatusI(), esDMemUpdateStatusI()
 * @api
 */
struct esMemStatus {
    size_t              size;                                                   /**<@brief Size of dynamic memory                           */
    size_t              freeSpaceTotal;                                         /**<@brief Total free space                                 */
    size_t              freeSpaceContiguous;                                    /**<@brief Contiguous free space                            */
};

/**@brief       Memory status type
 * @api
 */
typedef struct esMemStatus esMemStatus_T;

/*------------------------------------------------------------------------*//**
 * @name        Static memory manager - SMem
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Static memory instance handle structure
 * @details     This structure holds information about static memory instance.
 * @api
 */
struct esSMemHandle {
    portReg_T *         begin;                                                  /**<@brief Pointer to the beginning of static memory        */
    portReg_T           current;                                                /**<@brief Current index of managed memory                  */
    size_t              size;                                                   /**<@brief The size of static memory                        */

#if defined(OPT_GUARD_T) || defined(__DOXYGEN__)
    OPT_GUARD_T         guard;                                                  /**<@brief Memory allocator guard to implement MUTEX        */
#endif
#if (1U == CFG_DBG_API_VALIDATION) || defined(__DOXYGEN__)
    portReg_T           signature;                                              /**<@brief Structure signature, used during development only*/
#endif
};

/**@brief       Static memory instance handle type
 * @api
 */
typedef struct esSMemHandle esSMemHandle_T;

/**@} *//*----------------------------------------------------------------*//**
 * @name        Pool memory allocator - PMem
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Pool memory instance handle structure
 * @details     This structure holds information about pool memory instance.
 * @p           This structure hold information about pool and block sizes.
 *              Additionally, it holds a guard member which will ensure mutual
 *              exclusion in preemption environments.
 * @see         esPMemInit()
 * @api
 */
struct esPMemHandle {
    struct pMemBlock *  sentinel;                                               /**<@brief Pointer to the pool sentinel                     */
    size_t              size;                                                   /**<@brief The size of pool memory                          */
    size_t              blockSize;                                              /**<@brief Size of one block                                */

#if defined(OPT_GUARD_T) || defined(__DOXYGEN__)
    OPT_GUARD_T         guard;                                                  /**<@brief Memory allocator guard to implement MUTEX        */
#endif
#if (1U == CFG_DBG_API_VALIDATION) || defined(__DOXYGEN__)
    portReg_T           signature;                                              /**<@brief Structure signature, used during development only*/
#endif
};

/**@brief       Pool memory instance handle type
 * @api
 */
typedef struct esPMemHandle esPMemHandle_T;

/**@} *//*----------------------------------------------------------------*//**
 * @name        Dynamic memory allocator - DMem
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Dynamic memory instance handle structure
 * @details     This structure holds information about dynamic memory instance.
 * @see         esDMemInit()
 * @api
 */
struct esDMemHandle {
    struct dMemBlock *  sentinel;                                               /**<@brief Pointer to the memory sentinel                   */

#if defined(OPT_GUARD_T) || defined(__DOXYGEN__)
    OPT_GUARD_T         guard;                                                  /**<@brief Memory allocator guard to implement MUTEX        */
#endif
#if (1U == CFG_DBG_API_VALIDATION) || defined(__DOXYGEN__)
    portReg_T           signature;                                              /**<@brief Structure signature, used during development only*/
#endif
};

/**@brief       Dynamic memory instance handle type
 * @api
 */
typedef struct esDMemHandle esDMemHandle_T;

/** @} *//*-------------------------------------------------------------------*/

struct esMemClass {
    void *          handle;
    const PORT_C_ROM struct memMethods * methods;
#if (1U == CFG_DBG_API_VALIDATION)
    portReg_T       signature;
#endif
};

typedef struct esMemClass esMemClass_T;


/*======================================================  GLOBAL VARIABLES  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Default memory instance handles
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Default static memory handle
 * @api
 */
extern esSMemHandle_T gDefSMemHandle;

/**@brief       Default pool memory handle
 * @api
 */
extern esPMemHandle_T gDefPMemHandle;

/**@brief       Default dynamic memory handle
 * @api
 */
extern esDMemHandle_T gDefDMemHandle;

/** @} *//*-------------------------------------------------------------------*/

/*===================================================  FUNCTION PROTOTYPES  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Static memory manager - SMem
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Initializes static memory instance
 * @param       handle
 *              Pointer to handle type variable, see @ref esSMemHandle_T.
 * @param       storage
 *              Storage memory reserved for static memory manager.
 * @param       storageSize
 *              Size of reserved memory expresses in bytes.
 * @details     This function shall be called before any other static memory
 *              management function.
 * @api
 */
void esSMemInit(
    esSMemHandle_T *    handle,
    void *              storage,
    size_t              storageSize);

/**@brief       Allocates static memory of size @c size
 * @param       handle
 *              Pointer to static memory instance, see @ref esSMemHandle_T.
 * @param       size
 *              The size of requested memory in bytes.
 * @return      Pointer to free memory of requested size.
 * @iclass
 */
void * esSMemAllocI(
    esSMemHandle_T *    handle,
    size_t              size);

/**@brief       Allocates static memory of size @c size
 * @param       handle
 *              Pointer to static memory instance, see @ref esSMemHandle_T.
 * @param       size
 *              The size of requested memory in bytes.
 * @return      Pointer to free memory of requested size.
 * @api
 */
void * esSMemAlloc(
    esSMemHandle_T *    handle,
    size_t              size);

/**@brief       Returns various information about given memory instance
 * @param       handle
 *              Pointer to static memory instance, see @ref esSMemHandle_T.
 * @param       status
 *              Pointer to memory status type, see @ref esMemStatus_T.
 * @iclass
 */
void esSMemUpdateStatusI(
    esSMemHandle_T *    handle,
    esMemStatus_T *     status);

/**@brief       Returns various information about given memory instance
 * @param       handle
 *              Pointer to static memory instance, see @ref esSMemHandle_T.
 * @param       status
 *              Pointer to memory status type, see @ref esMemStatus_T.
 * @api
 */
void esSMemUpdateStatus(
    esSMemHandle_T *    handle,
    esMemStatus_T *     status);

/**@} *//*----------------------------------------------------------------*//**
 * @name        Pool memory manager - PMem
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Initializes pool memory instance
 * @param       handle
 *              Pointer to pool memory instance, see @ref esPMemHandle_T.
 * @param       pool
 *              Reserved memory area for pool allocator.
 * @param       poolSize
 *              The size of reserved memory area expressed in bytes.
 * @param       blockSize
 *              The size of one block expressed in bytes.
 * @details     This function must be called before any call to esPMemAllocI()
 *              or esPMemAlloc().
 * @warning     Pointers to @c handle and @c pool must be aligned to CPU defined
 *              alignment.
 * @api
 */
void esPMemInit(
    esPMemHandle_T *    handle,
    void *              pool,
    size_t              poolSize,
    size_t              blockSize);

/**@brief       Calculates required reserved memory size for defined number of
 *              blocks.
 * @param       blocks
 *              Number of required blocks.
 * @param       blockSize
 *              The size of one block.
 * @return      Required reserved memory size.
 * @api
 */
size_t esPMemCalcPoolSize(
    size_t          blocks,
    size_t          blockSize);

/**@brief       Allocate one block from memory pool
 * @param       handle
 *              Pointer to pool memory instance, see @ref esPMemHandle_T.
 * @return      Pointer to requested block.
 * @iclass
 */
void * esPMemAllocI(
    esPMemHandle_T * handle);

/**@brief       Allocate one block from memory pool
 * @param       handle
 *              Pointer to pool memory instance, see @ref esPMemHandle_T.
 * @return      Pointer to requested block.
 * @api
 */
void * esPMemAlloc(
    esPMemHandle_T * handle);

/**
 * @brief       Oslobadja prethodno alocirani blok
 * @param       [in] handle             Deskriptor pool alokatora
 * @param       [in] mem                Prethodno alociran blok memorije
 * @iclass
 */
void esPMemDeAllocI(
    esPMemHandle_T * handle,
    void *          mem);

/**
 * @brief       Oslobadja prethodno alocirani blok
 * @param       [in] handle             Deskriptor pool alokatora
 * @param       [in] mem                Prethodno alociran blok memorije
 * @note        Funkcija koristi makroe @ref OPT_GUARD_LOCK i
 *              @ref OPT_GUARD_UNLOCK za zastitu memorije od istovremenog
 *              pristupa.
 * @api
 */
void esPMemDeAlloc(
    esPMemHandle_T * handle,
    void *          mem);

/**
 * @brief       Vraca statusne informacije pool memorije
 * @param       [in] handle             Deskriptor pool alokatora
 * @param       [out] status            Status struktura pool alokatora
 * @iclass
 */
void esPMemUpdateStatusI(
    esPMemHandle_T * handle,
    esMemStatus_T * status);

/**
 * @brief       Vraca statusne informacije pool memorije
 * @param       [in] handle             Deskriptor pool alokatora
 * @param       [out] status            Status struktura pool alokatora
 * @api
 */
void esPMemUpdateStatus(
    esPMemHandle_T * handle,
    esMemStatus_T * status);

/**@} *//*----------------------------------------------------------------*//**
 * @name        Dinamicki memorijski alokator
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Inicijalizuje dinamican memorijski alokator
 * @param       [out] handle            Deskriptor dinamickog alokatora
 * @param       [in] storage            Predefinisani memorijski prostor koji se
 *                                      predaje dinamickom alokatoru na
 *                                      koriscenje
 * @param       storageSize             Velicina memorijskog prostora u
 *                                      bajtovima
 * @details     Ova funkcija se mora pozvati pre koriscenja funkcija dinamickog
 *              memorijskog alokatora.
 * @warning     Funkcija zahteva da pokazivaci handle i pool budu poravnani
 *              (aligned). Ukoliko se koriste eSolid alokatori za instaciranje
 *              @c handle strukture i @c poolStorage onda je poravnani pristup
 *              osiguran.
 * @warning     Funkcija zahteva da velicina memorijskog prostora @c storageSize
 *              bude poravnana (aligned). Na primer za 32-bitni procesor
 *              (poravnanje 4 bajta): ako je @c storageSize == 313 onda je
 *              potrebno poravnati na sledecu vecu vrednost koja je deljiva sa 4,
 *              u ovom slucaju ce to biti 316.
 * @api
 */
void esDMemInit(
    esDMemHandle_T *    handle,
    void *              storage,
    size_t              storageSize);

/**
 * @brief       Dodeljuje memorijski prostor velicine @c size
 * @param       [in] handle             Deskriptor dinamickog alokatora
 * @param       size                    Velicina zahtevanog memorijskog prostora
 *                                      u bajtovima.
 * @return      Pokazivac na rezervisani memorijski blok.
 * @details     U debug rezimu ova funkcija uvek vraca pokazivac, odnosno, ne
 *              moze se desiti da vrati NULL pokazivac, kao sto nalaze
 *              standardna implementacija @c malloc C funkcije. Ukoliko se
 *              zahtevana memorija ne moze dobaviti generisace se ASSERT greska.
 *              Kada se ne koristi debug rezim funkcija se ponasa u skladu sa
 *              standardom.
 * @iclass
 */
void * esDMemAllocI(
    esDMemHandle_T * handle,
    size_t          size);

/**
 * @brief       Dodeljuje memorijski prostor velicine @c size
 * @param       [in] handle             Deskriptor dinamickog alokatora
 * @param       size                    Velicina zahtevanog memorijskog prostora
 *                                      u bajtovima.
 * @return      Pokazivac na rezervisani memorijski blok.
 * @details     U debug rezimu ova funkcija uvek vraca pokazivac, odnosno, ne
 *              moze se desiti da vrati NULL pokazivac, kao sto nalaze
 *              standardna implementacija @c malloc C funkcije. Ukoliko se
 *              zahtevana memorija ne moze dobaviti generisace se ASSERT greska.
 *              Kada se ne koristi debug rezim funkcija se ponasa u skladu sa
 *              standardom.
 * @note        Funkcija koristi makroe @ref OPT_GUARD_LOCK i
 *              @ref OPT_GUARD_UNLOCK za zastitu memorije od istovremenog
 *              pristupa.
 * @api
 */
void * esDMemAlloc(
    esDMemHandle_T * handle,
    size_t          size);

/**
 * @brief       Reciklira memorijski prostor na koji pokazije @c mem
 *              pokazivac
 * @param       [in] handle             Deskriptor dinamickog alokatora
 * @param       [in] mem                Pokazivac na prethodno dodeljen
 *                                      memorijski prostor.
 * @iclass
 */
void esDMemDeAllocI(
    esDMemHandle_T * handle,
    void *          mem);

/**
 * @brief       Reciklira memorijski prostor na koji pokazije @c mem
 *              pokazivac
 * @param       [in] handle             Deskriptor dinamickog alokatora
 * @param       [in] mem                Pokazivac na prethodno dodeljen
 *                                      memorijski prostor.
 * @note        Funkcija koristi makroe @ref OPT_GUARD_LOCK i
 *              @ref OPT_GUARD_UNLOCK za zastitu memorije od istovremenog
 *              pristupa.
 * @api
 */
void esDMemDeAlloc(
    esDMemHandle_T * handle,
    void *          mem);

/**
 * @brief       Vraca velicinu trenutno slobodne memorije u bajtovima.
 * @param       [in] handle             Deskriptor dinamickog alokatora
 * @param       [out] status            Status struktura dinamickog alokatora
 * @details     Ukoliko je memorija jako fragmenitisana, sto je karakteristicno
 *              za first fir algoritam, moze se desiti da postoji dovoljno
 *              slobodne memorije, ali ne i bloka zahtevane velicine. U tom
 *              slucaju memorijski alokator nece biti u mogucnosti da ispuni
 *              zahtev.
 * @iclass
 */
void esDMemUpdateStatusI(
    esDMemHandle_T *    handle,
    esMemStatus_T *     status);

/**
 * @brief       Vraca velicinu trenutno slobodne memorije u bajtovima.
 * @param       [in] handle             Deskriptor dinamickog alokatora
 * @param       [out] status            Status struktura dinamickog alokatora
 * @details     Ukoliko je memorija jako fragmenitisana, sto je karakteristicno
 *              za first fir algoritam, moze se desiti da postoji dovoljno
 *              slobodne memorije, ali ne i bloka zahtevane velicine. U tom
 *              slucaju memorijski alokator nece biti u mogucnosti da ispuni
 *              zahtev.
 * @api
 */
void esDMemUpdateStatus(
    esDMemHandle_T *    handle,
    esMemStatus_T *     status);

void esSMemClassInit(
    esMemClass_T *      class,
    esSMemHandle_T *    handle);

void esPMemClassInit(
    esMemClass_T *      class,
    esPMemHandle_T *    handle);

void esDMemClassInit(
    esMemClass_T *      class,
    esDMemHandle_T *    handle);

/** @} *//*-----------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of mem.h
 ******************************************************************************/
#endif /* MEM_H_ */
