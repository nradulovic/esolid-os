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
 * ----------------------------------------------------------------------------
 *
 * Project settings:
 * - include paths:
 *      1. ./esolid-os/inc
 * - source paths:
 *      1. ./esolid-hal, exclude: doc/
 * - references
 *      1. esolid-hal
 * - exported include paths:
 *      1. ./esolid-os/inc
 * - exported lib include paths:
 *      1. ./esolid-os/<profile name>
 * - exported lib:
 *      1. esolid-os
 *//***********************************************************************//**
 * @file
 * @author      Nenad Radulovic
 * @brief       Interfejs eSolid operativnog sistema.
 * @addtogroup  kernel_intf
 *********************************************************************//** @{ */
/*
 * Include paths:
 * 		- esolid-os/inc
 *
 * Source paths:
 * 		- esolid-os/src
 *
 * Dependencies:
 * 		- CMSIS/trunk v3.0.1
 * 		- esolid-hal/tags/formula_display
 *
 * Exported unclude paths:
 * 		- esolid-os/inc
 *
 * Exported library:
 * 		- esolid-os
 *
 * Exported library paths:
 * 		- <configuration_name>
 *
 * Compiler:
 * 		- gcc version 4.7.2 (Sourcery CodeBench Lite 2012.09-63)
 * 		- optimization flags: -01 -fomit-frame-pointer
 */

#ifndef KERNEL_H_
#define KERNEL_H_

/*=========================================================  INCLUDE FILES  ==*/
#include "kernel/core.h"

/*===============================================================  MACRO's  ==*/

/**@brief       Znakovna konstanta koja pokazuje puno ime i verziju eSolid-a.
 */
#define ES_KERNEL_ID                    "eSolid v1.0"

/**@brief       Numericka vrednost major verzije eSolid-a.
 */
#define ES_KERNEL_VERSION_MAJOR         1U

/**@brief       Numericka vrednost minor verzije eSolid-a.
 */
#define ES_KERNEL_VERSION_MINOR         0U

/*------------------------------------------------------------------------*//**
 * @name        Error checking
 * @brief       Some basic infrastructure for error checking
 * @details     These macros provide basic detection of errors. For more
 *              datails see @ref errors.
 * @{ *//*--------------------------------------------------------------------*/

#if defined(OPT_KERN_API_VALIDATION) || defined(__DOXYGEN__)

/**@brief       Generic assert macro
 * @param       num
 *              Error number
 * @param       expr
 *              Expression which must be TRUE
 */
# define ES_KERN_ASSERT(num, expr)                                              \
    do {                                                                        \
        if (!(expr)) {                                                          \
            userAssert(C_FUNC, #num, #expr);                                    \
        }                                                                       \
    } while (0U)

/**@brief       Execute code to fulfill the contract
 * @param       expr
 *              Expression to be executed only if contracts need to be validated.
 */
# define ES_KERN_API_OBLIGATION(expr)                                           \
    expr

/**@brief       Make sure the caller has fulfilled all contract preconditions
 * @param       expr
 *              Expression which must be satisfied
 */
# define ES_KERN_API_REQUIRE(num, expr)                                         \
    ES_KERN_ASSERT(num, expr)

/**@brief       Make sure the callee has fulfilled all contract postconditions
 * @param       expr
 *              Expression which must be satisfied
 */
# define ES_KERN_API_ENSURE(num, expr)                                          \
    ES_KERN_ASSERT(num, expr)

#else
# define ES_KERN_ASSERT(num, expr)  	(void)0
# define ES_KERN_API_OBLIGATION(expr)   (void)0
# define ES_KERN_API_REQUIRE(num, expr)	(void)0
# define ES_KERN_API_ENSURE(num, expr) 	(void)0
#endif

/** @} *//*---------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/**@brief       Listing kodova gresaka koje se generisu kada je assert sistem
 *              ukljucen.
 */
enum esKernelStatus {
    ES_KERN_NO_ERROR,

/**@brief       Argument nije ispravan, pokazivac pokazuje na nesto drugo od
 *              ocekivanog.
 */
    ES_KERN_ARG_NOT_VALID = 0x100UL,

/**@brief       Vrednost argumenta je van opsega dozvoljenih vrednosti.
 */
    ES_KERN_ARG_OUT_OF_RANGE,

/**@brief       Pokazivac je NULL, a ocekuje se konkretna vrednost.
 */
    ES_KERN_ARG_NULL,

/**@brief       Nema dovoljno memorije za alociranje.
 */
    ES_KERN_NOT_ENOUGH_MEM,

/**@brief       Funkcija/objekat se koristi na nepravilan nacin
 */
    ES_KERN_USAGE_FAILURE
};

/**@brief       Stanje kernel-a
 */
typedef enum esKernelState {
/**@brief       Kernel se ne izvrsava
 */
    KERNEL_STOPPED,

/**@brief       Kernel se izvrsava
 */
    KERNEL_RUNNING
} esKernelState_T;

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Osnovne funkcije kernel-a
 * @brief       Ove funkcije se koriste za upravljanjem kernel-om
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Inicijalizacija kernel-a
 * @details     Ova funkcija vrsi najpre inicijalizaciju HAL-a, zatim
 *              memorijskog menadzera i prelazi na inicijalizaciju samog kernela.
 * @api
 */
void esKernelInit(
    void);

/**@brief       Pokrece izvrsavanje jezgra
 * @details     Pokrecu se svi prethodno kreirani EPA objekti.
 * @api
 */
void esKernelStart(
    void);

/**@brief       Vraca odgovor da li trenutno radi kernel
 * @return      Status izvrsavanja kernel-a.
 *  @retval     KERNEL_STOPPED - kernel se ne izvrsava,
 *  @retval     KERNEL_RUNNING - kernel se izvrsava,
 * @api
 */
esKernelState_T esKernelStatus(
    void);

/** @} *//*-------------------------------------------------------------------*/

/**@brief       An assertion has failed. This function should inform the user
 *              about failed assertion.
 * @param       fnName
 *              Function name: is pointer to the function name string where the
 *              assertion has failed. Macro will automatically fill in the
 *              function name.
 * @param       errNum
 *              Error number: is predefined number of error that just occured.
 * @param       errExpr
 *              Error Expression: is a pointer to the string containing the
 *              expression that failed to evaluate to `TRUE`.
 * @pre         1) `NULL != fnName`
 * @pre         2) `NULL != errNum`
 * @pre         3) `NULL != errExpr`
 * @note        1) The definition of this function must be written by the user.
 * @note        2) This function is called only if @ref CFG_HALAPI_VALIDATION
 *              option is active.
 * @details     Function will just print the information which was given by
 *              the macros. After the function informs the user it **must** go
 *              into infinite loop or HALT the processor.
 */
C_NORETURN extern void userAssert(
    const char *    fnName,
    const char *    errNum,
    const char *    errExpr);

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of kernel.h
 ******************************************************************************/
#endif /* KERNEL_H_ */
