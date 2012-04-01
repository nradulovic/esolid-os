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
 * @brief       Implementacija eS_hmemAllocI().
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

SMM_DBG_DEFINE_MODULE(eS_hmemAllocI);


/** @endcond *//*--------------------------------------------------------------------------------*
 * Local user defines
 *-----------------------------------------------------------------------------------------------*/
#define unlikely(x)     __builtin_expect((x),0)

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

/*-----------------------------------------------------------------------------------------------*/
/**
 * @ingroup smm_intf
 */
/*-----------------------------------------------------------------------------------------------*/
void * eS_hmemAllocI(
    size_t  aSize) {

#if (SMM_IS_FF == OPT_SMM_TYPE)

    SMM_FVAR smm_blkHdr_T * prevPhy;
    SMM_FVAR smm_blkHdr_T * freeBlk;

    SMM_DBG_CHECK(aSize < (size_t)BLOCK_STATUS_MASK);
    SMM_ASSERT((smm_blkHdr_T *)0 != dHeapSentinel);

    if (aSize < (sizeof(smm_blkHdr_T) - sizeof(smm_blk_T))) {
        aSize = sizeof(smm_blkHdr_T) - sizeof(smm_blk_T);
    }

#if (PORT_SUPP_UNALIGNED_ACCESS != PORT_TRUE) || defined(ES_OPTIMIZE_SPEED)
    aSize = ES_ALIGN(aSize, C_DATA_ALIGNMENT);
#endif

    DLS_FOR_EACH_ENTRY(
        smm_blkHdr_T,
        freeList,
        &(dHeapSentinel->freeList),
        freeBlk) {

        if (freeBlk->blk.size >= aSize) {

            if (freeBlk->blk.size >= (aSize + sizeof(smm_blkHdr_T))) {
                freeBlk->blk.size -= (aSize + sizeof(smm_blk_T));
                freeBlk = PHY_BLK_PREV(freeBlk);
                freeBlk->blk.size = aSize;
                prevPhy = PHY_BLK_PREV(freeBlk);
                eS_slsNodeAddAfterI(
                    &(prevPhy->blk.phyList),
                    &(freeBlk->blk.phyList));
            } else {
                eS_dlsNodeRemoveI(
                    &(freeBlk->freeList));
            }
            BLK_STAT_BUSY(freeBlk);

            return ((void *)((smm_blk_T *)freeBlk + 1U));
        }
    }
    SMM_ASSERT_ALWAYS("No free space to allocate.");

    return((void *)0);

#elif (SMM_IS_TLSF == OPT_SMM_TYPE)

    HAL_CRITICAL_DECL();
    SMM_FVAR size_t         blocks;
    SMM_FVAR size_t         rmngBlocks;
    SMM_FVAR size_t         frstIndx;
    SMM_FVAR size_t         scndIndx;
    SMM_FVAR smm_scndLst_T  tmpLst;
    SMM_FVAR smm_blk_T      * pFreeArea;
    SMM_FVAR smm_blk_T      * tmpPtr;

    SMM_ASSERT(aSize >= sizeof(smm_blkHdr_T));                                  /* Osiguraj da je minimalna trazena velicina slobodne       */
                                                                                /* memorije jednaka velicini zaglavlja.                     */
    blocks = ES_DIV_ROUNDUP(aSize, sizeof(smm_blk_T));                          /* Zaokruzi trazenu memoriju na granulaciju koja je jednaka */
                                                                                /* velicini jednog bloka i izvrsi deljenje sa tom velicinom */
                                                                                /* kako bi dobili broj potrebnih blokova da opsluzi zahtev. */
    if (SMM_TLSF_SWITCHLIST > blocks) {                                         /* Prvih nekoliko lista se tretiraju po alternativnom algo- */
                                                                                /* ritmu.                                                   */
        frstIndx = blocks >> SMM_TLSF_BETA;                                     /* Pronadji indeks prve liste (alternativni algoritam).     */
        scndIndx = blocks & (~((size_t)0) << SMM_TLSF_BETA);                    /* Pronadji indeks druge liste (alternativni algoritam).    */
    } else {
        frstIndx = HAL_CPU_FLS(blocks + ((size_t)1 << (HAL_CPU_FLS(blocks) - SMM_TLSF_BETA)) - 1);
        scndIndx = (blocks >> (frstIndx - SMM_TLSF_BETA)) &
            (~((size_t)1 << SMM_TLSF_BETA));
    }
    HAL_CRITICAL_ENTER();
    tmpLst = scndLstBitMaps[frstIndx] & (~((smm_scndLst_T)0) << scndIndx);

    if ((smm_scndLst_T)0 != tmpLst) {
        scndIndx = HAL_CPU_FFS(tmpLst);
    } else {                                                                    /* Ne postoji slobodna oblast trazene velicine, prelazi se  */
                                                                                /* pretragu sledece vece oblasti.                           */
        frstIndx = HAL_CPU_FFS(frstLstBitMap & (~((smm_frstLst_T)0) << (frstIndx + 1)));
        scndIndx = HAL_CPU_FFS(scndLstBitMaps[frstIndx]);
    }                                                                           /* U ovom trenutku imamo indekse pokazivaca.                */
    pFreeArea = pFreeMap[frstIndx][scndIndx];                                   /* Uzmi pokazivac na slobodnu oblast memorije.              */
    tmpPtr = pFreeMap[frstIndx][scndIndx] =
        C_PTRCAST(smm_blk_T, smm_blkHdr_T, pFreeArea).nextFree;                     /* Upisi u bitmapu pokazivac nove slobodne oblasti memorije.*/

    if ((smm_blk_T *)0 == tmpPtr) {                                             /* Ukoliko nema novog sledeceg bloka, resetuj bit u listi   */
        scndLstBitMaps[frstIndx] &= ~((smm_scndLst_T)1 << scndIndx);
                                                                                /* i proveri da li ima jos neki setovan bit.                */
        if ((smm_scndLst_T)0 == scndLstBitMaps[frstIndx]) {                     /* Ako nema drugih setovanih bitova resetuj primarnu listu. */
            frstLstBitMap &= ~((smm_frstLst_T)1 << frstIndx);
        }
    }
    rmngBlocks = TLSF_TO_BLOCKS(pFreeArea->size) - blocks - 1;                  /* Izracunaj koliko je slobodnih blokova je ostalo nekon    */
                                                                                /* dodeljivanja oblasti.                                    */
    if ((size_t)2 < rmngBlocks ) {                                              /* Da li izvrsiti podelu slobodne oblasti memorije.         */

        if (SMM_TLSF_SWITCHLIST > rmngBlocks) {
            frstIndx = rmngBlocks >> SMM_TLSF_BETA;
            scndIndx = rmngBlocks & (~((size_t)0) << SMM_TLSF_BETA);
        } else {
            frstIndx = HAL_CPU_FLS(rmngBlocks);
            scndIndx = (rmngBlocks >> (frstIndx - SMM_TLSF_BETA)) &
                (~((size_t)1 << SMM_TLSF_BETA));
        }                                                                       /* U ovom trenutku imamo indekse gde treba uvrstiti novu,   */
                                                                                /* slobodnu memorijsku oblast.                              */
        frstLstBitMap |= (smm_frstLst_T)1 << frstIndx;                          /* Postavi jedinice u bitovima koji asociraju na novu slo-  */
        scndLstBitMaps[frstIndx] |= (smm_scndLst_T)1 << scndIndx;               /* bodnu oblast.                                            */
        tmpPtr = pFreeArea;
        tmpPtr->size = TLSF_TO_BYTES(rmngBlocks);
        pFreeArea += rmngBlocks + 1;                                            /* Prebaci pokazivac na oblast koja se predaje klijentu.    */
        C_PTRCAST(smm_blk_T, smm_blkHdr_T, tmpPtr).nextFree =                       /* Ubaci prethodnu slobodnu oblast na drugo mesto u poveza- */
            pFreeMap[frstIndx][scndIndx];                                       /* noj listi.                                               */
        pFreeMap[frstIndx][scndIndx] = tmpPtr;                                  /* Ubaci ovu slobodnu oblast na pocetak povezane liste.     */
        pFreeArea->prev = tmpPtr;                                               /* Upisi pokazivac na prethodnu oblast.                     */
        pFreeArea->size = TLSF_TO_BYTES(blocks);                                /* Upisi velicinu tekuce oblasti.                           */
        (pFreeArea + blocks + 1)->prev = pFreeArea;                             /* Azuriraj sledecu oblast da pokazuje na tekucu oblast.    */

    }
    pFreeArea->size |= (size_t)1;                                               /* Oznaci da se ovaj blok koristi.                          */
    HAL_CRITICAL_EXIT();

    return ((void *)(pFreeArea + 1));
#endif
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
 * END of eS_hmemAllocI.c
 *************************************************************************************************/
