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
#include "../config/sys_config.h"

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

#if (OPT_LOG_LEVEL <= LOG_ERR) && defined(OPT_SYS_ENABLE_LOG)
/**
 * @brief       Obelezava greske u sistemu
 */
# define ES_LOG_IS_ERR(log, filter)                                             \
    ((log)->switches & (filter))

# define ES_LOG_ERR_START(log, msg, var)                                        \
    uint16_t logHandle_ = logMsg(log, LOG_TYPE_ERR, msg, (uint32_t)var)

# define ES_LOG_ERR_APPEND(log, msg, var)                                       \
    logMsgAppend(log, logHandle_, msg, (uint32_t)var)

# define ES_LOG_IF_ERR(log, filter, msg, var)                                   \
    do {                                                                        \
        if (ES_LOG_IS_ERR(log, filter)) {                                       \
            (void)logMsg(log, LOG_TYPE_ERR, msg, (uint32_t)var);                \
        }                                                                       \
    } while (0UL)

# define ES_LOGX_ERR_START(log, msg, var)                                       \
    uint16_t logHandle_ = logXMsg(log, LOG_TYPE_ERR, msg, (uint32_t)var)

# define ES_LOGX_ERR_APPEND(log, msg, var)                                      \
    logXMsgAppend(log, logHandle_, msg, (uint32_t)var)

# define ES_LOGX_IF_ERR(log, filter, msg, var)                                  \
    do {                                                                        \
        if (ES_LOG_IS_ERR(log, filter)) {                                       \
            (void)logXMsg(log, LOG_TYPE_ERR, msg, (uint32_t)var);               \
        }                                                                       \
    } while (0UL)
#else
# define ES_LOG_IS_ERR(log, filter)     FALSE
# define ES_LOG_ERR_START(log, msg, var)
# define ES_LOG_ERR_APPEND(log, msg, var)
# define ES_LOG_IF_ERR(log, filter, msg, var)
# define ES_LOGX_ERR_START(log, msg, var)
# define ES_LOGX_ERR_APPEND(log, msg, var)
# define ES_LOGX_IF_ERR(log, filter, msg, var)
#endif

/**
 * @brief       Obelezava upozorenja
 */
#if (OPT_LOG_LEVEL <= LOG_WARN) && defined(OPT_SYS_ENABLE_LOG)
# define ES_LOG_IS_WARN(log, filter)                                            \
    ((log)->switches & (filter))

# define ES_LOG_WARN_START(log, msg, var)                                       \
    uint16_t logHandle_ = logMsg(log, LOG_TYPE_WARN, msg, (uint32_t)var)

# define ES_LOG_WARN_APPEND(log, msg, var)                                      \
    logMsgAppend(log, logHandle_, msg, (uint32_t)var)

# define ES_LOG_IF_WARN(log, filter, msg, var)                                  \
    do {                                                                        \
        if (ES_LOG_IS_WARN(log, filter)) {                                      \
            (void)logMsg(log, LOG_TYPE_WARN, msg, (uint32_t)var);               \
        }                                                                       \
    } while (0UL)

# define ES_LOG_WARN_IF_INVALID(log, condition, msg, var)                       \
    do {                                                                        \
        if (!(condition)) {                                                     \
            (void)logMsg(log, LOG_TYPE_WARN, msg, (uint32_t)var);               \
        }                                                                       \
    } while (0UL)

# define ES_LOGX_WARN_START(log, msg, var)                                      \
    uint16_t logHandle_ = logXMsg(log, LOG_TYPE_WARN, msg, (uint32_t)var)

# define ES_LOGX_WARN_APPEND(log, msg, var)                                     \
    logXMsgAppend(log, logHandle_, msg, (uint32_t)var)

# define ES_LOGX_IF_WARN(log, filter, msg, var)                                 \
    do {                                                                        \
        if (ES_LOG_IS_WARN(log, filter)) {                                      \
            (void)logXMsg(log, LOG_TYPE_WARN, msg, (uint32_t)var);              \
        }                                                                       \
    } while (0UL)

# define ES_LOGX_WARN_IF_INVALID(log, condition, msg, var)                      \
    do {                                                                        \
        if (!(condition)) {                                                     \
            (void)logXMsg(log, LOG_TYPE_WARN, msg, (uint32_t)var);              \
        }                                                                       \
    } while (0UL)
#else
# define ES_LOG_IS_WARN(log, filter)    FALSE
# define ES_LOG_WARN_START(log, msg, var)
# define ES_LOG_WARN_APPEND(log, msg, var)
# define ES_LOG_IF_WARN(log, filter, msg, var)
# define ES_LOG_WARN_IF_INVALID(log, condition, msg, var)
# define ES_LOGX_WARN_START(log, msg, var)
# define ES_LOGX_WARN_APPEND(log, msg, var)
# define ES_LOGX_IF_WARN(log, filter, msg, var)
# define ES_LOGX_WARN_IF_INVALID(log, condition, msg, var)
#endif

