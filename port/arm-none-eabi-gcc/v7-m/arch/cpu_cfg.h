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
 * @brief   	Configuration of cpu_config.
 * @details     Detailed description
 * @addtogroup  arm-none-eabi-gcc-cm3_cfg
 * @brief		Configuration of CPU module.
 * @details     Each configuration option or setting has its own default value
 *              when not defined by the application. When application needs to
 *              change a setting it just needs to define a configuration macro
 *              with another value and the default configuration macro will be
 *              overridden.
 *********************************************************************//** @{ */

#ifndef CPU_CFG_H_
#define CPU_CFG_H_

/*=========================================================  INCLUDE FILES  ==*/
/*===============================================================  DEFINES  ==*/
/** @cond */

#define CPU_STM32F10X
#define CFG_SYSTMR_CLOCK_FREQUENCY      8000000UL

/** @endcond */
/*==============================================================  SETTINGS  ==*/

/*------------------------------------------------------------------------*//**
 * @name        ST Microelectronics STM32F10x
 * @{ *//*--------------------------------------------------------------------*/
#if defined(CPU_STM32F10X) || defined(__DOXYGEN__)

#define CPU_FOUND_                                                              /**< @brief Note that a port is found.                      */

/**@brief       Priority of critical sections in kernel
 * @details     Specify the priority in range: <code>0</code>\f$\leq\f$
 *              <code>CFG_CRITICAL_PRIO</code> \f$\leq\f$ <code>15</code>. The
 *              lower the number the higher the priority.
 * @note        When priority is set to @b 0 then critical section will not use
 *              priority levels bit it will just disable interrupts on entry and
 *              enable interrupts on exit.
 */
#if !defined(CFG_CRITICAL_PRIO) || defined(__DOXYGEN__)
# define CFG_CRITICAL_PRIO              7U
#endif

/**@brief       The frequency of clock which is used for the system timer
 * @details     System timer SysTick uses core clock (sometimes referred to as
 *              HCLK) for counting. Specify here the core clock so the OS can
 *              properly manage system tick event generation.
 */
#if !defined(CFG_SYSTMR_CLOCK_FREQUENCY) || defined(__DOXYGEN__)
# define CFG_SYSTMR_CLOCK_FREQUENCY     24000000UL
#endif

/**@brief       Port constant: interrupt priority bits implemented in MCU
 */
# define CPU_ISR_PRIO_BITS              4U

#endif

/** @} *//*---------------------------------------------------------------*//**
 * @name        Exception handlers used by the port
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       PendSV exception handler
 */
#if !defined(portPendSV) || defined(__DOXYGEN__)
# define portPendSV                     PendSV_Handler
#endif

/**@brief       SVC exception handler
 */
#if !defined(portSVC) || defined(__DOXYGEN__)
# define portSVC                        SVC_Handler
#endif

/**@brief       SysTick exception handler
 */
#if !defined(portSysTmr) || defined(__DOXYGEN__)
# define portSysTmr                    SysTick_Handler
#endif

/** @} *//*-------------------------------------------------------------------*/
/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/

#if !defined(CPU_FOUND_)
# error "Kernel CPU port: please define a valid port macro."
#endif

/** @endcond *//** @} *//******************************************************
 * END of cpu_cfg.h
 ******************************************************************************/
#endif /* CPU_CFG_H_ */
