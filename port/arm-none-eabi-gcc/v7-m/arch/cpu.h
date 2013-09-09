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
 * @author  	Nenad Radulovic
 * @brief       Interface of ARM Cortex-M3 cpu port
 * @addtogroup  arm-none-eabi-gcc-cm3
 * @brief       Interface of ARM Cortex-M3 cpu port
 *********************************************************************//** @{ */

#if !defined(CPU_H_)
#define CPU_H_

/*=========================================================  INCLUDE FILES  ==*/

#include "cpu_cfg.h"

/*===============================================================  MACRO's  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Port constants
 * @{ *//*--------------------------------------------------------------------*/

#define PORT_DATA_WIDTH                 32U                                     /**< @brief General purpose registers are 32bit wide        */

#define PORT_DATA_ALIGNMENT             4U                                      /**< @brief Data is aligned to 4 bytes boundary             */

#define PORT_STCK_MINSIZE               sizeof(struct portCtx)                  /**< @brief Minimal stack size value                        */

/**@} *//*----------------------------------------------------------------*//**
 * @name        Interrupt management
 * @{ *//*--------------------------------------------------------------------*/

#define PORT_INT_DISABLE()              portIntDisable_()

#define PORT_ISR_ENTER()                esKernIsrPrologueI()                    /**< @brief This port does not need this function call      */

#define PORT_ISR_EXIT()                 esKernIsrEpilogueI()                    /**< @brief This port does not need this function call      */

#define PORT_ISR_IS_LAST()              portIsrIsLast_()

/**@} *//*----------------------------------------------------------------*//**
 * @name        Critical section management
 * @{ *//*--------------------------------------------------------------------*/

#define PORT_CRITICAL_DECL              portReg_T intStatus_                    /**< @brief Critical section interrupt status holder        */

#define PORT_CRITICAL_ENTER()                                                   \
    do {                                                                        \
        intStatus_ = portIntGetSet_();                                          \
    } while (0U);

#define PORT_CRITICAL_EXIT()            portIntSet_(intStatus_)

/**@} *//*----------------------------------------------------------------*//**
 * @name        Scheduler support
 * @{ *//*--------------------------------------------------------------------*/

#define PORT_FIND_LAST_SET(val)         portFindLastSet_(val)

#define PORT_SYSTMR_INIT()              portSysTmrInit_()

#define PORT_SYSTMR_ISR_ENABLE()        portSysTmrEnable_()

#define PORT_SYSTMR_ISR_DISABLE()       portSysTmrDisable_()

/**@} *//*----------------------------------------------------------------*//**
 * @name        Dispatcher context switching
 * @{ *//*--------------------------------------------------------------------*/

#define PORT_CTX_INIT(stack, stackSize, thread, arg)                            \
    portCtxInit_(stack, stackSize, thread, arg)

#define PORT_CTX_SW()                   portCtxSw_()

#define PORT_CTX_SW_ISR()               PORT_CTX_SW()                           /**< @brief This port has identical context switch functions*/

#define PORT_THD_START()                portThdStart_()

/**@} *//*----------------------------------------------------------------*//**
 * @name        Generic port macros
 * @{ *//*--------------------------------------------------------------------*/

#define PORT_INIT_EARLY()               (void)0                                 /**< @brief This port does not need this function call      */

#define PORT_INIT()                     portInit_()

#define PORT_INIT_LATE()                (void)0                                 /**< @brief This port does not need this function call      */

#define PORT_TERM()                     while (TRUE)

/**@} *//*----------------------------------------------------------------*//**
 * @name        Port specific macros
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       This is the data that will be placed on task context at its
 *              creation
 * @details     This macro can be used if you need to specify different settings
 *              for Interruptible-continuable instructions. The setting is done
 *              in PSR register.
 */
#define CPU_PSR_DATA                    0UL

/**@brief       Calculate interrupt priority real value
 */
#define CPU_ISR_PRIO                    (CFG_CRITICAL_PRIO << (8 - CPU_ISR_PRIO_BITS))

/**@brief       This field determines the split of group priority from
 *              subpriority.
 * @warning     Change this value only if you are familiar with Cortex interrupt
 *              priority system and how kernel protects its critical sections.
 */
#define CPU_SCB_AIRCR_PRIGROUP          0U

#define CPU_SCS_BASE                    (0xE000E000UL)                          /**< @brief System Control Space Base Addr                  */

