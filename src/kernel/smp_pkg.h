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
/*==================================================================================  MACRO's  ==*/
/*-------------------------------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*===============================================================================  DATA TYPES  ==*/
/*=========================================================================  GLOBAL VARIABLES  ==*/
/*======================================================================  FUNCTION PROTOTYPES  ==*/

/**
 * @brief       Vraca kolika je potrebna velicina memorijskog prostora za
 *              cuvanje bafera stanja.
 * @param       aStateDept              Maksimalna hijerarhijska dubina stanja
 *                                      automata.
 * @return      Potreban memorijski prostor u bajtovima.
 * @notapi
 */
size_t hsmReqSize(
    size_t              aStateDept);

/**
 * @brief       Pokrece dati HSM automat.
 * @param       aEpa                    Pokazivac na strukturu HSM automata,
 * @param       aEvt                    podatak/pokazivac na podatak dogadjaja.
 * @details     Ovu funkcija se pokrece nakon zakljucivanja da je dati
 *              automat spreman za rad. Dispecer pokrece stateHandler funkcije i
 *              ispituje njihovu povratnu vrednost. U zavisnosti od povratne
 *              vrednosti funkcije stanja on preduzima dodatne akcije. Kada je
 *              zavrsena obrada dogadjaja, dispecer postavlja prazan signal
 *              (SIG_EMPTY) u pokazivac dogadjaja cime se govori da je zavrsena
 *              obrada prethodnog dogadjaja i da je automat spreman da prihvati
 *              nov dogadjaj.
 */
void hsmDispatch(
    esEpaHeader_T       * aEpa,
    const esEvtHeader_T * aEvt);

/**
 * @brief       Konstruise HSM automat
 * @param       aEpa                    Pokazivac na tek kreiran EPA objekat,
 * @param       aInitState              inicijalno stanje automata,
 * @param       aStateBuff              pokazivac na memorijski bafer za stanja,
 * @param       aStateDepth             maksimalna hijerarhijska dubina stanja
 *                                      automata.
 */
void hsmInit(
    esEpaHeader_T       * aEpa,
    esPtrState_T        aInitState,
    esPtrState_T        * aStateBuff,
    size_t              aStateDepth);

/**
 * @brief       Dekonstruise HSM automat
 * @param       aEpa                    Pokazivac na kreiran EPA objekat.
 */
void hsmDeInit(
    esEpaHeader_T       * aEpa);

/*---------------------------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*===================================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/

/** @endcond *//** @} *//*************************************************************************
 * END of sproc_pkg.h
 *************************************************************************************************/
#endif /* SPROC_PKG_H_ */
