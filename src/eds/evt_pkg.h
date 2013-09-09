/*
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
 *//***********************************************************************//**
 * @file
 * @author      Nenad Radulovic
 * @brief       Privatni interfejs Event objekta
 * @addtogroup  evt_impl
 *********************************************************************//** @{ */

#if !defined(EVT_PKG_H_)
#define EVT_PKG_H_

/*=========================================================  INCLUDE FILES  ==*/

#include "eds/evt.h"

/*===============================================================  MACRO's  ==*/
/*------------------------------------------------------  C++ extern begin  --*/
#if defined(__cplusplus)
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

/**
 * @brief       Povecava broj korisnika koji koriste dogadjaj
 * @param       evt                     Dogadjaj koji ce se koristiti
 */
static PORT_C_INLINE_ALWAYS void evtUsrAddI_(
    esEvt_T *       evt) {

    if (0U == (EVT_CONST_MASK & evt->attrib)) {         /* Da li je dogadjaj dinamičan?                             */
        ++evt->attrib;
    }
}

/**
 * @brief       Smanjuje broj korisnika koji koriste dogadjaj
 * @param       evt                     Dogadjaj koji se koristio
 */
static PORT_C_INLINE_ALWAYS void evtUsrRmI_(
    esEvt_T *       evt) {

    if (0U == (EVT_CONST_MASK & evt->attrib)) {         /* Da li je dogadjaj dinamičan?                             */
        --evt->attrib;
    }
}

/*--------------------------------------------------------  C++ extern end  --*/
#if defined(__cplusplus)
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of evt_pkg.h
 ******************************************************************************/
#endif /* EVT_PKG_H_ */
