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
 * @author      Nenad Radulovic
 *
 * @brief       Interfejs Memory Management podmodula.
 *
 * ------------------------------------------------------------------------------------------------
 *
 * @addtogroup  mm_intf
 *
 ****************************************************************************************//** @{ */


#ifndef MM_H_
#define MM_H_


/*************************************************************************************************
 * INCLUDE FILES
 *************************************************************************************************/

#include "core/cfg/mm_cfg.h"


/*-----------------------------------------------------------------------------------------------*
 * EXTERNS
 *-----------------------------------------------------------------------------------*//** @cond */

#ifdef MM_H_VAR
# define MM_H_EXT
#else
# define MM_H_EXT extern
#endif


/** @endcond*//***********************************************************************************
 * DEFINES
 *************************************************************************************************/


/*************************************************************************************************
 * MACRO's
 *************************************************************************************************/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief		Rezervise memorijski prostor za koriscenje od strane dinamickog
 * 				memorijskog alokatora (hmem)
 *
 * @param 		size					Zeljena velicina dinamicke memorije u
 * 										bajtovima.
 * @attention   Makro se mora pozvati pre upotrebe bilo koje funkcija iz MM
 *              modula.
 */
/*-----------------------------------------------------------------------------------------------*/
#define ES_MEMORY_DEFINE(name, size)		    								\
    static C_ALIGNED(ES_CPU_ATTRIB_ALIGNMENT) uint8_t name[ES_ALIGN(size, ES_CPU_ATTRIB_ALIGNMENT)]


/*-----------------------------------------------------------------------------------------------*
 * C/C++ #ifdef - open
 *-----------------------------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif


/*************************************************************************************************
 * DATA TYPES
 *************************************************************************************************/


/*************************************************************************************************
 * GLOBAL VARIABLES
 *************************************************************************************************/

/*-------------------------------------------------------------------------------------------*//**
 * @name        Klase memorijskog alokatora
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Dinamicki memorijski alokator (heap memory)
 */
/*-----------------------------------------------------------------------------------------------*/
extern const C_ROM esMemClass_T memHeapClass;

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Skladiste memorije (memory pool)
 */
/*-----------------------------------------------------------------------------------------------*/
extern const C_ROM esMemClass_T memPoolClass;

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Staticki memorijski alokator (static memory)
 */
/*-----------------------------------------------------------------------------------------------*/
extern const C_ROM esMemClass_T memStaticClass;

/** @} *//*--------------------------------------------------------------------------------------*/


/*************************************************************************************************
 * FUNCTION PROTOTYPES
 *************************************************************************************************/

/*-------------------------------------------------------------------------------------------*//**
 * @name        Funkcije statickog memorijskog alokatora
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Inicijalizuje memorijski alokator sa memorijom @c aHeap
 *              velicine @c aSize
 * @param       aHeap                   Pokazivac na unapred rezervisan
 *                                      memorijski prostor,
 * @param       aSize                   velicina memorijskog prostora u
 *                                      bajtovima.
 * @note        Nakon inicijalizacije memorijskog prostora, slobodna memorija
 *              nece biti jednaka aHeap velicini vec ce biti umanjena za iznos
 *              koji priblizno odgovara statickom zauzecu alokatora koji se
 *              koristi. Na primer, first fit alokator ce umanjiti vrednost
 *              slobodne memorije za 24B u Cortex-M3 portu.
 * @notapi
 *
 *              Pre pozivanja ove funkcije potrebno je izvrsiti rezervaciju
 *              memorijskog prostora. To se vrsi upotrebom
 *              @ref ES_MEMORY_DEFINE makroa.
 */
/*-----------------------------------------------------------------------------------------------*/
void esSmemCtor(
    void        * aHeap,
    size_t      aSize);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Dodeljuje memorijski prostor velicine @c aSize
 * @param       aSize                   Velicina zahtevanog memorijskog prostora
 *                                      u bajtovima.
 * @pre         Najpre treba inicijalizovati MM modul pozivom
 *              @ref ES_MEMORY_DEFINE makroa.
 * @api
 *
 *              U debug rezimu ova funkcija uvek vraca pokazivac, odnosno, ne
 *              moze se desiti da vrati NULL pokazivac, kao sto nalaze
 *              standardna implementacija @c malloc C funkcije. Ukoliko se
 *              zahtevana memorija ne moze dobaviti generisace se ASSERT greska.
 *              Kada se ne koristi debug rezim funkcija se ponasa u skladu sa
 *              standardom.
 */
