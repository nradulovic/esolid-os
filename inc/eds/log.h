/*
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
 *//*---------------------------------------------------------------------*//**
 * @file
 * @author  	Nenad Radulovic
 * @brief       LOGer API
 * @addtogroup  log_intf
 *********************************************************************//** @{ */

#if !defined(LOG_H_)
#define LOG_H_

/*=========================================================  INCLUDE FILES  ==*/

#include "compiler.h"
#include "eds/common.h"
#include "../config/sys_config.h"
#include "../config/log_config.h"

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

#if (OPT_LOG_LEVEL <= LOG_ERR) && defined(CFG_LOG_ENABLE)
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

# define ES_LOG_ERR_IF_INVALID(log, condition, msg, var)                        \
    do {                                                                        \
        if (!(condition)) {                                                     \
            (void)logMsg(log, LOG_TYPE_ERR, msg, (uint32_t)var);                \
        }                                                                       \
    } while (0UL);

# define ES_LOGX_ERR_START(log, msg)                                            \
    uint16_t logHandle_ = logXMsg(log, LOG_TYPE_ERR, &gModInfo_, msg)

# define ES_LOGX_ERR_APPEND(log, msg)                                           \
    logXMsgAppend(log, logHandle_, msg)

# define ES_LOGX_IF_ERR(log, filter, msg)                                       \
    do {                                                                        \
        if (ES_LOG_IS_ERR(log, filter)) {                                       \
            (void)logXMsg(log, LOG_TYPE_ERR, &gModInfo_, msg);                   \
        }                                                                       \
    } while (0UL)

# define ES_LOGX_ERR_IF_INVALID(log, condition, msg)                            \
    do {                                                                        \
        if (!(condition)) {                                                     \
            (void)logXMsg(log, LOG_TYPE_ERR, &gModInfo_, msg);                   \
        }                                                                       \
    } while (0UL);
#else
# define ES_LOG_IS_ERR(log, filter)     FALSE
# define ES_LOG_ERR_START(log, msg, var)
# define ES_LOG_ERR_APPEND(log, msg, var)
# define ES_LOG_IF_ERR(log, filter, msg, var)
# define ES_LOG_ERR_IF_INVALID(log, condition, msg, var)
# define ES_LOGX_ERR_START(log, msg, var)
# define ES_LOGX_ERR_APPEND(log, msg, var)
# define ES_LOGX_IF_ERR(log, filter, msg, var)
# define ES_LOGX_ERR_IF_INVALID(log, condition, msg, var)
#endif

/**
 * @brief       Obelezava upozorenja
 */
#if (OPT_LOG_LEVEL <= LOG_WARN) && defined(CFG_LOG_ENABLE)
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
#if (OPT_LOG_LEVEL <= LOG_INFO) && defined(CFG_LOG_ENABLE)
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
#if (OPT_LOG_LEVEL <= LOG_DBG) && defined(CFG_LOG_ENABLE)
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
#if (OPT_LOG_LEVEL <= LOG_TRACE) && defined(CFG_LOG_ENABLE)
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

/**@brief       LOG message enums
 * @api
 */
enum esLogType {
    LOG_TYPE_RECORD,                                                            /**<@brief Record, defines list of all messages in system   */
    LOG_TYPE_TRACE,                                                             /**<@brief Trace, records every action within system        */
    LOG_TYPE_DBG,                                                               /**<@brief Debug, messages that are enabled only during
                                                                                           development                                      */
    LOG_TYPE_INFO,                                                              /**<@brief Informational messages                           */
    LOG_TYPE_WARN,                                                              /**<@brief Warning messages                                 */
    LOG_TYPE_ERR                                                                /**<@brief An error has occurred, full stop                 */
};

/**@brief       LOG message type
 * @api
 */
typedef enum esLogType esLogType_T;

/**@brief       LOG record structure
 * @details     Records that use this structure should go into ROM part of
 *              address space to save RAM consumption.
 * @api
 */
struct esLogDescriptor {
    uint32_t            entries;                                                /**<@brief The number of messages in message table          */
    const PORT_C_ROM char * const PORT_C_ROM_VAR * textTable;                   /**<@brief Message text table                               */
    const PORT_C_ROM size_t * sizeTable;                                        /**<@brief Message size table                               */
    void * (* getBlock)(size_t);                                                /**<@brief getBlock method                                  */
    void (* pushBlock)(void *);                                                 /**<@brief pushBlock method                                 */
};

/**@brief       LOG record type
 */
typedef struct esLogDescriptor esLogDescriptor_T;

/**@brief       LOG instance structure
 */
struct esLog {
    uint32_t        switches;                                                   /**<@brief Enabled messages                                 */
    const PORT_C_ROM esLogDescriptor_T * PORT_C_ROM_VAR logDescriptor;          /**<@brief Used LOG descriptor structure                    */
};

/**@brief       LOG instance type
 */
typedef struct esLog esLog_T;

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

/**
 * @brief       Inicijalizuje LOG sa deskriptor strukturom
 * @note        Ova funkcija se mora prva pozvati
 */
void esLogInit(
    esLog_T *           log,
    const PORT_C_ROM esLogDescriptor_T * logDescriptor);

void esLogSwitchSetOn(
    esLog_T *           log,
    uint32_t            switches);

uint32_t logSwitchesGet(
    esLog_T *           log);

uint16_t logMsg(
    const esLog_T *     log,
    esLogType_T         type,
    uint32_t            msg,
    uint32_t            val);

uint16_t logXMsg(
    const esLog_T *     log,
    esLogType_T         type,
    uint32_t            msg,
    const char *        val);

void logMsgAppend(
    const esLog_T *     log,
    uint16_t            handle,
    uint32_t            msg,
    uint32_t            val);

void logXMsgAppend(
    const esLog_T *     log,
    uint16_t            handle,
    uint32_t            msg,
    const PORT_C_ROM char * val);

/*--------------------------------------------------------  C++ extern end  --*/
#if defined(__cplusplus)
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of log.h
 ******************************************************************************/
#endif /* LOG_H_ */
