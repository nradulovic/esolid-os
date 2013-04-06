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
 * @brief       Privatni interfejs Memory Management modula.
 * @addtogroup  mm_impl
 *********************************************************************//** @{ */


#ifndef MM_PKG_H_
#define MM_PKG_H_

/*=========================================================  INCLUDE FILES  ==*/
/*===============================================================  DEFINES  ==*/
/*===============================================================  MACRO's  ==*/
/*------------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/**
 * @brief       Klasa memorije
 */
struct esMemClass {
/**
 * @brief       Memorijski alokator
 */
    void * (*alloc)(size_t);

/**
 * @brief       Memorijski dealokator
 */
    void (*deAlloc)(void *);

};

struct mmObject {
/**
 * @brief       Pokazivac na klasu memorijskog alokatora
 */
    const C_ROM struct esMemClass * memClass;
};

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

static C_INLINE_ALWAYS void * mmCreateObject(
    const C_ROM esMemClass_T *  memClass,
    size_t          size) {

#if (OPT_MM_DISTRIBUTION == ES_MM_DYNAMIC_ONLY)
    ES_CRITICAL_DECL();
    void * mmObject;

    if (ES_LOG_IS_WARN(&gKernelLog, LOG_FILT_MM)) {                             /* Upozori ako je zahtevan drugi alokator                   */
        ES_LOG_WARN_IF_INVALID(&gKernelLog, &esMemDynClass == memClass, LOG_MM_CREATEO, ES_ERR_ARG_OUT_OF_RANGE);
    } else {
        (void)memClass;
    }

    ES_CRITICAL_ENTER(
        OPT_KERNEL_INTERRUPT_PRIO_MAX);
    mmObject = esDmemAllocI(
        size);
    ES_CRITICAL_EXIT();

    return (mmObject);
#elif (OPT_MM_DISTRIBUTION == ES_MM_STATIC_ONLY)
    ES_CRITICAL_DECL();
    void * mmObject;

    if (ES_LOG_IS_WARN(&gKernelLog, LOG_FILT_MM)) {                             /* Upozori ako je zahtevan drugi alokator                   */
        ES_LOG_WARN_IF_INVALID(&gKernelLog, esMemSClass == memClass, LOG_MM_CREATEO, ES_ERR_ARG_OUT_OF_RANGE);
    } else {
        (void)memClass;
    }

    ES_CRITICAL_ENTER(
        OPT_KERNEL_INTERRUPT_PRIO_MAX);
    mmObject = esSmemAllocI(
        size);
    ES_CRITICAL_EXIT();

    return (mmObject);
#else
    struct mmObject mmObject;

    mmObject = (* memClass->alloc)(sizeof(struct memObject) + size);
    mmObject->memClass = memClass;

    return ((void *)(mmObject + 1UL));
#endif
}

static C_INLINE_ALWAYS void mmDestroyObject(
    void *          object) {

#if (OPT_MM_DISTRIBUTION == ES_MM_DYNAMIC_ONLY)
    esDmemDeAllocI(
        object);
#elif (OPT_MM_DISTRIBUTION == ES_MM_STATIC_ONLY)
    /* Greska! Ne moze da se izbrise statican objekat */
#else
    struct mmObject mmObject;

    mmObject = (struct mmObject *)object - 1UL;
    (* mmObject->memClass->deAlloc)(object);
#endif
}

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of mem_pkg.h
 ******************************************************************************/
#endif /* MM_PKG_H_ */
