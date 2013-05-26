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
 * @brief       Interfejs eSolid operativnog sistema.
 * @details     Product description:
 *              - MM - Memory Management
 *              - SMP - State Machine Processor
 *              - KERNEL - Kernel
 *
 *              Text Editor Settings:
 *              - TAB: 4 spaces
 *              - Print Margin Columnt: 80 characters
 *              - Encoding: UTF-8
 *
 *              Development Environment:
 *              - Eclipse v4.2.1 + CDT v8.1.0
 *              - GNU make v3.8.2
 *
 *              Compiler make and version:
 *              - arm-none-eabi-gcc v4.6.3 (Sourcery CodeBench Lite 2012.03-56)
 *
 *              Libraries:
 *              - esolid-hal
 *              - esolid-ett
 *
 *              Configuraion:
 *              - config/kernel_config.h
 *
 *              CPU configuration:
 *              - independent
 *
 *              Include paths:
 *              - ./esolid-kernel/inc
 *              - ./esolid-hal/inc (and port defined paths)
 *              - ./esolid-ett/inc
 * @addtogroup  kernel_intf
 *********************************************************************//** @{ */


#ifndef KERNEL_H_
#define KERNEL_H_

/*=========================================================  INCLUDE FILES  ==*/
#include "kernel/evt.h"

/*===============================================================  DEFINES  ==*/

/**
 * @brief       Znakovna konstanta koja pokazuje puno ime i verziju eSolid-a.
 */
#define ES_KERNEL_ID                    "eSolid v1.0"

/**
 * @brief       Numericka vrednost major verzije eSolid-a.
 */
#define ES_KERNEL_VERSION_MAJOR         1U

/**
 * @brief       Numericka vrednost minor verzije eSolid-a.
 */
#define ES_KERNEL_VERSION_MINOR         0U

/*===============================================================  MACRO's  ==*/
/*------------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/**
 * @brief       Stanje kernel-a
 */
typedef enum esKernelStatus {
/**
 * @brief       Kernel se ne izvrsava
 */
    ES_KERNEL_STOPPED = 1UL,

/**
 * @brief       Kernel se izvrsava
 */
    ES_KERNEL_RUNNING
} esKernelStatus_T;

/**
 * @brief       Listing kodova gresaka koje se generisu kada je LOG sistem
 *              ukljucen
 */
enum esStatusCodes {
    ES_NO_ERROR,

/**
 * @brief       Argument nije ispravan, pokazuje na nesto drugo od ocekivanog.
 */
    ES_ARG_NOT_VALID = 0x100UL,

/**
 * @brief       Vrednost argumenta je van opsega dozvoljenih vrednosti.
 */
    ES_ARG_OUT_OF_RANGE,

/**
 * @brief       Pokazivac je NULL, a ocekuje se konkretna vrednost.
 */
    ES_ARG_NULL,

/**
 * @brief       Nema dovoljno memorije za alociranje.
 */
    ES_NOT_ENOUGH_MEM,

/**
 * @brief       Funkcija/objekat se koristi na nepravilan nacin
 */
    ES_USAGE_FAILURE
};

/**
 * @brief       Definiciona struktura koja opisuje jedan EPA objekat
 * @api
 */
typedef struct esEpaDef {
/**
 * @brief       Deskriptivno ime EPA objekta
 * @details     Pokazivac na znakovni niz koji cuva ime jedinice za obradu
 *              dogadjaja. Ime jedinice za obradu dogadjaja se moze cuvati samo
 *              u flash/ROM memoriji. Ime pruza samo deskriptivnu informaciju o
 *              EPA objektu i u druge svrhe se ne koristi. Vise EPA objekata
 *              mogu imati isto ime.
 */
    const PORT_C_ROM char * epaName;

/**
 * @brief       Prioritet EPA objekta
 * @details     Opseg vrednosti: 0 do @ref OPT_KERNEL_EPA_PRIO_MAX
 */
    uint8_t         epaPrio;

/**
 * @brief       Potrebna memorija radnog okruzenja za EPA objekat
 */
    size_t          epaWorkspaceSize;

/**
 * @brief       Velicina reda za cekanje za dogadjaje
 * @details     Opseg vrednosti: minimalno 1
 */
    size_t          evtQueueLevels;

/**
 * @brief       Inicijalno stanje automata
 */
/*    esState_T       smInitState; */

/**
 * @brief       Maksimalna dubina hijerarhije stanja automata.
 * @details     Opseg vrednosti: minimalno 2
 */
    uint8_t         smLevels;
} esEpaDef_T;

/**
 * @brief       Objekat Event Processing Agent-a
 * @api
 */
typedef struct esEpa esEpa_T;

/*======================================================  GLOBAL VARIABLES  ==*/

/**
 * TODO: Napraviti globalnu koja pokazuje kako je OS kompajliran, da bi
 * prekompajlirane biblioteke znale kako da se konfigurisu.
 */
extern const volatile uint8_t gKernelFeature;

enum kernelFeature {
    ES_DYNAMIC_MEMORY   = 0x00,
    ES_STATIC_MEMORY    = 0x01
};

#define ES_KERNEL_FEATURE(feature)                                              \
    (gKernelFeature & (1U << feature) ? TRUE : FALSE)

