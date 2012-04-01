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
 * @brief       Interfejs za kompajler.
 *
 * ------------------------------------------------------------------------------------------------
 *
 * @addtogroup  hal_compiler
 *
 ****************************************************************************************//** @{ */

#ifndef HAL_COMPILER_H_
#define HAL_COMPILER_H_


/*************************************************************************************************
 * INCLUDE FILES
 *************************************************************************************************/

/*-----------------------------------------------------------------------------------------------*
 * Module dependencies
 *-----------------------------------------------------------------------------------------------*/

#include "port/port.h"


/*-----------------------------------------------------------------------------------------------*
 * Module configuration and adapter
 *-----------------------------------------------------------------------------------------------*/

#include PORT_ARCH_HEADER(hal/compiler)
#include PORT_CMN_HEADER(hal/compiler)


/*-----------------------------------------------------------------------------------------------*
 * EXTERNS
 *-----------------------------------------------------------------------------------*//** @cond */

#ifdef HAL_COMPILER_H_VAR
#define HAL_COMPILER_H_EXT
#else
#define HAL_COMPILER_H_EXT extern
#endif


/** @endcond*//***********************************************************************************
 * DEFINES
 *************************************************************************************************/

/*-------------------------------------------------------------------------------------------*//**
 * @name        Svojstva compiler porta
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Poravnjanje podataka
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(C_DATA_ALIGNMENT) || defined(__DOXYGEN__)
# define C_DATA_ALIGNMENT               1
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Pokazuje da li se C++ rezervisane reci koriste u portu
 *
 *              Ukoliko je ova promenljiva definisana, onda port vec koristi
 *              C++ rezervisane reci.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(PORT_IS_POISONED) || defined(__DOXYGEN__)
# define PORT_IS_POISONED
#endif

/** @} *//*--------------------------------------------------------------------------------------*/


/*************************************************************************************************
 * MACRO's
 *************************************************************************************************/

