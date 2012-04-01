/*************************************************************************************************
 * This file is part of eSolid
 *
 * Copyright (C) 2011 - Nenad Radulovic
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
 *************************************************************************************************/


/*********************************************************************************************//**
 * @file
 *
 * @author      nesa
 *
 * @brief       Interfejs za GPIO modul.
 *
 * ------------------------------------------------------------------------------------------------
 *
 * @addtogroup  hal_gpio
 *
 ****************************************************************************************//** @{ */


#ifndef HAL_GPIO_H_
#define HAL_GPIO_H_


/*************************************************************************************************
 * INCLUDE FILES
 *************************************************************************************************/

/*-----------------------------------------------------------------------------------------------*
 * Module dependencies
 *-----------------------------------------------------------------------------------------------*/

#include "port/port.h"


/*-----------------------------------------------------------------------------------------------*
 * Module configuration and adapter
 *-----------------------------------------------------------------------------------------------*/

#include PORT_VARIANT_HEADER(hal/gpio)


/*-----------------------------------------------------------------------------------------------*
 * EXTERNS
 *-----------------------------------------------------------------------------------*//** @cond */

#ifdef HAL_GPIO_H_VAR
#define HAL_GPIO_H_EXT
#else
#define HAL_GPIO_H_EXT extern
#endif


/** @endcond*//***********************************************************************************
 * DEFINES
 *************************************************************************************************/

/*-------------------------------------------------------------------------------------------*//**
 * @name        Smer podataka - direction konstante
 *
 * @brief       Konstante koje definisu smer kretanja podataka na portu.
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Pin je ulaz.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_GPIO_IN) || defined(__DOXYGEN__)
# define HAL_GPIO_IN
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Pin je izlaz.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_GPIO_OUT) || defined(__DOXYGEN__)
# define HAL_GPIO_OUT
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Pin poseduje drugu funkciju.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_GPIO_AF) || defined(__DOXYGEN__)
# define HAL_GPIO_AF
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Pin je u analognom rezimu rada.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_GPIO_AN) || defined(__DOXYGEN__)
# define HAL_GPIO_AN
#endif


/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Izlazni rezim rada - omode konstante
 *
 * @brief       Konstante koje definisu moguce rezime rada za pin koji je
 *              konfigurisan kao izlaz.
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Izlaz je u push-pull konfiguraciji.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_GPIO_PP) || defined(__DOXYGEN__)
# define HAL_GPIO_PP
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Izlaz je u open-drain konfiguraciji.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_GPIO_OD) || defined(__DOXYGEN__)
# define HAL_GPIO_OD
#endif


/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Ulazni rezim rada - imode konstante
 *
 * @brief       Konstante koje definisu moguce rezime rada za pin koji je
 *              konfigurisan kao ulaz ili izlaz.
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Pin ima pull-up otpornik.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_GPIO_PU) || defined (__DOXYGEN__)
# define HAL_GPIO_PU
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Pin ima pull-down otpornik.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_GPIO_PD) || defined (__DOXYGEN__)
# define HAL_GPIO_PD
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Pin ima weak pull-up otpornik.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_GPIO_WPU) || defined (__DOXYGEN__)
# define HAL_GPIO_WPU
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Pin ima weak pull-down otpornik.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_GPIO_WPD) || defined(__DOXYGEN__)
# define HAL_GPIO_WPD
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Pin nema pull-up ili pull-down otpornik.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_GPIO_NP) || defined(_DOXYGEN__)
# define HAL_GPIO_NP
#endif

/** @} *//*--------------------------------------------------------------------------------------*/


/*************************************************************************************************
 * MACRO's
 *************************************************************************************************/

