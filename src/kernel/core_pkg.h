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

/**
 * @brief       Konstanta za potpis EPA objekta
 * @details     Konstanta se koristi prilikom debag procesa kako bi funkcije
 *              koje prihvate pokazivac na EPA objekat bile sigurne da je EPA
 *              objekat validan. EPA objekti koji su obrisani nemaju ovaj potpis.
 * @pre         Opcija @ref OPT_KERNEL_DBG mora da bude aktivna kako bi bila
 *              omogucena provera pokazivaca.
 */
#define EPA_SIGNATURE                   (0xDEEF)

/*==================================================================================  MACRO's  ==*/

/*-------------------------------------------------------------------------------------------*//**
 * @ingroup     Pomocni makroi za rad sa bitmapom
 * @{ *//*---------------------------------------------------------------------------------------*/
#if (OPT_KERNEL_EPA_PRIO_MAX <= ES_CPU_UNATIVE_BITS)
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

/**
 * @brief       Bitmap spremnih EPA objekata
 */
struct rdyBitmap {
#if !(OPT_KERNEL_EPA_PRIO_MAX <= ES_CPU_UNATIVE_BITS) || defined(__DOXYGEN__)
/**
 * @brief       Grupa prioriteta EPA objekata
 * @details     Prilikom trazenja sledeceg aktivnog EPA objekta prvo se
 *              pretrazuje ovaj clan.
 * @note        Ovaj clan se ne koristi kada uslov
 *              "OPT_KERNEL_EPA_PRIO_MAX < ES_CPU_UNATIVE_BITS" nije ispunjen. U
 *              tom slucaju, generisani kod je manji i efikasniji prilikom
 *              komutacije EPA objekata.
 */
    unative_T       bitGroup;
#endif

/**
 * @brief       Prioriteti EPA objekata
 * @details     Kad je pretragom bitGroup utvrdjeno da se ovde nalazi spreman
 *              EPA objekat, onda se pretraga nastavlja ovde.
 */
    unative_T       bit[PRIO_INDX_GROUP];

/**
 * @brief       Lista aktivnih EPA objekata;
 */
    esEpaHeader_T   * list[OPT_KERNEL_EPA_PRIO_MAX];
};

/*=========================================================================  GLOBAL VARIABLES  ==*/

/**
 * @brief       Bitmape spremnih EPA objekata
 */
CORE_PKG_H_EXT struct rdyBitmap gRdyBitmap;

/*======================================================================  FUNCTION PROTOTYPES  ==*/

/**
 * @brief       Ubacuje EPA objekat u red za cekanje.
 * @param       [in] aEpa               Pokazivac na EPA objekat koji je spreman
 *                                      za izvrsenje.
 * @details     EPA objekat na koji pokazuej pokazivac se ubacuje u listu
 *              spremnih EPA objekata na izvrsenje.
 */
C_INLINE_ALWAYS void schedRdyInsertI_(
    const esEpaHeader_T * aEpa) {

#if (OPT_KERNEL_EPA_PRIO_MAX <= ES_CPU_UNATIVE_BITS)
    gRdyBitmap.bit[0] |= (unative_T)1U << aEpa->prio;
#else
    unative_T indxGroup;
    unative_T indx;

    indx = aEpa->prio & (~((unative_T)0U) >> (ES_CPU_UNATIVE_BITS - PRIO_INDX_PWR));
    gRdyBitmap.bitGroup |= (unative_T)1U << indxGroup;
    gRdyBitmap.bit[indxGroup] |= (unative_T)1U << indx;
#endif
}

/**
 * @brief       Izbacuje EPA objekat iz reda za cekanje
 */
C_INLINE_ALWAYS void schedRdyRmI_(
    esEpaHeader_T       * aEpa) {

#if (OPT_KERNEL_EPA_PRIO_MAX <= ES_CPU_UNATIVE_BITS)
    gRdyBitmap.bit[0] &= ~((unative_T)1U << aEpa->prio);
#else
    unative_T indxGroup;
    unative_T indx;

    indx = aEpa->prio & (~((unative_T)0U) >> (ES_CPU_UNATIVE_BITS - PRIO_INDX_PWR));
    indxGroup = aEpa->prio >> PRIO_INDX_PWR;
    gRdyBitmap.bit[indxGroup] &= ~((unative_T)1U << indx);

    if ((unative_T)0U == gRdyBitmap.bit[indxGroup]) {
        gRdyBitmap.bitGroup &= ~((unative_T)1U << indxGroup);
    }
#endif
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
