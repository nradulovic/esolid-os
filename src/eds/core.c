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
 * @brief       Implementacija jezgra
 * @addtogroup  core_impl
 *********************************************************************//** @{ */

/*=========================================================  INCLUDE FILES  ==*/

#define CORE_PKG_H_VAR
#include "eds_private.h"
#include "eds/common.h"
#include "eds/mem.h"
#include "arch/cpu.h"
#include "../config/kernel_config.h"

/*=========================================================  LOCAL DEFINES  ==*/

/**
 * @brief       Konstanta za potpis EPA objekta
 * @details     Konstanta se koristi prilikom debag procesa kako bi funkcije
 *              koje prihvate pokazivac na EPA objekat bile sigurne da je EPA
 *              objekat validan. EPA objekti koji su obrisani nemaju ovaj potpis.
 * @pre         Opcija @ref OPT_KERNEL_DBG mora da bude aktivna kako bi bila
 *              omogucena provera pokazivaca.
 */
#define EPA_SIGNATURE                   (0xDEEF)

/*=========================================================  LOCAL MACRO's  ==*/

/*------------------------------------------------------------------------*//**
 * @ingroup     Pomocni makroi za rad sa bitmapom
 * @{ *//*--------------------------------------------------------------------*/
#if (OPT_KERNEL_EPA_PRIO_MAX <= PORT_DATA_WIDTH)
# define PRIO_INDX                      OPT_KERNEL_EPA_PRIO_MAX
# define PRIO_INDX_GROUP                1
#else
# define PRIO_INDX                      PORT_DATA_WIDTH
# define PRIO_INDX_GROUP                (ES_DIV_ROUNDUP(OPT_KERNEL_EPA_PRIO_MAX, PRIO_INDX))
#endif
#define PRIO_INDX_PWR                   ES_UINT8_LOG2(PRIO_INDX)

/** @} *//*-------------------------------------------------------------------*/
/*======================================================  LOCAL DATA TYPES  ==*/

/**
 * @brief       Bitmap spremnih EPA objekata
 */
struct rdyBitmap {
#if !(OPT_KERNEL_EPA_PRIO_MAX <= PORT_DATA_WIDTH) || defined(__DOXYGEN__)
/**
 * @brief       Grupa prioriteta EPA objekata
 * @details     Prilikom trazenja sledeceg aktivnog EPA objekta prvo se
 *              pretrazuje ovaj clan.
 * @note        Ovaj clan se ne koristi kada uslov
 *              "OPT_KERNEL_EPA_PRIO_MAX < PORT_DATA_WIDTH" nije ispunjen. U
 *              tom slucaju, generisani kod je manji i efikasniji prilikom
 *              komutacije EPA objekata.
 */
    volatile portReg_T       bitGroup;
#endif

/**
 * @brief       Prioriteti EPA objekata
 * @details     Kad je pretragom bitGroup utvrdjeno da se ovde nalazi spreman
 *              EPA objekat, onda se pretraga nastavlja ovde.
 */
    volatile portReg_T       bit[PRIO_INDX_GROUP];

/**
 * @brief       Lista aktivnih EPA objekata;
 */
    esEpa_T *       list[OPT_KERNEL_EPA_PRIO_MAX];
};

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

static PORT_C_INLINE bool_T schedRdyIsEmptyI_(
    void);

static PORT_C_INLINE esEpa_T *schedRdyGetEpaI_(
    void);

static PORT_C_INLINE bool_T schedRdyIsEpaRdy_(
    const esEpa_T * epa);

static PORT_C_INLINE void schedRdyRegI_(
    const esEpa_T * epa);

static PORT_C_INLINE void schedRdyUnRegI_(
    const esEpa_T * epa);

static void schedInit(
    void);

static PORT_C_INLINE void epaInit_(
    esEpa_T *       epa,
    esState_T *     stateQueue,
    esEvt_T **      evtQueue,
    const PORT_C_ROM esEpaDef_T * definition);

static PORT_C_INLINE void esEpaDeInit_(
    esEpa_T *       epa);

esEvt_T * evtFetchI(
    esEpa_T *       epa);

/*=======================================================  LOCAL VARIABLES  ==*/

