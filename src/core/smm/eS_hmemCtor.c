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
 *************************************************************************************************/


/*********************************************************************************************//**
 * @file
 *
 * @author  	nesa
 *
 * @brief   	Implementacija eS_hmemCtor()
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
SMM_DBG_DEFINE_MODULE(eS_hmemCtor);


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
void eS_hmemCtor(
    void        * aHeap,
    size_t      aSize) {

#if (SMM_IS_FF == OPT_SMM_TYPE)

	smm_blkHdr_T	* freeMemory;

    SMM_DBG_CHECK((void *)0 != aHeap);
    SMM_DBG_CHECK(aSize > (sizeof(smm_blkHdr_T) * 2));
    SMM_DBG_CHECK(aSize < (size_t)BLOCK_STATUS_MASK);
    SMM_DBG_CHECK(!(aSize & (C_DATA_ALIGNMENT - 1U)));
    SMM_DBG_CHECK(!((size_t)aHeap & (C_DATA_ALIGNMENT - 1U)));
    freeMemory = (smm_blkHdr_T *)aHeap;
    freeMemory->blk.size = aSize - sizeof(smm_blk_T) - sizeof(smm_blkHdr_T);
    dHeapSentinel = (smm_blkHdr_T *)((uint8_t *)freeMemory + freeMemory->blk.size + sizeof(smm_blk_T));
    dHeapSentinel->blk.size = (smm_blkSize_T)0;
    eS_slsSentinelInit(
        &(dHeapSentinel->blk.phyList));
    eS_slsNodeAddHeadI(
        &(dHeapSentinel->blk.phyList),
        &(freeMemory->blk.phyList));
    eS_dlsSentinelInit(
        &(dHeapSentinel->freeList));
    eS_dlsNodeAddHeadI(
        &(dHeapSentinel->freeList),
        &(freeMemory->freeList));
    BLK_STAT_BUSY(dHeapSentinel);
    BLK_STAT_FREE(freeMemory);

#if defined(OPT_DBG_SMM)
    smm_dbgHeapBegin = aHeap;
    smm_dbgHeapEnd = (void *)dHeapSentinel;
#endif

#elif (SMM_IS_TLSF == OPT_SMM_TYPE)

    SMM_ASSERT((void *)0 != aHeap);
    SMM_ASSERT(aSize > sizeof(smm_blkHdr_T));

#endif
}


/*************************************************************************************************
 * LOCAL FUNCTION DEFINITIONS
 *************************************************************************************************/


/*************************************************************************************************
 * CONFIGURATION ERRORS
 *************************************************************************************//** @cond */


/** @endcond *//** @} *//*************************************************************************
 * END of eS_hmemCtor.c
 *************************************************************************************************/
