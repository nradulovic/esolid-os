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
 *//******************************************************************************************//**
 * @file
 * @author  	Nenad Radulovic
 * @brief       Skup STP poruka
 * ------------------------------------------------------------------------------------------------
 * @addtogroup  stp_messages
 * @brief		Skup STP poruka
 ****************************************************************************************//** @{ */


#ifndef STP_MESSAGES_H_
#define STP_MESSAGES_H_

/*============================================================================  INCLUDE FILES  ==*/
/*==================================================================================  DEFINES  ==*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Definition group
 * @brief       brief description
 * @{ *//*---------------------------------------------------------------------------------------*/
#define STP_FILT_EPA_STATUS_0           (1UL << 0)
#define STP_FILT_EPA_EXECUTION_0        (1UL << 1)
#define STP_FILT_EPA_EXECUTION_1        (1UL << 2)
#define STP_FILT_EVT_0                  (1UL << 3)
#define STP_FILT_EVT_Q_0                (1UL << 4)
#define STP_FILT_KERN_STATUS_0          (1UL << 5)
#define STP_FILT_MEM_0                  (1UL << 6)

/** @} *//*--------------------------------------------------------------------------------------*/

/*==================================================================================  MACRO's  ==*/
/*-------------------------------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*===============================================================================  DATA TYPES  ==*/
/*=========================================================================  GLOBAL VARIABLES  ==*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Variables group
 * @brief       brief description
 * @{ *//*---------------------------------------------------------------------------------------*/
/* STP_FILT_EPA_STATUS 0 */
extern const char C_ROM * C_ROM_VAR const txtEpaCreate;
extern const char C_ROM * C_ROM_VAR const txtEpaCreateMemNeeded;
extern const char C_ROM * C_ROM_VAR const txtEpaCreateMemAddr;
extern const char C_ROM * C_ROM_VAR const txtEpaDestroy;
extern const char C_ROM * C_ROM_VAR const txtEpaInit;

/* STP_FILT_EVT 0 */
extern const char C_ROM * C_ROM_VAR const txtEvtCreate;
extern const char C_ROM * C_ROM_VAR const txtEvtPost;
extern const char C_ROM * C_ROM_VAR const txtEvtPostAhead;
extern const char C_ROM * C_ROM_VAR const txtEvtDestroy;
extern const char C_ROM * C_ROM_VAR const txtEvtReserve;
extern const char C_ROM * C_ROM_VAR const txtEvtUnReserve;

/* STP_FILT_EVT 1 */
extern const char C_ROM * C_ROM_VAR const txtEvtCreateMemNeeded;
extern const char C_ROM * C_ROM_VAR const txtEvtCreateMemAddr;

/* STP_FILT_EVT_Q_0 */
extern const char C_ROM * C_ROM_VAR const txtEvtQinit;
extern const char C_ROM * C_ROM_VAR const txtEvtQput;
extern const char C_ROM * C_ROM_VAR const txtEvtQputAhead;
extern const char C_ROM * C_ROM_VAR const txtEvtQget;
extern const char C_ROM * C_ROM_VAR const txtEvtQFree;

/* STP_FILT_EPA_EXECUTION 0 */
extern const char C_ROM * C_ROM_VAR const txtEpaSMinitialization;
extern const char C_ROM * C_ROM_VAR const txtEpaSMexec;
extern const char C_ROM * C_ROM_VAR const txtEpaSMtran;
extern const char C_ROM * C_ROM_VAR const txtEpaSMHandled;
extern const char C_ROM * C_ROM_VAR const txtEpaSMDeffered;
extern const char C_ROM * C_ROM_VAR const txtEpaSMignored;

/* STP_FILT_EPA_EXECUTION 1 */
extern const char C_ROM * C_ROM_VAR const txtEpaSMtranStart;
extern const char C_ROM * C_ROM_VAR const txtEpaSMtranFinished;
extern const char C_ROM * C_ROM_VAR const txtEpaSMgenTree;
extern const char C_ROM * C_ROM_VAR const txtEpaSMsuper;
extern const char C_ROM * C_ROM_VAR const txtEpaSMentry;
extern const char C_ROM * C_ROM_VAR const txtEpaSMexit;
extern const char C_ROM * C_ROM_VAR const txtEpaSMinit;

/* STP_FILT_KERN_STATUS 0 */
extern const char C_ROM * C_ROM_VAR const txtKernStart;
extern const char C_ROM * C_ROM_VAR const txtKernSearch;
extern const char C_ROM * C_ROM_VAR const txtKernEnterIdle;
extern const char C_ROM * C_ROM_VAR const txtKernExitIdle;
extern const char C_ROM * C_ROM_VAR const txtKernEpaDispatch;

/* STP_FILE_MEM_0 */
extern const char C_ROM * C_ROM_VAR const txtMemHeapInit;
extern const char C_ROM * C_ROM_VAR const txtMemHeapFree;
extern const char C_ROM * C_ROM_VAR const txtMemHeapAlloc;
extern const char C_ROM * C_ROM_VAR const txtMemHeapAllocated;
extern const char C_ROM * C_ROM_VAR const txtMemHeapDeAlloc;
/** @} *//*--------------------------------------------------------------------------------------*/

/*======================================================================  FUNCTION PROTOTYPES  ==*/
/*---------------------------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*===================================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/

/** @endcond *//** @} *//*************************************************************************
 * END of stp_messages.h
 *************************************************************************************************/
#endif /* STP_MESSAGES_H_ */
