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
 * @brief       Implementacija eS_hmemDeAllocI().
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

SMM_DBG_DEFINE_MODULE(eS_hmemDeAllocI);


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
void eS_hmemDeAllocI(
    void        * aMemory) {

#if (SMM_IS_FF == OPT_SMM_TYPE)

    SMM_FVAR smm_blkSize_T blkStat;
    SMM_FVAR smm_blkHdr_T * freeBlk;
    SMM_FVAR smm_blkHdr_T * currPhy;
    SMM_FVAR smm_blkHdr_T * tmpPhy;

    SMM_ASSERT((smm_blkHdr_T *)0 != dHeapSentinel);
    SMM_DBG_CHECK((smm_dbgHeapBegin < aMemory) && (aMemory < smm_dbgHeapEnd));
    freeBlk = (smm_blkHdr_T *)((smm_blk_T *)aMemory - 1U);
    SMM_DBG_CHECK(BLOCK_IS_BUSY == BLK_STAT_QUERY(freeBlk));
    currPhy = eS_slsNodeEntry(
        smm_blkHdr_T,
        blk.phyList,
        freeBlk->blk.phyList.next);
    blkStat = BLK_STAT_QUERY(currPhy);
    BLK_STAT_FREE(freeBlk);

    if (BLOCK_IS_FREE == blkStat) {
        eS_dlsNodeRemoveI(
            &(currPhy->freeList));
        tmpPhy = PHY_BLK_PREV(freeBlk);
        eS_slsNodeRemoveAfterI(
            &(tmpPhy->blk.phyList));
        currPhy->blk.size += freeBlk->blk.size + sizeof(smm_blk_T);
        freeBlk = currPhy;
    }
    currPhy = PHY_BLK_PREV(freeBlk);
    blkStat = BLK_STAT_QUERY(currPhy);

    if (BLOCK_IS_FREE == blkStat) {
        eS_dlsNodeRemoveI(
            &(currPhy->freeList));
        tmpPhy = PHY_BLK_PREV(currPhy);
        eS_slsNodeRemoveAfterI(
            &(tmpPhy->blk.phyList));
        freeBlk->blk.size += currPhy->blk.size + sizeof(smm_blk_T);
    }
    eS_dlsNodeAddHeadI(
        &(dHeapSentinel->freeList),
        &(freeBlk->freeList));

#elif (SMM_IS_TLSF == OPT_SMM_TYPE)

    HAL_CRITICAL_DECL();
    SMM_FVAR size_t         blocks;
    SMM_FVAR size_t         tmpSize;
    SMM_FVAR size_t         frstIndx;
    SMM_FVAR size_t         scndIndx;
    SMM_FVAR smm_blk_T      * pFreeArea;
    SMM_FVAR smm_blk_T      * tmpPtr;

    pFreeArea = (smm_blk_T *)aMemory - 1;
    HAL_CRITICAL_ENTER();
    blocks =  TLSF_TO_BLOCKS(pFreeArea->size);
    tmpPtr  = pFreeArea->prev;
    tmpSize = tmpPtr->size;

    if ((size_t)0 == (tmpSize & (size_t)1)) {                                   /* Da li je prethodni blok, takodje, slobodan?              */
        blocks += TLSF_TO_BLOCKS(tmpSize) + 1;
        pFreeArea = tmpPtr;
    }
    tmpPtr  = pFreeArea + blocks + 1;
    tmpSize = tmpPtr->size;

    if ((size_t)0 == (tmpSize & (size_t)1)) {                                   /* Da li je sledeci blok slobodan?                          */
        blocks += TLSF_TO_BLOCKS(tmpSize) + 1;
    }

    if (SMM_TLSF_SWITCHLIST > blocks) {
        frstIndx = blocks >> SMM_TLSF_BETA;
        scndIndx = blocks & (~((size_t)0) << SMM_TLSF_BETA);
    } else {
        frstIndx = HAL_CPU_FLS(blocks);
        scndIndx = (blocks >> (frstIndx - SMM_TLSF_BETA)) &
            (~((size_t)1 << SMM_TLSF_BETA));
    }                                                                           /* U ovom trenutku imamo indekse gde treba uvrstiti novu,   */
                                                                                /* slobodnu memorijsku oblast.                              */
    frstLstBitMap |= (smm_frstLst_T)1 << frstIndx;                              /* Postavi jedinice u bitovima koji asociraju na novu slo-  */
    scndLstBitMaps[frstIndx] |= (smm_scndLst_T)1 << scndIndx;                   /* bodnu oblast.                                            */
    C_PTRCAST(smm_blk_T, smm_blkHdr_T, pFreeArea).nextFree =                        /* Ubaci prethodnu slobodnu oblast na drugo mesto u poveza- */
        pFreeMap[frstIndx][scndIndx];                                           /* noj listi.                                               */
    pFreeMap[frstIndx][scndIndx] = pFreeArea;                                   /* Ubaci ovu slobodnu oblast na pocetak povezane liste.     */
    pFreeArea->size = TLSF_TO_BYTES(blocks);
    (pFreeArea + blocks + 1)->prev = pFreeArea;                                 /* Azuriraj sledecu oblast da pokazuje na tekucu oblast.    */

    HAL_CRITICAL_EXIT();
#endif
}


/*************************************************************************************************
 * LOCAL FUNCTION DEFINITIONS
 *************************************************************************************************/


/*************************************************************************************************
 * CONFIGURATION ERRORS
 *************************************************************************************//** @cond */


/** @endcond *//** @} *//*************************************************************************
 * END of eS_hmemDeAllocI.c
 *************************************************************************************************/
