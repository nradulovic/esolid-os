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
 * @brief       Interfejs Debug modula.
 *
 * ------------------------------------------------------------------------------------------------
 *
 * @addtogroup  dbg_intf
 *
 ****************************************************************************************//** @{ */


#ifndef DBG_H_
#define DBG_H_


/*************************************************************************************************
 * INCLUDE FILES
 *************************************************************************************************/

/*-----------------------------------------------------------------------------------------------*
 * Module dependencies
 *-----------------------------------------------------------------------------------------------*/

#include "hal/hal.h"


/*-----------------------------------------------------------------------------------------------*
 * Module configuration and adapter
 *-----------------------------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------------------------*
 * EXTERNS
 *-----------------------------------------------------------------------------------*//** @cond */


#ifdef DBG_H_VAR
#define DBG_H_EXT
#else
#define DBG_H_EXT extern
#endif


/** @endcond*//***********************************************************************************
 * DEFINES
 *************************************************************************************************/


/*************************************************************************************************
 * MACRO's
 *************************************************************************************************/

/*-------------------------------------------------------------------------------------------*//**
 * @name        Makroi bez dejstva
 *
 * @brief       Ovi makroi se koriste kao zamena za neaktivne debug makroe.
 *
 * 				Njihova uloga je da sprece razna upozorenja za vreme
 * 				kompajliranja.
 * @{ *//*---------------------------------------------------------------------------------------*/

#define DBG_EMPTY_DECL()														\
	extern C_UNUSED_VAR(uint8_t, ES_CONCAT(dbgVar, __LINE__))

