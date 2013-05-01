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
 * @brief       Konfiguracija eSolid Sistema
 * @defgroup    sys_cfg System configuration
 *********************************************************************//** @{ */

#ifndef SYS_CONFIG_H_
#define SYS_CONFIG_H_

/*=========================================================  INCLUDE FILES  ==*/
/*===============================================================  DEFINES  ==*/
/** @cond */

#define OPT_SYS_ENABLE_DMEM

/** @endcond */
/*==============================================================  SETTINGS  ==*/

/**
 * @brief       Maksimalan prioritet prekidnih rutina kernela
 * @details     Ovim se ogranicava prioritet prekidnih rutina koje jezgro
 *              koristi. Ovo podesavanje je izuzetno korisno kada postoji
 *              potreba da se pojedini hardverski prekidi ne prekidaju od strane
 *              jezgra.
 * @note        Podrazumevano podesavanje: ES_PRIO_REALTIME
 */
#if !defined(OPT_SYS_INTERRUPT_PRIO_MAX)
# define OPT_SYS_INTERRUPT_PRIO_MAX     ES_PRIO_REALTIME
#endif

/**
 * @brief       Omogucavanje dinamickog memorijskog menadzmenta
 * @details     Ukoliko se koristi eSolid dinamicki alokator ova opcija mora
 *              biti definisana.
 */
#if defined(__DOXYGEN__)
# define OPT_SYS_ENABLE_DMEM
#endif

/**
 * @brief       Omogucavanje LOGer-a
 */
#if defined(__DOXYGEN__)
# define OPT_SYS_ENABLE_LOG
#endif

/**
 * @brief       Omogucavanje integracije eSolid-a sa sistemskom bibliotekom
 */
#if defined(__DOXYGEN__)
# define OPT_SYS_ENABLE_SYSCALLS
#endif

#include "mem_config.h"
#include "log_config.h"

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of sys_config.h
 ******************************************************************************/
#endif /* SYS_CONFIG_H_ */
