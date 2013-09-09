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
 * @brief   	Configuration of Kernel - Template.
 * @addtogroup  template_kern_cfg
 * @details     Each configuration option or setting has its own default value
 *              when not defined by the application. When application needs to
 *              change a setting it just needs to define a configuration macro
 *              with another value and the default configuration macro will be
 *              overridden.
 *********************************************************************//** @{ */

#ifndef KERNEL_CONFIG_H_
#define KERNEL_CONFIG_H_

/*=========================================================  INCLUDE FILES  ==*/
/*===============================================================  DEFINES  ==*/
/** @cond */

/** @endcond */
/*==============================================================  SETTINGS  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Kernel configuration options and settings
 * @brief       Kernel default configuration
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Enable/disable API arguments validation
 * @details     During the development cycle of the application this option
 *              should be turned on. When this configuration option is turned on
 *              the kernel API functions will also check arguments passed to
 *              them. If an invalid argument is detected the execution of the
 *              application will stop and the user will be informed about the
 *              error condition.
 * @note        The error checking use userAssert() hook function to provide the
 *              information about the error condition.
 */
#if !defined(CFG_API_VALIDATION)
# define CFG_API_VALIDATION             1U
#endif

/**@brief       Scheduler priority groups
 */
#if !defined(CFG_SCHED_PRIO_GROUPS)
# define CFG_SCHED_PRIO_GROUPS          8U
#endif

/**@brief       Scheduler Round-Robin time quantum
 * @details     This constant is the number of system ticks allowed for the
 *              threads before preemption occurs. Setting this value to zero
 *              disables the preemption for threads with equal priority and the
 *              round robin becomes cooperative. Note that higher priority
 *              threads can still preempt, the kernel is always preemptive.
 * @note        Disabling the round robin preemption makes the kernel more
 *              compact and generally faster.
 */
#if !defined(CFG_SCHED_TIME_QUANTUM)
# define CFG_SCHED_TIME_QUANTUM         10U
#endif

/**@brief       The frequency of system tick event
 * @details     Specify the desired resolution system tick time source. This
 *              setting is valid only if setting @ref CFG_SYSTMR_CLOCK_FREQUENCY
 *              is properly configured in port configuration file cpu_cfg.h
 */
#if !defined(CFG_SYSTMR_EVENT_FREQUENCY)
# define CFG_SYSTMR_EVENT_FREQUENCY     100UL
#endif

/** @} *//*---------------------------------------------------------------*//**
 * @name        Kernel hooks
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       System event hook function
 * @details     This hook is called just a moment before a system timer event is
 *              processed.
 */
#if !defined(CFG_HOOK_SYSTMR_EVENT)
# define CFG_HOOK_SYSTMR_EVENT          0U
#endif

/**@brief       Kernel initialization hook function
 * @details     This hook is called at the beginning of esKernInit() function.
 */
#if !defined(CFG_HOOK_KERN_INIT)
# define CFG_HOOK_KERN_INIT             0U
#endif

/**@brief       Kernel start hook function
 * @details     This hook is called at the beginning of esKernStart() function.
 */
#if !defined(CFG_HOOK_KERN_START)
# define CFG_HOOK_KERN_START            0U
#endif

/**@brief       Kernel thread init hook function
 * @details     This hook is called at the end of esThdInit() function.
 */
#if !defined(CFG_HOOK_THD_INIT_END)
# define CFG_HOOK_THD_INIT_END          0U
#endif

/**@brief       Kernel context switch hook function
 * @details     This hook is called at each context switch.
 */
#if !defined(CFG_HOOK_CTX_SW)
# define CFG_HOOK_CTX_SW                0U
#endif

/** @} *//*-------------------------------------------------------------------*/
/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of kernel_config.h
 ******************************************************************************/
#endif /* KERNEL_CONFIG_H_ */
