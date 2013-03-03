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
 * @author      Nenad Radulovic
 * @brief       Menadzment memorije
 * @addtogroup  mm_intf
 ****************************************************************************************//** @{ */


#ifndef MM_H_
#define MM_H_

/*============================================================================  INCLUDE FILES  ==*/
/*==================================================================================  DEFINES  ==*/
/*==================================================================================  MACRO's  ==*/
/*-------------------------------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*===============================================================================  DATA TYPES  ==*/
/*=========================================================================  GLOBAL VARIABLES  ==*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Klase memorijskog alokatora
 * @{ *//*---------------------------------------------------------------------------------------*/

/**
 * @brief       Dinamicki memorijski alokator (heap memory)
 */
extern const C_ROM esMemClass_T esMemHeapClass;

/**
 * @brief       Staticki memorijski alokator (static memory)
 */
extern const C_ROM esMemClass_T esMemStaticClass;
/** @} *//*--------------------------------------------------------------------------------------*/

/*======================================================================  FUNCTION PROTOTYPES  ==*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Funkcije statickog memorijskog alokatora
 * @{ *//*---------------------------------------------------------------------------------------*/

/**
 * @brief       Dodeljuje memorijski prostor velicine @c aSize
 * @param       aSize                   Velicina zahtevanog memorijskog prostora
 *                                      u bajtovima.
 * @return      Pokazivac na rezervisani memorijski blok.
 * @details     U debug rezimu ova funkcija uvek vraca pokazivac, odnosno, ne
 *              moze se desiti da vrati NULL pokazivac, kao sto nalaze
 *              standardna implementacija @c malloc C funkcije. Ukoliko se
 *              zahtevana memorija ne moze dobaviti generisace se ASSERT greska.
 *              Kada se ne koristi debug rezim funkcija se ponasa u skladu sa
 *              standardom.
 * @api
 */
void * esSmemAlloc(
    size_t      aSize);

/**
 * @brief       Dodeljuje memorijski prostor velicine @c aSize
 * @param       aSize                   Velicina zahtevanog memorijskog prostora
 *                                      u bajtovima.
 * @return      Pokazivac na rezervisani memorijski blok.
 * @details     U debug rezimu ova funkcija uvek vraca pokazivac, odnosno, ne
 *              moze se desiti da vrati NULL pokazivac, kao sto nalaze
 *              standardna implementacija @c malloc C funkcije. Ukoliko se
 *              zahtevana memorija ne moze dobaviti generisace se ASSERT greska.
 *              Kada se ne koristi debug rezim funkcija se ponasa u skladu sa
 *              standardom.
 * @iclass
 */
void * esSmemAllocI(
    size_t      aSize);

/**
 * @brief       Vraca velicinu trenutno slobodne memorije u bajtovima.
 * @return      Velicina slobodne memorije u bajtovima.
 * @api
 */
size_t esSmemFreeSpace(
    void);

/**
 * @brief       Vraca velicinu trenutno slobodne memorije u bajtovima.
 * @return      Velicina slobodne memorije u bajtovima.
 * @iclass
 */
size_t esSmemFreeSpaceI(
    void);

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Funkcije dinamickog memorijskog alokatora
 * @{ *//*---------------------------------------------------------------------------------------*/

/**
 * @brief       Dodeljuje memorijski prostor velicine @c aSize
 * @param       aSize                   Velicina zahtevanog memorijskog prostora
 *                                      u bajtovima.
 * @return      Pokazivac na rezervisani memorijski blok.
 * @details     U debug rezimu ova funkcija uvek vraca pokazivac, odnosno, ne
 *              moze se desiti da vrati NULL pokazivac, kao sto nalaze
 *              standardna implementacija @c malloc C funkcije. Ukoliko se
 *              zahtevana memorija ne moze dobaviti generisace se ASSERT greska.
 *              Kada se ne koristi debug rezim funkcija se ponasa u skladu sa
 *              standardom.
 * @api
 */
void * esHmemAlloc(
    size_t      aSize);

/**
 * @brief       Dodeljuje memorijski prostor velicine @c aSize
 * @param       aSize                   Velicina zahtevanog memorijskog prostora
 *                                      u bajtovima.
 * @return      Pokazivac na rezervisani memorijski blok.
 * @details     U debug rezimu ova funkcija uvek vraca pokazivac, odnosno, ne
 *              moze se desiti da vrati NULL pokazivac, kao sto nalaze
 *              standardna implementacija @c malloc C funkcije. Ukoliko se
 *              zahtevana memorija ne moze dobaviti generisace se ASSERT greska.
 *              Kada se ne koristi debug rezim funkcija se ponasa u skladu sa
 *              standardom.
 * @iclass
 */
void * esHmemAllocI(
    size_t      aSize);

/**
 * @brief       Reciklira memorijski prostor na koji pokazije @c aMemory
 *              pokazivac
 * @param       aMemory                 Pokazivac na prethodno dodeljen
 * 										memorijski prostor.
 * @api
 */
void esHmemDeAlloc(
    void        * aMemory);

/**
 * @brief       Reciklira memorijski prostor na koji pokazije @c aMemory
 *              pokazivac
 * @param       aMemory                 Pokazivac na prethodno dodeljen
 *                                      memorijski prostor.
 * @iclass
 */
void esHmemDeAllocI(
    void        * aMemory);

/**
 * @brief       Vraca velicinu trenutno slobodne memorije u bajtovima.
 * @return      Velicina slobodne memorije u bajtovima.
 * @details     Ukoliko je memorija jako fragmenitisana, sto je karakteristicno
 *              za first fit algoritam, moze se desiti da postoji dovoljno
 *              slobodne memorije, ali ne i za blok zahtevane velicine. U tom
 *              slucaju memorijski alokator nece biti u mogucnosti da ispuni
 *              zahtev.
 * @api
 */
size_t esHmemFreeSpace(
    void);

/**
 * @brief       Vraca velicinu trenutno slobodne memorije u bajtovima.
 * @return      Velicina slobodne memorije u bajtovima.
 * @details     Ukoliko je memorija jako fragmenitisana, sto je karakteristicno
 *              za first fir algoritam, moze se desiti da postoji dovoljno
 *              slobodne memorije, ali ne i bloka zahtevane velicine. U tom
 *              slucaju memorijski alokator nece biti u mogucnosti da ispuni
 *              zahtev.
 * @iclass
 */
size_t esHmemFreeSpaceI(
    void);

/**
 * @brief		Vraca velicinu dodeljene memorije
 * @param 		aMemory					Pokazivac na prethodno dodeljen
 * 										memorijski prostor.
 * @return		Velicina dodeljenog memorijskog prostora.
 * @details     Sa obzirom da alokatori koriste granulaciju i/ili imaju
 *              implementaciono ogranicenje, prilikom dodeljivanja memorije moze
 *              se dodeliti veca memorija od zahtevane. Ova funkcija se moze
 *              koristiti ukoliko je potrebna informacija o stvarnoj velicini
 *              dodeljenog bloka.
 * @api
 */
size_t esHmemBlockSize(
	void 		* aMemory);

/** @} *//*--------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*===================================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/

/** @endcond *//** @} *//*************************************************************************
 * END of mem.h
 *************************************************************************************************/
#endif /* MM_H_ */
