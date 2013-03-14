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
 * @brief       Privatni interfejs Event objekta
 * @addtogroup  evt_impl
 ****************************************************************************************//** @{ */

#ifndef EVT_PKG_H_
#define EVT_PKG_H_

/*============================================================================  INCLUDE FILES  ==*/
/*==================================================================================  DEFINES  ==*/

/**
 * @brief       Bit maska za brojac korisnika dogadjaja
 * @details     Brojac korisnika je 6-bitni, što znači da maksimalan broj
 *              korisnika dogadjaja u jednom trenutku iznosi 63 EPA objekata.
 */
#define EVT_USERS_MASK                  ((uint_fast8_t)0x3F)

/**
 * @brief       Konstanta za potpis dogadjaja
 * @details     Konstanta se koristi prilikom debag procesa kako bi funkcije
 *              koje prime dogadjaj bile sigurne da je dogadjaj kreiran
 *              funkcijom esEvtCreate() i da je i dalje validan. Dogadjaji koji
 *              se obrisu nemaju ovaj potpis.
 * @pre         Opcija @ref OPT_KERNEL_DBG_EVT mora da bude aktivna kako bi bila
 *              omogucena provera pokazivaca.
 */
#define EVT_SIGNATURE                   (0xFEED)

/*==================================================================================  MACRO's  ==*/
/*-------------------------------------------------------------------------  C++ extern begin  --*/
#if defined(__cplusplus)
extern "C" {
#endif

/*===============================================================================  DATA TYPES  ==*/
/*=========================================================================  GLOBAL VARIABLES  ==*/
/*======================================================================  FUNCTION PROTOTYPES  ==*/

/**
 * @brief       Povecava broj korisnika koji koriste dogadjaj
 * @param       evt                     Dogadjaj koji ce se koristiti
 */
C_INLINE void evtUsrAddI_(
    esEvt_T         * evt) {

    if ((uint_fast8_t)0U == (EVT_CONST_MASK & evt->dynamic.s.attrib)) {           /* Da li je dogadjaj dinamičan?                             */
        ++evt->dynamic.s.counter;
    }
}

/**
 * @brief       Smanjuje broj korisnika koji koriste dogadjaj
 * @param       evt                     Dogadjaj koji se koristio
 */
C_INLINE void evtUsrRmI_(
    esEvt_T         * evt) {

    if ((uint_fast8_t)0U == (EVT_CONST_MASK & evt->dynamic.s.attrib)) {           /* Da li je dogadjaj dinamičan?                             */
        --evt->dynamic.s.counter;
    }
}

/*---------------------------------------------------------------------------  C++ extern end  --*/
#if defined(__cplusplus)
}
#endif

/*===================================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/

/** @endcond *//** @} *//*************************************************************************
 * END of evt_pkg.h
 *************************************************************************************************/
#endif /* EVT_PKG_H_ */
