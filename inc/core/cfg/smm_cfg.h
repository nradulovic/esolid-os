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
 * @brief       Konfiguracija Simple Memory Management (SMM) modula
 *
 * ------------------------------------------------------------------------------------------------
 *
 * @addtogroup  smm_cfg
 *
 ****************************************************************************************//** @{ */


#ifndef SMM_CFG_H_
#define SMM_CFG_H_


/*************************************************************************************************
 * INCLUDE FILES
 *************************************************************************************************/


/*************************************************************************************************
 * DEFINES
 *************************************************************************************//** @cond */

#define SMM_IS_FF                       1
#define SMM_IS_TLSF                     2


/** @endcond *//**********************************************************************************
 * SETTINGS
 *************************************************************************************************/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Debug podrska SMM modula
 *
 *              Ukoliko je ova opcija:
 *              - definisana: omoguceno je debagiranje modula,
 *              - nedefinisana: nije omoguceno debagiranje modula.
 *
 *              Omogucavanjem debagiranja mogu da se ukljucuje sledeci makroi:
 *              - @ref DBG_ASSERT
 *              - @ref DBG_ASSERT_ALWAYS
 *              - @ref DBG_ASSERT_COMPILE
 *              - @ref DBG_CHECK
 *              - @ref DBG_ENTRY
 *              - @ref DBG_EXIT
 *
 * @see         dbg_intf
 */
/*-----------------------------------------------------------------------------------------------*/
#if defined(__DOXYGEN__)
# define OPT_DBG_SMM
#endif

/*-------------------------------------------------------------------------------------------*//**
 * @name        Odabir eksternog memorijskog alokatora
 *
 *              Ovim opcijama se bira memorijski alokator koji je definisan van
 *              eSolid sistema. U slucaju da programer zeli da koristi neki
 *              drugi memorijski alokator dovoljno je da izvrsi definisanje
 *              navedenih promenljivih i eSolid ce koristiti eksterni memorijski
 *              alokator.
 *
 * @note        Podrazumevano podesavanje: koristi se eSolid interni alokator,
 * 				TLSF algoritam.
 *
 * @{ *//*---------------------------------------------------------------------------------------*/

#if !defined(OPT_MALLOC) || defined(__DOXYGEN__)
# define OPT_MALLOC                     eS_hmemAlloc
#endif

#if !defined(OPT_MALLOC_I) || defined(__DOXYGEN__)
# define OPT_MALLOC_I                   eS_hmemAllocI
#endif

#if !defined(OPT_MFREE) || defined(__DOXYGEN__)
# define OPT_MFREE                      eS_hmemDeAlloc
#endif

#if !defined(OPT_MFREE_I) || defined(__DOXYGEN__)
# define OPT_MFREE_I                    eS_hmemDeAllocI
#endif

#if !defined(OPT_MSPACE) || defined(__DOXYGEN__)
# define OPT_MSPACE                     eS_hmemFreeSpace
#endif

#if !defined(OPT_MSPACE_I) || defined(__DOXYGEN__)
# define OPT_MSPACE_I                   eS_hmemFreeSpaceI
#endif

#if !defined(OPT_MBLKSIZE) || defined(__DOXYGEN__)
# define OPT_MBLKSIZE					eS_hmemBlockSize
#endif

#if !defined(OPR_MCOPY) || defined(__DOXYGEN__)
# define OPR_MCOPY						eS_memCopy
#endif

#if !defined(OPR_MCOPY) || defined(__DOXYGEN__)
# define OPR_MMOVE						eS_memMove
#endif

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Opsta podesavanja za alokatore
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Odabir algoritma alokatora koji se koristi.
 *
 *              Postoje vise implementacija memorijskog alokatora. Ovom opcijom
 *              se moze konfigurisati koja implementacija memorijskog alokatora
 *              se koristi u zavisnosti od potrebe aplikacije. Videti
 *              @ref doc_smm.
 *
 *              Postoje dve implementacije:
 *              - SMM_IS_FF             @ref smm_ff_alloc
 *              - SMM_IS_TLSF           @ref smm_tlsf_alloc
 *
 * @note        Podrazumevano podesavanje: koristi se TLSF
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(OPT_SMM_TYPE) || defined(__DOXYGEN__)
# define OPT_SMM_TYPE                   SMM_IS_TLSF
#endif

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Podesavanja za TLSF alokator
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Alfa koeficijent za TLSF alokator.
 */
/*-----------------------------------------------------------------------------------------------*/
#define SMM_TLSF_ALPHA                  7U

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Beta koeficijent za TLSF alokator.
 */
/*-----------------------------------------------------------------------------------------------*/
#define SMM_TLSF_BETA                   3U

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Tip podataka za primarnu listu.
 */
/*-----------------------------------------------------------------------------------------------*/
#define SMM_TLSF_FLBM_T                 uint32_t

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Tip podataka za sekundardne liste.
 */
/*-----------------------------------------------------------------------------------------------*/
#define SMM_TLSF_SLBM_T                 uint32_t

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Tip podataka za indekse primarne i sekudnarne liste.
 */
/*-----------------------------------------------------------------------------------------------*/
#define SMM_TLSF_INDX_T                 uint32_t

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Doseg primarnog i alternativnog algoritma za raspored blokova
 *              u listama.
 *
 *              Do ovog broja lista koristi se alternativni algoritam, a ako se
 *              slobodni blok trazi u listi van ovog broja koristi se primarni
 *              algoritam. Ovo je uvedeno da se pri zahtevima za malim blokovima
 *              koristi efikasniji algoritam.
 */
/*-----------------------------------------------------------------------------------------------*/
#define SMM_TLSF_SMALLLIST              2U

/** @} *//*--------------------------------------------------------------------------------------*/


/*************************************************************************************************
 * CONFIGURATION RESOLUTION
 *************************************************************************************//** @cond */


/** @endcond *//** @} *//*************************************************************************
 * END of smm_cfg.h
 *************************************************************************************************/
#endif /* SMM_CFG_H_ */
