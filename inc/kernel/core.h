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
 * @brief       Osnovne funkcije kernel-a
 * ------------------------------------------------------------------------------------------------
 * @addtogroup  kernel_intf
 ****************************************************************************************//** @{ */


#ifndef CORE_H_
#define CORE_H_

/*============================================================================  INCLUDE FILES  ==*/
/*==================================================================================  DEFINES  ==*/
/*==================================================================================  MACRO's  ==*/
/*-------------------------------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif


/*===============================================================================  DATA TYPES  ==*/

/**
 * @brief       Stanje kernel-a
 */
enum esKernelStatus {
/**
 * @brief       Kernel se ne izvrsava
 */
    KERNEL_STOPPED,

/**
 * @brief       Kernel se izvrsava
 */
    KERNEL_RUNNING
};

/**
 * @brief       Definiciona struktura koja opisuje jedan EPA objekat
 * @api
 */
struct esEpaDef {
/**
 * @brief       Deskriptivno ime EPA objekta
 * @details     Pokazivac na znakovni niz koji cuva ime jedinice za obradu
 *              dogadjaja. Ime jedinice za obradu dogadjaja se moze cuvati samo
 *              u flash/ROM memoriji. Ime pruza samo deskriptivnu informaciju o
 *              EPA objektu i u druge svrhe se ne koristi. Vise EPA objekata
 *              mogu imati isto ime.
 */
    const C_ROM char * name;

/**
 * @brief       Prioritet EPA objekta
 * @details		Opseg vrednosti 1 - OPT_KERNEL_EPA_PRIO_MAX
 */
    uint8_t         prio;

/**
 * @brief       Potrebna memorija radnog okruzenja za EPA objekat
 * @details		Ovom promenljivom se specificira velicina potrebne memorije za
 * 				jednu instancu automata. Ukoliko se instacira samo jedna,
 * 				konstantan EPA objekat (videti esMemStaticClass) onda je ova
 * 				promenljiva tipicno nula.
 */
    size_t          workspaceSize;

/**
 * @brief       Velicina reda za cekanje za dogadjaje
 */
    size_t          evtQueueSize;

/**
 * @brief       Inicijalno stanje konačnog automata
 */
    esPtrState_T    smInitState;

/**
 * @brief       Maksimalna dubina hijerarhije stanja automata.
 */
    uint8_t         smStateDepth;
};

/*=========================================================================  GLOBAL VARIABLES  ==*/
/*======================================================================  FUNCTION PROTOTYPES  ==*/

/*-------------------------------------------------------------------------------------------*//**
 * @name        Osnovne funkcije kernal-a
 * @brief       Ove funkcije se koriste za upravljanjem kernel-om
 * @{ *//*---------------------------------------------------------------------------------------*/

/**
 * @brief       Inicijalizacija kernel-a
 * @details		Ova funkcija pokrece funkcije za inicijalizaciju HAL-a, a zatim
 * 				vrsi inicijalizaciju svih podsistema eSolid-kernela.
 * @api
 */
void esKernelInit(
    void);

/**
 * @brief       Pokrece izvrsavanje jezgra
 * @details		Ova funkcija se pokrece sve kreirane automate. Ukoliko nije
 * 				kreiran ni jedan automat pokrece se samo sistemski automat.
 * @api
 */
void esKernelStart(
    void);

/**
 * @brief       Vraca odgovor da li trenutno radi kernel
 * @return      Status izvrsavanja kernel-a.
 *  @retval     KERNEL_STOPPED - kernel se ne izvrsava,
 *  @retval     KERNEL_RUNNING - kernel se izvrsava,
 * @api
 */
esKernelStatus_T esKernelStatus(
    void);

/**
 * @brief       Vraca znakovni niz koji identifikuje eSolid kernel
 * @return      Verzija kernel-a u obliku znakovnog niza.
 * @api
 */
const C_ROM char * esKernelSysVer(
    void);

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Osnovne funkcije za menadzment EPA objekata
 * @{ *//*---------------------------------------------------------------------------------------*/

/**
 * @brief       Kreira EPA objekat.
 * @param       aMemClass               Klasa memorije koja se koristi za
 *                                      skladistenje:
 *  @arg        memHeapClass
 *  @arg        memPoolClass
 *  @arg        memStaticClass
 * @param       aDescription            pokazivac na opis EPA objekta.
 * @return      Pokazivac na strukturu EPA objekta.
 * @see         esEpaDef_T
 * @details     Rezervise potreban memorijski prostor upotrebom naznačenog
 * 				alokatora i vrsi neophodnu inicijalizaciju sa porametrima koji
 * 				su specificirani u esEpaDet_T strukturi.
 * @api
 */
esEpaHdr_T * esEpaCreate(
    const C_ROM esMemClass_T * aMemClass,
    const C_ROM esEpaDef_T * aDescription);

/**
 * @brief       Unistava EPA objekat.
 * @param       aEpa                    Pokazivac na strukturu EPA objekta.
 * @api
 */
void esEpaDestroy(
    esEpaHdr_T       	* aEpa);

/**
 * @brief       Vraca Id pokazivac EPA objekta.
 * @return      Id pokazivac trenutnog EPA objekta koji se izvrsava.
 * @pre         Da bi se koristila kernel funkcija mora se definisati
 *              @ref OPT_KERNEL_ENABLE.
 * @api
 */
esEpaHdr_T * esEpaGetHdr(
    void);

/**
 * @brief       Dobavlja prioritet EPA objekta
 * @param       aEpa                    Pokazivac na EPA objekat
 * @return      Trenutni prioritet EPA objekta.
 * @pre         Da bi se koristila kernel funkcija mora se definisati
 *              @ref OPT_KERNEL_ENABLE.
 * @api
 */
uint8_t esEpaPrioGet(
    const esEpaHdr_T 	* aEpa);

/**
 * @brief       Postavlja nov prioritet EPA objekta.
 * @param       aEpa                    Pokazivac na EPA objekat,
 * @param       aNewPrio                nov prioritet EPA objekta.
 * @warning     Ukoliko se ne koristi @ref OPT_KERNEL_USE_ROUND_ROBIN, a
 *              zahtevani prioritet je vec zauzet javice se assert obavestenje.
 * @api
 */
void esEpaPrioSet(
    esEpaHdr_T       	* aEpa,
    uint8_t             aNewPrio);

/** @} *//*--------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*===================================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/

/** @endcond *//** @} *//*************************************************************************
 * END of core.h
 *************************************************************************************************/
#endif /* CORE_H_ */