/**
 * @brief       Obelezava informativne dogadjaje
 */
#if (OPT_LOG_LEVEL <= LOG_INFO) && defined(OPT_SYS_ENABLE_LOG)
# define ES_LOG_IS_INFO(log, filter)                                            \
    ((log)->switches & (filter))

# define ES_LOG_INFO_START(log, msg, var)                                       \
    uint16_t logHandle_ = logMsg(log, LOG_TYPE_INFO, msg, (uint32_t)var)

# define ES_LOG_INFO_APPEND(log, msg, var)                                      \
    logMsgAppend(log, logHandle_, msg, (uint32_t)var)

# define ES_LOG_IF_INFO(log, filter, msg, var)                                  \
    do {                                                                        \
        if (ES_LOG_IS_INFO(log, filter)) {                                      \
            (void)logMsg(log, LOG_TYPE_INFO, msg, (uint32_t)var);               \
        }                                                                       \
    } while (0UL)

# define ES_LOGX_INFO_START(log, msg, var)                                      \
    uint16_t logHandle_ = logXMsg(log, LOG_TYPE_INFO, msg, (uint32_t)var)

# define ES_LOGX_INFO_APPEND(log, msg, var)                                     \
    logXMsgAppend(log, logHandle_, msg, (uint32_t)var)

# define ES_LOGX_IF_INFO(log, filter, msg, var)                                 \
    do {                                                                        \
        if (ES_LOG_IS_INFO(log, filter)) {                                      \
            (void)logXMsg(log, LOG_TYPE_INFO, msg, (uint32_t)var);              \
        }                                                                       \
    } while (0UL)

#else
# define ES_LOG_IS_INFO(log, filter)    FALSE
# define ES_LOG_INFO_START(log, msg, var)
# define ES_LOG_INFO_APPEND(log, msg, var)
# define ES_LOG_IF_INFO(log, filter, msg, var)
# define ES_LOGX_INFO_START(log, msg, var)
# define ES_LOGX_INFO_APPEND(log, msg, var)
# define ES_LOGX_IF_INFO(log, filter, msg, var)
#endif

/**
 * @brief       Obelezava dogadjaje sa informacijama o greskama
 */
#if (OPT_LOG_LEVEL <= LOG_DBG) && defined(OPT_SYS_ENABLE_LOG)
# define ES_LOG_IS_DBG(log, filter)                                             \
    ((log)->switches & (filter))

# define ES_LOG_DBG_START(log, msg, var)                                        \
    uint16_t logHandle_ = logMsg(log, LOG_TYPE_DBG, msg, (uint32_t)var)

# define ES_LOG_DBG_APPEND(log, msg, var)                                       \
    logMsgAppend(log, logHandle_, msg, (uint32_t)var)

# define ES_LOG_IF_DBG(log, filter, msg, var)                                   \
    do {                                                                        \
        if (ES_LOG_IS_DBG(log, filter)) {                                       \
            (void)logMsg(log, LOG_TYPE_DBG, msg, var);                          \
        }                                                                       \
    } while (0UL)

# define ES_LOG_DBG_IF_INVALID(log, condition, msg, var)                        \
    do {                                                                        \
        if (!(condition)) {                                                     \
            (void)logMsg(log, LOG_TYPE_DBG, msg, var);                          \
        }                                                                       \
    } while (0UL)

# define ES_LOGX_DBG_START(log, msg, var)                                       \
    uint16_t logHandle_ = logXMsg(log, LOG_TYPE_DBG, msg, (uint32_t)var)

# define ES_LOGX_DBG_APPEND(log, msg, var)                                      \
    logXMsgAppend(log, logHandle_, msg, (uint32_t)var)

# define ES_LOGX_IF_DBG(log, filter, msg, var)                                  \
    do {                                                                        \
        if (ES_LOG_IS_DBG(log, filter)) {                                       \
            (void)logXMsg(log, LOG_TYPE_DBG, msg, var);                         \
        }                                                                       \
    } while (0UL)

# define ES_LOGX_DBG_IF_INVALID(log, condition, msg, var)                       \
    do {                                                                        \
        if (!(condition)) {                                                     \
            (void)logXMsg(log, LOG_TYPE_DBG, msg, var);                         \
        }                                                                       \
    } while (0UL)
