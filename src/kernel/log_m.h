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
 * @brief       Skup LOG poruka
 * @addtogroup  logm
 *********************************************************************//** @{ */


#ifndef STP_MESSAGES_H_
#define STP_MESSAGES_H_

/*=========================================================  INCLUDE FILES  ==*/
/*---------------------------------------------------------------  EXTERNS  --*/
/** @cond */
#ifdef LOG_M_H_VAR
# define LOG_M_H_EXT
#else
# define LOG_M_H_EXT extern
#endif
/** @endcond*/

/*===============================================================  DEFINES  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Filteri modula
 * @{ *//*--------------------------------------------------------------------*/
#define LOG_FILT_EPA                    (1UL << 0)
#define LOG_FILT_EVT                    (1UL << 1)
#define LOG_FILT_MM                     (1UL << 2)
#define LOG_FILT_SMP                    (1UL << 3)
#define LOG_FILT_KERNEL                 (1UL << 4)
#define LOG_FILE_EVTQ                   (1UL << 5)
#define LOG_FILT_SCHED                  (1UL << 6)

/** @} *//*-------------------------------------------------------------------*/

/**
 * @brief       Tekst tabela
 */
#define TEXT_TABLE(ENTRY)                                                       \
    ENTRY(LOG_MEM," memory [B]")                                                \
    ENTRY(LOG_ID, " id")                                                        \
    ENTRY(LOG_SIZE, " size [B]")                                                \
    ENTRY(LOG_PRIO, " priority")                                                \
    ENTRY(LOG_ARGUMENT, " argument")                                            \
    ENTRY(LOG_EPA_CREATE,"EPA: create(memClass, definition)")                   \
    ENTRY(LOG_EPA_PRIOSET, "EPA: prioSet(epa, newPrio)")                        \
    ENTRY(LOG_EPA_PRIOGET, "EPA: prio prioGet(epa)")                            \
    ENTRY(LOG_EPA_DESTROY, "EPA: destroy(epa)")                                 \
    ENTRY(LOG_EPA_DISPATCH, "EPA: dispatch(epa, evt)")                          \
    ENTRY(LOG_EPA_EVTPOST, "EPA: evtPost(epa, evt)")                            \
    ENTRY(LOG_EPA_EVTPOSTA, "EPA: evtPostAhead(epa, evt)")                      \
    ENTRY(LOG_EPA_EVTFETCH, "EPA: evt evtFetch(epa)")                           \
    ENTRY(LOG_EPA_EVTPUSHBACK, "EPA: evtPushBack(epa, evt)")                    \
    ENTRY(LOG_EPA_INIT, " EPA: initialization")                                 \
    ENTRY(LOG_SCHED_REG, "scheduler EPA registration")                          \
    ENTRY(LOG_EVTQ_FULL, "evtq: cannot insert evt into evtq: evtq is full.")

/*===============================================================  MACRO's  ==*/

#if (OPT_KERNEL_ENABLE_LOG != LOG_DISABLED)
# define LOG_FILT_EPA                   (1UL << 0)
# define LOG_FILT_EVT                   (1UL << 1)
# define LOG_FILT_MM                    (1UL << 2)
# define LOG_FILT_SMP                   (1UL << 3)
# define LOG_FILT_KERNEL                (1UL << 4)
# define LOG_FILE_EVTQ                  (1UL << 5)
# define LOG_FILT_SCHED                 (1UL << 6)
#else
# define LOG_FILT_EPA                   0UL
# define LOG_FILT_EVT                   0UL
# define LOG_FILT_MM                    0UL
# define LOG_FILT_SMP                   0UL
# define LOG_FILT_KERNEL                0UL
# define LOG_FILE_EVTQ                  0UL
# define LOG_FILT_SCHED                 0UL
#endif

/*------------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/**
 * @brief       Indeksi tekstova
 */
enum logMsgIndex {
    TEXT_TABLE(EXPAND_AS_ENUMERATOR)
    LOG_LAST_ENTRY
};

/*======================================================  GLOBAL VARIABLES  ==*/

/**
 * @brief       Kernel LOG sistem
 */
LOG_M_H_EXT esLog_T gKernelLog;

/**
 * @brief       Kernel LOG deskriptor
 */
extern const C_ROM esLogDescriptor_T gKernelLogDescriptor;

/*===================================================  FUNCTION PROTOTYPES  ==*/
/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of stp_messages.h
 ******************************************************************************/
#endif /* STP_MESSAGES_H_ */
