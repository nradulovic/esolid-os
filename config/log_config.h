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
 * @author  	Nenad Radulovic
 * @brief       Konfiguracija LOGer sistema
 * @addtogroup  log_cfg
 *********************************************************************//** @{ */

#ifndef LOG_CONFIG_H_
#define LOG_CONFIG_H_

/*=========================================================  INCLUDE FILES  ==*/
/*===============================================================  DEFINES  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Predefinisane konstante za odabir opcija
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Koristi se za opciju @ref OPT_LOG_LEVEL : Ukljucena su samo
 *              upozorenja tipa @c error.
 */
#define LOG_ERR                         5U

/**
 * @brief       Koristi se za opciju @ref OPT_LOG_LEVEL : Ukljucena su
 *              upozorenja tipa: @c error i @c warning.
 */
#define LOG_WARN                        4U

/**
 * @brief       Koristi se za opciju @ref OPT_LOG_LEVEL : Ukljucena su
 *              upozorenja tipa: @c error,@c warning i @c info.
 */
#define LOG_INFO                        3U

/**
 * @brief       Koristi se za opciju @ref OPT_LOG_LEVEL : Ukljucena su
 *              upozorenja tipa: @c error,@c warning, @c info i @c debug.
 */
#define LOG_DBG                         2U

/**
 * @brief       Koristi se za opciju @ref OPT_LOG_LEVEL : Ukljucena su
 *              upozorenja tipa: @c error,@c warning, @c info,@c debug i
 *              @c trace.
 */
#define LOG_TRACE                       1U

#define LOG_DISABLED                    6U

/**
 * @brief       Koristi se za opciju @ref OPT_LOG_MSG_TYPE : LOG sistem salje
 *              poruke u vidu teksta
 */
#define LOG_MSG_IS_TEXT                 1U

/**
 * @brief       Koristi se za opciju @ref OPT_LOG_MSG_TYPE : LOG sistem salje
 *              poruke u vidu enumeratora
 */
#define LOG_MSG_IS_ENUM                 2U

/** @} *//*-------------------------------------------------------------------*/
/*==============================================================  SETTINGS  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Definition group
 * @brief       brief description
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Definisanje nivoa upisivanja u LOG
 * @details     LOG sistem definise nivoe upisivanja u LOG. Najvisi nivo je nivo
 *              greske (@ref LOG_ERR), a najnizi nivo je nivo pracenja (
 *              @ref LOG_TRACE).
 *              Opcije:
 *              - @ref LOG_ERR
 *              - @ref LOG_WARN
 *              - @ref LOG_INFO
 *              - @ref LOG_DBG
 *              - @ref LOG_TRACE
 *              - @ref LOG_DISABLED
 * @note        Podrazumevano podesavanje: @ref LOG_TRACE
 */
#if !defined(OPT_LOG_LEVEL) || defined(__DOXYGEN__)
# define OPT_LOG_LEVEL                  LOG_TRACE
#endif

/**
 * @brief       Definisanje tipa poruke
 * @details     Poruke je moguce slati u vidu teksta ili u vidu rednog broja
 *              poruke. Slanje poruka u vidu teksta zahteva vise RAM prostora i
 *              veci broj instrukcija se trosi u LOG sistemu. Prebacivanjem na
 *              redni broj, smanjuje se potrosnja RAM memorije i broj utrosenih
 *              ciklusa u LOG sistemu.
 *              Opcije:
 *              - @ref LOG_MSG_IS_TEXT - poruke su tekstualnog tipa
 *              - @ref LOG_MSG_IS_ENUM - poruke su nabrojanog tipa.
 * @note        Podrazumevano podesavanje: @ref LOG_MSG_IS_TEXT
 */
#if !defined(OPT_LOG_MSG_TYPE) || defined(__DOXYGEN__)
# define OPT_LOG_MSG_TYPE               LOG_MSG_IS_TEXT
#endif

/** @} *//*-------------------------------------------------------------------*/
/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of log_config.h
 ******************************************************************************/
#endif /* LOG_CONFIG_H_ */
