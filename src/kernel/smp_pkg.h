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
 * @brief       Privatni interfejs State Machine Processor podmodul.
 * @addtogroup  smp_impl
 ****************************************************************************************//** @{ */


#ifndef SMP_PKG_H_
#define SMP_PKG_H_

/*============================================================================  INCLUDE FILES  ==*/
/*==================================================================================  DEFINES  ==*/

#if (OPT_SMP_SM_TYPES == 1)
# define SM_DISPATCH(sm, evt)                                                   \
    fsmDispatch(sm, evt)
#elif (OPT_SMP_SM_TYPES == 2)
# define SM_DISPATCH(sm, evt)                                                   \
    hsmDispatch(sm, evt)
#else
# define SM_DISPATCH(sm, evt)                                                   \
    (*(sm)->dispatch)(sm, evt)
#endif

/*==================================================================================  MACRO's  ==*/
/*-------------------------------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*===============================================================================  DATA TYPES  ==*/
/*=========================================================================  GLOBAL VARIABLES  ==*/

extern const C_ROM esEvt_T evtSignal[];

/*======================================================================  FUNCTION PROTOTYPES  ==*/

/**
 * @brief       Vraca kolika je potrebna velicina memorijskog prostora za
 *              cuvanje bafera stanja.
 * @param       aStateDept              Maksimalna hijerarhijska dubina stanja
 *                                      automata.
 * @return      Potreban memorijski prostor u bajtovima.
 * @notapi
 */
size_t stateQReqSize(
    uint8_t         levels);

esStatus_T hsmDispatch(
    esSm_T *        sm,
    const esEvt_T * evt);

esStatus_T fsmDispatch(
    esSm_T *        sm,
    const esEvt_T * evt);

/**
 * @brief       Konstruise automat
 * @param       [out] sm                Pokazivac na tek kreiranu strukturu
 *                                      automata,
 * @param       [in] initState          inicijalno stanje automata,
 * @param       [in] stateQueue         pokazivac na memorijski bafer za stanja,
 * @param       [in] levels             maksimalna hijerarhijska dubina stanja
 *                                      automata.
 * @notapi
 */
void smInit (
    esSm_T *        sm,
    esState_T       initState,
    esState_T *     stateQueue,
    size_t          levels);

/**
 * @brief       Dekonstruise HSM automat
 * @param       [out] sm                Pokazivac na kreiran automat.
 * @notapi
 */
void smDeInit(
    esSm_T *        sm);

/*---------------------------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*===================================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/

/** @endcond *//** @} *//*************************************************************************
 * END of sproc_pkg.h
 *************************************************************************************************/
#endif /* SPROC_PKG_H_ */
