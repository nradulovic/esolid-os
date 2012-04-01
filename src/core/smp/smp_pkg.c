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
 * @brief   	Privatni objekti SMP modula.
 *
 * ------------------------------------------------------------------------------------------------
 *
 * @addtogroup  smp_impl
 *
 ****************************************************************************************//** @{ */


/*************************************************************************************************
 * INCLUDE FILES
 *************************************************************************************//** @cond */

#define SMP_PRIVATE_H_VAR
#include "../core_private.h"


/** @endcond *//**********************************************************************************
 * LOCAL DEFINES
 *************************************************************************************************/


/*************************************************************************************************
 * LOCAL MACRO's
 *************************************************************************************************/


/*************************************************************************************************
 * LOCAL CONSTANTS
 *************************************************************************************************/

/*-------------------------------------------------------------------------------------------*//**
 * @name        Klasa sistemskih dogadjaja
 * @{ *//*---------------------------------------------------------------------------------------*/

#if defined(OPT_EVT_USE_EXT)
const C_ROM eot_evtClass_T evtClassSystem = {
    (eot_evtSize_T)sizeof(smp_evtSystem_T),
# if defined(OPT_EVT_USE_TYPE)
    (eot_evtType_T)EVT_TYPE_SYSTEM,
# endif
    (eot_ptrEvtCtor_T)0,
    (eot_ptrEvtDtor_T)0
};
#endif

/** @} *//*--------------------------------------------------------------------------------------*/


/*************************************************************************************************
 * LOCAL DATA TYPES
 *************************************************************************************************/


/*************************************************************************************************
 * LOCAL TABLES
 *************************************************************************************************/

#if !defined(ES_OPTIMIZE_SPEED) && (PORT_SUPP_IDENTIC_ROM_RAM == PORT_TRUE)
const C_ROM eot_evt_T smp_controlEvt[] = {
#else
eot_evt_T smp_controlEvt[] = {
#endif

# if defined(OPT_EVT_USE_EXT)
    {(eot_evtClass_T *)&evtClassSystem,
# else
    {(eot_evtSize_T)sizeof(eot_evtPln_T),
#  if defined(OPT_EVT_USE_TYPE)
    (eot_evtType_T)EVT_TYPE_SYSTEM,
#  endif
# endif

# if defined(OPT_EVT_USE_TRANSMITTER)
    (void *)0,
# endif

# if defined(OPT_EVT_USE_TIME)
    (eot_evtTime_T)0,
# endif
    EVT_QUALIF_STATIC,
    SIG_EMPTY},

# if defined(OPT_EVT_USE_EXT)
    {(eot_evtClass_T *)&evtClassSystem,
# else
    {(eot_evtSize_T)sizeof(eot_evtPln_T),
#  if defined(OPT_EVT_USE_TYPE)
    (eot_evtType_T)EVT_TYPE_SYSTEM,
#  endif
# endif

# if defined(OPT_EVT_USE_TRANSMITTER)
    (void *)0,
# endif

# if defined(OPT_EVT_USE_TIME)
    (eot_evtTime_T)0,
# endif
    EVT_QUALIF_STATIC,
    SIG_ENTRY},

# if defined(OPT_EVT_USE_EXT)
    {(eot_evtClass_T *)&evtClassSystem,
# else
    {(eot_evtSize_T)sizeof(eot_evtPln_T),
#  if defined(OPT_EVT_USE_TYPE)
    (eot_evtType_T)EVT_TYPE_SYSTEM,
#  endif
# endif

# if defined(OPT_EVT_USE_TRANSMITTER)
    (void *)0,
# endif

# if defined(OPT_EVT_USE_TIME)
    (eot_evtTime_T)0,
# endif
    EVT_QUALIF_STATIC,
    SIG_EXIT},

# if defined(OPT_EVT_USE_EXT)
    {(eot_evtClass_T *)&evtClassSystem,
# else
    {(eot_evtSize_T)sizeof(eot_evtPln_T),
#  if defined(OPT_EVT_USE_TYPE)
    (eot_evtType_T)EVT_TYPE_SYSTEM,
#  endif
# endif

# if defined(OPT_EVT_USE_TRANSMITTER)
    (void *)0,
# endif

# if defined(OPT_EVT_USE_TIME)
    (eot_evtTime_T)0,
# endif
    EVT_QUALIF_STATIC,
    SIG_INIT},

# if defined(OPT_EVT_USE_EXT)
    {(eot_evtClass_T *)&evtClassSystem,
# else
    {(eot_evtSize_T)sizeof(eot_evtPln_T),
#  if defined(OPT_EVT_USE_TYPE)
    (eot_evtType_T)EVT_TYPE_SYSTEM,
#  endif
# endif

# if defined(OPT_EVT_USE_TRANSMITTER)
    (void *)0,
# endif

# if defined(OPT_EVT_USE_TIME)

    (eot_evtTime_T)0,
# endif
    EVT_QUALIF_STATIC,
    SIG_NOEX},

# if defined(OPT_EVT_USE_EXT)
    {(eot_evtClass_T *)&evtClassSystem,
# else
    {(eot_evtSize_T)sizeof(eot_evtPln_T),
#  if defined(OPT_EVT_USE_TYPE)
    (eot_evtType_T)EVT_TYPE_SYSTEM,
#  endif
# endif

# if defined(OPT_EVT_USE_TRANSMITTER)
    (void *)0,
# endif

# if defined(OPT_EVT_USE_TIME)
    (eot_evtTime_T)0,
# endif
    EVT_QUALIF_STATIC,
    SIG_SUPER}
};



/*************************************************************************************************
 * SHARED GLOBAL VARIABLES
 *************************************************************************************************/


/*************************************************************************************************
 * LOCAL GLOBAL VARIABLES
 *************************************************************************************************/


/*************************************************************************************************
 ***                                I M P L E M E N T A T I O N                                ***
 *************************************************************************************************/


/*************************************************************************************************
 * LOCAL FUNCTION DEFINITIONS
 *************************************************************************************************/


/*************************************************************************************************
 * GLOBAL FUNCTION DEFINITIONS
 *************************************************************************************************/


/*************************************************************************************************
 * CONFIGURATION ERRORS
 *************************************************************************************//** @cond */


/** @endcond *//** @} *//*************************************************************************
 * END of smp_pkg.c
 *************************************************************************************************/
