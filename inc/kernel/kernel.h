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
#include "kernel/epa.h"

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
    KERNEL_STOPPED,

/**
 * @brief       Kernel se izvrsava
 */
    KERNEL_RUNNING
} esKernelStatus_T;

/**
 * @brief       Listing kodova gresaka koje se generisu kada je LOG sistem
 *              ukljucen
 */
enum esErrorCodes {
/**
 * @brief       Argument nije ispravan, pokazuje na nesto drugo od ocekivanog.
 */
    ES_ERR_ARG_NOT_VALID = 0x100UL,

/**
 * @brief       Vrednost argumenta je van opsega dozvoljenih vrednosti.
 */
    ES_ERR_ARG_OUT_OF_RANGE,

/**
 * @brief       Pokazivac je NULL, a ocekuje se konkretna vrednost.
 */
    ES_ERR_ARG_NULL,

/**
 * @brief       Nema dovoljno memorije za alociranje.
 */
    ES_ERR_NOT_ENOUGH_MEM,

/**
 * @brief       Funkcija/objekat se koristi na nepravilan nacin
 */
    ES_ERR_USAGE_FAILURE
};

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Osnovne funkcije kernel-a
 * @brief       Ove funkcije se koriste za upravljanjem kernel-om
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Inicijalizuje kernel.
 * @details     Najpre vrsi inicijalizaciju HAL-a, zatim memorijskog menadzera,
 *              SMP modula i prelazi na inicijalizaciju samog kernela.
 * @api
 */
void esKernelInit(
    void);

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
 *  @retval     KERNEL_STOPPED - kernel se ne izvrsava,
 *  @retval     KERNEL_RUNNING - kernel se izvrsava,
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
