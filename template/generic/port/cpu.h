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
 * @brief       Interface of CPU port - Template
 * @addtogroup  template_cpu_intf
 * @details     Since this header file is included with the API of the kernel a
 *              few naming conventions are defined in order to avoid name
 *              clashing with the names of objects from libraries included by
 *              application code.
 *
 * @par         1) Macro naming conventions
 *              For macro naming try to follow these rules:
 *              - All standard PORT API macro names are prefixed with:
 *              @b @c PORT_.
 *              - All other macros which are specific to the port used are
 *              prefixed with: @b @c CPU_.
 *
 * @par         2) Type declaration naming conventions
 *              For type declaration naming try to follow these rules:
 *              - All type declaration names are prefixed with: @b @c cpu.
 *
 * @par         3) Global variable naming conventions
 *              For global variable naming try to follow these rules:
 *              - All global variable names are prefixed with: @b @c cpu.
 *
 * @par         4) Funcion naming convetions
 *              For functions naming try to follow these rules:
 *              - All function names are prefixed with: <b><code>port</code></b>
 *              and postfixed with: @b @c _ (underscore).
 *              - All other functions which are specific to the port used are
 *              prefixed with: <b><code>cpu</code></b> and postfixed with:
 *              @b @c _ (underscore).
 *              - The @c exception to above two rules are the names of functions
 *              used for Interrupt Service Routines. They can have any name
 *              required by port.
 *
 *********************************************************************//** @{ */

#ifndef CPU_H_
#define CPU_H_

/*=========================================================  INCLUDE FILES  ==*/
#include "cpu_cfg.h"

/*===============================================================  MACRO's  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Port constants
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       This macro specifies the bit width of CPU data registers
 */
#define PORT_DATA_WIDTH                 8U

/**@brief       This macro specifies the minimal size of the thread stack
 * @details     Generally minimal stack size is equal to the size of context
 *              structure
 */
#define PORT_STCK_MINSIZE               sizeof(struct portCtx)

/** @} *//*---------------------------------------------------------------*//**
 * @name        Interrupt management
 * @details     PORT_ISR_... macros are used by esKernIsrEnter() and
 *              esKernIsrExit() functions. They are used to keep the current
 *              level of ISR nesting. Scheduler should be invoked only from the
 *              last ISR that is executing.
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Disable all interrupt sources
 */
#define PORT_INT_DISABLE()              portIntDisable_()

/**@brief       Enter ISR. Increment isrNesting variable to keep track of ISR
 *              nesting.
 */
#define PORT_ISR_ENTER()                gPortIsrNesting_++

/**@brief       Exit ISR. Decrement isrNesting variable to keep track of ISR
 *              nesting.
 */
#define PORT_ISR_EXIT()                 gPortIsrNesting_--

/**@brief       If isrNesting variable is zero then the last ISR is executing
 *              and scheduler should be invoked
 * @return      Is the currently executed ISR the last one?
 *  @retval     TRUE - this is last ISR
 *  @retval     FALSE - this is not the last ISR
 */
#define PORT_ISR_IS_LAST()              (0U == gPortIsrNesting ? TRUE : FALSE)

/** @} *//*---------------------------------------------------------------*//**
 * @name        Critical section management
 * @brief       Disable/enable interrupts by preserving the status of interrupts.
 * @details     Generally speaking these macros would store the status of the
 *              interrupt disable flag in the local variable declared by
 *              @ref PORT_CRITICAL_DECL and then disable interrupts. Local
 *              variable is allocated in all of eSolid RTOS functions that need
 *              to disable interrupts.  Macros would restore the interrupt
 *              status by copying back the allocated variable into the CPU's
 *              status register.
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Declare the interrupt status variable
 */
#define PORT_CRITICAL_DECL()            portReg_T intStatus_

/**@brief       Enter critical section
 */
#define PORT_CRITICAL_ENTER()                                                   \
    do {                                                                        \
        intStatus_ = portIntGet_();                                             \
        portIntDisable_();                                                      \
    } while (0U)

/**@brief       Exit critical section
 */
#define PORT_CRITICAL_EXIT()            portIntSet_(intStatus_)

/** @} *//*---------------------------------------------------------------*//**
 * @name        Scheduler support
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Find last set bit in a word
 * @details     This function is used by the scheduler to efficiently determine
 *              the highest priority of thread ready for execution. For
 *              algorithm details see:
 *              http://en.wikipedia.org/wiki/Find_first_set.
 * @return      The position of the last set bit in a word
 */
#define PORT_FIND_LAST_SET(val)         portFindLastSet_(val)

/**@brief       Initialize systick timer and associated interrupt
 */
#define PORT_SYSTMR_INIT()              portSysTmrInit_()

/**@brief       Enable the systick interrupt
 * @note        In some cases this macro will also see if kernel is running
 *              before actually enabling the systick
 */
#define PORT_SYSTMR_ENABLE()            portSysTmrEnable_()

/**@brief       Disable the systick interrupt
 */
#define PORT_SYSTMR_DISABLE()           portSysTmrDisable_()

/** @} *//*---------------------------------------------------------------*//**
 * @name        Dispatcher context switching
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Initialize the thread context
 *
 * @param       [inout] stck
 *              Pointer to the allocated thread stck. The pointer points to the
 *              beginning of the memory as defined per C language. It's up to
 *              port function to adjust the pointer according to the stck type:
 *              full descending or full ascending one.
 *
 * @param       stackSize
 *              The size of allocated stck in bytes.
 *
 * @param       [in] thread
 *              Pointer to the thread function.
 *
 * @param       [in] arg
 *              Argument that will be passed to thread function at the starting
 *              of execution.
 * @return      The new top of stck after thread context initialization.
 */