/**
 * @brief       Pokazivac na EPA objekat koji se trenutno izvrsava
 */
static esEpa_T * gCurrentEpa;

/**
 * @brief       Trenutno stanje kernel-a
 */
static esKernelState_T gKernelState;

/**
 * @brief       Bitmape spremnih EPA objekata
 */
static struct rdyBitmap gRdyBitmap;

/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Scheduler funkcije
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Inicijalizacija scheduler-a
 */
static void schedInit(
    void) {

    gCurrentEpa = (esEpa_T *)0U;
    gKernelState = KERNEL_STOPPED;
}

/**
 * @brief       Vraca stanje reda za cekanje.
 * @return      Boolean vrednost koja karakterise stanje reda za cekanje
 *  @retval     TRUE - ne postoji EPA objekat koji ceka izvrsavanje,
 *  @retval     FALSE - postoji barem jedan EPA objekat koji ceka izvrsavanje.
 */
static PORT_C_INLINE bool_T schedRdyIsEmptyI_(
    void) {

#if (OPT_KERNEL_EPA_PRIO_MAX <= PORT_DATA_WIDTH)
    bool_T answer;

    if ((portReg_T)0U == gRdyBitmap.bit[0]) {
        answer = TRUE;
    } else {
        answer = FALSE;
    }

    return (answer);
#else
    bool_T answer;

    if ((portReg_T)0U == gRdyBitmap.bitGroup) {
        answer = TRUE;
    } else {
        answer = FALSE;
    }

    return (answer);
#endif
}

/**
 * @brief       Vraca pokazivac na sledeci EPA objekat sa najvecim prioritetom.
 * @return      EPA objekat sa najvecim prioritetom koji ceka na izvrsenje.
 */
static PORT_C_INLINE esEpa_T * schedRdyGetEpaI_(
    void) {

#if (OPT_KERNEL_INTERRUPT_PRIO_MAX < PORT_DATA_WIDTH)
    esEpa_T *epa;

    epa = gRdyBitmap.list[esCpuFindLastSet(gRdyBitmap.bit[0])];

    return (epa);
#else
    portReg_T indxGroup;
    portReg_T indx;
    esEpa_T * epa;

    indxGroup = ES_CPU_FLS(gRdyBitmap.bitGroup);
    indx = ES_CPU_FLS(gRdyBitmap.bit[indxGroup]);
    epa = gRdyBitmap.list[indx | (indxGroup << PRIO_INDX_PWR)];

    return (epa);
#endif
}

/**
 * @brief       Ispituje da li je EPA objekat u listi reda za cekanje.
 * @param       [in] epa               Pokazivac na EPA objekat cije stanje
 *                                      trazi.
 * @return      Stanje navedenog EPA objekta
 *  @retval     TRUE - EPA objekat ceka na izvrsenje
 *  @retval     FALSE - EPA objekat ne ceka na izvrsenje
 */
static PORT_C_INLINE bool_T schedRdyIsEpaRdy_(
    const esEpa_T * epa) {

#if (OPT_KERNEL_EPA_PRIO_MAX <= PORT_DATA_WIDTH)
    bool_T answer;

    if (gRdyBitmap.bit[0] & ((portReg_T)1U << epa->prio)) {
        answer = TRUE;
    } else {
        answer = FALSE;
    }

    return (answer);
#else
    portReg_T indxGroup;
    portReg_T indx;

    indx = epa->prio & (~((portReg_T)0U) >> (PORT_DATA_WIDTH - PRIO_INDX_PWR));
    indxGroup = epa->internals.prio >> PRIO_INDX_PWR;

    if (gRdyBitmap.bit[indxGroup] & ((portReg_T)1U << indx)) {
        answer = TRUE;
    } else {
        answer = FALSE;
    }

    return (answer);
#endif
}

/**
 * @brief       Prijavljuje EPA objekat u red za cekanje.
 */
static PORT_C_INLINE void schedRdyRegI_(
    const esEpa_T * epa) {

	ES_KERN_ASSERT(ES_KERN_USAGE_FAILURE, NULL == gRdyBitmap.list[epa->prio]);

    gRdyBitmap.list[epa->prio] = (esEpa_T *)epa;
}

