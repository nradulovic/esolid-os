/*************************************************************************************************
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
 *//******************************************************************************************//**
 * @file
 * @author  	Nenad Radulovic
 * @brief       Ucitavanje svih Kernel profila
 * @details     Ova datoteka ucitava sve podrzane profile kernel-a.
 * ------------------------------------------------------------------------------------------------
 * @addtogroup  kernelport_intf
 ****************************************************************************************//** @{ */


#ifndef KERNEL_PROFILES_H_
#define KERNEL_PROFILES_H_

/*============================================================================  INCLUDE FILES  ==*/

/*===============================================================================================*
 * Architecture: ARM Cortex-M3
 *===============================================================================================*/
#if defined(PORT_ARCH_ARM_CM3)
# define KERNEL_PORT_FOUND_
# include "port/arch/arm_cm3_kernel.h"
#endif

/*===============================================================================================*
 * Architecture: ARM Cortex-M4F
 *===============================================================================================*/
#if defined(PORT_ARCH_ARM_CM4)
# define KERNEL_PORT_FOUND_
# include "port/arch/arm_cm4_kernel.h"
#endif

/*==================================================================================  DEFINES  ==*/
/*==================================================================================  MACRO's  ==*/
/*===============================================================================  DATA TYPES  ==*/
/*=========================================================================  GLOBAL VARIABLES  ==*/
/*======================================================================  FUNCTION PROTOTYPES  ==*/
/*===================================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/

#if !defined(KERNEL_PORT_FOUND_)
# error "Kernel Profiles: Cannot determine MCU architecture. Kernel cannot be compiled."
#endif

/** @endcond *//** @} *//*************************************************************************
 * END of kernel_profiles.h
 *************************************************************************************************/
#endif /* KERNEL_PROFILES_H_ */
