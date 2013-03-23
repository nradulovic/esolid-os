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
 * @brief       LOGer API
 * @addtogroup  log_intf
 *********************************************************************//** @{ */

#ifndef LOG_H_
#define LOG_H_

/*=========================================================  INCLUDE FILES  ==*/
#include "hal/hal.h"
#include "../config/log_config.h"

/*===============================================================  DEFINES  ==*/
/*===============================================================  MACRO's  ==*/
/*------------------------------------------------------------------------*//**
 * @name        Makroi koji se koriste prilikom generisanja tabela
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Kreira enumeratore iz LOG tabele
 */
#define EXPAND_AS_ENUMERATOR(num, text) num,

/**
 * @brief       Kreira tekst iz LOG tabele
 */
#define EXPAND_AS_TEXT(num, text) [num] = text,

/**
 * @brief       Kreira sizeof(tekst) iz LOG tabele
 */
#define EXPAND_AS_SIZE(num, text) sizeof(text),

/** @} *//*-------------------------------------------------------------------*/

#if (OPT_LOG_LEVEL <= LOG_ERR)
/**
 * @brief       Obelezava greske u sistemu
 */
# define ES_LOG_ERR(log, package, msg, value)                                   \
    do {                                                                        \
        if (log.switches & ((package) | (1UL << (LOG_ERR + 24)))) {             \
            logMsg(&log, msg, value);                                           \
        }                                                                       \
    } while (0UL);

/**
 * @brief       Osigurava istinitost uslova @c condition
 */
# define ES_LOG_ASSERT(condition, log, msg, value)                              \
    do {                                                                        \
        if (!(condition)) {                                                     \
            logMsg(&log, msg, value);                                           \
        }                                                                       \
        ES_CPU_STOP();                                                          \
    } while (0UL);
#else
# define ES_LOG_ERR(log, package, msg, value)
#endif

/**
 * @brief       Obelezava upozorenja
 */
#if (OPT_LOG_LEVEL <= LOG_WARN)
# define ES_LOG_WARN(log, package, msg, value)                                  \
    do {                                                                        \
        if (log.switches & ((package) | (1UL << (LOG_WARN + 24)))) {            \
            logMsg(&log, msg, value);                                           \
        }                                                                       \
    } while (0UL);
#else
# define ES_LOG_WARN(log, package, msg, value)
#endif

/**
 * @brief       Obelezava informativne dogadjaje
 */
#if (OPT_LOG_LEVEL <= LOG_INFO)
# define ES_LOG_INFO(log, package, msg, value)                                  \
    do {                                                                        \
        if (log.switches & ((package) | (1UL << (LOG_INFO + 24)))) {            \
            logMsg(&log, msg, value);                                           \
        }                                                                       \
    } while (0UL);
#else
# define ES_LOG_INFO(log, package, msg, value)
#endif

/**
 * @brief       Obelezava dogadjaje sa informacijama o greskama
 */
#if (OPT_LOG_LEVEL <= LOG_DBG)
# define ES_LOG_DBG(log, package, msg, value)                                   \
    do {                                                                        \
        if (log.switches & ((package) | (1UL << (LOG_DBG + 24)))) {             \
            logMsg(&log, msg, value);                                           \
        }                                                                       \
    } while (0UL);
#else
# define ES_LOG_DBG(log, package, msg, value)
#endif

/**
 * @brief       Obelezava dogadjaje izvrsenja koda
 */
#if (OPT_LOG_LEVEL <= LOG_DBG)
# define ES_LOG_DBG(log, package, msg, value)                                   \
    do {                                                                        \
        if (log.switches & ((package) | (1UL << (LOG_DBG + 24)))) {             \
            logMsg(&log, msg, value);                                           \
        }                                                                       \
    } while (0UL);
#else
# define ES_LOG_DBG(log, package, msg, value)
#endif
/*------------------------------------------------------  C++ extern begin  --*/
#if defined(__cplusplus)
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/**
 * @brief       Struktura koja opisuje jedan LOG
 * @details     Ova struktura se instacira u ROM memoriji kako bi se smanjila
 *              potrosnja RAM memorije.
 */
typedef struct esLogDescriptor {
/**
 * @brief       Broj poruka u tabeli
 */
    uint32_t        entries;

/**
 * @brief       Tabela teksta
 */
    const C_ROM char * C_ROM_VAR const * textTable;

/**
 * @brief       Tabela velicina teksta
 */
    const C_ROM size_t * sizeTable;

/**
 * @brief       Funkcija koja se poziva kada zeli da se upise poruka,
 *              dobavljanje bloka memorije.
 */
    void * (* getBlock)(size_t);

/**
 * @brief       Funkcija koja se poziva kada je zavrsen upis podataka, predaja
 *              bloka memorije na slanje.
 */
    void (* pushBlock)(void *);
} esLogDescriptor_T;

/**
 * @brief       LOG instanca
 */
typedef struct esLog {
/**
 * @brief       Prekidaci ukljucenih poruka
 */
    uint32_t        switches;

/**
 * @brief       Pokazivac na deskriptor strukturu datog LOG-a
 */
    const C_ROM esLogDescriptor_T * C_ROM_VAR logDescriptor;
} esLog_T;

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

/**
 * @brief       Inicijalizuje LOG sa deskriptor strukturom
 * @note        Ova funkcija se mora prva pozvati
 */
void logInit(esLog_T * log,
    const C_ROM esLogDescriptor_T * C_ROM_VAR logDescriptor);

void logSetSwitches(
    esLog_T *       log,
    uint32_t        switches);

/**
 * @brief       Upisuje poruku u baffer
 * @param       log                     LOG sa kojim se trenutno radi
 * @param       msg                     redni broj poruke koji se salje
 * @param       val                     vrednost parametara koji ide uz poruku
 */
void logMsg(
    const esLog_T * log,
    uint32_t        msg,
    uint32_t        val);

/*--------------------------------------------------------  C++ extern end  --*/
#if defined(__cplusplus)
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of log.h
 ******************************************************************************/
#endif /* LOG_H_ */
