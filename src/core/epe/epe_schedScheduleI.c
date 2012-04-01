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
 *
 * @author      nenad
 *
 * @brief       Implementacija epe_schedScheduleI().
 *
 * ------------------------------------------------------------------------------------------------
 *
 * @addtogroup  epe_impl
 *
 ****************************************************************************************//** @{ */


/*************************************************************************************************
 * INCLUDE FILES
 *************************************************************************************************/

#include "../core_private.h"


/*************************************************************************************************
 * LOCAL DEFINES
 *************************************************************************************************/

/*-----------------------------------------------------------------------------------------------*
 * Local debug defines
 *-----------------------------------------------------------------------------------*//** @cond */

EPE_DBG_DEFINE_MODULE(epe_schedScheduleI);


/** @endcond *//*--------------------------------------------------------------------------------*
 * Local user defines
 *-----------------------------------------------------------------------------------------------*/


/*************************************************************************************************
 * LOCAL MACRO's
 *************************************************************************************************/


/*************************************************************************************************
 * LOCAL CONSTANTS
 *************************************************************************************************/


/*************************************************************************************************
 * LOCAL DATA TYPES
 *************************************************************************************************/


/*************************************************************************************************
 * LOCAL TABLES
 *************************************************************************************************/


/*************************************************************************************************
 * SHARED GLOBAL VARIABLES
 *************************************************************************************************/


/*************************************************************************************************
 * LOCAL GLOBAL VARIABLES
 *************************************************************************************************/

/*************************************************************************************************
 * LOCAL FUNCTION DECLARATIONS
 *************************************************************************************************/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Dobavlja prioritet EPA objekta sa najvecim prioritetom u redu
 *              cekanja.
 */
/*-----------------------------------------------------------------------------------------------*/
C_INLINE unative_T epe_rdyBitmapPeek(
    void) {

    unative_T indxGroup;
    unative_T indx;

#if (OPT_EPA_PRIO_MAX < HAL_UNATIVE_BITS)
    indxGroup = (unative_T)0U;
#else
    indxGroup = HAL_CPU_FLS(epe_rdyBitmap.group);
#endif
    indx = HAL_CPU_FLS(epe_rdyBitmap.bits[indxGroup]);

    return (indx | (indxGroup << PRIO_INDX_PWR));
}


/*************************************************************************************************
 ***                                I M P L E M E N T A T I O N                                ***
 *************************************************************************************************/

void epe_schedScheduleI(
    void) {

    unative_T newPrio;

    newPrio = epe_rdyBitmapPeek();

    if (newPrio > epe_curr.prio) {
        epe_epaPrio_T savedPrio;
        epe_epa_T     * savedEpa;

        savedPrio = epe_curr.prio;
        savedEpa = epe_curr.epa;

        do {
            eot_evt_T * currEvt;
            epe_epa_T * currEpa;

            epe_curr.prio = newPrio;
            currEpa = epe_rdyList[newPrio];
            epe_curr.epa = currEpa;
            currEvt = eot_evtFetchI(
                currEpa);
            HAL_INT_ENABLE();
            eS_hsmDispatch(
                (smp_exec_T *)currEpa,
                currEvt);
            HAL_INT_DISABLE();
            eS_evtDestroyI(
                currEvt);
            newPrio = epe_rdyBitmapPeek();
        } while (newPrio > savedPrio);
        epe_curr.prio = savedPrio;
        epe_curr.epa = savedEpa;
    }
}


/*************************************************************************************************
 * GLOBAL FUNCTION DEFINITIONS
 *************************************************************************************************/


/*************************************************************************************************
 * LOCAL FUNCTION DEFINITIONS
 *************************************************************************************************/


/*************************************************************************************************
 * CONFIGURATION ERRORS
 *************************************************************************************//** @cond */


/** @endcond *//** @} *//*************************************************************************
 * END of epe_schedScheduleI.c
 *************************************************************************************************/
