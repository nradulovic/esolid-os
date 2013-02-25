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
 * @brief   	Interfejs State Machine Processor (SMP) modula.
 * ------------------------------------------------------------------------------------------------
 * @addtogroup  sproc_intf
 * @brief       Interfejs State Machine Processor (SMP) modula.
 ****************************************************************************************//** @{ */


#ifndef SMP_H_
#define SMP_H_

/*============================================================================  INCLUDE FILES  ==*/
/*==================================================================================  DEFINES  ==*/
/*==================================================================================  MACRO's  ==*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Makroi za tranziciju stanja
 * @{ *//*---------------------------------------------------------------------------------------*/

/**
 * @brief       Vraca dispeceru informaciju da treba da se izvrsi tranzicija.
 * @param       epa                     Pokazivac na strukturu HSM automata,
 * @param       state                   naredno stanje automata.
 * @details     Makro koristi binarni operator @a zarez (,) koji grupise izraze
 *              sa leva na desno. Vrednost i tip celokupnog izraza je vrednost i
 *              tip desnog izraza.
 */
#define ES_STATE_TRAN(epa, state)                                              \
    (((esEpaHdr_T *)(epa))->pState = (esPtrState_T)(state), RETN_TRAN)

/**
 * @brief       Vraca dispeceru informaciju o super stanju trenutnog stanja.
 * @param       epa                     Pokazivac na strukturu HSM automata,
 * @param       state                   super stanje trenutnog stanja.
 * @details     Makro koristi binarni operator @a zarez (,) koji grupise izraze
 *              sa leva na desno. Vrednost i tip celokupnog izraza je vrednost i
 *              tip desnog izraza.
 * @note        Koristi se samo kod HSM automata.
 */
#define ES_STATE_SUPER(epa, state)                                             \
    (((esEpaHdr_T *)(epa))->pState = (esPtrState_T)(state), RETN_SUPER)

/**
 * @brief       Vraca dispeceru informaciju da treba da se izvrsi inicijalna
 *              tranzicija.
 * @param       epa                     Pokazivac na strukturu HSM automata,
 * @param       state                   naredno stanje automata.
 */
#define ES_STATE_INIT(epa, state)                                              \
    (((esEpaHdr_T *)(epa))->pState = (esPtrState_T)(state), RETN_INIT)

/**
 * @brief       Vraca dispeceru informaciju da automat ulazi u flowchart deo
 *              dijagrama stanja.
 * @details     Ovaj makro se koristi kada se zeli tranzicija u sledece stanje
 *              bez dogadjaja. Naime, kada se ovaj makro koristi, dispecer sam
 *              generise @ref SIG_NOEX dogadjaj tako da automat moze da obradi
 *              flowchart bez spoljasnih dogadjaja. Flowchart se pise u odeljku
 *              koji obradjuje SIG_NOEX, dok se u SIG_ENTRY odeljku samo
 *              koristi ovaj makro.
 */
#define ES_STATE_NOEX()                                                        \
    (RETN_NOEX)

/**
 * @brief       Vraca dispeceru informaciju da je dogadjaj opsluzen.
 * @details     Ovaj makro samo obavestava dispecer da je dogadjaj opsluzen i ne
 *              treba da se izvrsi promena stanja.
 */
#define ES_STATE_HANDLED()                                                     \
    (RETN_HANDLED)

/**
 * @brief       Vraca dispeceru informaciju da je dogadjaj ignorisan.
 * @details     Dogadjaj se ignorise i ne dolazi do promene stanja automata.
 */
#define ES_STATE_IGNORED()                                                     \
    (RETN_IGNORED)

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*===============================================================================  DATA TYPES  ==*/

/**
 * @brief       Nabrajanje odgovora state handler funkcije.
 * @details		State handler funkcija preko ovih nabrajanja govori SM
 *              dispeceru da li treba da se preuzme neka akcija kao odgovor na
 *              dogadjaj.
 */
enum state {
/**
 * @brief       Ne treba izvrsiti nikakve dalje akcije.
 * @details		Ovo je odgovor state handler funkcije da je potpuno opsluzila
 *              dogadjaj i nikakve dodatne akcije ne treba da se preduzmu.
 */
    RETN_HANDLED,

/**
 * @brief       Treba odloziti pristigli dogadjaj.
 * @details		Sistem ce predati dogadjaj vratiti ponovo u red za cekanje za
 *              dogadjaje i poslati ga prilikom sledeceg ciklusa.
 */
    RETN_DEFERRED,

/**
 * @brief       Pristigli dogadjaj nije obradjen i ignorisan je.
 * @details		Obicno se ovakav odgovor u top state-u automata i koristi se u
 *              svrhe debagiranja sistema. Dogadjaj se brise iz sistema ako nema
 *              jos korisnika.
 */
    RETN_IGNORED,

/**
 * @brief       Treba izvrsiti tranziciju ka drugom stanju.
 * @details		Akcija koja je potrebna za odgovor na dogadjaj je tranzicija ka
 *              drugom stanju.
 */
    RETN_TRAN,

/**
 * @brief       Vraca se koje je super stanje date state handler funkcije.
 * @details		Ova vrednost se vraca kada state handler funkcija ne zna da
 *              obradi neki dogadjaj ili je od nje zahtevano da vrati koje je
 *              njeno super stanje.
 */
    RETN_SUPER
};

/*=========================================================================  GLOBAL VARIABLES  ==*/
/*======================================================================  FUNCTION PROTOTYPES  ==*/

/**
 * @brief       Najvisi nivo u hijerarhiji automata.
 * @param       aEpa                    Pokazivac na strukturu HSM automata,
 * @param       aEvt                    pokazivac/podatak na strukturu dogadjaja.
 * @return      esState_T             status funkcije.
 * @note        Uvek vraca odgovor IGNORED, jer ona ne prihvata nikakav
 *              dogadjaj.
 * @api
 */
esState_T esSmTopState(
    esEpaHdr_T       * aEpa,
    esEvtHdr_T       * aEvt);

/*---------------------------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*===================================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/

/** @endcond *//** @} *//*************************************************************************
 * END of smp.h
 *************************************************************************************************/
#endif /* SMP_H_ */
