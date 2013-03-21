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
 * @brief       Privatni interfejs EPA objekta
 * @addtogroup  epa_impl
 ****************************************************************************************//** @{ */


#ifndef EPA_PKG_H_
#define EPA_PKG_H_

/*============================================================================  INCLUDE FILES  ==*/
/*==================================================================================  DEFINES  ==*/
/*==================================================================================  MACRO's  ==*/

/**
 * @brief       Dispecer EPA objekta
 */
#define EPA_DISPATCH(epa, evt)                                                  \
    SM_DISPATCH(&epa->sm, evt)

/*-------------------------------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*===============================================================================  DATA TYPES  ==*/

/**
 * @details     EPA objekat se sastoji od internih podataka, redova cekanja za
 *              dogadjaje i stanja automata (HSM) i korisničkih podataka.
 * @notapi
 */
struct esEpa {
/**
 * @brief       Red cekanja za dogadjaje.
 */
    struct evtQueue evtQueue;

#if (OPT_KERNEL_ENABLE_LOG == 1U) || defined(__DOXYGEN__)
/**
 * @brief       Potpis koji pokazuje da je ovo zaista EPA objekat.
 */
    uint32_t        signature;
#endif

/**
 * @brief       Prioritet EPA objekta.
 * @details     Ova promenljiva odredjuje prioritet datog EPA objekta.
 */
    uint_fast8_t    prio;

/**
 * @brief       Ime EPA objekta
 */
    const C_ROM char * name;

/**
 * @brief       Automat (FSM ili HSM)
 */
    struct esSm     sm;
};

/*=========================================================================  GLOBAL VARIABLES  ==*/
/*======================================================================  FUNCTION PROTOTYPES  ==*/

/**
 * @brief       Dobavlja dogadjaj iz reda za cekanje epa objekta
 * @param       epa                    Pokazivac na epa objekat
 * @return      Dogadjaj iz reda cekanja.
 * @notapi
 */
esEvt_T * evtFetchI(
    esEpa_T *       epa);

/**
 * @brief       Ubacuje dogadjaj nazad na red cekanja.
 * @param       epa                     Pokazivac na epa objekat
 * @param       evt                     Pokazivac na dogadjaj
 * @details     Ovo se dogadja kada dispecer javi da je dogadjaj odbačen
 *              (DEFERRED).
 */
void evtPushBackI(
    esEpa_T *       epa,
    esEvt_T *       evt);

/** @} *//*--------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*===================================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//*************************************************************************
 * END of epa_pkg.h
 *************************************************************************************************/
#endif /* EPA_PKG_H_ */
