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
 * @brief       Konfiguracija Event Object Transportation (EOT) modula.
 *
 * ------------------------------------------------------------------------------------------------
 *
 * @addtogroup  eot_cfg
 *
 ****************************************************************************************//** @{ */

#ifndef EOT_CFG_H_
#define EOT_CFG_H_


/*************************************************************************************************
 * INCLUDE FILES
 *************************************************************************************************/


/*************************************************************************************************
 * DEFINES
 *************************************************************************************//** @cond */


/** @endcond *//**********************************************************************************
 * SETTINGS
 *************************************************************************************************/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Debug podrska EOT modula
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
# define OPT_DBG_EOT
#endif

/*-------------------------------------------------------------------------------------------*//**
 * @name        Podesavanje formata zaglavlja dogadjaja
 *
 * @brief       Ovim opcijama se specificira tip zaglavlja dogadjaja i koja
 *              polja dogadjaja se koriste.
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Koristi se prosirena reprezentacija dogadjaja.
 *
 *              Moguce vrednosti:
 *              - nedefinisano - prosta reprezentacija (@ref eot_evtPln_T)
 *              - definisano   - prosirena reprezentacija (@ref eot_evtExt_T)
 */
/*-----------------------------------------------------------------------------------------------*/
#if defined(__DOXYGEN__)
# define OPT_EVT_USE_EXT
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Koristi se pokazivac na proizvodjaca dogadjaja.
 *
 *              Moguce vrednosti:
 *              - nedefinisano - pokazivac na objekat se ne koristi
 *              - definisano   - pokazivac na objekat se koristi
 */
/*-----------------------------------------------------------------------------------------------*/
#if defined(__DOXYGEN__)
# define OPT_EVT_USE_TRANSMITTER
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Koristi se vremenski marker dogadjaja.
 *
 *              Moguce vrednosti:
 *              - nedefinisano - vrem. marker se ne koristi
 *              - definisano   - vrem. marker se koristi
 *
 *              Podesavanje tipa se vrsi pomocu @ref OPT_EVT_TIME_T.
 */
/*-----------------------------------------------------------------------------------------------*/
#if defined(__DOXYGEN__)
# define OPT_EVT_USE_TIME
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Koristi se tip dogadjaja.
 *
 *              Moguce vrednosti:
 *              - nedefinisano - tip dogadjaja se ne koristi
 *              - definisano   - tip dogadjaja se koristi
 *
 *              Podesavanje tipa se vrsi pomocu @ref OPT_EVT_TYPE_T.
 */
/*-----------------------------------------------------------------------------------------------*/
#if defined(__DOXYGEN__)
# define OPT_EVT_USE_TYPE
#endif

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Fino podesavanje dogadjaja
 *
 * @brief       Ovim opcijama modifikuju se podrazumevani tipovi promenljiva
 *              koje cine zaglavlje jednog dogadjaja.
 *
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Tip promenljive za identifikator dogadjaja.
 *
 *              Tip podatka treba da dovoljne velicine moze da predstavi sve
 *              dogadjaje u sistemu. Na primer, ako u sistemu postoje manje od
 *              255 identifikatora dogadjaja onda ovaj tip moze najmanje biti
 *              uint8_t tipa. Ukoliko sistem poseduje vise od 255 razlicitih
 *              dogadjaja onda ovaj tip treba da bude najmanje uint16_t. Naravno,
 *              treba voditi racuna o zauzecu strukture koja opisuje dogadjaj i
 *              njenom poravnjanju (alignement) u memoriji, gde se moze javiti
 *              potreba za vecim tipom od minimalnog.
 *
 * @note        Ovo podesavanje moze da se zaobidje definisanjem OPT_EVT_ID_T.
 */
/*-----------------------------------------------------------------------------------------------*/
#if defined(__DOXYGEN__)
# define OPT_EVT_ID_T                   uint8_t
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Tip podataka za cuvanje velicine prosirenog dogadjaja.
 *
 * @note        Ovo podesavanje moze da se zaobidje definisanjem OPT_EVT_SIZE_T.
 */
/*-----------------------------------------------------------------------------------------------*/
#if defined(__DOXYGEN__)
# define OPT_EVT_SIZE_T                 uint8_t
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Tip za cuvanje vremenskih atributa dogadjaja.
 *
 * @note        Ovo podesavanje moze da se zaobidje definisanjem OPT_EVT_TIME_T.
 */
/*-----------------------------------------------------------------------------------------------*/
#if defined(__DOXYGEN__)
# define OPT_EVT_TIME_T                 uint8_t
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Tip za cuvanje tipa dogadjaja.
 *
 * @note        Ovo podesavanje moze da se zaobidje definisanjem OPT_EVT_TYPE_T.
 */
