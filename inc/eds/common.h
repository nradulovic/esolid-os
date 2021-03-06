/*
 * This file is part of eSolid-OS
 *
 * Copyright (C) 2011, 2012 - Nenad Radulovic
 *
 * eSolid-OS is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * eSolid-OS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with eSolid-OS; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301  USA
 *
 * web site:    http://blueskynet.dyndns-server.com
 * e-mail  :    blueskyniss@gmail.com
 *//***********************************************************************//**
 * @file
 * @author  	Nenad Radulovic
 * @brief       Common and generic macros
 * @details     Common macros for various eSolid modules
 * @addtogroup  common_intf
 * @brief		Common and generic macros
 *********************************************************************//** @{ */

#if !defined(COMMON_H_)
#define COMMON_H_

/*=========================================================  INCLUDE FILES  ==*/
/*===============================================================  MACRO's  ==*/

/*------------------------------------------------------------------------*//**
 * @name        General Purpose macros
 * @details     These macros are used for:
 *              - string concatenation
 *              - simple arithmetic operations
 *              - address and bitwise operations
 * @{ *//*--------------------------------------------------------------------*/

/**@brief       Izracunavanje dimenzije elementa jedno-dimenzionalnog niza
 *              @a expr.
 * @param       string                  Niz
 * @mseffect
 */
#define GP_DIMENSION(string)                                                    \
    (sizeof(string) / sizeof(string[0]))

/**@brief       Vrsi deljenje dva broja i zaokruzuje rezultat.
 * @param       expr1                   deljenik
 * @param       expr2                   delitelj
 * @note        Pozeljno je da @a expr2 bude konstanta, inace, generisace se dva
 *              deljenja.
 * @mseffect
 * @par         Primer:
 * @include     es_div_round.c
 *
 * @par         Izlaz:
 *
 *              @code
 *              Deljenje 28 / 5 = 5
 *              Zaokruzeno: 6
 *              @endcode
 */
#define GP_DIV_ROUND(numerator, denominator)                                    \
    (((numerator) + ((denominator) / 2U)) / (denominator))

/**@brief       Vrsi deljenje dva broja sa zaokruzivanjem broja navise.
 * @param       expr1                   deljenik
 * @param       expr2                   delitelj
 * @note        Pozeljno je da @a expr2 bude konstanta, inace, generisace se
 *              jedno oduzimanje i jedno deljenje.
 * @mseffect
 * @par         Primer:
 * @include     es_div_roundup.c
 *
 * @par         Izlaz:
 *
 *              @code
 *              Deljenje 27 / 5 = 5
 *              Zaokruzeno: 6
 *              @endcode
 */
#define GP_DIV_ROUNDUP(numerator, denominator)                                  \
    (((numerator) + (denominator) - 1U) / (denominator))

/**@brief       Vraca vecu vrednost od ponudjenih vrednosti @a expr1 i @a expr2
 */
#define GP_MAX(expr1, expr2)                                                    \
    ((expr1 > expr2) ? expr1 : expr2))

/**@brief       Vraca manju vrednost od ponudjenih vrednosti @a expr1 i @a expr2
 */
#define GP_MIN(expr1, expr2)                                                    \
    ((expr1 < expr2) ? expr1 : expr2))

/**@brief       Vrsi poravnjanje @a num promenjive sa granicama specificarane
 *              u @a align
 * @param       num                     Promenjiva koju treba poravnati,
 * @param       align                   granica poravnanja.
 * @details     Primer:
 *              @a expr1 ima binarnu vrednost: 10010101 = 149
 *              @a expr2 ima binarnu vrednost: 00000100 = 4
 *              Dobija se 152.
 * @mseffect
 */
#define GP_ALIGN(num, align)                                                    \
    ((num) & ~((align) - 1U))

/**@brief       Vrsi poravnjanje @a num promenjive sa granicama specificarane
 *              u @a align
 * @param       num                     Promenjiva koju treba poravnati,
 * @param       align                   granica poravnanja.
 * @details     Primer:
 *              @a expr1 ima binarnu vrednost: 10010101 = 149
 *              @a expr2 ima binarnu vrednost: 00000100 = 4
 *              Dobija se 152.
 * @mseffect
 */
#define GP_ALIGN_UP(num, align)                                                 \
    (((num) + (align) - 1U) & ~((align) - 1U))

/**@brief       Da li je @c expr jednak nekom stepenu dvojke?
 * @details     Makro vraca TRUE kada je vrednost @c expr izraza jednaka nekom
 *              stepenu dvojke, inace, vraca FALSE.
 * @mseffect
 */
#define GP_IS_POW2(num)                                                         \
    (!((num) & ((num) - 1)))

/**@brief       Izracunava log2 za vreme kompajliranja za uint8_t
 * @mseffect
 */
#define GP_UINT8_LOG2(x)                                                        \
    ((x) < 2 ? 0 :                                                              \
     ((x) < 4 ? 1 :                                                             \
      ((x) < 8 ? 2 :                                                            \
       ((x) < 16 ? 3 :                                                          \
        ((x) < 32 ? 4 :                                                         \
         ((x) < 64 ? 5 :                                                        \
          ((x) < 128 ? 6 : 7)))))))

/**@brief       Kreira masku za MSB bit koji odgovara tipu @c type
 * @param       type                    Tip podataka za koji se trazi MSB.
 * @return      Odgovarajuca binarna maska za MSB.
 */
#define GP_MASK_MSB(type)                                                       \
    (1UL << ((sizeof(type) * 8U) - 1U))

/**@brief       Postavlja MSB bit na jedan, "1".
 * @param       var                     Promenljiva kojoj se postavlja MSB bit
 *                                      na jedan.
 * @mseffect
 */
#define GP_SET_MSB(var)                                                         \
    do {                                                                        \
        var |= GP_MASK_MSB(var);                                                \
    } while (0)

/**@brief       Postavlja MSB bit na nulu, "0".
 * @param       var                     Promenljiva kojoj se postavlja MSB bit
 *                                      na nulu.
 * @mseffect
 */
#define GP_CLR_MSB(var)                                                         \
    do {                                                                        \
        var &= ~GP_MASK_MSB(var);                                               \
    } while (0)

/**@} *//*--------------------------------------------------------------------*/

/*============================================================  DATA TYPES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/
/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of common.h
 ******************************************************************************/
#endif /* COMMON_H_ */