/*-------------------------------------------------------------------------------------------*//**
 * @name        Makroi za prosirene opcije C programskog jezika
 *
 * @brief       Ovi makroi definisu interfejs za rad sa naprednim mogucnostima
 *              C kompajlera.
 *
 *              Arhitekturna compiler.h datoteka treba da pruzi odgovarajuce
 *              makroe koji ce zameniti ove podrazumevane makroe. Za detalje
 *              oko implementacije navedenih makroa treba konsultovati
 *              compiler.h arhitekturnu datoteku u direktorijumu
 *              common/arhitektura.
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Podrska za automatsko popunjavanje imena funkcija u makroima.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(C_FUNC) || defined(__DOXYGEN__)
# define C_FUNC                         "<unknown>"
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Podrska za C ekstenzije i uklanjanje naivnih upozorenja.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(C_EXT) || defined(__DOXYGEN__)
# define C_EXT
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Atribut za pakovanje clanova jedne strukture.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(C_PACKED) || defined(__DOXYGEN__)
# define C_PACKED
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Atribut za poravnjanje clanova jedne strukture.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(C_ALIGNED) || defined(__DOXYGEN__)
# define C_ALIGNED
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Sprecava kompajler optimizator da izvrsi reorganizaciju koda
 *              zbog optimizacije.
 *
 *              Koristi se u kriticnim sekcijama koda koji mora da se izvrsi bas
 *              po istom redosledu po kojem je kod i napisan. Pozivanjem ovog
 *              makroa vrsi se sprecavanje optimizatora da rasporedi instrukcije
 *              kako bi izvrsio optimizaciju.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(C_COMPILE_BARRIER) || defined(__DOXYGEN__)
# define C_COMPILE_BARRIER()
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Sprecava upozoravanje kompajlera da funkcija ne vraca parametar
 *              iako njena deklaracija govori suprotno.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(C_NORETURN) || defined(__DOXYGEN__)
# define C_NORETURN
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Sprecava upozoravanje kompajlera da se argument funkcije ne
 *              koristi.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(C_UNUSED_ARG) || defined(__DOXYGEN__)
# define C_UNUSED_ARG(type,arg)
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Sprecava upozoravanje kompajlera da se promenljiva ne koristi.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(C_UNUSED_VAR) || defined(__DOXYGEN__)
# define C_UNUSED_VAR(type,name)
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       FIXME objasnjenje?
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(C_USED_VAR) || defined(__DOXYGEN__)
# define C_USED_VAR(type,name)
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Direktiva za inline kompajliranje funkcije.
 *
 * @note        Ova direktiva se moze zanemariti od strane kompajlera.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(C_INLINE) || defined(__DOXYGEN__)
# define C_INLINE
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Direktiva koja govori da kompajler ne sme da izvrsi inline
 *              kompajliranje ove funkcuje.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(C_NOINLINE) || defined(__DOXYGEN__)
# define C_NOINLINE
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       FIXME objasnjenje?
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(C_PURE_FUNC) || defined(__DOXYGEN__)
# define C_PURE_FUNC
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       FIXME objasnjenje?
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(C_CONST_FUNC) || defined(__DOXYGEN__)
# define C_CONST_FUNC
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Sprecava upozorenje kompajlera da se neka funkcija ne koristi.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(C_UNUSED_FUNC) || defined(__DOXYGEN__)
# define C_UNUSED_FUNC
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       FIXME objasnjenje?
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(C_USED_FUNC) || defined(__DOXYGEN__)
# define C_USED_FUNC
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       ISO C99 ekstenzija, ne podrzavaju je svi kompajleri. FIXME
 *              dodatno objasnjenje?
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(C_RESTRICT) || defined(__DOXYGEN__)
# define C_RESTRICT
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       FIXME objasnjenje?
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(C_MUST_CHECK) || defined(__DOXYGEN__)
# define C_MUST_CHECK
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Direktiva koja govori kompajleru da kompajlira funkcuju bez
 *              ulaza i izlaza (prologue/epilogue)
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(C_NAKED) || defined(__DOXYGEN__)
# define C_NAKED
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Izvrsi kast pokazivaca na nov tip podatka.
 *
 *              Nove verzije GCC-a postuju standard C90 i C99 , preciznije,
 *              namece se opcija -fstrict-aliasing koja ce u neko odredjeno
 *              vreme postati podrazumevano ponasanje GCC-a. Aliasing se moze
 *              prevesti kao "preklapanje objekata" u memoriji i njegova pojava
 *              se izbegava jer sve load-store arhitekture procesora imaju lose
 *              performanse sa pokazivacima koji pokazuju na objekte koji se
 *              preklapaju.
 *              Programer je sada duzan da prilikom definisanja strukture, unije
 *              ili promenjive za koju zna da ce vise pokazivaca istovremeno
 *              (u okviru C bloka) ukazivati na nju koristi GCC atribut koji
 *              govori da se nad tim objektom ne primenjuju strict-aliasing
 *              algoritmi kompajliranja. Odsustvo ovog atributa moze dovesti do
 *              kompajliranja koda koji je neispravan.
 *
 *              Postoje tri slucaja:
 *              - objekat ne koristi atribut i kvalifikator. U ovom slucaju GCC
 *              moze da izbaci upozorenje da se ne postuje strict-aliasing
 *              pravilo. Kod koji generise, takodje, moze biti neispravan.
 *              - objekat koristi atribut <c>__attribute__ ((__may_alias__))</c>
 *              koji govori GCC-u da ne primenjuje strict-aliasing algoritam
 *              kompajliranja. Performanse ce biti na udaru jer postoji mnostvo
 *              load/store instrukcija, ali je generisani kod ispravan sa
 *              stanovista algoritma.
 *              - pokazivac na objekat koristi kvalifikator @c restrict. Bez
 *              ukljucene podrske za C99 ovaj kvalifikator pokazivaca se
 *              ukljucuje pomocu @c __restrict__. Ovaj kvalifikator se koristi
 *              @b SAMO kada je programer siguran da dva ili vise pokazivaca ne
 *              pokazuju na isti memorijski prostor za vreme izvrsavanja bloka u
 *              kome su deklarisani. Kvalifikator omogucava kompajleru da
 *              generise visoko performansni kod.
 *
 *              Ovaj makro se koristi da se izbegnu upozorenja kompajlera
 *              prilikom kastovanja pokazivaca.
 *
 * @param       srctype                 Tip na koji pokazuje pokazivac,
 * @param       dsttype                 tip na koji pokazivac treba da se
 *                                      kastuje,
 * @param       var                     ime pokazivaca.
 *
 *              Primer:
 *              @code
 *              typedef struct smm_blk {
 *                  uint8_t a;
 *                  uint8_t b;
 *              } __attribute__ ((__may_alias__)) smm_blk_T;
 *
 *              typedef struct smm_blkHdr {
 *                  smm_blk_T super;
 *                  uint8_t   c;
 *              } __attribute__ ((__may_alias__)) smm_blkHdr_T;
 *
 *              E_PTRCAST(smm_blk_T, smm_blkHdr_T, pFreeArea).c = data;
 *              @endcode
 *
 *              U primeru pokazivac pFreeArea se kastuje iz tipa
 *              <c>smm_blk_T *</c> u <c>smm_blkHdr_T *</c> tip, odnosno, stari
 *              zapis bi izgledao:
 *              @code
 *              ((smm_blkHdr_T *)pFreeArea)->c = data;
 *              @endcode
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(C_PTRCAST) || defined(__DOXYGEN__)
# define C_PTRCAST(srctype, dsttype, var)
#endif


/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Opcije C kompajlera za embedded sisteme
 *
 *              Klasa: common
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Direktiva za smestanje podataka u ROM tip memorije.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(C_ROM) || defined(__DOXYGEN__)
# define C_ROM
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Modifikator za pristup podataka koji su u ROM memoriji.
 *
 *              Mnogi kompajleri nude ne-standardne ekstenzije - direktive za
 *              definisanje i pristup podataka u ROM memoriji. Biblioteka libe
 *              koristi E_ROM_VAR pretprocesorsku direktivu za definisanje
 *              kompajlerske direktive za pristup objekata u ROM memoriji.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(C_ROM_VAR) || defined(__DOXYGEN__)
# define C_ROM_VAR
#endif

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Makroi opste namene - Extended Support
 *
 *              Ovde su dostupni makroi za:
 *              - rad sa stringovima i konkatenacija
 *              - jednostavne aritmeticke operacije
 *
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Koristi se u makrou ES_CONCAT().
 *
 * @note        Samo za internu upotrebu.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(ES_ICONCAT) || defined(__DOXYGEN__)
# define ES_ICONCAT(expr1, expr2)                                               \
    expr1 ## expr2
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Objedinjuje argumente u jedan niz.
 *
 *              Nakon poziva ES_CONCAT(var, 123) dobija se var123.
 *
 * @param       expr1                   Prvi argument koji se spaja
 * @param       expr2                   Drugi argumenr koji se spaja
 * @return      expression = expr1 + expr2
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(ES_CONCAT) || defined(__DOXYGEN__)
# define ES_CONCAT(expr1, expr2)                                                \
    ES_ICONCAT(expr1, expr2)

#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Koristi se u makrou ES_STRING().
 *
 * @note        Samo za internu upotrebu.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(ES_ISTRING) || defined(__DOXYGEN__)
# define ES_ISTRING(expr)                                                       \
    #expr
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Pretvaranje izraza u niz karaktera.
 *
 *              Nakon poziva ES_STRING(E_CONCAT(Ne, sa)) dobija se "Nesa".
 *
 * @param       expr                    Argument koji treba da se pretvori u niz
 *                                      karaktera.
 * @return      "expr"
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(ES_STRING) || defined(__DOXYGEN__)
# define ES_STRING(expr)                                                        \
    ES_ISTRING(expr)
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Izracunavanje dimenzije elementa jedno-dimenzionalnog niza
 *              @a expr.
 *
 * @param       string                  Niz
 * @mseffect
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(ES_DIMENSION) || defined(__DOXYGEN__)
# define ES_DIMENSION(string)                                                   \
    (sizeof(string) / sizeof(string[0]))
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Vraca apsolutnu vrednost @a expr izraza.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(ES_ABS) || defined(__DOXYGEN__)
# define ES_ABS(expr)
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Vrsi deljenje dva broja i zaokruzuje rezultat.
 *
 *              Izvrsavanjem koda:
 *              @code
 *              int i = 28;
 *              int y = 5;
 *
 *              printf("Deljenje %d / %d = %d", i, y, i/y);
 *              printf("Zaokruzeno: %d", ES_DIV_ROUND(i, y));
 *              @endcode
 *
 *              dobicemo:
 *
 *              @code
 *              Deljenje 28 / 5 = 5
 *              Zaokruzeno: 6
 *              @endcode
 *
 * @note        Pozeljno je da @a expr2 bude konstanta, inace, generisace se dva
 *              deljenja.
 * @mseffect
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(ES_DIV_ROUND) || defined(__DOXYGEN__)
# define ES_DIV_ROUND(expr1, expr2)                                             \
    (((expr1) + (expr2) / 2) / (expr2))
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Vrsi deljenje dva broja sa zaokruzivanjem broja navise.
 *
 *              Izvrsavanjem koda:
 *              @code
 *              int i = 27;
 *              int y = 5;
 *
 *              printf("Deljenje %d / %d = %d", i, y, i/y);
 *              printf("Zaokruzeno: %d", ES_DIV_ROUND(i, y));
 *              printf("Zaokruzeno UP: %d", ES_DIV_ROUNDUP(i, y));
 *              @endcode
 *
 *              dobicemo:
 *
 *              @code
 *              Deljenje 27 / 5 = 5
 *              Zaokruzeno: 5
 *              Zaokruzeno UP: 6
 *              @endcode
 *
 * @note        Pozeljno je da @a expr2 bude konstanta, inace, generisace se
 *              jedno oduzimanje i jedno deljenje.
 * @mseffect
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(ES_DIV_ROUNDUP) || defined(__DOXYGEN__)
# define ES_DIV_ROUNDUP(expr1, expr2)                                           \
    (((expr1) + (expr2) - 1) / (expr2))
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Vraca vecu vrednost od ponudjenih vrednosti @a expr1 i @a expr2
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(ES_MAX) || defined(__DOXYGEN__)
# define ES_MAX(expr1, expr2)                                                    \
    ((expr1 > expr2) ? expr1 : expr2))
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Vraca manju vrednost od ponudjenih vrednosti @a expr1 i @a expr2
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(ES_MIN) || defined(__DOXYGEN__)
# define ES_MIN(expr1, expr2)                                                   \
    ((expr1 < expr2) ? expr1 : expr2))
#endif

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Makroi za bit manipulaciju - Extended Support
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Konvertovanje vrednost bita u marker bita (8-bita).
 *
 *              Ovaj makro se koristi za generisanje markera trazenog bita. Moze
 *              se koristiti u uslovima koji ispituju vrednost samo jednog bita.
 *              BV8 vrsi kast izlazne vrednosti na uint8_t tip.
 *
 * @param		bit						Bit pozicija koja se trazi.
 * @return		vrednost bit na @c bit poziciji.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(ES_BV8) || defined(__DOXYGEN__)
# define ES_BV8(bit)                                                            \
    (uint8_t)((uint8_t)1 << (bit))
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Konvertovanje vrednost bita u marker bita (16-bita).
 *
 *              Ovaj makro se koristi za generisanje markera trazenog bita. Moze
 *              se koristiti u uslovima koji ispituju vrednost samo jednog bita.
 *              BV16 vrsi kast izlazne vrednosti na uint16_t tip.
 *
 * @param		bit						Bit pozicija koja se trazi.
 * @return		vrednost bit na @c bit poziciji.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(ES_BV16) || defined(__DOXYGEN__)
# define ES_BV16(bit)                                                           \
    (uint16_t)((uint16_t)1 << (bit))
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Konvertovanje vrednost bita u marker bita (32-bita).
 *
 *              Ovaj makro se koristi za generisanje markera trazenog bita. Moze
 *              se koristiti u uslovima koji ispituju vrednost samo jednog bita.
 *              BV32 vrsi kast izlazne vrednosti na uint32_t tip.
 *
 * @param		bit						Bit pozicija koja se trazi.
 * @return		vrednost bit na @c bit poziciji.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(ES_BV32) || defined(__DOXYGEN__)
# define ES_BV32(bit)                                                           \
    (uint32_t)((uint32_t)1 << (bit))
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Vrsi poravnjanje @a expr1 promenjive sa granicama specificarane
 *              u @a expr2
 *
 *              Primer:
 *
 *              @a expr1 ima binarnu vrednost: 10010101 = 149
 *              @a expr2 ima binarnu vrednost: 00000100 = 4
 *
 *				Dobija se 152.
 *
 * @param       num                     Promenjiva koju treba poravnati,
 * @param       align                   granica poravnanja.
 *
 * @mseffect
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(ES_ALIGN) || defined(__DOXYGEN__)
# define ES_ALIGN(num, align)                                                   \
    (((num) + (align) - 1U) & ~((align) - 1U))
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Da li je @c expr jednak nekom stepenu dvojke?
 *
 *              Makro vraca TRUE kada je vrednost @c expr izraza jednaka nekom
 *              stepenu dvojke, inace, vraca FALSE.
 *
 * @mseffect
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(ES_IS_POW2) || defined(__DOXYGEN__)
# define ES_IS_POW2(num)                                                        \
    (!((num) & ((num) - 1)))
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Izracunava log2 za vreme kompajliranja za uint8_t
 *
 * @mseffect
 */