/*-----------------------------------------------------------------------------------------------*/
#if defined(__DOXYGEN__)
# define OPT_EVT_TYPE_T                 uint8_t
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Atribut za strukture dogadaja
 *
 *              Prilikom slanja dogadjaja drugim sistemima javlja se problem
 *              pakovanja podataka unutar strukture dogadjaja.
 *
 *              Ovom promenljivom se moze definisati koja direktiva ce koristi
 *              za strukture dogadjaja (poravnjanje, pakovanje).
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(OPT_EVT_STRUCT_ATTRIB) || defined(__DOXYGEN__)
# define OPT_EVT_STRUCT_ATTRIB
#endif

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Podesavanja za tipove podataka za dogadjaje
 * @{ *//*---------------------------------------------------------------------------------------*/

/*
 * Bez optimizacija
 */
#if !defined(ES_OPTIMIZE_SIZE) && !defined(ES_OPTIMIZE_SPEED) || defined(__DOXYGEN__)

# if defined(OPT_EVT_ID_T)
typedef OPT_EVT_ID_T                    eot_evtId_T;
# else
typedef uint8_t                         eot_evtId_T;
# endif

# if defined(OPT_EVT_SIZE_T)
typedef OPT_EVT_SIZE_T                  eot_evtSize_T;
# else
typedef uint8_t                         eot_evtSize_T;
# endif

# if defined(OPT_EVT_USE_TIME)
#  if defined(OPT_EVT_TIME_T)
typedef OPT_EVT_TIME_T                  eot_evtTime_T;
#  else
typedef uint32_t                        eot_evtTime_T;
#  endif
# endif

# if defined(OPT_EVT_USE_TYPE)
#  if defined(OPT_EVT_TYPE_T)
typedef OPT_EVT_TYPE_T                  eot_evtType_T;
#  else
typedef uint8_t                         eot_evtType_T;
#  endif
# endif

#endif

/*
 * Optimizacija po brzini
 */
#if !defined(ES_OPTIMIZE_SIZE) && defined(ES_OPTIMIZE_SPEED)

# if defined(OPT_EVT_ID_T)
typedef OPT_EVT_ID_T                    eot_evtId_T;
# else
typedef uint_fast8_t                    eot_evtId_T;
# endif

# if defined(OPT_EVT_SIZE_T)
typedef OPT_EVT_SIZE_T                  eot_evtSize_T;
# else
typedef uint_fast8_t                    eot_evtSize_T;
# endif

# if defined(OPT_EVT_USE_TIME)
#  if defined(OPT_EVT_TIME_T)
typedef OPT_EVT_TIME_T                  eot_evtTime_T;
#  else
typedef uint32_t                        eot_evtTime_T;
#  endif
# endif

# if defined(OPT_EVT_USE_TYPE)
#  if defined(OPT_EVT_TYPE_T)
typedef OPT_EVT_TYPE_T                  eot_evtType_T;
#  else
typedef uint_fast8_t                    eot_evtType_T;
#  endif
# endif
#endif

/*
 * Optimizacija po zauzecu
 */
#if defined(ES_OPTIMIZE_SIZE) && !defined(ES_OPTIMIZE_SPEED)

# if defined(OPT_EVT_ID_T)
typedef OPT_EVT_ID_T                    eot_evtId_T;
# else
typedef uint8_t                         eot_evtId_T;
# endif

# if defined(OPT_EVT_SIZE_T)
typedef OPT_EVT_SIZE_T                  eot_evtSize_T;
# else
typedef uint8_t                         eot_evtSize_T;
# endif

# if defined(OPT_EVT_USE_TIME)
#  if defined(OPT_EVT_TIME_T)
typedef OPT_EVT_TIME_T                  eot_evtTime_T;
#  else
typedef uint32_t                        eot_evtTime_T;
#  endif
# endif

# if defined(OPT_EVT_USE_TYPE)
#  if defined(OPT_EVT_TYPE_T)
typedef OPT_EVT_TYPE_T                  eot_evtType_T;
#  else
typedef uint8_t                         eot_evtType_T;
#  endif
# endif
#endif

/** @} *//*--------------------------------------------------------------------------------------*/


/*************************************************************************************************
 * CONFIGURATION RESOLUTION
 *************************************************************************************//** @cond */

#if defined(OPT_EVT_USE_EXT)
typedef struct eot_evtExt eot_evt_T;
#endif

#if !defined(OPT_EVT_USE_EXT)
typedef struct eot_evtPln eot_evt_T;
#endif


/** @endcond *//** @} *//*************************************************************************
 * END of HEADER FILE
 *************************************************************************************************/
#endif /* EOT_CFG_H_ */
