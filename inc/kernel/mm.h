/******************************************************************************
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
 *//***********************************************************************//**
 * @file
 * @author      Nenad Radulovic
 * @brief       Menadzment memorije
 * @details     This file is not meant to be included in application code
 *              directly but through the inclusion of "kernel.h" file.
 * @addtogroup  mm_intf
 *********************************************************************//** @{ */


#ifndef MM_H_
#define MM_H_

/*=========================================================  INCLUDE FILES  ==*/
#include "hal/hal.h"
#include "../config/kernel_config.h"

/*===============================================================  DEFINES  ==*/
/*===============================================================  MACRO's  ==*/
/*------------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/**
 * @brief       Memorijska klasa alokatora
 */
typedef struct esMemClass esMemClass_T;

/*======================================================  GLOBAL VARIABLES  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Klase memorijskog alokatora
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Dinamicki memorijski alokator (heap memory)
 */
extern const C_ROM esMemClass_T esMemDynClass;

/**
 * @brief       Staticki memorijski alokator (static memory)
 */
extern const C_ROM esMemClass_T esMemStaticClass;

/** @} *//*-------------------------------------------------------------------*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

/**
 * @brief       Inicijalizuje memorijske alokatore
 * @details     Ova funkcija se mora pozvati pre koriscenja funkcija memorijskih
 *              alokatora.
 * @note        U slucaju da se MM modul koristi od strane SMP ili Kernel-a ova
 *              funkcija se automatski poziva.
 */
void esMemInit(
    void);

/*------------------------------------------------------------------------*//**
 * @name        Funkcije statickog memorijskog alokatora
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Dodeljuje memorijski prostor velicine @c size
 * @param       size                    Velicina zahtevanog memorijskog prostora
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
    size_t          size);

/**
 * @brief       Dodeljuje memorijski prostor velicine @c size
 * @param       size                    Velicina zahtevanog memorijskog prostora
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
    size_t          size);

/**
 * @brief       Vraca velicinu trenutno slobodne memorije u bajtovima.
 * @return      Velicina slobodne memorije u bajtovima.
 * @api
 */
size_t esSmemFreeSpace(
    void);

/** @} *//*-------------------------------------------------------------------*/
/*------------------------------------------------------------------------*//**
 * @name        Funkcije dinamickog memorijskog alokatora
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Dodeljuje memorijski prostor velicine @c size
 * @param       size                    Velicina zahtevanog memorijskog prostora
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
void * esDmemAlloc(
    size_t          size);

/**
 * @brief       Dodeljuje memorijski prostor velicine @c size
 * @param       size                    Velicina zahtevanog memorijskog prostora
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
void * esDmemAllocI(
    size_t          size);

/**
 * @brief       Reciklira memorijski prostor na koji pokazije @c mem
 *              pokazivac
 * @param       mem                     Pokazivac na prethodno dodeljen
 * 										memorijski prostor.
 * @api
 */
void esDmemDeAlloc(
    void *          mem);

/**
 * @brief       Reciklira memorijski prostor na koji pokazije @c mem
 *              pokazivac
 * @param       mem                     Pokazivac na prethodno dodeljen
 *                                      memorijski prostor.
 * @iclass
 */
void esDmemDeAllocI(
    void *          mem);

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
size_t esDmemFreeSpace(
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
size_t esDmemFreeSpaceI(
    void);

/**
 * @brief		Vraca velicinu dodeljene memorije
 * @param 		mem					    Pokazivac na prethodno dodeljen
 * 										memorijski prostor.
 * @return		Velicina dodeljenog memorijskog prostora.
 * @details     Sa obzirom da alokatori koriste granulaciju i/ili imaju
 *              implementaciono ogranicenje, prilikom dodeljivanja memorije moze
 *              se dodeliti veca memorija od zahtevane. Ova funkcija se moze
 *              koristiti ukoliko je potrebna informacija o stvarnoj velicini
 *              dodeljenog bloka.
 * @api
 */
size_t esDmemBlockSize(
	void *          mem);

/** @} *//*-------------------------------------------------------------------*/
/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of mem.h
 ******************************************************************************/
#endif /* MM_H_ */