#else
# define ES_LOG_IS_DBG(log, filter)     FALSE
# define ES_LOG_DBG_START(log, msg, var)
# define ES_LOG_DBG_APPEND(log, msg, var)
# define ES_LOG_IF_DBG(log, filter, msg, var)
# define ES_LOG_DBG_IF_INVALID(log, condition, msg, var)
# define ES_LOGX_DBG_START(log, msg, var)
# define ES_LOGX_DBG_APPEND(log, msg, var)
# define ES_LOGX_IF_DBG(log, filter, msg, var)
# define ES_LOGX_DBG_IF_INVALID(log, condition, msg, var)
#endif

/**
 * @brief       Obelezava dogadjaje izvrsenja koda
 */
#if (OPT_LOG_LEVEL <= LOG_TRACE) && defined(OPT_SYS_ENABLE_LOG)
# define ES_LOG_IS_TRACE(log, filter)                                           \
    ((log)->switches & (filter))

# define ES_LOG_TRACE_START(log, msg, var)                                      \
    uint16_t logHandle_ = logMsg(log, LOG_TYPE_TRACE, msg, (uint32_t)var)

# define ES_LOG_TRACE_APPEND(log, msg, var)                                     \
    logMsgAppend(log, logHandle_, msg, (uint32_t)var)

# define ES_LOG_IF_TRACE(log, filter, msg, var)                                 \
    do {                                                                        \
        if (ES_LOG_IS_TRACE(log, filter)) {                                     \
            (void)logMsg(log, LOG_TYPE_TRACE, msg, (uint32_t)var);              \
        }                                                                       \
    } while (0UL)

# define ES_LOGX_TRACE_START(log, msg, var)                                     \
    uint16_t logHandle_ = logXMsg(log, LOG_TYPE_TRACE, msg, (uint32_t)var)

# define ES_LOGX_TRACE_APPEND(log, msg, var)                                    \
    logXMsgAppend(log, logHandle_, msg, var)

# define ES_LOGX_IF_TRACE(log, filter, msg, var)                                \
    do {                                                                        \
        if (ES_LOG_IS_TRACE(log, filter)) {                                     \
            (void)logXMsg(log, LOG_TYPE_TRACE, msg, (uint32_t)var);             \
        }                                                                       \
    } while (0UL)

#else
# define ES_LOG_IS_TRACE(log, filter)   FALSE
# define ES_LOG_TRACE_START(log, msg, var)
# define ES_LOG_TRACE_APPEND(log, msg, var)
# define ES_LOG_IF_TRACE(log, filter, msg, var)
# define ES_LOGX_TRACE_START(log, msg, var)
# define ES_LOGX_TRACE_APPEND(log, msg, var)
# define ES_LOGX_IF_TRACE(log, filter, msg, var)
#endif

/*------------------------------------------------------  C++ extern begin  --*/
#if defined(__cplusplus)
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/**
 * @brief       Tipovi LOG poruka
 */
typedef enum logType {
/**
 * @brief       Record, salje se kada server zatrazi spisak svih poruka u sistemu
 */
    LOG_TYPE_RECORD,

/**
 * @brief       Zapis rada sistema
 */
    LOG_TYPE_TRACE,

/**
 * @brief       Poruke o greskama
 */
    LOG_TYPE_DBG,

/**
 * @brief       Poruke koje pruzaju informacije o radu sistema, njegovom stanju
 *              i slicno
 */
    LOG_TYPE_INFO,

/**
 * @brief       Nastala je greska u sistemu koja nije kriticna za dalji rad
 */
    LOG_TYPE_WARN,

/**
 * @brief       Nastala je greska u sistemu, ne moze se nastaviti sa daljim
 *              radom
 */
    LOG_TYPE_ERR,

} logType_T;

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
void esLogInit(
    esLog_T *       log,
    const C_ROM esLogDescriptor_T * C_ROM_VAR logDescriptor);

void esLogSwitchSetOn(
    esLog_T *       log,
    uint32_t        switches);

uint32_t logSwitchesGet(
    esLog_T *       log);

uint16_t logMsg(
    const esLog_T * log,
    logType_T       type,
    uint32_t        msg,
    uint32_t        val);

uint16_t logXMsg(
    const esLog_T * log,
    logType_T       type,
    uint32_t        msg,
    const C_ROM char * val);

void logMsgAppend(
    const esLog_T * log,
    uint16_t        handle,
    uint32_t        msg,
    uint32_t        val);

void logXMsgAppend(
    const esLog_T * log,
    uint16_t        handle,
    uint32_t        msg,
    const C_ROM char * val);

/*--------------------------------------------------------  C++ extern end  --*/
#if defined(__cplusplus)
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of log.h
 ******************************************************************************/
#endif /* LOG_H_ */
