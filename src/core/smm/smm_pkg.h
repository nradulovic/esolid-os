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
 * @author      nenad
 *
 * @brief       Privatni objekti SMM podmodula
 *
 * ------------------------------------------------------------------------------------------------
 *
 * @addtogroup  smm_impl
 *
 ****************************************************************************************//** @{ */


#ifndef SMM_PKG_H_
#define SMM_PKG_H_


/*************************************************************************************************
 * INCLUDE FILES
 *************************************************************************************************/

#include PORT_ARCH_HEADER(core/smm)


/*-----------------------------------------------------------------------------------------------*
 * EXTERNS
 *-----------------------------------------------------------------------------------*//** @cond */

#ifdef SMM_PKG_H_VAR
#define SMM_PKG_H_EXT
#else
#define SMM_PKG_H_EXT extern
#endif


/** @endcond*//***********************************************************************************
 * DEFINES
 *************************************************************************************************/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Pomocni makro koji se koristi prilikom odlucivanja koji
 *              algoritam da se koristi u TLSF alokatoru.
 */
/*-----------------------------------------------------------------------------------------------*/
#define SMM_TLSF_SWITCHLIST             (SMM_TLSF_SMALLLIST * (1 << SMM_TLSF_BETA))

/*-------------------------------------------------------------------------------------------*//**
 * @name        Konstante za status blokova
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief		Maska koja se koristi za definisanje statusa bloka
 *
 * 				U implementaciji se koristi MSB bit za oznacavanje da li je
 * 				blok zauzet ili nije.
 *
 * 				Ukoliko je bit setovan na MSB poziciji onda je blok zauzet, u
 * 				suprotnom nije zauzet.
 */
/*-----------------------------------------------------------------------------------------------*/
#define BLOCK_STATUS_MASK				(smm_blkSize_T)ES_MASK_MSB(smm_blkSize_T)

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief		Marker koji pokazuje da je blok zauzet
 */
/*-----------------------------------------------------------------------------------------------*/
#define BLOCK_IS_BUSY					(smm_blkSize_T)ES_MASK_MSB(smm_blkSize_T)

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief		Marker koji pokazuje da blok nije zauzet
 */
/*-----------------------------------------------------------------------------------------------*/
#define BLOCK_IS_FREE   				(smm_blkSize_T)0

/** @} *//*--------------------------------------------------------------------------------------*/


/*************************************************************************************************
 * MACRO's
 *************************************************************************************************/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Vrsi konverziju broja bajtova u broj blokova
 *
 * @param       bytes                   Broj bajtova kojeg treba konvertovati.
 */
/*-----------------------------------------------------------------------------------------------*/
#define TLSF_TO_BLOCKS(bytes)                                                   \
    ((bytes) / sizeof(smm_blk_T))

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Vrsi konverziju broja blokova u broj bajtova
 *
 * @param       blocks                  Broj blokova kojeg treba konvertovati.
 */
/*-----------------------------------------------------------------------------------------------*/
#define TLSF_TO_BYTES(blocks)                                                   \
    ((blocks) * sizeof(smm_blk_T))

/*-------------------------------------------------------------------------------------------*//**
 * @name        Makroi za manipulaciju sa statusom blokova
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief		Izvlacenje statusa bloka iz @c size clana.
 *
 * @return		status da li je blok slobodan.
 */
/*-----------------------------------------------------------------------------------------------*/
#define BLK_STAT_QUERY(currBlk)												\
	((currBlk)->blk.size & BLOCK_STATUS_MASK)

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief		Vrsi postavljanje statusa bloka da je zauzet.
 */
/*-----------------------------------------------------------------------------------------------*/
#define BLK_STAT_BUSY(currBlk)												\
	(currBlk)->blk.size |= BLOCK_STATUS_MASK

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief		Vrsi postavljanje statusa bloka da je slobodan.
 */
