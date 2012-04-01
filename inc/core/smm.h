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
 * @brief       Interfejs Simple Memory Management (SMM) modula.
 *
 * ------------------------------------------------------------------------------------------------
 *
 * @addtogroup  smm_intf
 *
 ****************************************************************************************//** @{ */


#ifndef SMM_H_
#define SMM_H_


/*************************************************************************************************
 * INCLUDE FILES
 *************************************************************************************************/

/*-----------------------------------------------------------------------------------------------*
 * Zavisnosti modula
 *-----------------------------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------------------------*
 * Konfiguracija i adaptacija modula
 *-----------------------------------------------------------------------------------------------*/

#include "core/cfg/smm_cfg.h"


/*-----------------------------------------------------------------------------------------------*
 * EXTERNS
 *-----------------------------------------------------------------------------------*//** @cond */

#ifdef SMM_H_VAR
# define SMM_H_EXT
#else
# define SMM_H_EXT extern
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
 * @attention   Makro se mora pozvati pre upotrebe funkcija: eS_hmemAlloc(),
 *              eS_hmemAllocI(), eS_hmemDeAlloc(), eS_hmemDeAllocI(),
 *              eS_hmemFreeSpaceI() i eS_hmemBlockSize().
 */
/*-----------------------------------------------------------------------------------------------*/
#define SMM_HEAP_MEM_DEFINE(size)													\
	do {																		\
		static uint8_t smm_heapMemory[ES_ALIGN(size, C_DATA_ALIGNMENT)];		\
		eS_hmemCtor((void *)smm_heapMemory, sizeof(smm_heapMemory));				\
	} while (0)


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

typedef struct smm_memClass smm_memClass_T;

/*-------------------------------------------------------------------------------------------*//**
 * @name        Klase memorijskog alokatora
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Dinamicki memorijski alokator (heap memory)
 */
/*-----------------------------------------------------------------------------------------------*/
extern const C_ROM smm_memClass_T memHeapClass;

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Skladiste memorije (memory pool)
 */
/*-----------------------------------------------------------------------------------------------*/
extern const C_ROM smm_memClass_T memPoolClass;

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Staticki memorijski alokator (static memory)
 */
/*-----------------------------------------------------------------------------------------------*/
extern const C_ROM smm_memClass_T memStaticClass;

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
 *
 *              Pre pozivanja ove funkcije potrebno je izvrsiti rezervaciju
 *              memorijskog prostora. To se vrsi upotrebom SMM_HEAP_MEM_DEFINE
 *              makroa.
 *
 * @param       aHeap                   Pokazivac na unapred rezervisan
 *                                      memorijski prostor,
 * @param       aSize                   velicina memorijskog prostora u
 *                                      bajtovima.
 * @note        Nakon inicijalizacije memorijskog prostora, slobodna memorija
 *              nece biti jednaka aHeap velicini vec ce biti umanjena za iznos
 *              koji priblizno odgovara statickom zauzecu alokatora koji se
 *              koristi. Na primer, first fit alokator ce umanjiti vrednost
 *              slobodne memorije za oko 24B (32-bitna arhitektura).
 * @notapi
 */
/*-----------------------------------------------------------------------------------------------*/
void eS_cmemCtor(
    void        * aHeap,
    size_t      aSize);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Dodeljuje memorijski prostor velicine @c aSize
 *
 *              U debug rezimu ova funkcija uvek vraca uspesno, odnosno, ne moze
 *              se desiti da vrati NULL pokazivac, kao sto nalaze standardna
 *              implementacija @c malloc C funkcije. Ukoliko se zahtevana
 *              memorija ne moze dobaviti generisace se ASSERT greska.
 *              Kada se ne koristi debug rezim funkcija se ponasa u skladu sa
 *              standardom.
 *
 * @param       aSize                   Velicina zahtevanog memorijskog prostora
 *                                      u bajtovima.
 * @pre         Prvo treba inicijalizovati SMM modul pozivom SMM_HEAP_MEM_DEFINE
 *              makroa.
 * @api
 */
