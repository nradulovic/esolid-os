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
 * @brief       Privatni objekti kernel-a.
 * @addtogroup  kernel_impl
 ****************************************************************************************//** @{ */

#ifndef KERNEL_PRIVATE_H_
#define KERNEL_PRIVATE_H_

/*============================================================================  INCLUDE FILES  ==*/
#include "kernel/kernel.h"
#include "stp/stp.h"
#include "stp_messages.h"
#include "mm_pkg.h"
#include "evt_pkg.h"
#include "smp_pkg.h"
#include "core_pkg.h"

/*==================================================================================  DEFINES  ==*/
/*==================================================================================  MACRO's  ==*/

/*-------------------------------------------------------------------------------------------*//**
 * @name        Debug podrska
 * @brief       Makroi za debug podrsku. Pogledati @ref dbg_intf.
 * @{ *//*---------------------------------------------------------------------------------------*/

#if defined(OPT_KERNEL_DBG) || defined(__DOXYGEN__)
# define KERN_ASSERT                  DBG_ASSERT
# define KERN_ASSERT_ALWAYS           DBG_ASSERT_ALWAYS
# define KERN_COMPILE_ASSERT          DBG_COMPILE_ASSERT
# define KERN_DBG_DECL                DBG_DECL
# define KERN_DBG_DEFINE_MODULE       DBG_DEFINE_MODULE
# define KERN_DBG_ENTRY               DBG_ENTRY
# define KERN_DBG_EXIT                DBG_EXIT
# define KERN_DBG_MACRO               DBG_MACRO
# define KERN_DBG_CHECK               DBG_CHECK
#else
# define KERN_ASSERT(expr)            DBG_EMPTY_MACRO()
# define KERN_ASSERT_ALWAYS(expr)     DBG_EMPTY_MACRO()
# define KERN_COMPILE_ASSERT(expr)    DBG_EMPTY_DECL()
# define KERN_DBG_DECL(expr)          DBG_EMPTY_DECL()
# define KERN_DBG_DEFINE_MODULE(expr) DBG_EMPTY_DECL()
# define KERN_DBG_ENTRY()             DBG_EMPTY_MACRO()
# define KERN_DBG_EXIT()              DBG_EMPTY_MACRO()
# define KERN_DBG_MACRO(expr)         DBG_EMPTY_MACRO()
# define KERN_DBG_CHECK(expr)         DBG_EMPTY_MACRO()
#endif

/** @} *//*--------------------------------------------------------------------------------------*/

/*===============================================================================  DATA TYPES  ==*/
/*=========================================================================  GLOBAL VARIABLES  ==*/
/*======================================================================  FUNCTION PROTOTYPES  ==*/
/*===================================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/

/** @endcond *//** @} *//*************************************************************************
 * END of kernel_private.h
 *************************************************************************************************/
#endif /* KERNEL_PRIVATE_H_ */