/*-----------------------------------------------------------------------------------------------*/
#define ES_UINT8_LOG2(x)                                                        \
    ((x) < 2 ? 0 :                                                              \
     ((x) < 4 ? 1 :                                                             \
      ((x) < 8 ? 2 :                                                            \
       ((x) < 16 ? 3 :                                                          \
        ((x) < 32 ? 4 :                                                         \
         ((x) < 64 ? 5 :                                                        \
          ((x) < 128 ? 6 : 7)))))))

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief		Kreira masku za MSB bit koji odgovara tipu @c type
 *
 * @param		type					Tip podataka za koji se trazi MSB.
 * @return		Odgovarajuca binarna maska za MSB.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(ES_MASK_MSB) || defined(__DOXYGEN__)
# define ES_MASK_MSB(type)														\
	(1UL << ((sizeof(type) * 8U) - 1U))
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief		Postavlja MSB bit na jedan, "1".
 *
 * @param		var						Promenljiva kojoj se postavlja MSB bit
 * 										na jedan.
 * @mseffect
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(ES_SET_MSB) || defined(__DOXYGEN__)
# define ES_SET_MSB(var)														\
	do {																		\
		var |= ES_MASK_MSB(var);												\
	} while (0)
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief		Postavlja MSB bit na nulu, "0".
 *
 * @param		var						Promenljiva kojoj se postavlja MSB bit
 * 										na nulu.
 * @mseffect
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(ES_CLR_MSB) || defined(__DOXYGEN__)
# define ES_CLR_MSB(var)														\
	do {																		\
		var &= ~ES_MASK_MSB(var);												\
	} while (0)