/*-----------------------------------------------------------------------------------------------*/
void * eS_cmemAlloc(
    size_t      aSize);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Dodeljuje memorijski prostor velicine @c aSize
 *
 *              U debug rezimu ova funkcija uvek vraca uspesno, odnosno, ne moze
 *              se desiti da vrati NULL pokazivac, kao sto nalaze standardna
 *              implementacija @c malloc C funkcije. Ukoliko se zahtevana
 *              memorija ne moze dobaviti generisace se ASSERT greska.
 *              Kada se ne koristi debug rezim funkcija se ponasa u skladu sa
 *              standardom.
 *
 * @param       aSize                   Velicina zahtevanog memorijskog prostora
 *                                      u bajtovima.
 * @pre         Prvo treba inicijalizovati SMM modul pozivom SMM_HEAP_MEM_DEFINE
 *              makroa.
 * @iclass
 */
/*-----------------------------------------------------------------------------------------------*/
void * eS_cmemAllocI(
    size_t      aSize);

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Funkcije dinamickog memorijskog alokatora
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Inicijalizuje memorijski alokator sa memorijom @c aHeap
 *              velicine @c aSize
 *
 *              Pre pozivanja ove funkcije potrebno je izvrsiti rezervaciju
 *              memorijskog prostora. To se moze izvrsiti na nekoliko nacina,
 *              ali se preporucuje metoda upotrebom SMM_HEAP_MEM_DEFINE makroa.
 *
 *              Primer rezervacije i inicijalizacije memorije velicine 1kB:
 *
 *              @code
 *              uint8_t smm_memory[1024];
 *
 *              eS_hmemCtor((void *)&smm_memory, sizeof(smm_memory[1024]));
 *              @endcode
 *
 *              ili koristeci dinamicku memoriju:
 *
 *              @code
 *              uint8_t * pMemory;
 *
 *              pMemory = (uint8_t *)malloc(1024);
 *
 *              eS_hmemCtor((void *)pMemory, 1024);
 *              @endcode
 *
 * @param       aHeap                   Pokazivac na unapred rezervisan
 *                                      memorijski prostor,
 * @param       aSize                   velicina memorijskog prostora u
 *                                      bajtovima.
 * @note		Nakon inicijalizacije memorijskog prostora, slobodna memorija
 * 				nece biti jednaka aHeap velicini vec ce biti umanjena za iznos
 * 				koji priblizno odgovara statickom zauzecu alokatora koji se
 * 				koristi. Na primer, first fit alokator ce umanjiti vrednost
 * 				slobodne memorije za 24B.
 * @api
 */
/*-----------------------------------------------------------------------------------------------*/
void eS_hmemCtor(
    void        * aHeap,
    size_t      aSize);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Dodeljuje memorijski prostor velicine @c aSize
 *
 *              U debug rezimu ova funkcija uvek vraca uspesno, odnosno, ne moze
 *              se desiti da vrati NULL pokazivac, kao sto nalaze standardna
 *              implementacija @c malloc C funkcije. Ukoliko se zahtevana
 *              memorija ne moze dobaviti generisace se ASSERT greska.
 *              Kada se ne koristi debug rezim funkcija se ponasa u skladu sa
 *              standardom.
 *
 * @param       aSize                   Velicina zahtevanog memorijskog prostora
 *                                      u bajtovima.
 * @pre         Prvo treba inicijalizovati SMM modul pozivom SMM_HEAP_MEM_DEFINE
 *              makroa.
 * @api
 */
/*-----------------------------------------------------------------------------------------------*/
void * eS_hmemAlloc(
    size_t      aSize);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Dodeljuje memorijski prostor velicine @c aSize
 *
 *              U debug rezimu ova funkcija uvek vraca uspesno, odnosno, ne moze
 *              se desiti da vrati NULL pokazivac, kao sto nalaze standardna
 *              implementacija @c malloc C funkcije. Ukoliko se zahtevana
 *              memorija ne moze dobaviti generisace se ASSERT greska.
 *              Kada se ne koristi debug rezim funkcija se ponasa u skladu sa
 *              standardom.
 *
 * @param       aSize                   Velicina zahtevanog memorijskog prostora
 *                                      u bajtovima.
 * @pre         Prvo treba inicijalizovati SMM modul pozivom SMM_HEAP_MEM_DEFINE
 *              makroa.
 * @iclass
 */
/*-----------------------------------------------------------------------------------------------*/
void * eS_hmemAllocI(
    size_t      aSize);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Reciklira memorijski prostor na koji pokazije @c aMemory
 *              pokazivac
 *
 * @param       aMemory                 Pokazivac na prethodno dodeljen
 * 										memorijski prostor.
 * @api
 */
