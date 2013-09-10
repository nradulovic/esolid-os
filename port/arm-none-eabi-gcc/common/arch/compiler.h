/******************************************************************************
 * This file is part of esolid-rtos
 *
 * Copyright (C) 2011, 2012 - Nenad Radulovic
 *
 * esolid-rtos is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * esolid-rtos is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with esolid-rtos; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301  USA
 *
 * web site:    http://blueskynet.dyndns-server.com
 * e-mail  :    blueskyniss@gmail.com
 *//***********************************************************************//**
 * @file
 * @author  	Nenad Radulovic
 * @brief       Interface of arm-none-eabi-gcc.
 * @addtogroup  arm-none-eabi-gcc-v7-m_impl
 *********************************************************************//** @{ */

#ifndef COMPILER_H_
#define COMPILER_H_

/*=========================================================  INCLUDE FILES  ==*/

#include <stddef.h>
#include <stdint.h>

/*===============================================================  MACRO's  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Compiler provided macros
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       C extension - make a function inline
 */
#define PORT_C_INLINE                   __inline__

/**@brief       C extension - make a function inline - always
 */
#define PORT_C_INLINE_ALWAYS            __inline__ __attribute__((__always_inline__))

/**@brief       Omit function prologue/epilogue sequences
 */
#define PORT_C_NAKED                    __attribute__((naked))

#if (__STDC_VERSION__ >= 199901L) || defined(__DOXYGEN__)

/**@brief       Provides function name for assert macros
 */
# define PORT_C_FUNC                    __func__
#elif (__GNUC__ >= 2)
# define PORT_C_FUNC                    __FUNCTION__
#else
/**@brief       Provides function name for assert macros
 */
# define PORT_C_FUNC                    "unknown"
#endif

/**@brief       Provides currently compiled file name
 */
#define PORT_C_FILE                     __FILE__

/**@brief       Provides source line number
 */
#define PORT_C_LINE                     __LINE__

/**@brief       Declare a weak function
 */
#define PORT_C_WEAK                     __attribute__((weak))

/**@brief       Declare a function that will never return
 */
#define PORT_C_NORETURN                 __attribute__((noreturn))

/**@brief       Declare a variable that will be stored in ROM address space
 */
#define PORT_C_ROM

/**@brief       Declare a pointer that will be stored in ROM address space
 */
#define PORT_C_ROM_VAR

/**@brief       This attribute specifies a minimum alignment (in bytes) for
 *              variables of the specified type.
 */
#define PORT_C_ALIGNED(expr)            __attribute__((aligned (expr)))

/**@brief       A standardized way of properly setting the value of HW register
 * @param       reg
 *              Register which will be written to
 * @param       mask
 *              The bit mask which will be applied to register and @c val
 *              argument
 * @param       val
 *              Value to be written into the register
 */
#define PORT_HWREG_SET(reg, mask, val)                                          \
    do {                                                                        \
        portReg_T tmp;                                                          \
        tmp = (reg);                                                            \
        tmp &= ~(mask);                                                         \
        tmp |= ((mask) & (val));                                                \
        (reg) = tmp;                                                            \
    } while (0U)

/** @} *//*---------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Compiler provided data types
 * @brief       All required data types are found in @c stdint.h and @c stddef.h
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Bool data type
 */
typedef enum boolType {
    TRUE = 1U,                                                                  /**< TRUE                                                   */
    FALSE = 0U                                                                  /**< FALSE                                                  */
} bool_T;

/** @} *//*-------------------------------------------------------------------*/

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/
/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of compiler.h
 ******************************************************************************/
#endif /* COMPILER_H_ */
