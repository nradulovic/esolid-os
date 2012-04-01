/*************************************************************************************************
 * This file is part of eSolid
 *
 * Copyright (C) 2011 - Nenad Radulovic
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
 *************************************************************************************************/


/*********************************************************************************************//**
 * @file
 *
 * @author      nesa
 *
 * @brief       Interfejs za GCC kompajler, port klasa: common.
 *
 * ------------------------------------------------------------------------------------------------
 *
 * @addtogroup  hal_compiler
 *
 ****************************************************************************************//** @{ */

#ifndef CMN_COMPILER_H_
#define CMN_COMPILER_H_


/*************************************************************************************************
 * INCLUDE FILES
 *************************************************************************************************/

/*-----------------------------------------------------------------------------------------------*
 * Module dependencies
 *-----------------------------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------------------------*
 * Module configuration and adapter
 *-----------------------------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------------------------*
 * EXTERNS
 *-----------------------------------------------------------------------------------*//** @cond */

#ifdef CMN_COMPILER_H_VAR
#define CMN_COMPILER_H_EXT
#else
#define CMN_COMPILER_H_EXT extern
#endif


/** @endcond*//***********************************************************************************
 * DEFINES
 *************************************************************************************************/

/*-------------------------------------------------------------------------------------------*//**
 * @name        Svojstva HAL porta
 * @{ *//*---------------------------------------------------------------------------------------*/

#if !defined(PORT_IS_POISONED)
# define PORT_IS_POISONED
# if !defined(__cplusplus)
/*
 * Disallow some C++ keywords as identifiers in C programs,
 * for improved portability.
 */
#  pragma GCC poison new delete class template typename
#  pragma GCC poison private protected public operator
#  pragma GCC poison friend mutable using namespace
#  pragma GCC poison cin cout cerr clog
# endif
#endif

/** @} *//*--------------------------------------------------------------------------------------*/


/*************************************************************************************************
 * MACRO's
 *************************************************************************************************/

/*-------------------------------------------------------------------------------------------*//**
 * @name        Implementacija makroa za GCC kompajler
 * @{ *//*---------------------------------------------------------------------------------------*/

#if !defined(C_FUNC) || defined(__DOXYGEN__)
# if (__STDC_VERSION < 199901L) && (__GNUC__ >= 2) || defined(__DOXYGEN__)
#  define C_FUNC                        __FUNCTION__
# endif
#endif

#if !defined(C_EXT) || defined(__DOXYGEN__)
# define C_EXT                          __extension__
#endif

#if !defined(C_PACKED) || defined(__DOXYGEN__)
# define C_PACKED                       __attribute__ ((packed))
#endif

#if !defined(C_ALIGNED) || defined(__DOXYGEN__)
# define C_ALIGNED(expr)                __attribute__ ((aligned (expr)))
#endif

#if !defined(C_COMPILE_BARRIER) || defined(__DOXYGEN__)
# define C_COMPILE_BARRIER()            asm volatile ("" : : : "memory")
#endif

#if !defined(C_NORETURN) || defined(__DOXYGEN__)
# define C_NORETURN                     __attribute__((__noreturn__))
#endif

#if !defined(C_UNUSED_ARG) || defined(__DOXYGEN__)
# define C_UNUSED_ARG(type,arg)         __attribute__((__unused__)) type arg
#endif

#if !defined(C_UNUSED_VAR) || defined(__DOXYGEN__)
# define C_UNUSED_VAR(type,name)        __attribute__((__unused__)) type name
#endif

#if !defined(C_USED_VAR) || defined(__DOXYGEN__)
# define C_USED_VAR(type,name)          __attribute__((__used__)) type name
#endif

#if !defined(C_INLINE) || defined(__DOXYGEN__)
# define C_INLINE                       static inline __attribute__((__always_inline__))
#endif

#if !defined(C_NOINLINE) || defined(__DOXYGEN__)
# define C_NOINLINE                     __attribute__((noinline))
#endif

#if !defined(C_PURE_FUNC) || defined(__DOXYGEN__)
# define C_PURE_FUNC                    __attribute__((pure))
#endif

#if !defined(C_CONST_FUNC) || defined(__DOXYGEN__)
# define C_CONST_FUNC                   __attribute__((const))
#endif

