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
 * @author  	Nenad Radulovic
 * @brief       Privatni interfejs scheduler modula.
 * @addtogroup  sched_impl
 *********************************************************************//** @{ */

#ifndef SCHED_PKG_H_
#define SCHED_PKG_H_

#ifdef SCHED_PKG_H_VAR
# define SCHED_PKG_EXT
#else
# define SCHED_PKG_EXT extern
#endif

/*=========================================================  INCLUDE FILES  ==*/
/*===============================================================  DEFINES  ==*/
/*===============================================================  MACRO's  ==*/
/*------------------------------------------------------------------------*//**
 * @ingroup     Pomocni makroi za rad sa bitmapom
 * @{ *//*--------------------------------------------------------------------*/
#if (OPT_KERNEL_EPA_PRIO_MAX <= ES_CPU_UNATIVE_BITS)
# define PRIO_INDX                      OPT_KERNEL_EPA_PRIO_MAX
# define PRIO_INDX_GROUP                1
#else
# define PRIO_INDX                      HAL_UNATIVE_BITS
# define PRIO_INDX_GROUP                (ES_DIV_ROUNDUP(OPT_KERNEL_EPA_PRIO_MAX, PRIO_INDX))
#endif
#define PRIO_INDX_PWR                   ES_UINT8_LOG2(PRIO_INDX)

/** @} *//*-------------------------------------------------------------------*/
/*------------------------------------------------------  C++ extern begin  --*/
#if defined(__cplusplus)
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

/**
 * @brief       Bitmap spremnih EPA objekata
 */
struct rdyBitmap {
#if !(OPT_KERNEL_EPA_PRIO_MAX <= ES_CPU_UNATIVE_BITS) || defined(__DOXYGEN__)
/**
 * @brief       Grupa prioriteta EPA objekata
 * @details     Prilikom trazenja sledeceg aktivnog EPA objekta prvo se
 *              pretrazuje ovaj clan.
 * @note        Ovaj clan se ne koristi kada uslov
 *              "OPT_KERNEL_EPA_PRIO_MAX < ES_CPU_UNATIVE_BITS" nije ispunjen. U
 *              tom slucaju, generisani kod je manji i efikasniji prilikom
 *              komutacije EPA objekata.
 */
    volatile unative_T       bitGroup;
#endif

/**
 * @brief       Prioriteti EPA objekata
 * @details     Kad je pretragom bitGroup utvrdjeno da se ovde nalazi spreman
 *              EPA objekat, onda se pretraga nastavlja ovde.
 */
    volatile unative_T       bit[PRIO_INDX_GROUP];

/**
 * @brief       Lista aktivnih EPA objekata;
 */
    esEpa_T *       list[OPT_KERNEL_EPA_PRIO_MAX];

/**
 * @brief       EPA objekat koji se trenutno izvrsava
 */
    esEpa_T *       current;
};

/*======================================================  GLOBAL VARIABLES  ==*/

/**
 * @brief       Bitmape spremnih EPA objekata
 */
SCHED_PKG_EXT struct rdyBitmap gRdyBitmap;

/*===================================================  FUNCTION PROTOTYPES  ==*/

/**
 * @brief       Inicijalizacija scheduler-a
 */
void schedInit(
    void);

/**
 * @brief       Kooperativni scheduler
 */
void sched(
    void);

/**
 * @brief       Prijavljuje EPA objekat u red za cekanje.
 * @param       [in] epa                Pokazivac na EPA objekat
 * @param       [in] prio               Prioritet datog epa objekta
 * @details     Pre nego sto EPA objekat moze da se pozove od strane scheduler-a
 *              on mora najpre da se prijavi redu za cekanje.
 */
void schedRdyRegI(
    const esEpa_T * epa,
    uint_fast8_t    prio);

/**
 * @brief       Odjavljuje EPA objekat iz reda za cekanje.
 * @param       [in] prio               Prioritet EPA objekata
 */
void schedRdyUnRegI(
    uint_fast8_t    prio);

/**
 * @brief       Ubacuje EPA objekat u red za cekanje.
 * @param       [in] prio               Prioritet EPA objekata koji je spreman
 *                                      za izvrsenje.
 * @details     EPA objekat sa datim prioritetom se smesta u listu spremnih EPA
 *              objekata na izvrsenje.
 * @inline
 */
static C_INLINE void schedRdyInsertI_(
    uint_fast8_t    prio) {

#if (OPT_KERNEL_EPA_PRIO_MAX <= ES_CPU_UNATIVE_BITS)
    gRdyBitmap.bit[0] |= (unative_T)1U << prio;
#else
    unative_T indxGroup;
    unative_T indx;

    indx = prio & (~((unative_T)0U) >> (ES_CPU_UNATIVE_BITS - PRIO_INDX_PWR));
    indxGroup = prio >> PRIO_INDX_PWR;
    gRdyBitmap.bitGroup |= (unative_T)1U << indxGroup;
    gRdyBitmap.bit[indxGroup] |= (unative_T)1U << indx;
#endif
}

/**
 * @brief       Izbacuje EPA objekat iz reda za cekanje
 * @param       [in] prio               Prioritet EPA objekata
 * @inline
 */
static C_INLINE void schedRdyRmI_(
    uint_fast8_t    prio) {

#if (OPT_KERNEL_EPA_PRIO_MAX <= ES_CPU_UNATIVE_BITS)
    gRdyBitmap.bit[0] &= ~((unative_T)1U << prio);
#else
    unative_T indxGroup;
    unative_T indx;

    indx = prio & (~((unative_T)0U) >> (ES_CPU_UNATIVE_BITS - PRIO_INDX_PWR));
    indxGroup = prio >> PRIO_INDX_PWR;
    gRdyBitmap.bit[indxGroup] &= ~((unative_T)1U << indx);

    if ((unative_T)0U == gRdyBitmap.bit[indxGroup]) {
        gRdyBitmap.bitGroup &= ~((unative_T)1U << indxGroup);
    }
#endif
}

/**
 * @brief       Ispituje da li je EPA objekat u listi reda za cekanje.
 * @param       [in] prio               Prioritet EPA objekata
 * @return      Stanje navedenog EPA objekta
 *  @retval     TRUE - EPA objekat ceka na izvrsenje
 *  @retval     FALSE - EPA objekat ne ceka na izvrsenje
 */
bool_T schedEpaIsRdyI(
    uint_fast8_t    prio);

/**
 * @brief       Koji EPA objekat se trenutno izvrsava
 * @return      Vraca pokazivac na trenutno izvrsavan EPA objekat
 * @inline
 */
static C_INLINE esEpa_T * schedEpaGetCurrent_(
    void) {

    return (gRdyBitmap.current);
}

/*--------------------------------------------------------  C++ extern end  --*/
#if defined(__cplusplus)
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of sched_pkg.h
 ******************************************************************************/
#endif /* SCHED_PKG_H_ */