#define DBG_EMPTY_MACRO()														\
	(void)0

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Makroi za proveru argumenata
 *
 * @brief       Koriste za ispitivanje da li su argumenti predati funkciji
 * 				ispravni
 *
 * @note		Ukljucivanje/iskljucivanje se vrsi opcijom OPT_DBG_USE_CHECK.
 * @{ *//*---------------------------------------------------------------------------------------*/

/**
 * @brief       Vrsi proveru argumenata
 *
 * @mseffect
 */
#if (defined(OPT_DBG_ENABLE) && defined(OPT_DBG_USE_CHECK)) || defined(__DOXYGEN__)
# define DBG_CHECK(expr)														\
	((expr) ? (void)0 : assertFailed((char *)DBG_THIS_MODULE, (char *)DBG_THIS_FILE, (char *)ES_STRING(expr), (uint16_t)__LINE__))
#else
# define DBG_CHECK(expr)				DBG_EMPTY_MACRO()
#endif

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Assert makroi
 *
 * @brief       Koriste za ispitivanje da li su uslovi dati u argumentu
 *              ispunjeni.
 *
 * @note		Ukljucivanje/iskljucivanje se vrsi opcijom OPT_DBG_USE_ASSERT.
 * @{ *//*---------------------------------------------------------------------------------------*/

#if (defined(OPT_DBG_ENABLE) && defined(OPT_DBG_USE_ASSERT)) || defined(__DOXYGEN__)
/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Izvrsava akcije ASSERT makroa.
 *
 *				Ukoliko uslov @c expr nije ispravan poziva se @c extern funkcija
 *				@c assertFailed koja stampa mesto nastanka greske.
 *
 * @param       expr                    Izraz/uslov koji treba da prodje
 *                                      ispitivanje.
 * @mseffect
 */
/*-----------------------------------------------------------------------------------------------*/
# define DBG_ASSERT(expr)                                                       \
    ((expr) ? (void)0 : assertFailed((char *)DBG_THIS_MODULE, (char *)DBG_THIS_FILE, (char *)ES_STRING(expr), (uint16_t)__LINE__))

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Assert makro koji bezuslovno prikazuje gresku.
 *
 * @param       msg                     Poruka o gresci koja se ispisuje
 *                                      korisniku.
 */
/*-----------------------------------------------------------------------------------------------*/
# define DBG_ASSERT_ALWAYS(msg)                                                 \
    assertFailed((char *)DBG_THIS_MODULE, (char *)DBG_THIS_FILE, (char *)ES_STRING(msg), (uint16_t)__LINE__)

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Makro koji se izvrsava za vreme kompajliranja.
 *
 *              Ovaj makro se najcesce postavlja u delovima koda koji su poznati
 *              za vreme kompajliranja.
 *
 * @param       expr                    Izraz/uslov koji treba da prodje
 *                                      ispitivanje.
 */
/*-----------------------------------------------------------------------------------------------*/
# define DBG_ASSERT_COMPILE(expr)                                                \
    extern char ES_CONCAT(compileAssert, __LINE__) [(expr) ? 1 : -1]

#else
# define DBG_ASSERT(expr)				DBG_EMPTY_MACRO()
# define DBG_ASSERT_ALWAYS(msg)			DBG_EMPTY_MACRO()
# define DBG_ASSERT_COMPILE(expr)		DBG_EMPTY_DECL()
#endif

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Entry/exit makroi
 *
 * @brief       Makroi koji pokazuju kada se ulazi/izlazi iz neke funkcije.
 * @{ *//*---------------------------------------------------------------------------------------*/

# if defined(OPT_DBG_USE_ENTRY_EXIT) || defined(__DOXYGEN__)
/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Prikazi informaciju da se ulazi u funkciju.
 */
/*-----------------------------------------------------------------------------------------------*/
# define DBG_ENTRY()                                                            \
    onEntry((const char *)__func__)

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Prikazi informaciju da se izlazi iz funkcije.
 */
/*-----------------------------------------------------------------------------------------------*/
# define DBG_EXIT()                                                             \
    onExit((const char *)__func__, (uint16_t)__LINE__)

#else
# define DBG_ENTRY()					DBG_EMPTY_MACRO()
# define DBG_EXIT()						DBG_EMPTY_MACRO()
#endif

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Opsti debug makroi
 *
 * @brief       Opsti makroi se koriste za definisanje modula koji se kompajlira
 *              i uslovno izvrsavanje koda.
 * @{ *//*---------------------------------------------------------------------------------------*/

#if defined(OPT_DBG_ENABLE) || defined(__DOXYGEN__)
/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Definisanje imena modula.
 *
 * @param       moduleName              Ime modula koji se definise.
 */
/*-----------------------------------------------------------------------------------------------*/
# define DBG_DEFINE_MODULE(moduleName)                                          \
    static const C_ROM char * const C_ROM_VAR DBG_THIS_FILE = __FILE__;         \
    static const C_ROM char * const C_ROM_VAR DBG_THIS_MODULE = ES_STRING(moduleName)

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Izvrsavanje @c expr koda samo kada je definisan debug rezim.
 */
/*-----------------------------------------------------------------------------------------------*/
# define DBG_MACRO(expr)                                                        \
    expr

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Deklaracija promenljive samo kada je definisan debug rezim.
 */
/*-----------------------------------------------------------------------------------------------*/
# define DBG_DECL(expr)                                                         \
    expr

#else
# define DBG_DEFINE_MODULE(expr)		DBG_EMPTY_DECL()
# define DBG_MACRO(expr)				DBG_EMPTY_MACRO()
# define DBG_DECL(expr)					DBG_EMPTY_DECL()
#endif

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Debug Print makroi
 *
 * @brief       Koriste za stampanje Debug vrednosti brojeva u nekom od
 *              sledecih formata DEC, HEX, BIN.
 * @{ *//*---------------------------------------------------------------------------------------*/

#if defined(OPT_DBG_USE_PRINT) || defined(__DOXYGEN__)
/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Konvertovanje i stampanje neoznacenih binarnih brojeva velicine
 *              8 do 32 bita u decimalnom formatu.
 *
 * @todo		Izvrsiti refactor argumenta bin u udec
 */
/*-----------------------------------------------------------------------------------------------*/
# define DBG_PRINT_UDEC(bin)                                                    \
    eS_printUdec(bin)

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Konvertovanje i stampanje oznacenih binarnih brojeva velicine
 *              8 do 32 bita u decimalnom formatu.
 *
 * @todo		Izvrsiti refactor argumenta bin u sdec
 */
/*-----------------------------------------------------------------------------------------------*/
# define DBG_PRINT_SDEC(bin)                                                    \
    eS_printSdec(bin)

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Konvertovanje i stampanje neoznacenih binarnih brojeva velicine
 *              8 do 32 bita u heksadecimalnom formatu.
 *
 * @todo		Izvrsiti refactor argumenta bin u hex
 */
/*-----------------------------------------------------------------------------------------------*/
# define DBG_PRINT_HEX(bin)                                                    	\
    eS_printHex(bin)

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Konvertovanje i stampanje neoznacenih binarnih brojeva velicine
 *              8 do 32 bita u binarnom formatu.
 */
/*-----------------------------------------------------------------------------------------------*/
# define DBG_PRINT_BIN(bin)                                                    	\
    eS_printBin(bin)

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Stampa znakovni niz
 */
/*-----------------------------------------------------------------------------------------------*/
# define DBG_PRINT_STRING(string)                                               \
    eS_printString((const char *)string)

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Konfiguracija periferije mikrokontrolera za komunikaciju.
 */
/*-----------------------------------------------------------------------------------------------*/
# define DBG_PRINT_CONFIG()                                                     \
    eS_printConfig()
#else
# define DBG_PRINT_UDEC(bin)            DBG_EMPTY_MACRO()
# define DBG_PRINT_SDEC(bin)			DBG_EMPTY_MACRO()
# define DBG_PRINT_HEX(bin)				DBG_EMPTY_MACRO()
# define DBG_PRINT_BIN(bin)				DBG_EMPTY_MACRO()
# define DBG_PRINT_CONFIG()				DBG_EMPTY_MACRO()
# define DBG_PRINT_STRING(string)		DBG_EMPTY_MACRO()
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


/*************************************************************************************************
 * GLOBAL VARIABLES
 *************************************************************************************************/


/*************************************************************************************************
 * FUNCTION PROTOTYPES
 *************************************************************************************************/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Proverava da li @c aBool ima vrednost TRUE ili FALSE.
 *
 * @param       aBool                   Parametar koji treba proveriti.
 * @return      Da li je @c aBool boolean tipa.
 *  @retval     TRUE - aBool je TRUE ili FALSE,
 *  @retval     FALSE - aBool nije TRUE i FALSE.
 * @api
 */
/*-----------------------------------------------------------------------------------------------*/
bool_T eS_dbgValidBool(
    bool_T 		aBool);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Proverava da li je @c aValue u opsegu @c aMin i @c aMax,
 *              neoznaceni broj.
 *
 * @param       aValue                  Parametar koji treba proveriti.
 * @param       aMin                    Minimalna vrednost koju parametar sme
 *                                      imati,
 * @param       aMax                    Maksimalna vrednost koju parametar sme
 *                                      imati.
 * @return      Da li je @c aValue u validnom opsegu.
 *  @retval     TRUE ako vazi uslov: aMin < aValue < aMax
 *  @retval     FALSE suprotan slucaj.
 * @api
 */
/*-----------------------------------------------------------------------------------------------*/
bool_T eS_dbgValidUint32(
    uint32_t 	aValue,
    uint32_t 	aMin,
    uint32_t 	aMax);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Proverava da li je @c aValue u opsegu @c aMin i @c aMax,
 *              oznaceni broj.
 *
 * @param       aValue                  Parametar koji treba proveriti.
 * @param       aMin                    Minimalna vrednost koju parametar sme
 *                                      imati,
 * @param       aMax                    Maksimalna vrednost koju parametar sme
 *                                      imati.
 * @return      Da li je @c aValue u validnom opsegu.
 *  @retval     TRUE ako vazi uslov: aMin < aValue < aMax
 *  @retval     FALSE suprotan slucaj.
 * @api
 */
/*-----------------------------------------------------------------------------------------------*/
bool_T eS_dbgValidSint32(
    int32_t 	aValue,
    int32_t 	aMin,
    int32_t 	aMax);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Proverava da li je niz @c aString date velicine @c aSize.
 *
 *              Funkcija ocekuje da je niz terminisan @c 0x00 karakterom. U
 *              sustini ona broji @c ne-null karaktere i poredi sa aSize.
 *              Ukoliko je @c null karakter pronadjen na lokaciji manjoj od @c
 *              aSize smatra se da je niz ispravan.
 *
 * @param       aString                 Pokazivac na niz koji treba ispitati,
 * @param       aSize                   maksimalna dozvoljena velicina niza.
 * @return      Da li je niz @c aString dozvoljene velicine.
 *  @retval     TRUE ako je velicina niza manja od @c aSize,
 *  @retval     FALSE u suprotnom slucaju.
 * @api
 */
/*-----------------------------------------------------------------------------------------------*/
bool_T eS_dbgValidString(
    const uint8_t   * aString,
    size_t          aSize);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Konvertuje neoznaceni broj iz binarnog u decadni format.
 *
 *              Ovu funkciju poziva makro DBG_PRINT_UDEC.
 *
 * @param       u32num                  neoznaceni binarni broj.
 * @notapi
 */
/*-----------------------------------------------------------------------------------------------*/
void eS_printUdec(
    uint32_t u32num);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Konvertuje oznaceni broj iz binarnog u decadni format.
 *
 *              Ovu funkciju poziva makro DBG_PRINT_SDEC.
 *
 * @param       i32num                  oznaceni binarni broj.
 * @notapi
 */
/*-----------------------------------------------------------------------------------------------*/
void eS_printSdec(
     int32_t i32num);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Konvertuje neoznaceni broj iz binarnog u heksadecimalni format.
 *
 *              Ovu funkciju poziva makro DBG_PRINT_HEX.
 *
 * @param       hex                     neoznaceni binarni broj.
 * @notapi
 */
/*-----------------------------------------------------------------------------------------------*/
void eS_printHex(
     uint32_t hex);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Konvertuje neoznaceni broj iz binarnog u binarno-decimalni format.
 *
 *              Ovu funkciju poziva makro DBG_PRINT_BIN.
 *
 * @param       hex                     neoznaceni binarni broj. (FIXME: bin parametar ili hex?)
 * @notapi
 */
/*-----------------------------------------------------------------------------------------------*/
void eS_printBin(
     uint32_t hex);


/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Stampa znakovni niz.
 *
 *              Ovu funkciju poziva makro DBG_PRINT_STRING.
 *
 * @param       aString                 Znakovni niz koji treba da se odstampa.
 */
/*-----------------------------------------------------------------------------------------------*/
void eS_printString(
     const char * aString);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Konfiguracija periferije mikrokontrolera za komunikaciju
 *              Ovu funkciju poziva makro DBG_PRINT_CONFIG.
 * @notapi
 */
/*-----------------------------------------------------------------------------------------------*/
void eS_printConfig(
     void);

/*-------------------------------------------------------------------------------------------*//**
 * @name        Eksterne funkcije
 *
 *              Sledece funkcije je korisnik duzan da napise prema potrebi
 *              aplikacije. U ovoj sekciji je samo predstavljen interfejs koji
 *              treba da se ispostuje.
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Prikazuje ulazak u funkciju
 *
 * @param       aFunction               Ime funkcije u koju se upravo uslo.
 */
/*-----------------------------------------------------------------------------------------------*/
extern void onEntry(
    const C_ROM char * aFunction);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Prikazuje izlazak iz funkcije
 *
 * @param       aFunction               Ime funkcije iz koje se izlazi,
 * @param       aLine                   linija koda na kojoj se izlazi.
 */
/*-----------------------------------------------------------------------------------------------*/
extern void onExit(
    const C_ROM char * aFunction,
    uint16_t aLine);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Prikazuje mesto nastanka greske
 *
 *              Ovu funkciju pozivaju assert makroi kada se detektuje greska.
 *
 * @param       aModule                 Ime modula gde je detektovana greska,
 * @param       aFile                   ime datoteke izvornog koda,
 * @param       aAssert                 uslov nastanka greske,
 * @param       aLine                   linija koda na kojoj je detektovana
 *                                      greska.
 */
/*-----------------------------------------------------------------------------------------------*/
C_NORETURN extern void assertFailed(
    const C_ROM char * aModule,
    const C_ROM char * aFile,
    const C_ROM char * aAssert,
    uint16_t aLine);

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
 * END of dbg.h
 *************************************************************************************************/
#endif /* DBG_H_ */