#define CPU_SCB_BASE                    (CPU_SCS_BASE + 0x0D00UL)               /**< @brief System Control Block Base Addr                  */
#define CPU_SCB_ICSR_BASE               (CPU_SCB_BASE + 0x04UL)                 /**< @brief Interrupt Control and State Register Base Addr  */
#define CPU_SCB_ICSR_PENDSVSET_POS      28                                      /**< @brief SCB icsr: PENDSVSET Position                    */
#define CPU_SCB_ICSR_PENDSVSET_MSK      (1UL << CPU_SCB_ICSR_PENDSVSET_POS)     /**< @brief SCB icsr: PENDSVSET Mask                        */
#define CPU_SCB_ICSR_RETTOBASE_POS      11                                      /**< @brief SCB icsr: RETTOBASE Position                    */
#define CPU_SCB_ICSR_RETTOBASE_MSK      (1UL << CPU_SCB_ICSR_RETTOBASE_POS)     /**< @brief SCB icsr: RETTOBASE Mask                        */

#define CPU_SYST_BASE                   (CPU_SCS_BASE + 0x0010UL)               /**< @brief System Timer Base Addr                          */
#define CPU_SYST_CSR_BASE               (CPU_SYST_BASE + 0x0UL)                 /**< @brief Control and Status Register Base Addr           */
#define CPU_SYST_CSR_TICKINT_POS        1                                       /**< @brief SYST csr: TICKINT Position                      */
#define CPU_SYST_CSR_TICKINT_MSK        (1UL << CPU_SYST_CSR_TICKINT_POS)       /**< @brief SYST csr: TICKINT Mask                          */

/** @} *//*---------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

typedef uint32_t portReg_T;                                                     /**< @brief General purpose registers are 32bit wide.       */

/**@brief       Structure of the context switch
 * @details     There are 16, 32-bit core (integer) registers visible to the ARM
 *              and Thumb instruction sets. These are labeled r0-r15.
 */
struct portCtx {
/* Registers saved by the context switcher                                    */
    portReg_T         r4;                                                       /**< @brief R4, Variable register 1                         */
    portReg_T         r5;                                                       /**< @brief R5, Variable register 2                         */
    portReg_T         r6;                                                       /**< @brief R6, Variable register 3                         */
    portReg_T         r7;                                                       /**< @brief R7, Variable register 4                         */
    portReg_T         r8;                                                       /**< @brief R8, Variable register 5                         */
    portReg_T         r9;                                                       /**< @brief R9, Platform register/variable register 6       */
    portReg_T         r10;                                                      /**< @brief R10, Variable register 7                        */
    portReg_T         r11;                                                      /**< @brief R11, Variable register 8                        */

/* Registers saved by the hardware                                            */
    portReg_T         r0;                                                       /**< @brief R0, Argument/result/scratch register 1          */
    portReg_T         r1;                                                       /**< @brief R1, Argument/result/scratch register 2          */
    portReg_T         r2;                                                       /**< @brief R2, Argument/scratch register 3                 */
    portReg_T         r3;                                                       /**< @brief R3, Argument/scratch register 3                 */
    portReg_T         r12;                                                      /**< @brief R12, IP, The Intra-Procedure-call scratch reg.  */
    portReg_T         lr;                                                       /**< @brief R14, LR, The Link Register                      */
    portReg_T         pc;                                                       /**< @brief R15, PC, The Program Counter                    */
    portReg_T         xpsr;                                                     /**< @brief Special, Program Status Register                */
};

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Interrupt management
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Disable interrupts
 * @inline
 */
static PORT_C_INLINE_ALWAYS void portIntDisable_(
    void) {

    __asm volatile (
        "   cpsid   i                                       \n\t");
}

/**@brief       Set the interrupt priority mask
 * @param       val
 *              New interrupt priority mask
 * @inline
 */
static PORT_C_INLINE_ALWAYS void portIntSet_(
    portReg_T     val) {

#if (0 != CFG_CRITICAL_PRIO)
    __asm volatile (
        "   msr    basepri, %0                              \n\t"
        :
        : "r"(val));
#else
    __asm volatile (
        "   msr    primask, %0                              \n\t"
        :
        : "r"(val));
#endif
}

/**@brief       Get current and set new interrupt priority mask
 * @return      Current interrupt priority mask
 * @inline
 */
static PORT_C_INLINE_ALWAYS portReg_T portIntGetSet_(
    void) {

    portReg_T         result;

#if (0 != CFG_CRITICAL_PRIO)
    portReg_T         val;

    val = CPU_ISR_PRIO;
    __asm volatile (
        "   mrs     %0, basepri                             \n\t"
        "   msr     basepri, %1                             \n\t"
        : "=r"(result)
        : "r"(val));
#else
    __asm volatile (
        "   mrs     %0, primask                             \n\t"
        "   cpsid   i                                       \n\t"
        : "=r"(result));
#endif

    return (result);
}

/**@brief       Check if this is the last ISR executing
 * @return      Is the currently executed ISR the last one?
 *  @retval     TRUE - this is last ISR
 *  @retval     FALSE - this is not the last ISR
 * @inline
 */