#if !defined(C_UNUSED_FUNC) || defined(__DOXYGEN__)
# define C_UNUSED_FUNC                  __attribute__((unused))
#endif

#if !defined(C_USED_FUNC) || defined(__DOXYGEN__)
# define C_USED_FUNC                    __attribute__((__used__))
#endif

#if !defined(C_RESTRICT) || defined(__DOXYGEN__)
# define C_RESTRICT                     __restrict__
#endif

#if !defined(C_MUST_CHECK) || defined(__DOXYGEN__)
# define C_MUST_CHECK                   __attribute__((warn_unused_result))
#endif

#if !defined(C_NAKED) || defined(__DOXYGEN__)
# define C_NAKED                        __attribute__((naked))
#endif

#if !defined(C_PTRCAST) || defined(__DOXYGEN__)
# define C_PTRCAST(srctype, dsttype, var)                                       \
    C_EXT (((union {srctype src; dsttype dst;} __attribute__ ((__may_alias__)) *)var)->dst)
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Ovaj makro ima svojstva funkcije i zasticen je od bocnih efekata.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(ES_ABS) || defined(__DOXYGEN__)
# define ES_ABS(expr)                                                           \
    C_EXT ({                                                                    \
        typeof(expr) expr_ = (expr);                                            \
        (expr_ < 0) ? -expr_ : expr_;                                           \
    })
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Ovaj makro ima svojstva funkcije i zasticen je od bocnih efekata.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(ES_MAX) || defined(__DOXYGEN__)
# define ES_MAX(expr1, expr2)                                                   \
    C_EXT ({                                                                    \
        typeof(expr1) expr1_ = (expr1);                                         \
        typeof(expr2) expr2_ = (expr2);                                         \
        ((typeof(expr1_))((expr1_ > expr2_) ? expr1_ : expr2_));                \
    })
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Ovaj makro ima svojstva funkcije i zasticen je od bocnih efekata.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(ES_MIN) || defined(__DOXYGEN__)
# define ES_MIN(expr1, expr2)                                                   \
    C_EXT ({                                                                    \
        typeof(expr1) expr1_ = (expr1);                                         \
        typeof(expr2) expr2_ = (expr2);                                         \
        ((typeof(expr1_))((expr1_ < expr2_) ? expr1_ : expr2_));                \
    })
#endif

/** @} *//*--------------------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------------------------*
 * C/C++ #ifdef - open
 *-----------------------------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif


/*************************************************************************************************
 * DATA TYPES
 *************************************************************************************************/

/*-------------------------------------------------------------------------------------------*//**
 * @name        Tipovi podataka za GCC kompajler
 * @{ *//*---------------------------------------------------------------------------------------*/

#if !defined(HAL_STD_TYPES) || defined(__DOXYGEN__)
# include <stdint.h>
# define HAL_STD_TYPES
#endif

#if !defined(HAL_BOOL_TYPE) || defined(__DOXYGEN__)
# include <stdbool.h>
# define HAL_BOOL_TYPE
typedef enum {
    FALSE = false,
    TRUE = true
} bool_T;
#endif

#if !defined(HAL_FAST_TYPES) || defined(__DOXYGEN__)
# include <stdint.h>
#define HAL_FAST_TYPES
#endif

#if !defined(HAL_DEF_TYPES) || defined(__DOXYGEN__)
# include <stddef.h>
# define HAL_DEF_TYPES
#endif

#if !defined(HAL_VARARG_SUPP) || defined(__DOXYGEN__)
# include <stdarg.h>
# define HAL_VARARG_SUPP
#endif

/** @} *//*--------------------------------------------------------------------------------------*/


/*************************************************************************************************
 * GLOBAL VARIABLES
 *************************************************************************************************/


/*************************************************************************************************
 * FUNCTION PROTOTYPES
 *************************************************************************************************/


/*-----------------------------------------------------------------------------------------------*
 * C/C++ #endif - close
 *-----------------------------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif


/*************************************************************************************************
 * CONFIGURATION ERRORS
 *************************************************************************************//** @cond */


/** @endcond *//** @} *//*************************************************************************
 * END of HEADER FILE
 *************************************************************************************************/
#endif /* CMN_COMPILER_H_ */