/*-----------------------------------------------------------------------------------------------*/
void * esSmemAlloc(
    size_t      aSize);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Dodeljuje memorijski prostor velicine @c aSize
 * @param       aSize                   Velicina zahtevanog memorijskog prostora
 *                                      u bajtovima.
 * @pre         Najpre treba inicijalizovati MM modul pozivom
 *              @ref ES_MEMORY_DEFINE makroa.
 * @iclass
 *
 *              U debug rezimu ova funkcija uvek vraca pokazivac, odnosno, ne
 *              moze se desiti da vrati NULL pokazivac, kao sto nalaze
 *              standardna implementacija @c malloc C funkcije. Ukoliko se
 *              zahtevana memorija ne moze dobaviti generisace se ASSERT greska.
 *              Kada se ne koristi debug rezim funkcija se ponasa u skladu sa
 *              standardom.
 */
/*-----------------------------------------------------------------------------------------------*/
void * esSmemAllocI(
    size_t      aSize);

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Funkcije dinamickog memorijskog alokatora
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Inicijalizuje memorijski alokator sa memorijom @c aHeap
 *              velicine @c aSize
 * @param       aHeap                   Pokazivac na unapred rezervisan
 *                                      memorijski prostor,
 * @param       aSize                   velicina memorijskog prostora u
 *                                      bajtovima.
 * @note		Nakon inicijalizacije memorijskog prostora, slobodna memorija
 * 				nece biti jednaka aHeap velicini vec ce biti umanjena za iznos
 * 				koji priblizno odgovara statickom zauzecu alokatora koji se
 * 				koristi. Na primer, first fit alokator ce umanjiti vrednost
 * 				slobodne memorije za 24B u Cortex-M3 portu.
 * @api
 *
 *              Pre pozivanja ove funkcije potrebno je izvrsiti rezervaciju
 *              memorijskog prostora. To se moze izvrsiti na nekoliko nacina,
 *              ali se preporucuje metoda upotrebom MM_HEAP_MM_DEFINE makroa.
 *
 *              Primer rezervacije i inicijalizacije memorije velicine 1kB:
 *
 *              @code
 *              uint8_t mem_memory[1024];
 *
 *              esHmemInit((void *)&mem_memory, sizeof(mem_memory[1024]));
 *              @endcode
 *
 *              ili koristeci dinamicku memoriju:
 *
 *              @code
 *              uint8_t * pMemory;
 *
 *              pMemory = (uint8_t *)malloc(1024);
 *
 *              esHmemInit((void *)pMemory, 1024);
 *              @endcode
 */
/*-----------------------------------------------------------------------------------------------*/
void esHmemInit(
    void        * aHeap,
    size_t      aSize);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Dodeljuje memorijski prostor velicine @c aSize
 * @param       aSize                   Velicina zahtevanog memorijskog prostora
 *                                      u bajtovima.
 * @pre         Najpre treba inicijalizovati MM modul pozivom
 *              @ref ES_MEMORY_DEFINE makroa.
 * @api
 *
 *              U debug rezimu ova funkcija uvek vraca pokazivac, odnosno, ne
 *              moze se desiti da vrati NULL pokazivac, kao sto nalaze
 *              standardna implementacija @c malloc C funkcije. Ukoliko se
 *              zahtevana memorija ne moze dobaviti generisace se ASSERT greska.
 *              Kada se ne koristi debug rezim funkcija se ponasa u skladu sa
 *              standardom.
 */
/*-----------------------------------------------------------------------------------------------*/
void * esHmemAlloc(
    size_t      aSize);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Dodeljuje memorijski prostor velicine @c aSize
 * @param       aSize                   Velicina zahtevanog memorijskog prostora
 *                                      u bajtovima.
 * @pre         Najpre treba inicijalizovati MM modul pozivom
 *              @ref ES_MEMORY_DEFINE makroa.
 * @iclass
 *
 *              U debug rezimu ova funkcija uvek vraca pokazivac, odnosno, ne
 *              moze se desiti da vrati NULL pokazivac, kao sto nalaze
 *              standardna implementacija @c malloc C funkcije. Ukoliko se
 *              zahtevana memorija ne moze dobaviti generisace se ASSERT greska.
 *              Kada se ne koristi debug rezim funkcija se ponasa u skladu sa
 *              standardom.
 */
/*-----------------------------------------------------------------------------------------------*/
void * esHmemAllocI(
    size_t      aSize);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Reciklira memorijski prostor na koji pokazije @c aMemory
 *              pokazivac
 * @param       aMemory                 Pokazivac na prethodno dodeljen
 * 										memorijski prostor.
 * @api
 */
