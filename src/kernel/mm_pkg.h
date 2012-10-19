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
 * @author      Nenad Radulovic
 * @brief       Privatni interfejs Memory Management podmodula.
 * ------------------------------------------------------------------------------------------------
 * @addtogroup  mm_impl
 ****************************************************************************************//** @{ */


#ifndef MM_PKG_H_
#define MM_PKG_H_

/*============================================================================  INCLUDE FILES  ==*/
/*==================================================================================  DEFINES  ==*/
/*==================================================================================  MACRO's  ==*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Debug podrska
 * @brief       Makroi za debug podrsku
 * @{ *//*---------------------------------------------------------------------------------------*/

#if defined(OPT_KERNEL_DBG_MM) || defined(__DOXYGEN__)
# define MM_ASSERT                      ES_DBG_ASSERT
# define MM_ASSERT_ALWAYS               ES_DBG_ASSERT_ALWAYS
# define MM_ASSERT_COMPILE              ES_DBG_ASSERT_COMPILE
# define MM_DBG_DECL                    ES_DBG_DECL
# define MM_DBG_DEFINE_MODULE           ES_DBG_DEFINE_MODULE
# define MM_DBG_MACRO                   ES_DBG_MACRO
# define MM_DBG_CHECK                   ES_DBG_CHECK
#else
# define MM_ASSERT(expr)                ES_DBG_EMPTY_MACRO()
# define MM_ASSERT_ALWAYS(expr)         ES_DBG_EMPTY_MACRO()
# define MM_ASSERT_COMPILE(expr)        ES_DBG_EMPTY_DECL()
# define MM_DBG_DECL(expr)              ES_DBG_EMPTY_DECL()
# define MM_DBG_DEFINE_MODULE(expr)     ES_DBG_EMPTY_DECL()
# define MM_DBG_MACRO(expr)             ES_DBG_EMPTY_MACRO()
# define MM_DBG_CHECK(expr)             ES_DBG_EMPTY_MACRO()
#endif

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*===============================================================================  DATA TYPES  ==*/
/*-------------------------------------------------------------------------------------------*//**
 * @brief       Klasa memorije
 *//*--------------------------------------------------------------------------------------------*/
struct esMemClass {
/**
 * @brief       Memorijski alokator
 */
    void * (*pAlloc)(size_t);

/**
 * @brief       Memorijski dealokator
 */
    void (*pDeAlloc)(void *);

/**
 * @brief       Velicina alocirane memorijske oblasti
 */
    size_t (*pBlockSize)(void *);
};

/*=========================================================================  GLOBAL VARIABLES  ==*/
/*======================================================================  FUNCTION PROTOTYPES  ==*/
/*---------------------------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*===================================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/

/** @endcond *//** @} *//*************************************************************************
 * END of mem_pkg.h
 *************************************************************************************************/
#endif /* MM_PKG_H_ */
