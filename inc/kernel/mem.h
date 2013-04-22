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
 * @brief       Memory Management API
 * @defgroup    mem_intf Memory Management API
 * @brief       Javni interfejs
 *********************************************************************//** @{ */

#ifndef MEM_H_
#define MEM_H_

/*=========================================================  INCLUDE FILES  ==*/
#include "hal/hal.h"
#include "../config/sys_config.h"

/*===============================================================  MACRO's  ==*/
/*------------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/**
 * @brief       Abstraktni tip, pokazivac na deskriptor strukturu pool alokatora.
 */
typedef struct esPMemDesc esPmemDesc_T;

/**
 * @brief       Abstraktni tip, pokazivac na deskriptor strukturu dinamickog
 *              alokatora.
 */
typedef struct esDMemDesc esDmemDesc_T;

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Funkcije statickog memorijskog alokatora
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Inicijalizuje staticni memorijski alokator
 * @details     Ova funkcija se mora pozvati pre koriscenja funkcija staticnog
 *              memorijskog alokatora.
 * @api
 */
void esSmemInit(
    void);

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
 * @brief       Inicijalizuje dinamican memorijski alokator
 * @param       [out] desc              Deskriptor dinamickog alokatora
 * @param       [in] array              Predefinisani memorijski prostor koji se
 *                                      predaje dinamickom alokatoru na
 *                                      koriscenje
 * @param       elements                Velicina memorijskog prostora u
 *                                      bajtovima
 * @details     Ova funkcija se mora pozvati pre koriscenja funkcija dinamickog
 *              memorijskog alokatora.
 */
void esDmemInit(
    esDmemDesc_T *  desc,
    void *          array,
    size_t          elements);

/**
 * @brief       Dodeljuje memorijski prostor velicine @c size
 * @param       [in] desc               Deskriptor dinamickog alokatora
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
    esDmemDesc_T *  desc,
    size_t          size);

/**
 * @brief       Dodeljuje memorijski prostor velicine @c size
 * @param       [in] desc               Deskriptor dinamickog alokatora
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
    esDmemDesc_T *  desc,
    size_t          size);

/**
 * @brief       Reciklira memorijski prostor na koji pokazije @c mem
 *              pokazivac
 * @param       [in] desc               Deskriptor dinamickog alokatora
 * @param       mem                     Pokazivac na prethodno dodeljen
 * 										memorijski prostor.
 * @api
 */
void esDmemDeAlloc(
    esDmemDesc_T *  desc,
    void *          mem);

/**
 * @brief       Reciklira memorijski prostor na koji pokazije @c mem
 *              pokazivac
 * @param       [in] desc               Deskriptor dinamickog alokatora
 * @param       mem                     Pokazivac na prethodno dodeljen
 *                                      memorijski prostor.
 * @iclass
 */
void esDmemDeAllocI(
    esDmemDesc_T *  desc,
    void *          mem);

/**
 * @brief       Vraca velicinu trenutno slobodne memorije u bajtovima.
 * @param       [in] desc               Deskriptor dinamickog alokatora
 * @return      Velicina slobodne memorije u bajtovima.
 * @details     Ukoliko je memorija jako fragmenitisana, sto je karakteristicno
 *              za first fit algoritam, moze se desiti da postoji dovoljno
 *              slobodne memorije, ali ne i za blok zahtevane velicine. U tom
 *              slucaju memorijski alokator nece biti u mogucnosti da ispuni
 *              zahtev.
 * @api
 */
size_t esDmemFreeSpace(
    esDmemDesc_T *  desc);

/**
 * @brief       Vraca velicinu trenutno slobodne memorije u bajtovima.
 * @param       [in] desc               Deskriptor dinamickog alokatora
 * @return      Velicina slobodne memorije u bajtovima.
 * @details     Ukoliko je memorija jako fragmenitisana, sto je karakteristicno
 *              za first fir algoritam, moze se desiti da postoji dovoljno
 *              slobodne memorije, ali ne i bloka zahtevane velicine. U tom
 *              slucaju memorijski alokator nece biti u mogucnosti da ispuni
 *              zahtev.
 * @iclass
 */
size_t esDmemFreeSpaceI(
    esDmemDesc_T *  desc);

/** @} *//*-------------------------------------------------------------------*/
/*------------------------------------------------------------------------*//**
 * @name        Funkcije Pool memorijskog alokatora
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Inicijalizuje pool memorijski alokator
 * @param       [out] desc              Deskriptor pool alokatora
 * @param       [in] array              Predefinisani memorijski prostor koji se
 *                                      predaje pool alokatoru na koriscenje
 * @param       blocks                  Broj blokova
 * @param       blockSize               Velicina jednog bloka u bajtovima
 * @details     Ova funkcija se mora pozvati pre koriscenja funkcija pool
 *              memorijskog alokatora.
 */
void esPmemInit(
    esPmemDesc_T *  desc,
    void *          array,
    size_t          blocks,
    size_t          blockSize);

/** @} *//*-------------------------------------------------------------------*/
/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of mem.h
 ******************************************************************************/
#endif /* MEM_H_ */
