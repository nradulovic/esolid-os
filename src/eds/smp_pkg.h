/******************************************************************************
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
 * @brief       Privatni interfejs State Machine Processor objekta.
 * @addtogroup  smp_impl
 *********************************************************************//** @{ */


#ifndef SMP_PKG_H_
#define SMP_PKG_H_

/*=========================================================  INCLUDE FILES  ==*/
/*===============================================================  DEFINES  ==*/

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

/*===============================================================  MACRO's  ==*/
/*------------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

extern const PORT_C_ROM esEvt_T evtSignal[];

/**
 * @brief       Pristigli dogadjaj nije obradjen i ignorisan je.
 * @details     Obicno se ovakav odgovor u top state-u automata i koristi se u
 *              svrhe debagiranja sistema. Dogadjaj se brise iz sistema ako nema
 *              jos korisnika.
 */
    RETN_IGNORED,

/**
 * @brief       Vraca se koje je super stanje date state handler funkcije.
 * @details     Ova vrednost se vraca kada state handler funkcija ne zna da
 *              obradi neki dogadjaj ili je od nje zahtevano da vrati koje je
 *              njeno super stanje.
 */
    RETN_SUPER
};

/**
 * @details     Struktura sadrzi trenutno stanje automata. Ostali clanovi su
 *              opcioni. Ukoliko se koriste HSM automati koriste se pokazivaci
 *              na redove cekanja za stanja automata.
 * @notapi
 */
struct esSm {

#if (OPT_LOG_LEVEL <= LOG_DBG)
/**
 * @brief       Potpis koji pokazuje da je ovo zaista SM objekat.
 */
    uint16_t        signature;
#endif

/**
 * @brief       Pokazivac na state handler funkciju.
 * @details     Ovaj pokazivac pokazuje na funkciju stanja koja vrsi obradu
 *              dogadjaja.
 */
    esState_T       state;

#if (OPT_SMP_SM_TYPES == ES_SMP_FSM_AND_HSM)
/**
 * @brief       Pokazivac na dispecer funkciju datog automata
 * @details     Ovaj clan strukture se koristi samo ukoliko se istovremeno
 *              koriste FSM i HSM automati.
 */
    esStatus_T (* dispatch)(struct esSm *, const esEvt_T *);
#endif

#if (OPT_SMP_SM_TYPES != ES_SMP_FSM_ONLY) || defined(__DOXYGEN__)
/**
 * @brief       Niz za cuvanje izvornih stanja HSM automata
 * @details     Ovaj clan se koristi samo ukoliko se koriste HSM automati.
 */
    esState_T *     stateQBegin;

/**
 * @brief       Niz za cuvanje odredisnih stanja HSM automata
 * @details     Ovaj clan se koristi samo ukoliko se koriste HSM automati.
 */
    esState_T *     stateQEnd;
#endif
};

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

/**
 * @brief       Vraca kolika je potrebna velicina memorijskog prostora za
 *              cuvanje bafera stanja.
 * @param       levels                  Maksimalna hijerarhijska dubina stanja
 *                                      automata.
 * @return      Potreban memorijski prostor u bajtovima.
 * @notapi
 */
size_t stateQReqSize(
    uint8_t         levels);

/**
 * @brief       Dispecer HSM automata
 * @param       [in] sm                 Pokazivac na strukturu HSM automata
 * @param       [in] evt                Dogadjaj koji treba da se obradi
 * @return      Status obrade dogadjaja.
 */
esStatus_T hsmDispatch(
    esSm_T *        sm,
    const esEvt_T * evt);

/**
 * @brief       Dispecer HSM automata
 * @param       [in] sm                 Pokazivac na strukturu FSM automata
 * @param       [in] evt                Dogadjaj koji treba da se obradi
 * @return      Status obrade dogadjaja.
 */
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
 */
void smInit (
    esSm_T *        sm,
    esState_T       initState,
    esState_T *     stateQueue,
    size_t          levels);

/**
 * @brief       Dekonstruise automat
 * @param       [out] sm                Pokazivac na kreiran automat.
 */
void smDeInit(
    esSm_T *        sm);

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of smp_pkg.h
 ******************************************************************************/
#endif /* SMP_PKG_H_ */
