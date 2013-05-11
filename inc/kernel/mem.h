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
 * @addtogroup  mem_intf
 * @brief       Javni interfejs
 *********************************************************************//** @{ */

#ifndef MEM_H_
#define MEM_H_

/*=========================================================  INCLUDE FILES  ==*/
#include "hal/hal_compiler.h"
#include "../config/sys_config.h"
#include "../config/mem_config.h"

/*===============================================================  MACRO's  ==*/
/*------------------------------------------------------  C++ extern begin  --*/
#ifdef __cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/**
 * @brief       Status memorije
 * @details     Ovom strukturom opisuje se status memorijske instance. Instanca
 *              moze biti @c static, @c dynamic i @c pool tipa. Ukoliko se mora
 *              dobaviti informacija o velicini, zauzecu i dostupnosti memorije
 *              moraju se koristiti odgovarajuce updateStatus funckcije koje ce
 *              vratiti trazene podatke u ovoj strukturi.
 * @see         esSMemUpdateStatusI(), esPMemUpdateStatusI(), esDMemUpdateStatusI()
 * @api
 */
typedef struct esMemStatus {
/** @brief      Velicina dinamicke memorije                                   */
    size_t          size;

/** @brief      Iznos ukupne slobodne memorije                                */
    size_t          freeSpaceTotal;

/** @brief      Iznos maksimalno dostupne memorije kao jedinstven blok        */
    size_t          freeSpaceAvailable;
} esMemStatus_T;

/*------------------------------------------------------------------------*//**
 * @name        Podaci dinamickog alokatora
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Deskriptor Dinamickog alokatora
 * @details     Ovom strukturom se referenciraju instance dinamickog alokatora.
 * @p           Cuva se samo podatak o cuvaru dinamickog memorijskog prostora.
 * @see         esDMemInit()
 * @api
 */
typedef struct esDMemHandle {
/** @brief      Pokazivac na cuvara memorije                                  */
    struct dMemBlock * sentinel;

#if defined(GUARD_T) || defined(__DOXYGEN__)
/** @brief      Cuvar dinamickog alokatora                                    */
    GUARD_T         guard;
#endif
} esDMemHandle_T;

/** @} *//*-------------------------------------------------------------------*/
/*------------------------------------------------------------------------*//**
 * @name        Podaci pool alokatora
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Deskriptor Pool alokatora
 * @details     Ovom strukturom se referenciraju instance pool alokatora.
 * @p           Ovde se cuvaju podaci o velicini pool memorije i o velicini
 *              jednog bloka. Pored tih informacija nalazi se cuvar pool
 *              memorijskog prostora. Ove informacije su potrebne za rad ostalim
 *              funkcijama pool alokatora.
 * @see         esPMemInit()
 * @api
 */
typedef struct esPMemHandle {
/** @brief      Velicina pool memorije                                        */
    size_t          size;

/** @brief      Velicina jednog bloka                                         */
    size_t          blockSize;

/** @brief      Pokazivac na cuvara memorije                                  */
    struct pMemBlock * sentinel;

#if defined(GUARD_T) || defined(__DOXYGEN__)
/** @brief      Cuvar pool alokatora                                          */
    GUARD_T         guard;
#endif
} esPMemHandle_T;

/** @} *//*-------------------------------------------------------------------*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Funkcije Statickog memorijskog alokatora
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Inicijalizuje staticni memorijski alokator
 * @details     Ova funkcija se mora pozvati pre koriscenja funkcija staticnog
 *              memorijskog alokatora.
 * @api
 */