#define PORT_CTX_INIT(stck, stackSize, thread, arg)                             \
    portCtxInit_(stck, stackSize, thread, arg)

/**@brief       Do the context switch - invoked from API
 */
#define PORT_CTX_SW()                   portCtxSw_()

/**@brief       Do the context switch - invoked from ISR
 */
#define PORT_CTX_SW_ISR()               portCtxSwIsr_()

/**@brief       Start the first thread
 */
#define PORT_THD_START()                portThdStart_()

/** @} *//*---------------------------------------------------------------*//**
 * @name        General port macros
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Early port initialization
 * @details     This macro will be called at early initialization stage from
 *              esKernInit() function. It is called before any kernel data
 *              initialization. Usually this macro would be used to setup
 *              memory space, fill the memory with debug value or something
 *              similar.
 */
#define PORT_INIT_EARLY()               portInitEarly_()

/**@brief       Port initialization
 * @details     This macro will be called after kernel data structure
 *              initialization from esKernInit() function.
 */
#define PORT_INIT()                     portInit_()

/**@brief       Late port initialization
 * @details     This macro will be called just a moment before the multitasking
 *              is started. The macro is called from esKernStart() function.
 */
#define PORT_INIT_LATE()                portInitLate_()

/** @} *//*---------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/**@brief       Data type which corresponds to the general purpose register
 */
typedef uint8_t portReg_T;

/**@brief       Port context structure
 */
struct portCtx {
    portReg_T       r0;                                                         /**< @brief Data pushed on stack during context switching   */
};

/*======================================================  GLOBAL VARIABLES  ==*/

/**@brief       Variable to keep track of ISR nesting.
 */
extern uint8_t portReg_T gPortIsrNesting_;

/*===================================================  FUNCTION PROTOTYPES  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Interrupt management
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Get the current status of enabled/disabled interrupts
 * @return      Interrupt status
 */
esReg_T portIntGet_(
    void);

/**@brief       Disable interrupts
 */
void portIntDisable_(
    void);

/**@brief       Set the status of interrupts according to the @c status argument
 * @param       status
 *              The status of interrupts that will be set by the function.
 */
void portIntSet_(
    esReg_T         status);

/** @} *//*---------------------------------------------------------------*//**
 * @name        Scheduler support
 * @note        These functions are extensively used by the scheduler and
 *              therefore they should be optimized for the architecture being
 *              used.
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Find last set bit in a word
 * @param       val
 *              Value which needs to be evaluated
 * @details     This function is used by the scheduler to efficiently determine
 *              the highest priority of thread ready for execution. For
 *              algorithm details see:
 *              http://en.wikipedia.org/wiki/Find_first_set.
 * @return      The position of the last set bit in a word
 */
uint_fast8_t portFindLastSet_(
    esReg_T     val);

/**@brief       Initialize systick timer and associated interrupt
 * @details     This function will be called just a moment before the
 *              multitasking is started. The function is called from
 *              esKernStart() function. It should setup:
 *              - systick timer (scheduler uses tick event to switch between
 *                  threads of same priority)
 *              - systick timer interrupt
 * @note        This function MUST NOT enable system timer events. System timer
 *              events are enabled/disabled by portSysTmrEnable_() and
 *              portSysTmrDisable_() functions.
 */
void portSysTmrInit_(
    void);

/**@brief       Enable the systick interrupt
 */
void portSysTmrEnable_(
    void);

/**@brief       Disable the systick interrupt
 */
void portSysTmrDisable_(
    void);

/**@brief       Start the first thread
 */
void portThdStart_(
    void);

/** @} *//*---------------------------------------------------------------*//**
 * @name        Dispatcher context switching
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Initialize the thread context
 * @param       [inout] stck
 *              Pointer to the allocated thread stck. The pointer points to the
 *              beginning of the memory as defined per C language. It's up to
 *              port function to adjust the pointer according to the stck type:
 *              full descending or full ascending one.
 * @param       stckSize
 *              The size of allocated stck in bytes.
 * @param       [in] thdf
 *              Pointer to the thread function.
 * @param       [in] arg
 *              Argument that will be passed to thread function at the starting
 *              of execution.
 * @return      The new top of stck after thread context initialization.
 */
void * portCtxInit_(
    void *          stck,
    size_t          stckSize,
    void (* thdf)(void *),
    void *          arg);

/**@brief       Do the context switch - invoked from API
 */
void portCtxSw_(
    void);

/**@brief       Do the context switch - invoked from ISR
 */
void portCtxSwIsr_(
    void);

/** @} *//*---------------------------------------------------------------*//**
 * @name        General port functions
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Early port initialization
 * @details     This function will be called at early initialization stage from
 *              esKernInit() function. It is called before any kernel data
 *              initialization. Usually this function would be used to setup
 *              memory space, fill the memory with debug value or something
 *              similar.
 */
void portInitEarly_(
    void);

/**@brief       Port initialization
 * @details     This function will be called after kernel data structure
 *              initialization from esKernInit() function.
 */
void portInit_(
    void);

/**@brief       Late port initialization
 * @details     This function will be called just a moment before the
 *              multitasking is started. The function is called from
 *              esKernStart() function.
 */
void portInitLate_(
    void);


/** @} *//*---------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of cpu.h
 ******************************************************************************/
#endif /* CPU_H_ */