/**
 * @brief       Odjavljuje EPA objekat iz reda za cekanje.
 */
static PORT_C_INLINE void schedRdyUnRegI_(
    const esEpa_T * epa) {

    gRdyBitmap.list[epa->prio] = (esEpa_T *)0U;
}

/**
 * @brief       Ubacuje EPA objekat u red za cekanje.
 * @param       [in] epa               Pokazivac na EPA objekat koji je spreman
 *                                      za izvrsenje.
 * @details     EPA objekat na koji pokazuje pokazivac se ubacuje u listu
 *              spremnih EPA objekata na izvrsenje.
 */
static PORT_C_INLINE void schedRdyInsertI_(
    const esEpa_T * epa) {

#if (OPT_KERNEL_EPA_PRIO_MAX <= PORT_DATA_WIDTH)
    gRdyBitmap.bit[0] |= (portReg_T)1U << epa->prio;
#else
    portReg_T indxGroup;
    portReg_T indx;

    indx = epa->prio & (~((portReg_T)0U) >> (PORT_DATA_WIDTH - PRIO_INDX_PWR));
    gRdyBitmap.bitGroup |= (portReg_T)1U << indxGroup;
    gRdyBitmap.bit[indxGroup] |= (portReg_T)1U << indx;
#endif
}

/**
 * @brief       Izbacuje EPA objekat iz reda za cekanje
 * @param       [in] epa               Pokazivac na EPA objekat koji nije
 *                                      spreman za izvrsenje.
 */
static PORT_C_INLINE void schedRdyRmI_(
    const esEpa_T * epa) {

#if (OPT_KERNEL_EPA_PRIO_MAX <= PORT_DATA_WIDTH)
    gRdyBitmap.bit[0] &= ~((portReg_T)1U << epa->prio);
#else
    portReg_T indxGroup;
    portReg_T indx;

    indx = epa->prio & (~((portReg_T)0U) >> (PORT_DATA_WIDTH - PRIO_INDX_PWR));
    indxGroup = epa->prio >> PRIO_INDX_PWR;
    gRdyBitmap.bit[indxGroup] &= ~((portReg_T)1U << indx);

    if ((portReg_T)0U == gRdyBitmap.bit[indxGroup]) {
        gRdyBitmap.bitGroup &= ~((portReg_T)1U << indxGroup);
    }
#endif
}

/** @} *//*-------------------------------------------------------------------*/
/*------------------------------------------------------------------------*//**
 * @name        Inicijalizacija i deinicijalizacija EPA objekta
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Inicijalizuje EPA objekat
 * @param       [out] epa               Pokazivac na strukturu EPA objekta,
 * @param       [in] stateQueue         memorija za cuvanje stanja HSM automata,
 * @param       [in] evtQueue           memorija za cuvanje reda za cekanje,
 * @param       [in] definition         pokazivac na definicionu strukturu EPA
 *                                      objekta.
 * @notapi
 */
static PORT_C_INLINE void epaInit_(
    esEpa_T *       epa,
    esState_T *     stateQueue,
    esEvt_T **      evtQueue,
    const PORT_C_ROM esEpaDef_T * definition) {

    PORT_CRITICAL_DECL;

    smInit(
        (esSm_T *)epa,
        definition->smInitState,
        stateQueue,
        definition->smLevels);
    evtQInit(
        &epa->evtQueue,
        evtQueue,
        definition->evtQueueLevels);
    epa->prio = definition->epaPrio;
    epa->name = definition->epaName;
    ES_KERN_API_OBLIGATION(epa->signature = EPA_SIGNATURE);
    PORT_CRITICAL_ENTER();
    schedRdyRegI_(
        epa);
    esEvtPostI(                                                                 /* Postavi dogadjaj INIT u redu cekanja ovog automata.      */
        epa,
        (esEvt_T *)&evtSignal[SIG_INIT]);
    PORT_CRITICAL_EXIT();
}

/**
 * @brief       DeInicijalizuje EPA objekat
 * @param       [out] epa               Pokazivac na strukturu EPA objekta.
 * @notapi
 */