void esSMemInit(
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
 * @iclass
 */
void * esSMemAllocI(
    size_t          size);

/**
 * @brief       Vraca velicinu trenutno slobodne memorije u bajtovima.
 * @param       [out] status            Status struktura static alokatora
 * @iclass
 */
void esSMemUpdateStatusI(
    esMemStatus_T * status);

/** @} *//*-------------------------------------------------------------------*/
/*------------------------------------------------------------------------*//**
 * @name        Funkcije Dinamickog memorijskog alokatora
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Inicijalizuje dinamican memorijski alokator
 * @param       [out] handle            Deskriptor dinamickog alokatora
 * @param       [in] storage            Predefinisani memorijski prostor koji se
 *                                      predaje dinamickom alokatoru na
 *                                      koriscenje
 * @param       storageSize             Velicina memorijskog prostora u
 *                                      bajtovima
 * @details     Ova funkcija se mora pozvati pre koriscenja funkcija dinamickog
 *              memorijskog alokatora.
 * @pre         Opcija @ref OPT_MEM_DMEM_ENABLE mora da bude aktivna
 * @warning     Funkcija zahteva da pokazivaci handle i pool budu poravnani
 *              (aligned). Ukoliko se koriste eSolid alokatori za instaciranje
 *              @c handle strukture i @c poolStorage onda je poravnani pristup
 *              osiguran.
 * @warning     Funkcija zahteva da velicina memorijskog prostora @c storageSize
 *              bude poravnana (aligned). Na primer za 32-bitni procesor
 *              (poravnanje 4 bajta): ako je @c storageSize == 313 onda je
 *              potrebno poravnati na sledecu vecu vrednost koja je deljiva sa 4,
 *              u ovom slucaju ce to biti 316.
 * @api
 */
void esDMemInit(
    esDMemHandle_T * handle,
    void *          storage,
    size_t          storageSize);

/**
 * @brief       Dodeljuje memorijski prostor velicine @c size
 * @param       [in] handle             Deskriptor dinamickog alokatora
 * @param       size                    Velicina zahtevanog memorijskog prostora
 *                                      u bajtovima.
 * @return      Pokazivac na rezervisani memorijski blok.
 * @details     U debug rezimu ova funkcija uvek vraca pokazivac, odnosno, ne
 *              moze se desiti da vrati NULL pokazivac, kao sto nalaze
 *              standardna implementacija @c malloc C funkcije. Ukoliko se
 *              zahtevana memorija ne moze dobaviti generisace se ASSERT greska.
 *              Kada se ne koristi debug rezim funkcija se ponasa u skladu sa
 *              standardom.
 * @pre         Opcija @ref OPT_MEM_DMEM_ENABLE mora da bude aktivna
 * @iclass
 */
void * esDMemAllocI(
    esDMemHandle_T * handle,
    size_t          size);

/**
 * @brief       Dodeljuje memorijski prostor velicine @c size
 * @param       [in] handle             Deskriptor dinamickog alokatora
 * @param       size                    Velicina zahtevanog memorijskog prostora
 *                                      u bajtovima.
 * @return      Pokazivac na rezervisani memorijski blok.
 * @details     U debug rezimu ova funkcija uvek vraca pokazivac, odnosno, ne
 *              moze se desiti da vrati NULL pokazivac, kao sto nalaze
 *              standardna implementacija @c malloc C funkcije. Ukoliko se
 *              zahtevana memorija ne moze dobaviti generisace se ASSERT greska.
 *              Kada se ne koristi debug rezim funkcija se ponasa u skladu sa
 *              standardom.
 * @pre         Opcija @ref OPT_MEM_DMEM_ENABLE mora da bude aktivna
 * @note        Funkcija koristi makroe @ref OPT_GUARD_LOCK i @ref OPT_GUARD_UNLOCK za
 *              zastitu memorije od istovremenog pristupa.
 * @api
 */
void * esDMemAlloc(
    esDMemHandle_T * handle,
    size_t          size);

/**
 * @brief       Reciklira memorijski prostor na koji pokazije @c mem
 *              pokazivac
 * @param       [in] handle             Deskriptor dinamickog alokatora
 * @param       [in] mem                Pokazivac na prethodno dodeljen
 *                                      memorijski prostor.
 * @pre         Opcija @ref OPT_MEM_DMEM_ENABLE mora da bude aktivna
 * @iclass
 */
void esDMemDeAllocI(
    esDMemHandle_T * handle,
    void *          mem);

/**
 * @brief       Reciklira memorijski prostor na koji pokazije @c mem
 *              pokazivac
 * @param       [in] handle             Deskriptor dinamickog alokatora
 * @param       [in] mem                Pokazivac na prethodno dodeljen
 *                                      memorijski prostor.
 * @pre         Opcija @ref OPT_MEM_DMEM_ENABLE mora da bude aktivna
 * @note        Funkcija koristi makroe @ref OPT_GUARD_LOCK i @ref OPT_GUARD_UNLOCK za
 *              zastitu memorije od istovremenog pristupa.
 * @api
 */
void esDMemDeAlloc(
    esDMemHandle_T * handle,
    void *          mem);

/**
 * @brief       Vraca velicinu trenutno slobodne memorije u bajtovima.
 * @param       [in] handle             Deskriptor dinamickog alokatora
 * @param       [out] status            Status struktura dinamickog alokatora
 * @details     Ukoliko je memorija jako fragmenitisana, sto je karakteristicno
 *              za first fir algoritam, moze se desiti da postoji dovoljno
 *              slobodne memorije, ali ne i bloka zahtevane velicine. U tom
 *              slucaju memorijski alokator nece biti u mogucnosti da ispuni
 *              zahtev.
 * @pre         Opcija @ref OPT_MEM_DMEM_ENABLE mora da bude aktivna
 * @iclass
 */
void esDMemUpdateStatusI(
    esDMemHandle_T * handle,
    esMemStatus_T * status);

/** @} *//*-------------------------------------------------------------------*/
/*------------------------------------------------------------------------*//**
 * @name        Funkcije Pool memorijskog alokatora
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Inicijalizuje pool memorijski alokator
 * @param       [out] handle            Deskriptor pool alokatora
 * @param       [in] pool               Predefinisani memorijski prostor koji se
 *                                      predaje pool alokatoru na koriscenje
 * @param       poolSize                Velicina pool memorijskog prostora
 * @param       blockSize               Velicina jednog bloka u bajtovima
 * @details     Ova funkcija se mora pozvati pre koriscenja funkcija pool
 *              memorijskog alokatora. Ona ce izracunati koliko blokova se mogu
 *              formirati u pool.
 * @warning     Funkcija zahteva da pokazivaci handle i pool budu poravnani
 *              (aligned). Ukoliko se koriste eSolid alokatori za instaciranje
 *              @c handle strukture i @c poolStorage onda je poravnani pristup
 *              osiguran.
 * @warning     Funkcija zahteva da velicina bloka @c blockSize bude poravnana
 *              (aligned). Na primer za 32-bitni procesor (poravnanje 4 bajta):
 *              ako je @c blockSize == 3 onda je potrebno poravnati na sledecu
 *              vecu vrednost koja je deljiva sa 4, odnosno, u ovom slucaju ce
 *              to biti 4.
 * @api
 */
void esPMemInit(
    esPMemHandle_T * handle,
    void *          pool,
    size_t          poolSize,
    size_t          blockSize);

/**
 * @brief       Racuna potrebnu velicinu @c pool-a za cuvanje blokova
 * @param       blocks                  Koliko je blokova potrebno
 * @param       blockSize               Velicina jednog bloka
 * @return      Velicina potrebnog niza u bajtovima.
 * @api
 */
size_t esPMemCalcPoolSize(
    size_t          blocks,
    size_t          blockSize);

/**
 * @brief       Alocira jedan blok iz memory pool-a
 * @param       [in] handle             Deskriptor pool alokatora
 * @return      Pokazivac na alocirani memorijski blok
 * @iclass
 */
void * esPMemAllocI(
    esPMemHandle_T * handle);

/**
 * @brief       Alocira jedan blok iz memory pool-a
 * @param       [in] handle             Deskriptor pool alokatora
 * @return      Pokazivac na alocirani memorijski blok
 * @note        Funkcija koristi makroe @ref OPT_GUARD_LOCK i @ref OPT_GUARD_UNLOCK za
 *              zastitu memorije od istovremenog pristupa.
 * @api
 */
void * esPMemAlloc(
    esPMemHandle_T * handle);

/**
 * @brief       Oslobadja prethodno alocirani blok
 * @param       [in] handle             Deskriptor pool alokatora
 * @param       [in] mem                Prethodno alociran blok memorije
 * @iclass
 */
void esPMemDeAllocI(
    esPMemHandle_T * handle,
    void *          mem);

/**
 * @brief       Oslobadja prethodno alocirani blok
 * @param       [in] handle             Deskriptor pool alokatora
 * @param       [in] mem                Prethodno alociran blok memorije
 * @note        Funkcija koristi makroe @ref OPT_GUARD_LOCK i @ref OPT_GUARD_UNLOCK za
 *              zastitu memorije od istovremenog pristupa.
 * @api
 */
void esPMemDeAlloc(
    esPMemHandle_T * handle,
    void *          mem);

/**
 * @brief       Vraca statusne informacije pool memorije
 * @param       [in] handle             Deskriptor pool alokatora
 * @param       [out] status            Status struktura pool alokatora
 * @iclass
 */
void esPMemUpdateStatusI(
    esPMemHandle_T * handle,
    esMemStatus_T * status);

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