/*-----------------------------------------------------------------------------------------------*/
void eS_hmemDeAlloc(
    void        * aMemory);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Reciklira memorijski prostor na koji pokazije @c aMemory
 *              pokazivac
 *
 * @param       aMemory                 Pokazivac na prethodno dodeljen
 *                                      memorijski prostor.
 * @iclass
 */
/*-----------------------------------------------------------------------------------------------*/
void eS_hmemDeAllocI(
    void        * aMemory);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Vraca velicinu trenutno slobodne memorije u bajtovima.
 *
 *              Ukoliko je memorija jako fragmenitisana, sto je karakteristicno
 *              za first fir algoritam, moze se desiti da postoji dovoljno
 *              slobodne memorije, ali ne i bloka zahtevane velicine. U tom
 *              slucaju ce memorijski alokator pasti.
 *
 * @return      Velicina slobodne memorije u bajtovima.
 * @pre         Prvo treba inicijalizovati SMM modul pozivom SMM_HEAP_MEM_DEFINE
 *              makroa.
 * @api
 */
/*-----------------------------------------------------------------------------------------------*/
size_t eS_hmemFreeSpace(
    void);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Vraca velicinu trenutno slobodne memorije u bajtovima.
 *
 *				Ukoliko je memorija jako fragmenitisana, sto je karakteristicno
 *				za first fir algoritam, moze se desiti da postoji dovoljno
 *				slobodne memorije, ali ne i bloka zahtevane velicine. U tom
 *				slucaju ce memorijski alokator pasti.
 *
 * @return      Velicina slobodne memorije u bajtovima.
 * @pre         Prvo treba inicijalizovati SMM modul pozivom SMM_HEAP_MEM_DEFINE
 *              makroa.
 * @iclass
 */
/*-----------------------------------------------------------------------------------------------*/
size_t eS_hmemFreeSpaceI(
    void);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief		Vraca velicinu dodeljene memorije
 *
 * 				Sa obzirom da alokatori koriste granulaciju i/ili imaju
 * 				implementaciono ogranicenje, prilikom dodeljivanja memorije moze
 * 				se dodeliti veca memorija od zahtevane. Ova funkcija se moze
 * 				koristiti ukoliko je potrebna informacija o stvarnoj velicini
 * 				dodeljenog bloka.
 *
 * @param 		aMemory					Pokazivac na prethodno dodeljen
 * 										memorijski prostor.
 *
 * @return		Velicina dodeljenog memorijskog prostora.
 * @api
 */
/*-----------------------------------------------------------------------------------------------*/
size_t eS_hmemBlockSize(
	void 		* aMemory);

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Ostale operacije sa memorijom
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Vrsi kopiranje @c aLength bajtova sa izvorne adrese @c aSrc na
 *              odredisnu adresu @c aDst
 *
 * @param       aDst                    Pokazivac na odredisnu adresu,
 * @param       aSrc                    pokazivac na izvorisnu adresu,
 * @param       aLength                 broj bajtova koje treba kopirati.
 * @warning     Zbog primena optimizacionih tehnika ova funkcija moze da radi
 *              samo sa blokovima koji se ne preklapaju, odnosno, adrese blokova
 *              izvorista i odredista ne smeju da se preklapaju jer je u tom
 *              slucaju ponasanje funkcije nedefinisano. Ukoliko je potrebno
 *              kopirati blokove cije se adrese preklapaju treba koristiti
 *              funkciju eS_memMove().
 * @api
 */
/*-----------------------------------------------------------------------------------------------*/
void * eS_memCopy(
    void        * C_RESTRICT aDst,
    const void  * C_RESTRICT aSrc,
    size_t      aLength);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Vrsi kopiranje @c aLength bajtova sa izvorne adrese @c aSrc na
 *              odredisnu adresu @c aDst
 *
 * @param       aDst                    Pokazivac na odredisnu adresu,
 * @param       aSrc                    pokazivac na izvorisnu adresu,
 * @param       aLength                 broj bajtova koje treba kopirati.
 * @note        Za razliku od eS_memMove() ova funkcija moze se koristiti u svim
 *              slucajevima, jer se ne primenjuju optimizacione tehnike.
 * @api
 */
/*-----------------------------------------------------------------------------------------------*/
void * eS_memMove(
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
 * END of smm.h
 *************************************************************************************************/
#endif /* SMM_H_ */
