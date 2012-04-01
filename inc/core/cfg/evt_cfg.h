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
 *************************************************************************************************/


/*********************************************************************************************//**
 * @file
 * @author      Nenad Radulovic
 * @brief       Konfiguracija Event Object (EO) modula.
 * ------------------------------------------------------------------------------------------------
 * @addtogroup  core_cfg
 ****************************************************************************************//** @{ */

#ifndef EVT_CFG_H_
#define EVT_CFG_H_

/*************************************************************************************************
 * INCLUDE FILES
 *************************************************************************************************/


/*************************************************************************************************
 * DEFINES
 *************************************************************************************//** @cond */


/** @endcond *//**********************************************************************************
 * SETTINGS
 *************************************************************************************************/

/*-------------------------------------------------------------------------------------------*//**
 * @name        Podesavanje Event Object modula
 * @details     Ovim opcijama se specificira:
 *              - debug podrska,
 *              - metod pakovanja strukture zaglavlja dogadjaja,
 *              - struktura zaglavlja dogadjaja i koja,
 *              - tipovi clanova strukture zaglavlja dogadjaja.
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Debug podrska Event Object modula
 * @details     Ukoliko je ova opcija:
 *              - definisana: omoguceno je debagiranje modula,
 *              - nedefinisana: nije omoguceno debagiranje modula.
 *
 *              Omogucavanjem debagiranja mogu da se ukljucuje sledeci makroi:
 *              - @ref DBG_ASSERT
 *              - @ref DBG_ASSERT_ALWAYS
 *              - @ref DBG_ASSERT_COMPILE
 *              - @ref DBG_CHECK
 *              - @ref DBG_ENTRY
 *              - @ref DBG_EXIT
 *
 * @see         dbg_intf
 *//*--------------------------------------------------------------------------------------------*/
#if defined(__DOXYGEN__)
# define OPT_DBG_EO
#endif

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Koristi se atribut o velicini dogadjaja
 * @details     Moguce vrednosti:
 *              - nedefinisano - ne koristi se atribut o velicini
 *              - definisano   - koristi se atribut o velicini
 *//*--------------------------------------------------------------------------------------------*/
#if defined(__DOXYGEN__)
# define OPT_EVT_USE_SIZE
#endif

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Koristi se pokazivac na proizvodjaca dogadjaja.
 * @details     Moguce vrednosti:
 *              - nedefinisano - pokazivac na objekat se ne koristi
 *              - definisano   - pokazivac na objekat se koristi
 *//*--------------------------------------------------------------------------------------------*/
#if defined(__DOXYGEN__)
# define OPT_EVT_USE_TRANSMITTER
#endif

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Koristi se vremenski marker dogadjaja.
 * @details     Moguce vrednosti:
 *              - nedefinisano - vrem. marker se ne koristi
 *              - definisano   - vrem. marker se koristi
 *
 *              Podesavanje tipa se vrsi pomocu @ref OPT_EVT_TIME_T.
 *//*--------------------------------------------------------------------------------------------*/
#if defined(__DOXYGEN__)
# define OPT_EVT_USE_TIME
#endif

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Koristi se tip dogadjaja.
 * @details     Moguce vrednosti:
 *              - nedefinisano - tip dogadjaja se ne koristi
 *              - definisano   - tip dogadjaja se koristi
 *
 *              Podesavanje tipa se vrsi pomocu @ref OPT_EVT_TYPE_T.
 *//*--------------------------------------------------------------------------------------------*/
#if defined(__DOXYGEN__)
# define OPT_EVT_USE_TYPE
#endif

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Tip promenljive za identifikator dogadjaja.
 * @details     Tip podatka treba da dovoljne velicine moze da predstavi sve
 *              dogadjaje u sistemu. Na primer, ako u sistemu postoje manje od
 *              255 identifikatora dogadjaja onda ovaj tip moze najmanje biti
 *              uint8_t tipa. Ukoliko sistem poseduje vise od 255 razlicitih
 *              dogadjaja onda ovaj tip treba da bude najmanje uint16_t. Naravno,
 *              treba voditi racuna o zauzecu strukture koja opisuje dogadjaj i
 *              njenom poravnjanju (alignement) u memoriji, gde se moze javiti
 *              potreba za vecim tipom od minimalnog.
 * @note        Ovo podesavanje moze da se zaobidje definisanjem OPT_EVT_ID_T.
 *//*--------------------------------------------------------------------------------------------*/
