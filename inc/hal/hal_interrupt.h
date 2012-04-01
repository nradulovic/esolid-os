/*************************************************************************************************
 * Standard header file: hal_interrupt.h
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
 * @author      Nenad Radulovic
 *
 * @brief       Interfejs za rad sa prekidima.
 *
 * ------------------------------------------------------------------------------------------------
 *
 * @addtogroup  intr_intf
 *
 ****************************************************************************************//** @{ */


#ifndef HAL_INTERRUPT_H_
#define HAL_INTERRUPT_H_


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
#include PORT_ARCH_HEADER(hal/interrupt)


/*-----------------------------------------------------------------------------------------------*
 * EXTERNS
 *-----------------------------------------------------------------------------------*//** @cond */


#ifdef HAL_INTERRUPT_H_VAR
#define HAL_INTERRUPT_H_EXT
#else
#define HAL_INTERRUPT_H_EXT extern
#endif


/** @endcond*//***********************************************************************************
 * DEFINES
 *************************************************************************************************/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Maksimalni prioritet koji eSolid HAL koristi.
 *
 *              Ova opcija ogranicava maksimalni prioritet izvrsavanja eSolid
 *              kriticnih sekcija. Ovo omogucava da se koriste odredjeni prekidi
 *              u sistemu i u toku izvrsenja kriticnih sekcija koda.
 *
 * @note        Ovo podesavanje moze da se zaobidje definisanjem
 *              OPT_MAX_INTERRUPT_PRIO.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(OPT_MAX_INTERRUPT_PRIO) || defined(__DOXYGEN__)
# define OPT_MAX_INTERRUPT_PRIO
#endif

/*************************************************************************************************
 * MACRO's
 *************************************************************************************************/

/*-------------------------------------------------------------------------------------------*//**
 * @name        Kontrola glavnog prekida
 *
 * @brief       Makroi za omogucavanje/onemogucavanje svih prekida u sistemu.
 *
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Omoguci prekide
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_INT_ENABLE) || defined(__DOXYGEN__)
# define HAL_INT_ENABLE()
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Onemoguci prekide
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_INT_DISABLE) || defined(__DOXYGEN__)
# define HAL_INT_DISABLE()
#endif

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Maskiranje prioriteta prekidnih rutina
 *
 * @brief       Ovi makroi se koriste za manipulaciju sa globalnom maskom
 *              prioriteta prekidnih rutina.
 *
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Postavi masku prioriteta
 *
 *              Ovim makroom se postavlja granica prioriteta iznad koje mogu da
 *              se dese prekidi. Najvisi prioritet prekida je 0, a najnizi
 *              prioritet je 255. Maksimalni prioritet koji se moze podesiti
 *              ovim makroom je 1, je postavljanje na 0 efektivno onemogucuje
 *              sve prekide. Zbog toga je 0 rezervisana za iskljucivanje maske.
 *              Ukoliko korisnik zeli da iskljuci sve prekide onda treba da
 *              koristi HAL_INT_DISABLE. Podrazumevano, maska je 0, sto znaci
 *              da su prekidi svih prioriteta omoguceni.
 *
 * @param       prio                    Nova maska prioriteta.
 *  @arg        0                       - maskiranje je onemoguceno
 *  @arg        1-255                   - maskiranje je omoguceno
 *
 * @note        Parametar @c prio mora da bude konstanta.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_INT_PRIO_MASK_SET) || defined(__DOXYGEN__)
# define HAL_INT_PRIO_MASK_SET(prio)
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Dobavi trenutnu masku prioriteta prekidnih rutina.
 *
 * @return      trenutni prioritet.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_INT_PRIO_MASK_GET) || defined(__DOXYGEN__)
# define HAL_INT_PRIO_MASK_GET()
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Sacuvaj trenutnu masku prioriteta i postavi novu masku
 *              prioriteta prekidnih rutina.
 *
 * @param       variable                Promenljiva u kojoj treba sacuvati
 *                                      trenutnu masku,
 * @param       prio                    nova maska prioriteta.
 *
 * @note        Parametar @c prio mora da bude konstanta.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_INT_PRIO_MASK_SAVE_SET) || defined(__DOXYGEN__)
# define HAL_INT_PRIO_MASK_SAVE_SET(variable, prio)
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Vrati masku prioriteta koja je snimljena u promenljivoj
 *              @c variable.
 *
 * @param       variable                Promenljiva koja je cuvala prethodno
 *                                      sacuvanu masku prioriteta.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_INT_PRIO_MASK_RESTORE) || defined(__DOXYGEN__)
# define HAL_INT_PRIO_MASK_RESTORE(variable)
#endif

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Ulazak i izlazak iz kriticnih sekcija koda
 *
 * @brief       Kriticne sekcije koda su zasticene od odredjenih prekida.
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Deklaracija promenljive u kojoj se cuva trenutnu masku
 *              prioriteta tekuceg koda.
 *
 *              Ovaj makro se koristi u kombinaciji sa makroima za ulazak i
 *              izlazak iz kriticnih sekcija koda.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_CRITICAL_DECL) || defined(__DOXYGEN__)
# define HAL_CRITICAL_DECL()
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Dobavlja trenutno stanje prekida
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_CRITICAL_GET) || defined(__DOXYGEN__)
# define HAL_CRITICAL_GET()
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Ulazak u kriticni deo koda, postavlja se najvisi dozvoljeni
 *              prioritet za eSolid
 *
 *              Najvisi dozvoljeni prioritet se definise pomocu
 *              @ref OPT_MAX_INTERRUPT_PRIO
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_CRITICAL_ENTER) || defined(__DOXYGEN__)
# define HAL_CRITICAL_ENTER()
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Izlazak iz kriticnog dela koda, vraca se prethodni prioritet.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_CRITICAL_EXIT) || defined(__DOXYGEN__)
# define HAL_CRITICAL_EXIT()
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


/** @endcond *//** @} *//*************************************************************************
 * END of hal_interrupt.h
 *************************************************************************************************/
#endif /* HAL_INTERRUPT_H_ */