/*-----------------------------------------------------------------------------------------------*/
void esHmemDeAlloc(
    void        * aMemory);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Reciklira memorijski prostor na koji pokazije @c aMemory
 *              pokazivac
 * @param       aMemory                 Pokazivac na prethodno dodeljen
 *                                      memorijski prostor.
 * @iclass
 */
/*-----------------------------------------------------------------------------------------------*/
void esHmemDeAllocI(
    void        * aMemory);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Vraca velicinu trenutno slobodne memorije u bajtovima.
 * @return      Velicina slobodne memorije u bajtovima.
 * @pre         Najpre treba inicijalizovati MM modul pozivom
 *              @ref ES_MEMORY_DEFINE makroa.
 * @api
 *
 *              Ukoliko je memorija jako fragmenitisana, sto je karakteristicno
 *              za first fir algoritam, moze se desiti da postoji dovoljno
 *              slobodne memorije, ali ne i bloka zahtevane velicine. U tom
 *              slucaju ce memorijski alokator pasti.
 */
/*-----------------------------------------------------------------------------------------------*/
size_t esHmemFreeSpace(
    void);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Vraca velicinu trenutno slobodne memorije u bajtovima.
 * @return      Velicina slobodne memorije u bajtovima.
 * @pre         Najpre treba inicijalizovati MM modul pozivom
 *              @ref ES_MEMORY_DEFINE makroa.
 * @iclass
 *
 *              Ukoliko je memorija jako fragmenitisana, sto je karakteristicno
 *              za first fir algoritam, moze se desiti da postoji dovoljno
 *              slobodne memorije, ali ne i bloka zahtevane velicine. U tom
 *              slucaju ce memorijski alokator pasti.
 */
/*-----------------------------------------------------------------------------------------------*/
size_t esHmemFreeSpaceI(
    void);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief		Vraca velicinu dodeljene memorije
 * @param 		aMemory					Pokazivac na prethodno dodeljen
 * 										memorijski prostor.
 * @return		Velicina dodeljenog memorijskog prostora.
 * @api
 *
 *              Sa obzirom da alokatori koriste granulaciju i/ili imaju
 *              implementaciono ogranicenje, prilikom dodeljivanja memorije moze
 *              se dodeliti veca memorija od zahtevane. Ova funkcija se moze
 *              koristiti ukoliko je potrebna informacija o stvarnoj velicini
 *              dodeljenog bloka.
 */
/*-----------------------------------------------------------------------------------------------*/
size_t esHmemBlockSize(
	void 		* aMemory);

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Ostale operacije sa memorijom
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Vrsi kopiranje @c aLength bajtova sa izvorne adrese @c aSrc na
 *              odredisnu adresu @c aDst.
 * @param       aDst                    Pokazivac na odredisnu adresu,
 * @param       aSrc                    pokazivac na izvorisnu adresu,
 * @param       aLength                 broj bajtova koje treba kopirati.
 * @warning     Zbog primena optimizacionih tehnika ova funkcija moze da radi
 *              samo sa blokovima koji se ne preklapaju, odnosno, adrese blokova
 *              izvorista i odredista ne smeju da se preklapaju jer je u tom
 *              slucaju ponasanje funkcije nedefinisano. Ukoliko je potrebno
 *              kopirati blokove cije se adrese preklapaju treba koristiti
 *              funkciju esMemMove().
 * @api
 */
/*-----------------------------------------------------------------------------------------------*/
void * esMemCopy(
    void        * C_RESTRICT aDst,
    const void  * C_RESTRICT aSrc,
    size_t      aLength);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Vrsi kopiranje @c aLength bajtova sa izvorne adrese @c aSrc na
 *              odredisnu adresu @c aDst.
 * @param       aDst                    Pokazivac na odredisnu adresu,
 * @param       aSrc                    pokazivac na izvorisnu adresu,
 * @param       aLength                 broj bajtova koje treba kopirati.
 * @note        Za razliku od esMemMove() ova funkcija moze se koristiti u svim
 *              slucajevima, jer se ne primenjuju optimizacione tehnike.
 * @api
 */
/*-----------------------------------------------------------------------------------------------*/
void * esMemMove(
    void        * aDst,
    const void  * aSrc,
    size_t      aLength);

/** @} *//*--------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*
 * C/C++ #endif - close
 *-----------------------------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif


/*************************************************************************************************
 * CONFIGURATION ERRORS
 *************************************************************************************//** @cond */


/** @endcond *//** @} *//*************************************************************************
 * END of mem.h
 *************************************************************************************************/
#endif /* MM_H_ */