/*-----------------------------------------------------------------------------------------------*/
#define BLK_STAT_FREE(currBlk)												\
	(currBlk)->blk.size &= ~BLOCK_STATUS_MASK

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Makroi za manipulaciju sa listama fizickih blokova
 *
 * @brief       Koristi se posebna implementacija Single Linked list with
 *              Sentinel gde nije potrebno vrsiti skeniranje liste prilikom
 *              dodavanja ili oduzimanja elemenata
 * @{ *//*---------------------------------------------------------------------------------------*/

#define PHY_BLK_PREV(currBlk)											    \
	((smm_blkHdr_T *)((uint8_t *)(currBlk) + currBlk->blk.size + sizeof(smm_blk_T)))

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Debug podrska
 *
 * @brief       Makroi za debug podrsku. Pogledati @ref dbg_intf.
 *
 * @{ *//*---------------------------------------------------------------------------------------*/

#if defined(OPT_DBG_SMM)
# define SMM_ASSERT                     DBG_ASSERT
# define SMM_ASSERT_ALWAYS              DBG_ASSERT_ALWAYS
# define SMM_COMPILE_ASSERT             DBG_ASSERT_COMPILE
# define SMM_DBG_DECL                   DBG_DECL
# define SMM_DBG_DEFINE_MODULE          DBG_DEFINE_MODULE
# define SMM_DBG_ENTRY                  DBG_ENTRY
# define SMM_DBG_EXIT                   DBG_EXIT
# define SMM_DBG_MACRO                  DBG_MACRO
# define SMM_DBG_CHECK                  DBG_CHECK
#else
# define SMM_ASSERT(expr)               DBG_EMPTY_MACRO()
# define SMM_ASSERT_ALWAYS(expr)        DBG_EMPTY_MACRO()
# define SMM_COMPILE_ASSERT(expr)       DBG_EMPTY_DECL()
# define SMM_DBG_DECL(expr)             DBG_EMPTY_DECL()
# define SMM_DBG_DEFINE_MODULE(expr)    DBG_EMPTY_DECL()
# define SMM_DBG_ENTRY()                DBG_EMPTY_MACRO()
# define SMM_DBG_EXIT()                 DBG_EMPTY_MACRO()
# define SMM_DBG_MACRO(expr)            DBG_EMPTY_MACRO()
# define SMM_DBG_CHECK(expr)            DBG_EMPTY_MACRO()
#endif

/** @} *//*--------------------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------------------------*
 * C/C++ #ifdef - open
 *-----------------------------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif


/*************************************************************************************************
 * DATA TYPES
 *************************************************************************************************/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Klasa memorije
 */
/*-----------------------------------------------------------------------------------------------*/
struct smm_memClass {
/**
 * @brief       Memorijski alokator
 */
    void * (*pAlloc)(size_t);

/**
 * @brief       Memorijski dealokator
 */
    void (*pDeAlloc)(void *);

/**
 * @brief       Velicina alocirane memorijske oblasti
 */
    size_t (*pBlockSize)(void *);
};

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Tip podataka za primarnu listu.
 */
/*-----------------------------------------------------------------------------------------------*/
typedef SMM_TLSF_FLBM_T smm_frstLst_T;

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Tip podataka za sekundardne liste.
 */
/*-----------------------------------------------------------------------------------------------*/
typedef SMM_TLSF_SLBM_T smm_scndLst_T;

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Tip podataka za indekse primarne i sekudnarnih lista.
 */
/*-----------------------------------------------------------------------------------------------*/
typedef SMM_TLSF_INDX_T smm_blkIndx_T;

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Struktura jednog bloka memorije.
 */
/*-----------------------------------------------------------------------------------------------*/
typedef struct smm_blk {
/**
 * @brief       Velicina ovog bloka memorije u bajtovima.
 */
	smm_blkSize_T   size;

/**
 * @brief       Lista fizickih blokova memorije.
 */
	pl_slsList_T    phyList;
} smm_blk_T;

/*-----------------------------------------------------------------------------------------------*/
/**
 * @extends     smm_blk_T
 * @brief       Zaglavlje jedne slobodne oblasti memorije.
 *
 * 				Ovo zaglavlje postoji samo u slobodnim oblastima memorije.
 */
/*-----------------------------------------------------------------------------------------------*/
typedef struct smm_blkHdr {
/**
 * @brief       Lista fizickih blokova i zauzece blokova
 */
    smm_blk_T   blk;
/**
 * @brief       Lista slobodnih blokova
 */
    pl_dlsList_T freeList;
} smm_blkHdr_T;


/*************************************************************************************************
 * GLOBAL VARIABLES
 *************************************************************************************************/

#if (SMM_IS_FF == OPT_SMM_TYPE)

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief		Cuvar liste slobodnih blokova
 */
/*-----------------------------------------------------------------------------------------------*/
SMM_PKG_H_EXT smm_blkHdr_T * dHeapSentinel;

#elif (SMM_IS_TLSF == OPT_SMM_TYPE)

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Bitmapa primarne liste.
 */
/*-----------------------------------------------------------------------------------------------*/
SMM_PKG_H_EXT smm_frstLst_T frstLstBitMap;

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Bitmape sekundardnih lista.
 */
/*-----------------------------------------------------------------------------------------------*/
SMM_PKG_H_EXT smm_scndLst_T scndLstBitMaps[SMM_TLSF_ALPHA];

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Mapa pokazivaca na memorijske blokove.
 */
/*-----------------------------------------------------------------------------------------------*/
SMM_PKG_H_EXT smm_blk_T * pFreeMap[SMM_TLSF_ALPHA][(1 << SMM_TLSF_BETA)];

#endif

/*-------------------------------------------------------------------------------------------*//**
 * @name        Promenljive koje se koriste u slucaju da je omoguceno debagovanje SMM modula
 * @{ *//*---------------------------------------------------------------------------------------*/

#if defined(OPT_DBG_SMM) || defined(__DOXYGEN__)
SMM_PKG_H_EXT void * smm_dbgHeapBegin;
SMM_PKG_H_EXT void * smm_dbgHeapEnd;
#endif

/** @} *//*--------------------------------------------------------------------------------------*/


/*************************************************************************************************
 * FUNCTION PROTOTYPES
 *************************************************************************************************/


/*-----------------------------------------------------------------------------------------------*
 * C/C++ #endif - close
 *-----------------------------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif


/*************************************************************************************************
 * CONFIGURATION ERRORS
 *************************************************************************************//** @cond */

#if defined(OPT_SMM_FVAR_STATIC)
# define SMM_FVAR                       static
#else
# define SMM_FVAR                       auto
#endif


/** @endcond *//** @} *//*************************************************************************
 * END of smm_pkg.h
 *************************************************************************************************/
#endif /* SMM_PKG_H_ */