#endif

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Ostali makroi
 * @{ *//*---------------------------------------------------------------------------------------*/

#if !defined(ES_ATOMIC) || defined(__DOXYGEN__)
# define ES_ATOMIC(code)                                                        \
    do {                                                                        \
        HAL_CRITICAL_DECL();                                                    \
        HAL_CRITICAL_ENTER();                                                   \
        code;                                                                   \
        HAL_CRITICAL_EXIT();                                                    \
    } while (0)
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

/*-------------------------------------------------------------------------------------------*//**
 * @name        Standardni tipovi podataka
 *
 * @brief       Uobicajna je praksa da platforma definise ove tipove, medjutim,
 *              neke platforme u potpunosti zanemaruju ove specifikacije.
 * @{ *//*---------------------------------------------------------------------------------------*/

#if !defined(HAL_STD_TYPES) || defined(__DOXYGEN__)
typedef unsigned char                   uint8_t;
typedef unsigned short                  uint16_t;
typedef unsigned int                    uint32_t;
typedef signed   char                   int8_t;
typedef signed   short                  int16_t;
typedef signed   int                    int32_t;
#endif

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Standardni brzi tipovi podataka
 *
 * @brief       Ovi tipovi definisu najbrze moguce tipove podataka.
 * @{ *//*---------------------------------------------------------------------------------------*/

#if !defined(HAL_FAST_TYPES) || defined(__DOXYGEN__)
typedef int8_t                          int_fast8_t;
typedef int16_t                         int_fast16_t;
typedef int32_t                         int_fast32_t;
typedef uint8_t                         uint_fast8_t;
typedef uint16_t                        uint_fast16_t;
typedef uint32_t                        uint_fast32_t;
#endif

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Tip podataka koji odgovara sirini magistrale podataka
 *
 * @brief       Ovi tipovi definisu tipove podataka koji su @b zavisni od
 *              koriscene arhitekture/varijante.
 * @{ *//*---------------------------------------------------------------------------------------*/

#if !defined(HAL_NATIVE_TYPES) || defined(__DOXYGEN__)
# define HAL_NATIVE_BITS                8U
# define HAL_UNATIVE_BITS               8U
typedef int8_t                          native_T;
typedef uint8_t                         unative_T;
#endif

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Tip podataka size_t, ptrdiff_t i wchar_t
 *
 * @brief       Ovi tipovi definisu tipove podataka koji su @b zavisni od
 *              koriscene arhitekture/varijante.
 * @{ *//*---------------------------------------------------------------------------------------*/

#if !defined(HAL_DEF_TYPES) || defined(__DOXYGEN__)
typedef native_t                        size_t
typedef unative_t                       ptrdiff_t
typedef wchar_t                         uint16_t
#endif

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Podrska za funkcije sa promenljivim brojem argumenata
 * @{ *//*---------------------------------------------------------------------------------------*/

#if !defined(HAL_VARARG_SUPP) || defined(__DOXYGEN__)
typedef void * va_list;

# define va_start(ap,l)                                                         \
    do{                                                                         \
        (ap) = (void *)((char *)&(l));                                          \
    } while(0)

# define va_end(ap)

# define va_arg(ap,type)                                                        \
    (*((type *)((ap) = ((type *)ap) - 1)))

# define va_copy(dst, src)                                                      \
    ((dst) = (src))

#endif

/** @} *//*--------------------------------------------------------------------------------------*/


/*************************************************************************************************
 * GLOBAL VARIABLES
 *************************************************************************************************/


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


/** @endcond *//** @} *//*************************************************************************
 * END of hal_compiler.h
 *************************************************************************************************/
#endif /* HAL_COMPILER_H_ */