/*===================================================  FUNCTION PROTOTYPES  ==*/


/*------------------------------------------------------------------------*//**
 * @name        Osnovne funkcije za menadzment EPA objekata
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Kreira EPA objekat.
 * @param       [in] memClass           Klasa memorije koja se koristi za
 *                                      skladistenje:
 *  @arg        esMemDynClass
 *  @arg        esMemStaticClass
 * @param       [in] definition         Definiciona struktura EPA objekta.
 * @return      Pokazivac na strukturu zaglavlja EPA objekta.
 * @see         esEpaDef_T
 * @details     Nakon dobavljanja odgovarajuceg memorijskog prostora ova
 *              funkcija vrsi inicijalizaciju EPA objekta sa odgovarajucim
 *              parametrima.
 * @api
 * @error
 *              - @ref ES_ARG_NULL, jedan od argumenata je NULL pokazivac
 *              - @ref ES_ARG_NOT_VALID, argument memorijske klase nije
 *                  ispravan. Pogledati @ref esMemClass.
 *              - @ref ES_ARG_OUT_OF_RANGE, argumenti predati funkciji su
 *                  van moguceg opsega. Pogledati @ref esEpaDef_T.
 *//*
esEpa_T * esEpaCreate(
    const PORT_C_ROM esMemClass_T *  memClass,
    const PORT_C_ROM esEpaDef_T *    definition);
*/
/**
 * @brief       Unistava EPA objekat.
 * @param       [in] epa                Pokazivac na strukturu EPA objekta.
 * @details     Vrsi se oslobadjanje memorije ukoliko je EPA objekat koristio
 *              dinamicki memorijski alokator.
 * @api
 * @error
 *              - @ref ES_ARG_NULL
 *              - @ref ES_ARG_NOT_VALID - pokazivac @c epa pokazuje na neki
 *                  drugi objekat ili na EPA objekat koji je vec unisten.
 *              - @ref ES_USAGE_FAILURE - pokusaj brisanja EPA objekta koji
 *                  je statican.
 */
void esEpaDestroy(
    esEpa_T *       epa);

/**
 * @brief       Dobavlja prioritet EPA objekta
 * @param       [in] epa                Pokazivac na EPA objekat
 * @return      Trenutni prioritet EPA objekta.
 * @api
 * @error
 *              - @ref ES_ARG_NULL
 *              - @ref ES_ARG_NOT_VALID - pokazivac @c epa pokazuje na neki
 *                  drugi objekat ili na EPA objekat koji je vec unisten.
 */
uint8_t esEpaPrioGet(
    const esEpa_T * epa);

/**
 * @brief       Postavlja nov prioritet EPA objekta.
 * @param       [out] epa               Pokazivac na EPA objekat,
 * @param       [in] newPrio            nov prioritet EPA objekta.
 * @api
 * @error
 *              - @ref ES_ARG_NULL
 *              - @ref ES_ARG_NOT_VALID - pokazivac @c epa pokazuje na neki
 *                  drugi objekat ili na EPA objekat koji je vec unisten.
 *              - @ref ES_ARG_OUT_OF_RANGE - argument @c newPrio je van
 *                  dozvoljenog opsega. (@ref OPT_KERNEL_EPA_PRIO_MAX)
 *              - @ref ES_USAGE_FAILURE - vec postoji EPA objekat sa datim
 *                  @c newPrio prioritetom.
 */
void esEpaPrioSet(
    esEpa_T *       epa,
    uint8_t         newPrio);

/** @} *//*-------------------------------------------------------------------*/
/*------------------------------------------------------------------------*//**
 * @name        Osnovne funkcije kernel-a
 * @brief       Ove funkcije se koriste za upravljanjem kernel-om
 * @{ *//*--------------------------------------------------------------------*/

#if !defined(ES_HAL_ENABLE_STARTUP)
/**
 * @brief       Vrsi inicijalizaciju eSolid Kernel sistema
 * @details     Poziva se samo kada se ne koristi podrazumevani HAL start-up kod.
 *              U slučaju kada se ne koristi HAL start-up kod korisnik mora da
 *              explicitno pozove ovu funkciju pre pozivanja bilo koje funkcije
 *              iz eSolid Kernel sistema.
 */
void esKernelInit(
    void);
#endif

/**
 * @brief       Pokrece izvrsavanje jezgra
 * @details     Pokrecu se svi prethodno kreirani EPA objekti.
 * @api
 */
void esKernelStart(
    void);

/**
 * @brief       Vraca odgovor da li trenutno radi kernel
 * @return      Status izvrsavanja kernel-a.
 *  @retval     ES_KERNEL_STOPPED - kernel se ne izvrsava,
 *  @retval     ES_KERNEL_RUNNING - kernel se izvrsava,
 * @api
 */
esKernelStatus_T esKernelStatus(
    void);

/**
 * @brief       Vraca Id pokazivac EPA objekta.
 * @return      Id pokazivac trenutnog EPA objekta koji se izvrsava.
 * @api
 */
esEpa_T * esKernelEpaGet(
    void);

/** @} *//*-------------------------------------------------------------------*/
/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of kernel.h
 ******************************************************************************/
#endif /* KERNEL_H_ */
