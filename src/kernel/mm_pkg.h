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
 * @addtogroup  mm_impl
 ****************************************************************************************//** @{ */


#ifndef MM_PKG_H_
#define MM_PKG_H_

/*============================================================================  INCLUDE FILES  ==*/
/*==================================================================================  DEFINES  ==*/
/*==================================================================================  MACRO's  ==*/
/*-------------------------------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*===============================================================================  DATA TYPES  ==*/

/**
 * @brief       Klasa memorije
 */
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

/**
 * @brief       Initializes Static Memory manager
 * @notapi
 */
void smemInitI(
    void);

/**
 * @brief       Inicijalizuje heap memorijski alokator velicinom koja je
 *              odredjena opcijom OPT_MM_HEAP_SIZE
 * @pre         Staticna memorija mora da bude inicijalizovana pre ove funkcije.
 * @notapi
 */
void hmemInitI(
    void);

/*---------------------------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*===================================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/

/** @endcond *//** @} *//*************************************************************************
 * END of mem_pkg.h
 *************************************************************************************************/
#endif /* MM_PKG_H_ */