/*-------------------------------------------------------------------------------------------*//**
 * @name        Makroi za inicijalizaciju GPIO modula.
 *
 * @brief       GPIO varijanta i GPIO arhitektura treba da pruze pravu
 *              implementaciju makroa. U ovom odelju se nalaze samo prazne
 *              deklaracije.
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Izvrsi inicijalizaciju @c pina na datom @c portu.
 *
 * @param       port                    Port na kome se nalazi pin koji se
 * 										konfigurise,
 *  @arg		0 - n					, gde je @c n broj GPIO portova na datom
 *  									kontroleru
 * @param       pins                    binarna maska pinova koji se konfigurisu,
 * @param       direction               smer podataka,
 *  @arg		HAL_GPIO_IN				- izabrani pinovi su ulaz
 *  @arg		HAL_GPIO_OUT			- izabrani pinovi su izlaz
 * @param       omode                   izlazni rezim rada pina (zanemaruje se
 * 										ukoliko su pinovi ulaznog tipa),
 *  @arg		HAL_GPIO_PP				- izlaz je push-pull
 *  @arg		HAL_GPIO_OD             - izlaz je open-drain
 * @param       imode                   ulazni rezim rada pina,
 *  @arg		HAL_GPIO_PU				- aktivan je pull-up otpornik
 *  @arg		HAL_GPIO_PD				- aktivan je pull-down otpornik
 *  @arg		HAL_GPIO_WPU			- aktivan je weak pull-up otpornik
 *  @arg		HAL_GPIO_WPD			- aktivan je weak pull-down otpornik
 *  @arg		HAL_GPIO_NP				- nije aktivan ni jedan pull otpornik
 * @param       speed                   brzina rada GPIO pina.
 *
 * @note        Kod pojedinih portova neke vrednosti parametara namaju uticaja
 * 				na konfiguraciju.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_GPIO_INIT) || defined(__DOXYGEN__)
# define HAL_GPIO_INIT(port, pins, direction, omode, imode, speed)
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Izvrsava inicijalizaciju GPIO porta na podrazumevane vrednosti.
 *
 * @param       port                    Port na kome se nalazi pin,
 * @param       pin                     pin koji se postavlja na podrazumevanu
 * 										vrednost.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_GPIO_DEINIT) || defined(__DOXYGEN__)
# define HAL_GPIO_DEINIT(port, pin)
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Remapiranje GPIO pinova.
 *
 * @param       gpioremap               Periferija za koju se vrsi remapiranje,
 * @param       command                 Komanda za dozvolu ili zabranu remapiranja.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_GPIO_REMAP_CMD) || defined(__DOXYGEN__)
# define HAL_GPIO_REMAP_CMD(gpioremap, command)
#endif

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Osnovni brzi makroi za rad sa jednim pinom.
 *
 * @brief       GPIO varijanta i GPIO arhitektura treba da pruze pravu
 *              implementaciju makroa. U ovom odelju se nalaze samo prazne
 *              deklaracije.
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Postavi @c pin na definisanom @c portu na stanje @a nisko.
 *
 *              Pre pozivanja ovog makroa potrebno je izvrsiti inicijalizaciju
 *              hardvera.
 *
 * @param       port                    Port na kome se nalazi pin,
 * @param       pin                     pin koji se postavlja na @c nisko.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_GPIO_LOW) || defined(__DOXYGEN__)
# define HAL_GPIO_LOW(port, pin)
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Postavi @c pin na definisanom @c portu na stanje @a visoko.
 *
 * @param       port                    Port na kome se nalazi pin,
 * @param       pin                     pin koji se postavlja na @c visoko.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_GPIO_HIGH) || defined(__DOXYGEN__)
# define HAL_GPIO_HIGH(port, pin)
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Invertuj @c pin na definisanom @c portu.
 *
 * @param       port                    Port na kome se nalazi pin,
 * @param       pin                     pin koji se invertuje.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_GPIO_TOGGLE) || defined(__DOXYGEN__)
# define HAL_GPIO_TOGGLE(port, pin)
#endif

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Citanje i upis bita i podatka velicine 8, 16 i 32 bita.
 *
 * @brief       GPIO varijanta i GPIO arhitektura treba da pruze pravu
 *              implementaciju makroa. U ovom odelju se nalaze samo prazne
 *              deklaracije.
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Procitaj stanje jedno bita na portu.
 *
 * @param       port                    Port sa kojeg se cita,
 * @param       bit                     bit cija se vrednost zahteva.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_GPIO_RD_BIT) || defined(__DOXYGEN__)
# define HAL_GPIO_RD_BIT(port, bit)
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Procitaj jedan 8-bitni podatak sa porta.
 *
 * @param       port                    Port sa kojeg se cita jedan bajt podataka.
 * @return      Vrednost jednog bajta sa porta (uint8_t)
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_GPIO_RD_U8) || defined(__DOXYGEN__)
# define HAL_GPIO_RD_U8(port)
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Procitaj jedan 16-bitni podatak sa porta.
 *
 * @param       port                    Port sa kojeg se cita podatak.
 * @return      Vrednost 16-bitnog podatka sa porta (uint16_t)
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_GPIO_RD_U16) || defined(__DOXYGEN__)
# define HAL_GPIO_RD_U16(port)
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Procitaj jedan 32-bitni podataka sa porta.
 *
 * @param       port                    Port sa kojeg se cita podatak.
 * @return      Vrednost 32-bitnog podatka sa porta (uint32_t)
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_GPIO_RD_U32) || defined(__DOXYGEN__)
# define HAL_GPIO_RD_U32(port)
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Upisi vrednost jednog bita na port.
 *
 * @param       port                    Port na kojem se nalazi bit,
 * @param       bit                     bit koji se modifikuje,
 * @param       value                   vrednost koja se upisuje.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_GPIO_WR_BIT) || defined(__DOXYGEN__)
# define HAL_GPIO_WR_BIT(port, bit, value)
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Upisi vrednost 8-bitnog podatka na port.
 *
 * @param       port                    Port koji se modifikuje,
 * @param       value                   vrednost koju treba upisati.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_GPIO_WR_U8) || defined(__DOXYGEN__)
# define HAL_GPIO_WR_U8(port, value)
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Upisi vrednost 16-bitnog podatka na port.
 *
 * @param       port                    Port koji se modifikuje,
 * @param       value                   vrednost koju treba upisati.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_GPIO_WR_U16) || defined(__DOXYGEN__)
# define HAL_GPIO_WR_U16(port, value)
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Upisu vrednost 32-bitnog podatka na port.
 *
 * @param       port                    Port koji se modifikuje,
 * @param       value                   vrednost koju treba upisati.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_GPIO_WR_U32) || defined(__DOXYGEN__)
# define HAL_GPIO_WR_U32(port, value)
#endif

/** @} *//*--------------------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------------------------*
 * C/C++ #ifdef - open
 *-----------------------------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif


/*************************************************************************************************
 * DATA TYPES
 *************************************************************************************************/


/*************************************************************************************************
 * GLOBAL VARIABLES
 *************************************************************************************************/


/*************************************************************************************************
 * FUNCTION PROTOTYPES
 *************************************************************************************************/


/*-----------------------------------------------------------------------------------------------*
 * C/C++ #endif - close
 *-----------------------------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif


/*************************************************************************************************
 * CONFIGURATION ERRORS
 *************************************************************************************//** @cond */


/** @endcond *//** @} *//**************************************************************************
 * END of hal_gpio.h
 *************************************************************************************************/
#endif /* HAL_GPIO_H_ */
