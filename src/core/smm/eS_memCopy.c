/*************************************************************************************************
 * This file is part of lcsw-eSolid
 *
 * Copyright (C) 2011, 2012 - Nenad Radulovic
 *
 * lcsw-eSolid is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * lcsw-eSolid is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with lcsw-eSolid; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301  USA
 *************************************************************************************************/


/*********************************************************************************************//**
 * @file
 *
 * @author      nesa
 *
 * @brief       Implementacijaf eS_memCopy().
 *
 * ------------------------------------------------------------------------------------------------
 *
 * @addtogroup  smm_impl
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
SMM_DBG_DEFINE_MODULE(eS_memCopy);


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
 ***                                I M P L E M E N T A T I O N                                ***
 *************************************************************************************************/


/*************************************************************************************************
 * GLOBAL FUNCTION DEFINITIONS
 *************************************************************************************************/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @ingroup smm_intf
 */
/*-----------------------------------------------------------------------------------------------*/
void * eS_memCopy(
	void        * C_RESTRICT aDst,
	const void  * C_RESTRICT aSrc,
	size_t      aLength) {

#if !defined(ES_OPTIMIZE_SPEED)
    SMM_FVAR uint8_t * dst;
    SMM_FVAR uint8_t * src;

    SMM_DBG_CHECK((void *)0 != aDst);
    SMM_DBG_CHECK((void *)0 != aSrc);
    SMM_DBG_CHECK(aDst != aSrc);
    dst = (uint8_t *)aDst;
    src = (uint8_t *)aSrc;

    while ((size_t)0 != aLength--) {
        *dst++ = *src++;
    }

    return (aDst);
#else

    /*
     * todo Napisati funkciju optimizovanu za brzinu (kopiranje 4-32 bajta u jednom ciklusu)
     */
#endif
}


/*************************************************************************************************
 * LOCAL FUNCTION DEFINITIONS
 *************************************************************************************************/


/*************************************************************************************************
 * CONFIGURATION ERRORS
 *************************************************************************************//** @cond */


/** @endcond *//** @} *//*************************************************************************
 * END of eS_memCopy.c
 *************************************************************************************************/