static PORT_C_INLINE void esEpaDeInit_(
    esEpa_T *       epa) {

    PORT_CRITICAL_DECL;

    PORT_CRITICAL_ENTER();
    schedRdyRmI_(
        epa);
    schedRdyUnRegI_(
        epa);
    PORT_CRITICAL_EXIT();

    while (FALSE == evtQIsEmptyI_(&epa->evtQueue)) {
        esEvt_T * evt;

        evt = evtQGetI_(
            &epa->evtQueue);
        PORT_CRITICAL_ENTER();
        esEvtDestroyI(
            evt);
        PORT_CRITICAL_EXIT();
    }
    evtQDeInit(
        &epa->evtQueue);
    smDeInit(
        &epa->sm);

    ES_KERN_API_OBLIGATION(epa->signature = ~EPA_SIGNATURE);
}

/** @} *//*-------------------------------------------------------------------*/
/*------------------------------------------------------------------------*//**
 * @name        Ostale lokalne funkcije
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Dobavlja dogadjaj iz reda za cekanje @c aEvtQueue
 * @param       epa                    Pokazivac na red za cekanje.
 * @return      Dogadjaj iz reda cekanja.
 * @notapi
 */
esEvt_T * evtFetchI(
    esEpa_T *       epa) {

    esEvt_T * evt;

    evt = evtQGetI_(
        &epa->evtQueue);
    evtUsrRmI_(
        evt);

    if (TRUE == evtQIsEmptyI_(&epa->evtQueue)) {
        schedRdyRmI_(
            epa);
    }

    return (evt);
}

/** @} *//*-------------------------------------------------------------------*/
/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

/*----------------------------------------------------------------------------*/
void esEvtPost(
    esEpa_T *       epa,
    esEvt_T *       evt) {

    PORT_CRITICAL_DECL;

    PORT_CRITICAL_ENTER();
    esEvtPostI(
       epa,
       evt);
    PORT_CRITICAL_EXIT();
}

/*----------------------------------------------------------------------------*/
void esEvtPostI(
    esEpa_T *       epa,
    esEvt_T *       evt) {

    ES_KERN_API_REQUIRE(ES_KERN_ARG_NULL, NULL != epa);
    ES_KERN_API_REQUIRE(ES_KERN_ARG_NOT_VALID, EPA_SIGNATURE == epa->signature);
    ES_KERN_API_REQUIRE(ES_KERN_ARG_NULL, NULL != evt);
    ES_KERN_API_REQUIRE(ES_KERN_ARG_NOT_VALID, EVT_SIGNATURE == evt->signature);

    if (TRUE == evtQIsEmptyI_(&epa->evtQueue)) {
        schedRdyInsertI_(
            epa);
        evtQPutI_(
            &epa->evtQueue,
            evt);
        evtUsrAddI_(
            evt);
    } else if (FALSE == evtQIsFullI_(&epa->evtQueue)) {
        evtQPutI_(
            &epa->evtQueue,
            evt);
        evtUsrAddI_(
            evt);
    } else {
        /* Greska! Red je pun. */
    	ES_KERN_ASSERT(ES_KERN_NOT_ENOUGH_MEM, FALSE);

        esEvtDestroyI(
            evt);
    }
}

/*----------------------------------------------------------------------------*/
void esEvtPostAhead(
    esEpa_T *       epa,
    esEvt_T *       evt) {

    PORT_CRITICAL_DECL;

    PORT_CRITICAL_ENTER();
    esEvtPostAheadI(
        epa,
        evt);
    PORT_CRITICAL_EXIT();
}

/*----------------------------------------------------------------------------*/
void esEvtPostAheadI(
    esEpa_T *       epa,
    esEvt_T *       evt) {

    ES_KERN_API_REQUIRE(ES_KERN_ARG_NULL, NULL != epa);
    ES_KERN_API_REQUIRE(ES_KERN_ARG_NOT_VALID, EPA_SIGNATURE == epa->signature);
    ES_KERN_API_REQUIRE(ES_KERN_ARG_NULL, NULL != evt);
    ES_KERN_API_REQUIRE(ES_KERN_ARG_NOT_VALID, EVT_SIGNATURE == evt->signature);

    if (TRUE == evtQIsEmptyI_(&epa->evtQueue)) {
        schedRdyInsertI_(
            epa);
        evtQPutAheadI_(
            &epa->evtQueue,
            evt);
        evtUsrAddI_(
            evt);
    } else if (FALSE == evtQIsFullI_(&epa->evtQueue)) {
        evtQPutAheadI_(
            &epa->evtQueue,
            evt);
        evtUsrAddI_(
            evt);
    } else {
        /* Greska! Red je pun. */
        esEvtDestroyI(
            evt);
    }
}