#if defined(__DOXYGEN__)
# define OPT_EVT_ID_T
#endif

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Tip podataka za cuvanje velicine prosirenog dogadjaja.
 * @note        Ovo podesavanje moze da se zaobidje definisanjem OPT_EVT_SIZE_T.
 *//*--------------------------------------------------------------------------------------------*/
#if defined(__DOXYGEN__)
# define OPT_EVT_SIZE_T
#endif

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Tip za cuvanje vremenskih atributa dogadjaja.
 * @note        Ovo podesavanje moze da se zaobidje definisanjem OPT_EVT_TIME_T.
 *//*--------------------------------------------------------------------------------------------*/
#if defined(__DOXYGEN__)
# define OPT_EVT_TIME_T
#endif

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Tip za cuvanje tipa dogadjaja.
 * @note        Ovo podesavanje moze da se zaobidje definisanjem OPT_EVT_TYPE_T.
 *//*--------------------------------------------------------------------------------------------*/
#if defined(__DOXYGEN__)
# define OPT_EVT_TYPE_T
#endif

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Atribut za strukture dogadaja
 * @details     Prilikom slanja dogadjaja drugim sistemima javlja se problem
 *              pakovanja podataka unutar strukture dogadjaja.
 *
 *              Ovom promenljivom se moze definisati koja direktiva ce koristi
 *              za strukture dogadjaja (poravnjanje, pakovanje).
 *//*--------------------------------------------------------------------------------------------*/
#if !defined(OPT_EVT_STRUCT_ATTRIB) || defined(__DOXYGEN__)
# define OPT_EVT_STRUCT_ATTRIB
#endif

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Dobavlja vremenski marker za dogadjaje
 *//*--------------------------------------------------------------------------------------------*/
#if !defined(EVT_TIMESTAMP_GET) || defined(__DOXYGEN__)
# define EVT_TIMESTAMP_GET()            0U
#endif
/** @} *//*--------------------------------------------------------------------------------------*/


/*************************************************************************************************
 * CONFIGURATION RESOLUTION
 *************************************************************************************************/

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Tip podataka za identifikator dogadjaja
 *//*--------------------------------------------------------------------------------------------*/
#if defined(OPT_EVT_ID_T)
typedef OPT_EVT_ID_T                    esEvtId_T;
#else
typedef uint8_t                         esEvtId_T;
#endif

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Tip podataka za vremenski marker dogadjaja
 *//*--------------------------------------------------------------------------------------------*/
#if defined(OPT_EVT_USE_TIME) || defined(__DOXYGEN__)
# if defined(OPT_EVT_TIME_T)
typedef OPT_EVT_TIME_T                  esEvtTime_T;
# else
typedef uint32_t                        esEvtTime_T;
# endif
#endif

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Tip podataka za atribut velicine dogadjaja
 *//*--------------------------------------------------------------------------------------------*/
#if defined(OPT_EVT_USE_SIZE) || defined(__DOXYGEN__)
# if defined(OPT_EVT_SIZE_T)
typedef OPT_EVT_SIZE_T                  esEvtSize_T;
# else
typedef uint8_t                         esEvtSize_T;
# endif
#endif

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Tip podataka za atribut tipa dogadjaja
 *//*--------------------------------------------------------------------------------------------*/
#if defined(OPT_EVT_USE_TYPE) || defined(__DOXYGEN__)
# if defined(OPT_EVT_TYPE_T)
typedef OPT_EVT_TYPE_T                  esEvtType_T;
# else
typedef uint8_t                         esEvtType_T;
# endif
#endif

/** @} *//*--------------------------------------------------------------------------------------*/


/** @} *//****************************************************************************************
 * END of evt_cfg.h
 *************************************************************************************************/
#endif /* EVT_CFG_H_ */