static PORT_C_INLINE_ALWAYS bool_T portIsrIsLast_(
    void) {

    portReg_T *       icsr;
    bool_T          ans;

    icsr = (portReg_T *)CPU_SCB_ICSR_BASE;

    if (0U != (*icsr & CPU_SCB_ICSR_RETTOBASE_MSK)) {
        ans = TRUE;
    } else {
        ans = FALSE;
    }

    return (ans);
}

/**@} *//*----------------------------------------------------------------*//**
 * @name        Scheduler support
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Find last set bit in a word
 * @param       value
 *              32 bit value which will be evaluated
 * @return      Last set bit in a word
 * @details     This implementation uses @c clz instruction and then it computes
 *              the result using the following expression:
 *              <code>fls(x) = w − clz(x)</code>.
 * @inline
 */
static PORT_C_INLINE_ALWAYS uint_fast8_t portFindLastSet_(
    uint32_t value) {

    uint_fast8_t    clz;

    __asm volatile (
        "   clz    %0, %1                                   \n\t"
        : "=r"(clz)
        : "r"(value));

    return (31UL - clz);
}

/**@brief       Disable the system timer interrupt
 * @inline
 */
static PORT_C_INLINE_ALWAYS void portSysTmrEnable_(
    void) {

    portReg_T * csr;

    csr = (portReg_T *)CPU_SYST_CSR_BASE;
    *csr |= CPU_SYST_CSR_TICKINT_MSK;
}

/**@brief       Enable the system timer interrupt
 * @inline
 */
static PORT_C_INLINE_ALWAYS void portSysTmrDisable_(
    void) {

    portReg_T * csr;

    csr = (portReg_T *)CPU_SYST_CSR_BASE;
    *csr &= ~CPU_SYST_CSR_TICKINT_MSK;
}

/**@brief       Initialize system timer
 */
void portSysTmrInit_(
    void);

/**@brief       Start the first thread
 * @details     This function will set the main stack register to point at the
 *              beginning of stack disregarding all previous stack information
 *              after which it will call service to start the first thread.
 * @warning     This function requires valid Vector Table Offset Register in
 *              System control block.
 */
void portThdStart_(
    void);

/**@} *//*----------------------------------------------------------------*//**
 * @name        Dispatcher context switching
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Do the context switch
 * @details     This function will just initiate PendSV exception which will do
 *              the actual context switch
 * @inline
 */
static PORT_C_INLINE_ALWAYS void portCtxSw_(
    void) {

    portReg_T * icsr;

    icsr = (portReg_T *)CPU_SCB_ICSR_BASE;
    *icsr |= CPU_SCB_ICSR_PENDSVSET_MSK;
}

/**@brief       Initializes the thread context
 * @param       stck
 *              Pointer to the allocated thread stack. The pointer points to the
 *              beginning of the memory as defined per C language. The function
 *              will adjust the pointer according to the stack full descending
 *              type.
 * @param       stckSize
 *              The size of allocated stack in bytes.
 * @param       thdf
 *              Pointer to the thread function.
 * @param       arg
 *              Argument that will be passed to thread function at the starting
 *              of execution.
 * @note        1) Interrupts are enabled when your task starts executing.
 * @note        2) All tasks run in Thread mode, using process stack.
 * @note        3) ARM Cortex M3 requires 8B aligned stack.
 */
void * portCtxInit_(
    void *          stck,
    size_t          stckSize,
    void (* thdf)(void *),
    void *          arg);

/**@} *//*----------------------------------------------------------------*//**
 * @name        Generic port functions
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Initialize port
 * @details     Function will set up sub-priority bits to zero and handlers
 *              interrupt priority.
 */
void portInit_(
    void);

/**@} *//*----------------------------------------------------------------*//**
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Pop the first thread stack
 * @details     During the thread initialization a false stack was created
 *              mimicking the real interrupt stack described in @ref portCtx. With
 *              this function we restore the false stack and start the thread.
 *              This function is invoked only once from esKernStart() function.
 */
PORT_C_NAKED void portSVC(
    void);

/**@brief       Execute context switching
 * @details     PendSV is used to cause a context switch. This is a recommended
 *              method for performing context switches with Cortex-M3. This is
 *              because the Cortex-M3 auto-saves half of the processor context
 *              on any exception, and restores same on return from exception. So
 *              only saving of R4-R11 is required and setting up the stack
 *              pointers. Using the PendSV exception this way means that context
 *              saving and restoring is identical whether it is initiated from a
 *              thread or occurs due to an interrupt or exception.
 */
PORT_C_NAKED void portPendSV(
    void);

/**@brief       System timer event handler
 * @details     System timer events are used by the scheduler
 */
void portSysTmr(
    void);

/** @} *//*-----------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of cpu.h
 ******************************************************************************/
#endif /* CPU_H_ */
