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
 * @brief       Privatni interfejs osnove kernel-a.
 * ------------------------------------------------------------------------------------------------
 * @addtogroup  kernel_impl
 ****************************************************************************************//** @{ */


#ifndef CORE_PKG_H_
#define CORE_PKG_H_

/*============================================================================  INCLUDE FILES  ==*/
/*----------------------------------------------------------------------------------  EXTERNS  --*/
/** @cond */
#ifdef CORE_PKG_H_VAR
# define CORE_PKG_H_EXT
#else
# define CORE_PKG_H_EXT extern
#endif
/** @endcond*/

/*==================================================================================  DEFINES  ==*/
/*==================================================================================  MACRO's  ==*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Debug podrska
 * @brief       Makroi za debug podrsku. Pogledati @ref dbg_intf.
 * @{ *//*---------------------------------------------------------------------------------------*/

#if defined(OPT_KERNEL_DBG_CORE) || defined(__DOXYGEN__)
# define CORE_ASSERT                  ES_DBG_ASSERT
# define CORE_ASSERT_ALWAYS           ES_DBG_ASSERT_ALWAYS
# define CORE_COMPILE_ASSERT          ES_DBG_COMPILE_ASSERT
# define CORE_DBG_DECL                ES_DBG_DECL
# define CORE_DBG_DEFINE_MODULE       ES_DBG_DEFINE_MODULE
# define CORE_DBG_MACRO               ES_DBG_MACRO
# define CORE_DBG_CHECK               ES_DBG_CHECK
#else
# define CORE_ASSERT(expr)            ES_DBG_EMPTY_MACRO()
# define CORE_ASSERT_ALWAYS(expr)     ES_DBG_EMPTY_MACRO()
# define CORE_COMPILE_ASSERT(expr)    ES_DBG_EMPTY_DECL()
# define CORE_DBG_DECL(expr)          ES_DBG_EMPTY_DECL()
# define CORE_DBG_DEFINE_MODULE(expr) ES_DBG_EMPTY_DECL()
# define CORE_DBG_MACRO(expr)         ES_DBG_EMPTY_MACRO()
# define CORE_DBG_CHECK(expr)         ES_DBG_EMPTY_MACRO()
#endif

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @ingroup     Pomocni makroi za rad sa bitmapom
 * @{ *//*---------------------------------------------------------------------------------------*/
#if (OPT_KERNEL_EPA_PRIO_MAX < ES_CPU_UNATIVE_BITS)
# define PRIO_INDX                      OPT_KERNEL_EPA_PRIO_MAX
# define PRIO_INDX_GROUP                1
#else
# define PRIO_INDX                      HAL_UNATIVE_BITS
# define PRIO_INDX_GROUP                (ES_DIV_ROUNDUP(OPT_KERNEL_EPA_PRIO_MAX, PRIO_INDX))
#endif
#define PRIO_INDX_PWR                   ES_UINT8_LOG2(PRIO_INDX)

/** @} *//*--------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*===============================================================================  DATA TYPES  ==*/
/*-------------------------------------------------------------------------------------------*//**
 * @brief       Bitmap spremnih EPA objekata
 *//*--------------------------------------------------------------------------------------------*/
typedef struct rdyBitmap {
/**
 * @brief       Grupa prioriteta EPA objekata
 *
 *              Prilikom trazenja sledeceg aktivnog EPA objekta prvo se
 *              pretrazuje ovaj clan.
 */
    unative_T       bitGroup;

/**
 * @brief       Prioriteti EPA objekata
 *
 *              Kad je pretragom bitGroup utvrdjeno da se ovde nalazi spreman
 *              EPA objekat, onda se pretraga nastavlja ovde.
 */
    unative_T       bit[PRIO_INDX_GROUP];

/**
 * @brief       Lista aktivnih EPA objekata;
 */
    esEpaHeader_T   * epaList[OPT_KERNEL_EPA_PRIO_MAX];
} rdyBitmap_T;

/*=========================================================================  GLOBAL VARIABLES  ==*/
/*-------------------------------------------------------------------------------------------*//**
 * @brief       Bitmape spremnih EPA objekata
 *//*--------------------------------------------------------------------------------------------*/
CORE_PKG_H_EXT rdyBitmap_T rdyBitmap;

/*======================================================================  FUNCTION PROTOTYPES  ==*/
/*-------------------------------------------------------------------------------------------*//**
 * @brief       Ubacuje EPA objekat u red za cekanje
 *//*--------------------------------------------------------------------------------------------*/
C_INLINE_ALWAYS void schedRdyInsertI_(
    const esEpaHeader_T * aEpa) {

    unative_T indxGroup;
    unative_T indx;

    indx = aEpa->internals.kernCtrl.prio & (~((unative_T)0U) >> (ES_CPU_UNATIVE_BITS - PRIO_INDX_PWR));

#if (OPT_KERNEL_EPA_PRIO_MAX < ES_CPU_UNATIVE_BITS)
    indxGroup = (unative_T)0U;
#else
    indxGroup = aEpa->internals.kernCtrl.prio >> PRIO_INDX_PWR;
#endif
    rdyBitmap.bitGroup |= (unative_T)1U << indxGroup;
    rdyBitmap.bit[indxGroup] |= (unative_T)1U << indx;
}

/*---------------------------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*===================================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/

/** @endcond *//** @} *//*************************************************************************
 * END of core_pkg.h
 *************************************************************************************************/
#endif /* CORE_PKG_H_ */
