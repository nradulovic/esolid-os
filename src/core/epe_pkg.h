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
 *************************************************************************************************/


/*********************************************************************************************//**
 * @file
 * @author  	nenad
 * @brief       Privatni interfejs Event Processing Engine podmodula.
 * ------------------------------------------------------------------------------------------------
 * @addtogroup  epe_impl
 ****************************************************************************************//** @{ */


#ifndef EPE_PKG_H_
#define EPE_PKG_H_

/*============================================================================  INCLUDE FILES  ==*/
/*----------------------------------------------------------------------------------  EXTERNS  --*/
/** @cond */
#ifdef EPE_PKG_H_VAR
# define EPE_PKG_H_EXT
#else
# define EPE_PKG_H_EXT extern
#endif
/** @endcond*/

/*==================================================================================  DEFINES  ==*/
/*==================================================================================  MACRO's  ==*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Debug podrska
 * @brief       Makroi za debug podrsku. Pogledati @ref dbg_intf.
 * @{ *//*---------------------------------------------------------------------------------------*/

#if defined(OPT_DBG_EPE) || defined(__DOXYGEN__)
# define EPE_ASSERT                     DBG_ASSERT
# define EPE_ASSERT_ALWAYS              DBG_ASSERT_ALWAYS
# define EPE_COMPILE_ASSERT             DBG_COMPILE_ASSERT
# define EPE_DBG_DECL                   DBG_DECL
# define EPE_DBG_DEFINE_MODULE          DBG_DEFINE_MODULE
# define EPE_DBG_ENTRY                  DBG_ENTRY
# define EPE_DBG_EXIT                   DBG_EXIT
# define EPE_DBG_MACRO                  DBG_MACRO
# define EPE_DBG_CHECK                  DBG_CHECK
#else
# define EPE_ASSERT(expr)               DBG_EMPTY_MACRO()
# define EPE_ASSERT_ALWAYS(expr)        DBG_EMPTY_MACRO()
# define EPE_COMPILE_ASSERT(expr)       DBG_EMPTY_DECL()
# define EPE_DBG_DECL(expr)             DBG_EMPTY_DECL()
# define EPE_DBG_DEFINE_MODULE(expr)    DBG_EMPTY_DECL()
# define EPE_DBG_ENTRY()                DBG_EMPTY_MACRO()
# define EPE_DBG_EXIT()                 DBG_EMPTY_MACRO()
# define EPE_DBG_MACRO(expr)            DBG_EMPTY_MACRO()
# define EPE_DBG_CHECK(expr)            DBG_EMPTY_MACRO()
#endif

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @ingroup     Pomocni makroi za rad sa bitmapom
 * @{ *//*---------------------------------------------------------------------------------------*/
#if (OPT_KERNEL_EPA_PRIO_MAX < HAL_UNATIVE_BITS)
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
EPE_PKG_H_EXT rdyBitmap_T rdyBitmap;

/*======================================================================  FUNCTION PROTOTYPES  ==*/
/*-------------------------------------------------------------------------------------------*//**
 * @brief       Ubacuje EPA objekat u red za cekanje
 *//*--------------------------------------------------------------------------------------------*/
C_INLINE void schedRdyInsertI_(
    const esEpaHeader_T * aEpa) {

    unative_T indxGroup;
    unative_T indx;

    indx = aEpa->internals.kernCtrl.prio & (~((unative_T)0U) >> (HAL_UNATIVE_BITS - PRIO_INDX_PWR));

#if (OPT_KERNEL_EPA_PRIO_MAX < HAL_UNATIVE_BITS)
    indxGroup = (unative_T)0U;
#else
    indxGroup = aEpa->internals.kernCtrl.prio >> PRIO_INDX_PWR;
#endif
    rdyBitmap.bitGroup |= (unative_T)1U << indxGroup;
    rdyBitmap.bit[indxGroup] |= (unative_T)1U << indx;
}

#if defined(OPT_OPTIMIZE_SIZE) || defined(__DOXYGEN__)
void schedRdyInsertI(
    const esEpaHeader_T * aEpa);
#endif

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Vrsi inicijalizaciju hardvera za rad sa schedulerom
 * @details     Postavlja prioritete SVCall i PendSV prekida na najnizu vrednost.
 *//*--------------------------------------------------------------------------------------------*/
void portSchedInit(
    void);

/*---------------------------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*===================================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/

/** @endcond *//** @} *//*************************************************************************
 * END of epe_pkg.h
 *************************************************************************************************/
#endif /* EPE_PKG_H_ */