/*----------------------------------------------------------------------------*/
esEpa_T * esEpaCreate(
    const PORT_C_ROM esMemClass_T *  memClass,
    const PORT_C_ROM esEpaDef_T *    definition) {

    uint8_t * newEpa;
    size_t coreSize;
    size_t smpQSize;
    size_t evtQSize;

    ES_KERN_API_REQUIRE(ES_KERN_ARG_NOT_VALID, (memClass == &esMemDynClass) || (memClass == &esMemStaticClass));
    ES_KERN_API_REQUIRE(ES_KERN_ARG_NULL, NULL != definition);
    ES_KERN_API_REQUIRE(ES_KERN_ARG_OUT_OF_RANGE, OPT_KERNEL_EPA_PRIO_MAX > definition->epaPrio);
    ES_KERN_API_REQUIRE(ES_KERN_ARG_OUT_OF_RANGE, 0U < definition->evtQueueLevels);
    ES_KERN_API_REQUIRE(ES_KERN_ARG_NULL, NULL != definition->smInitState);
    ES_KERN_API_REQUIRE(ES_KERN_ARG_OUT_OF_RANGE, 2U <= definition->smLevels);
    ES_KERN_API_REQUIRE(ES_KERN_USAGE_FAILURE, sizeof(esEpa_T) <= definition->epaWorkspaceSize);

#if !defined(PORT_SUPP_UNALIGNED_ACCESS) || defined(OPT_OPTIMIZE_SPEED)         /* Ukoliko port ne podrzava UNALIGNED ACCESS ili je ukljuce-*/
                                                                                /* na optimizacija za brzinu vrsi se zaokruzivanje velicina */
                                                                                /* radi brzeg pristupa memoriji.                            */
    coreSize = GP_ALIGN(
        definition->epaWorkspaceSize, PORT_DATA_ALIGNMENT);
    smpQSize = GP_ALIGN(
        stateQReqSize(
            definition->smLevels),
        PORT_DATA_ALIGNMENT);
    evtQSize = GP_ALIGN(
        evtQReqSize(
            definition->evtQueueLevels),
        PORT_DATA_ALIGNMENT);
#else
    coreSize = definition->epaWorkspaceSize;
    smpQSize = stateQReqSize(
        definition->smLevels);
    evtQSize = evtQReqSize(
        definition->evtQueueLevels
#endif

#if (OPT_MM_DISTRIBUTION == ES_MM_DYNAMIC_ONLY)
    {
        PORT_CRITICAL_DECL;

        (void)memClass;
        PORT_CRITICAL_ENTER();
        newEpa = esDMemAllocI(
            &DMemDefStorage,
            coreSize + smpQSize + evtQSize);
        PORT_CRITICAL_EXIT();
    }
#elif (OPT_MM_DISTRIBUTION == ES_MM_STATIC_ONLY)
    {
        PORT_CRITICAL_DECL;

        (void)memClass;
        PORT_CRITICAL_ENTER();
        newEpa = esSmemAllocI(
            coreSize + smpQSize + evtQSize);
        PORT_CRITICAL_EXIT();
    }
#else
    newEpa = (* memClass->alloc)(coreSize + smpQSize + evtQSize);
    *((const PORT_C_ROM struct memClass **)newEpa) = memClass;
#endif
    epaInit_(
        (esEpa_T *)newEpa,
        (esState_T *)(newEpa + coreSize),
        (esEvt_T **)(newEpa + coreSize + smpQSize),
        definition);

    return ((esEpa_T *)newEpa);
}

/*----------------------------------------------------------------------------*/
void esEpaDestroy(
    esEpa_T *       epa) {

#if (OPT_MM_DISTRIBUTION == ES_MM_STATIC_ONLY)
    /* Greska! Statican objekat */
#elif (OPT_MM_DISTRIBUTION == ES_MM_DYNAMIC_ONLY)
    PORT_CRITICAL_DECL;

    ES_KERN_API_REQUIRE(ES_KERN_ARG_NULL, NULL != epa);
    ES_KERN_API_REQUIRE(ES_KERN_ARG_NOT_VALID, EPA_SIGNATURE == epa->signature);

    esEpaDeInit_(
        epa);
    PORT_CRITICAL_ENTER();
    esDMemDeAllocI(
        &DMemDefStorage,
        epa);
    PORT_CRITICAL_EXIT();
#else
    esEpaDeInit_(
        epa);
    ((const PORT_C_ROM struct memClass *)epa)->deAlloc(epa);
#endif
}

/*----------------------------------------------------------------------------*/
esEpa_T * esEpaGet(
    void) {

    return (gCurrentEpa);
}

/*----------------------------------------------------------------------------*/
uint8_t esEpaPrioGet(
    const esEpa_T * epa) {

    ES_KERN_API_REQUIRE(ES_KERN_ARG_NULL, NULL != epa);
    ES_KERN_API_REQUIRE(ES_KERN_ARG_NOT_VALID, EPA_SIGNATURE == epa->signature);

    return (epa->prio);
}

/*----------------------------------------------------------------------------*/
void esEpaPrioSet(
    esEpa_T *       epa,
    uint8_t         newPrio) {

    PORT_CRITICAL_DECL;
    bool_T status;

    ES_KERN_API_REQUIRE(ES_KERN_ARG_NULL, NULL != epa);
    ES_KERN_API_REQUIRE(ES_KERN_ARG_NOT_VALID, EPA_SIGNATURE == epa->signature);
    ES_KERN_API_REQUIRE(ES_KERN_ARG_OUT_OF_RANGE, OPT_KERNEL_EPA_PRIO_MAX > newPrio);

    PORT_CRITICAL_ENTER();
    status = schedRdyIsEpaRdy_(
        epa);
    schedRdyRmI_(
        epa);
    schedRdyUnRegI_(
        epa);
    epa->prio = (uint_fast8_t)newPrio;
    schedRdyRegI_(
        epa);

    if (TRUE == status) {
        schedRdyInsertI_(
            epa);
    }
    PORT_CRITICAL_EXIT();
}

/*----------------------------------------------------------------------------*/
void esKernelInit(
    void) {

    ES_KERN_API_REQUIRE(ES_KERN_USAGE_FAILURE, gKernelState == KERNEL_STOPPED);

    esSmpInit();
    schedInit();

#if defined(OPT_STP_ENABLE)
    esStpInit();
#endif
}

/*----------------------------------------------------------------------------*/
void esKernelStart(
    void) {

    PORT_CRITICAL_DECL;

    gKernelState = KERNEL_RUNNING;
    PORT_CRITICAL_ENTER();

    while (TRUE) {

        while (FALSE == schedRdyIsEmptyI_()) {
            esEvt_T * evt;
            esEpa_T * epa;
            esStatus_T status;

            gCurrentEpa = epa = schedRdyGetEpaI_();
            evt = evtFetchI(
                epa);
            PORT_CRITICAL_EXIT();
            status = SM_DISPATCH(
                (esSm_T *)epa,
                evt);
            PORT_CRITICAL_ENTER();

            if (RETN_DEFERRED == status) {
                esEvtPostI(
                    epa,
                    evt);
            } else {
                esEvtDestroyI(
                    evt);
            }
        }
        gCurrentEpa = (esEpa_T *)0U;
        PORT_CRITICAL_EXIT();
        /* ES_CPU_SLEEP(); */
        PORT_CRITICAL_ENTER();
    }
}

/*----------------------------------------------------------------------------*/
esKernelState_T esKernelStatus(
    void) {

    return (gKernelState);
}

/** @} *//*-------------------------------------------------------------------*/
/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of core.c
 ******************************************************************************/
