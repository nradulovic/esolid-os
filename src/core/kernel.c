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
 * @author      Nenad Radulovic
 * @brief       Implementacija kernel-a i registra.
 * ------------------------------------------------------------------------------------------------
 * @addtogroup  kernel_impl
 ****************************************************************************************//** @{ */

/*============================================================================  INCLUDE FILES  ==*/
#define KERNEL_PKG_H_VAR
#include "core_private.h"

/*============================================================================  LOCAL DEFINES  ==*/
/*-------------------------------------------------------------------------------------------*//**
 * @brief       Local debug define macro.
 *//*--------------------------------------------------------------------------------------------*/
KERNEL_DBG_DEFINE_MODULE(Kernel);

/*============================================================================  LOCAL MACRO's  ==*/
/*=========================================================================  LOCAL DATA TYPES  ==*/
/*-------------------------------------------------------------------------------------------*//**
 * @brief       Cuvara registra
 *//*--------------------------------------------------------------------------------------------*/
typedef struct regSentinel {
/**
 * @brief       Lista svih aktivnih EPA jedinica u sistemu
 */
    esDlsList_T     activeList;

/**
 * @brief       Broj aktivnih jedinica u EPA sistemu
 */
    uint8_t         activeCnt;

/**
 * @brief       Zadnja vrednost dodeljenog PID broja
 */
    uint8_t         lastPid;

/**
 * @brief       Events Per Second
 */
    uint32_t        eps;
} regSentinel_T;

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Trenutno izvrsavana EPA jedinica
 *//*--------------------------------------------------------------------------------------------*/
typedef struct currCtx {
/**
 * @brief       Pokazivac na trenutni EPA objekat koji se izvrsava
 */
    esEpaHeader_T   * epa;

/**
 * @brief       Prioritet trenutnog EPA objekata koji se izvrsava
 */
    esEpaPrio_T     prio;

/**
 * @brief       Trenutno stanje schedulera
 */
    esKernelStatus_T status;
} currCtx_T;

/*================================================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*==========================================================================  LOCAL VARIABLES  ==*/

#if defined(OPT_KERNEL_ENABLE) || defined(__DOXYGEN__)

# if defined(OPT_KERNEL_USE_REGISTRY) || defined(__DOXYGEN__)
/*-------------------------------------------------------------------------------------------*//**
 * @brief       Cuvar registra
 *//*--------------------------------------------------------------------------------------------*/
static regSentinel_T regSentinel;
# endif

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Interni podaci schedulera
 *//*--------------------------------------------------------------------------------------------*/
static currCtx_T currCtx;
#endif /* OPT_KERNEL_ENABLE */

/*=========================================================================  GLOBAL VARIABLES  ==*/
/*===============================================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*-----------------------------------------------------------------------------------------------*
 *              Funkcije koje su dostupne samo ako je kernel omogucen.
 *-----------------------------------------------------------------------------------------------*/
#if defined(OPT_KERNEL_ENABLE) || defined(__DOXYGEN__)

#if defined(OPT_OPTIMIZE_SIZE) || defined(__DOXYGEN__)
void schedRdyInsertI(
    const esEpaHeader_T * aEpa) {

    schedRdyInsertI_(
        aEpa);
}
#endif

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Inicijalizije ready listu
 *//*--------------------------------------------------------------------------------------------*/
C_INLINE_ALWAYS void schedRdyInit_(
    void) {

    rdyBitmap.bit[0] = (unative_T)1U;
    rdyBitmap.bitGroup = (unative_T)1U;
}

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Izbacuje EPA objekat iz reda za cekanje
 *//*--------------------------------------------------------------------------------------------*/
C_INLINE_ALWAYS void schedRdyRemoveI_(
    esEpaHeader_T       * aEpa) {

    unative_T indxGroup;
    unative_T indx;

    indx = aEpa->internals.kernCtrl.prio & (~((unative_T)0U) >> (HAL_UNATIVE_BITS - PRIO_INDX_PWR));

#if (OPT_KERNEL_EPA_PRIO_MAX < HAL_UNATIVE_BITS)
    indxGroup = (unative_T)0U;
#else
    indxGroup = aEpa->internals.kernCtrl.prio >> PRIO_INDX_PWR;
#endif
    rdyBitmap.bit[indxGroup] &= ~((unative_T)1U << indx);

    if ((unative_T)0U == rdyBitmap.bit[indxGroup]) {
        rdyBitmap.bitGroup &= ~((unative_T)1U << indxGroup);
    }
}

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Dobavlja prioritet EPA objekta sa najvecim prioritetom u redu
 *              cekanja.
 *//*--------------------------------------------------------------------------------------------*/
C_INLINE_ALWAYS unative_T schedRdyQueryI_(
    void) {

    unative_T indxGroup;
    unative_T indx;

#if (OPT_KERNEL_EPA_PRIO_MAX < HAL_UNATIVE_BITS)
    indxGroup = (unative_T)0U;
#else
    indxGroup = ES_CPU_FLS(rdyBitmap.bitGroup);
#endif
    indx = ES_CPU_FLS(rdyBitmap.bit[indxGroup]);

    return (indx | (indxGroup << PRIO_INDX_PWR));
}

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Prijavljuje EPA objekat u red za cekanje.
 *//*--------------------------------------------------------------------------------------------*/
C_INLINE_ALWAYS void schedRegisterI_(
    const esEpaHeader_T * aEpa) {

    EPE_ASSERT((esEpaHeader_T *)0U == rdyBitmap.epaList[aEpa->internals.kernCtrl.prio]);  /* Provera: da nije ovaj prioritet vec zauzet?              */
    rdyBitmap.epaList[aEpa->internals.kernCtrl.prio] = (esEpaHeader_T *)aEpa;
}

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Odjavljuje EPA objekat iz reda za cekanje.
 *//*--------------------------------------------------------------------------------------------*/
C_INLINE_ALWAYS void schedUnRegisterI_(
    const esEpaHeader_T * aEpa) {

    schedRdyRemoveI_(
        (esEpaHeader_T *)aEpa);
    rdyBitmap.epaList[aEpa->internals.kernCtrl.prio] = (esEpaHeader_T *)0U;
}

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Ispituje da li je EPA objekat u listi reda za cekanje.
 *//*--------------------------------------------------------------------------------------------*/
C_INLINE_ALWAYS bool_T schedEpaIsRdy_(
    const esEpaHeader_T * aEpa) {

    unative_T indxGroup;
    unative_T indx;

    EPE_ASSERT((esEpaHeader_T *)0U != rdyBitmap.epaList[aEpa->internals.kernCtrl.prio]);          /* Provera: da li je dati EPA prijavljen u sistem?          */
    indx = aEpa->internals.kernCtrl.prio & (~((unative_T)0U) >> (HAL_UNATIVE_BITS - PRIO_INDX_PWR));

#if (OPT_KERNEL_EPA_PRIO_MAX < HAL_UNATIVE_BITS)
    indxGroup = (unative_T)0U;
#else
    indxGroup = aEpa->internals.kernCtrl.prio >> PRIO_INDX_PWR;
#endif

    if (rdyBitmap.bit[indxGroup] & ((unative_T)1U << indx)) {

        return (TRUE);
    } else {

        return (FALSE);
    }
}

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Inicijalizacija scheduler-a
 *//*--------------------------------------------------------------------------------------------*/
static void schedInit(
    void) {

    currCtx.epa = (esEpaHeader_T *)0U;
    currCtx.prio = (esEpaPrio_T)0U;
    currCtx.status = KERNEL_STOPPED;
    schedRdyInit_();
    portSchedInit();
}

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Scheduler kernel-a
 *//*--------------------------------------------------------------------------------------------*/
void scheduleI(
    uint32_t irqLock_) {

    esEpaPrio_T newPrio;

    newPrio = schedRdyQueryI_();

    if (newPrio > currCtx.prio) {
        esEpaPrio_T savedPrio;
        esEpaHeader_T * savedEpa;

        savedPrio = currCtx.prio;
        savedEpa = currCtx.epa;

        do {
            esEvtHeader_T * newEvt;
            esEpaHeader_T * newEpa;

            currCtx.prio = newPrio;
            newEpa = rdyBitmap.epaList[newPrio];
            currCtx.epa = newEpa;
            newEvt = evtQGetI(
                newEpa);
            ES_CRITICAL_EXIT();
            hsmDispatch(
                newEpa,
                newEvt);
            ES_CRITICAL_ENTER();
            evtDestroyI_(
                newEvt);

            if (TRUE == evtQIsEmpty_(newEpa)) {
                schedRdyRemoveI_(newEpa);
            }
            newPrio = schedRdyQueryI_();
        } while (newPrio > savedPrio);
        currCtx.prio = savedPrio;
        currCtx.epa = savedEpa;
    }
}

/*-----------------------------------------------------------------------------------------------*
 *              Funkcije koje su dostupne samo ako su kernel i registar omoguceni.
 *-----------------------------------------------------------------------------------------------*/
# if defined(OPT_KERNEL_USE_REGISTRY) || defined(__DOXYGEN__)

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Dodeljuje pid broj
 * @return      Pid broj koji nije zauzet.
 *//*--------------------------------------------------------------------------------------------*/
static uint8_t regPidAllocI(
    void) {

    esDlsList_T * currNode;
    uint8_t tmpPid;
    bool_T pidFound;

    tmpPid = regSentinel.lastPid;
    pidFound = FALSE;

    do {
        ++tmpPid;

        DLS_FOR_EACH(
            &(regSentinel.activeList),
            currNode) {

            if (((regNode_T *)currNode)->pid == tmpPid) {
                pidFound = TRUE;

                break;
            }
        }
    } while (TRUE == pidFound);
    regSentinel.lastPid = tmpPid;

    return (tmpPid);
}

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Vrsi inicijalizaciju cuvara za registar bazu
 *//*--------------------------------------------------------------------------------------------*/
static void regInit(
    void) {

    esDlsSentinelInit(
        &(regSentinel.activeList));
    regSentinel.activeCnt = (uint8_t)0U;
    regSentinel.lastPid = (uint8_t)0U;
}

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Registruje dati EPA objekat u registar bazu.
 *//*--------------------------------------------------------------------------------------------*/
static void regRegisterI(
    esEpaHeader_T       * aEpa,
    const C_ROM esEpaDef_T * aDescription) {

    aEpa->internals.registry.description = aDescription;
    aEpa->internals.registry.pid = regPidAllocI();
    esDlsNodeAddHeadI(
        &regSentinel.activeList,
        &(aEpa->internals.registry.epaList));
    ++regSentinel.activeCnt;
}

/*-------------------------------------------------------------------------------------------*//**
 * @brief       Uklanja registrovani EPA objekat iz registar baze.
 *//*--------------------------------------------------------------------------------------------*/
static void regUnRegisterI(
    esEpaHeader_T       * aEpa) {

    ES_CRITICAL_DECL();

    ES_CRITICAL_ENTER();
    esDlsNodeRemoveI(
        &(aEpa->internals.registry.epaList));
    --regSentinel.activeCnt;
    ES_CRITICAL_EXIT();
    esDlsNodeInitI(
        &(aEpa->internals.registry.epaList));
}
# endif /* OPT_KERNEL_USE_REGISTRY */
#endif /* OPT_KERNEL_ENABLE */

/*==============================================================  GLOBAL FUNCTION DEFINITIONS  ==*/
/*-------------------------------------------------------------------------------------------*//**
 * @ingroup     epe_intf
 * @{ *//*---------------------------------------------------------------------------------------*/
void esEpaInit(
    esEpaHeader_T       * aEpa,
    esPtrState_T        * aStateBuff,
    esEvtHeader_T       ** aEvtBuff,
    const C_ROM esEpaDef_T * aDescription) {

#if defined(OPT_KERNEL_ENABLE)
    ES_CRITICAL_DECL();
#endif
    KERNEL_DBG_CHECK((uint8_t)OPT_KERNEL_EPA_PRIO_MAX > aDescription->epaPrio);           /* Provera par: prioritet EPA ne sme da bude veci od zada-  */
                                                                                /* te granice OPT_KERNEL_EPA_PRIO_MAX.                             */
    KERNEL_DBG_CHECK((uint8_t)0U != aDescription->epaPrio);                        /* Prioritet 0 je rezervisan.                               */
    hsmInit(
        aEpa,
        aDescription->hsmInitState,
        aStateBuff,
        aDescription->hsmStateDepth);
    evtQInit(
        aEpa,
        aEvtBuff,
        aDescription->evtQueueSize);

#if defined(OPT_KERNEL_ENABLE) || defined(__DOXYGEN__)
    aEpa->internals.kernCtrl.prio = aDescription->epaPrio;
    ES_CRITICAL_ENTER();
    schedRegisterI_(
        aEpa);

# if defined(OPT_KERNEL_USE_REGISTRY) || defined(__DOXYGEN__)
    regRegisterI(
        aEpa,
        aDescription);
# endif
    evtQPutAheadI(
        aEpa,
        (esEvtHeader_T *)&evtSignal[SIG_INIT]);
    schedRdyInsertI_(
        aEpa);
    ES_CRITICAL_EXIT();

    if (KERNEL_RUNNING == esKernelStatus()) {
        EPE_SCHED_NOTIFY_RDY();
    }
#endif
}

/*-----------------------------------------------------------------------------------------------*/
esEpaHeader_T * esEpaCreate(
    const C_ROM esMemClass_T * aMemClass,
    const C_ROM esEpaDef_T * aDescription) {

    esEpaHeader_T * newEpa;
    size_t epaSize;
    size_t stateBuff;
    size_t evtBuff;

    KERNEL_DBG_CHECK((const C_ROM esEpaDef_T *)0U != aDescription);                /* Provera par: da li je aDescription inicijalizovan?       */
    KERNEL_DBG_CHECK(sizeof(esEpaHeader_T) <= aDescription->epaMemory);            /* Provera par: zahtevana memorija se koristi za cuvanje ove*/
                                                                                /* strukture.                                               */
    epaSize = aDescription->epaMemory;
    stateBuff = epaSize + 1U;
    epaSize += hsmReqSize_(
        aDescription->hsmStateDepth);
    evtBuff = epaSize + 1U;
    epaSize += evtQReqSize_(
        aDescription->evtQueueSize);

#if defined(OPT_KERNEL_USE_DYNAMIC) || defined(__DOXYGEN__)
    KERNEL_DBG_CHECK((const C_ROM esMemClass_T *)0U != aMemClass);                 /* Provera par: da li je aMemClass inicijalizovan?          */
    newEpa = (esEpaHeader_T *)(* aMemClass->pAlloc)(epaSize);
#else
    KERNEL_DBG_CHECK(&memStaticClass != aMemClass);                                /* Provera par: da li je aMemClass ispravno inicijalizovan? */
    (void)aMemClass;
    newEpa = (esEpaHeader_T *)esHmemAlloc(epaSize);
#endif
    DBG_ASSERT((esEpaHeader_T *)0U != newEpa);
    esEpaInit(
        newEpa,
        (esPtrState_T *)((uint8_t *)newEpa + stateBuff),
        (esEvtHeader_T **)((uint8_t *)newEpa + evtBuff),
        aDescription);

#if defined(OPT_KERNEL_USE_DYNAMIC)
    newEpa->internals.memClass = aMemClass;
#endif

    return (newEpa);
}

/*-----------------------------------------------------------------------------------------------*/
/**
 * @todo        Treba da se razmotri sta sve treba da radi ova funkcija
 */
void esEpaDeInit(
    esEpaHeader_T       * aEpa) {

#if defined(OPT_KERNEL_ENABLE) || defined(__DOXYGEN__)
    ES_CRITICAL_DECL();

    ES_CRITICAL_ENTER();
    schedUnRegisterI_(
        aEpa);

# if defined(OPT_KERNEL_USE_REGISTRY) || defined(__DOXYGEN__)
    regUnRegisterI(
        aEpa);
# endif
    ES_CRITICAL_EXIT();
#endif
    evtQDeInit(
        aEpa);
    hsmDeInit(
        aEpa);
}

/*-----------------------------------------------------------------------------------------------*/
void esEpaDestroy(
    esEpaHeader_T       * aEpa) {

#if !defined(OPT_KERNEL_USE_DYNAMIC)
    (void)aEpa;                                                                 /* Skloni upozorenje o neiskoriscenom parametru.            */
    KERNEL_ASSERT_ALWAYS("Trying to delete static EPA object!");
#else
    const esMemClass_T * tmpMemClass;

    KERNEL_DBG_CHECK((esEpaHeader_T *)0U != aEpa);                                 /* Provera par: da li je aEpa inicijalizovan?               */
    tmpMemClass = aEpa->internals.memClass;
    KERNEL_DBG_CHECK(&memStaticClass != tmpMemClass);                              /* Provera par: osiguraj da ne brisemo "staticne" objekte.  */
    esEpaDeInit(
        aEpa);
    (*tmpMemClass->pDeAlloc)((void *)aEpa);
#endif
}

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @ingroup     kern_intf
 * @note        Funkcije su dostupne samo ako je kernel omogucen.
 * @{ *//*---------------------------------------------------------------------------------------*/
#if defined(OPT_KERNEL_ENABLE) || defined(__DOXYGEN__)

/*-----------------------------------------------------------------------------------------------*/
esEpaHeader_T * esEpaGetId(
    void) {

    return (currCtx.epa);
}

/*-----------------------------------------------------------------------------------------------*/
esEpaPrio_T esEpaPrioGet(
    const esEpaHeader_T * aEpa) {

    KERNEL_DBG_CHECK((const esEpaHeader_T *)0U != aEpa);                           /* Provera par: da li je aEpa inicijalizovan?               */

    return (aEpa->internals.kernCtrl.prio);
}

/*-----------------------------------------------------------------------------------------------*/
void esEpaPrioSet(
    esEpaHeader_T       * aEpa,
    esEpaPrio_T         aNewPrio) {

    ES_CRITICAL_DECL();
    bool_T state;

    ES_CRITICAL_ENTER();
    state = schedEpaIsRdy_(
        aEpa);
    schedUnRegisterI_(
        aEpa);
    aEpa->internals.kernCtrl.prio = (esEpaPrio_T)aNewPrio;
    schedRegisterI_(
        aEpa);

    if (TRUE == state) {
        schedRdyInsertI_(
            aEpa);
    }
    ES_CRITICAL_EXIT();
}

/*-----------------------------------------------------------------------------------------------*/
void esKernelInit(
    void                * aMemory,
    size_t              aMemorySize,
    size_t              aHeapSize) {

    KERNEL_DBG_CHECK((void *)0U != aMemory);                                       /* Provera par: da li je aMemory inicijalizovan?            */
    KERNEL_DBG_CHECK(aHeapSize < aMemorySize);                                     /* Provara par: heap mora da bude manji od ukupne memorije. */
    KERNEL_DBG_CHECK(sizeof(esEpaHeader_T) + sizeof(esEvtHeader_T) <= aMemorySize);    /* Provera par: mora da se preda neka minimalna kolician*/
                                                                                /* memorije. Za sada je to samo velicina strukture EPA      */
                                                                                /* objekta i velicina jednog dogadjaja.                     */
    /**
     * @todo Kada se napravi statican alokator treba da se prepravi i ova funkcija
     */
    (void)aHeapSize;
    esHmemInit(
        aMemory,
        aMemorySize);
    schedInit();

#if defined(OPT_KERNEL_USE_REGISTRY) || defined(__DOXYGEN__)
    regInit();
#endif
}

/*-----------------------------------------------------------------------------------------------*/
void esKernelStart(void) {
    currCtx.status = KERNEL_RUNNING;

    if (0U != schedRdyQueryI_()) {
        EPE_SCHED_NOTIFY_RDY();
    }
}

/*-----------------------------------------------------------------------------------------------*/
esKernelStatus_T esKernelStatus(
    void) {

    return (currCtx.status);
}

/*-----------------------------------------------------------------------------------------------*/
const C_ROM char * esKernelSysVer(
    void) {

    static const C_ROM char version[] = "1.0alpha";

    return (version);
}

/** @} *//*--------------------------------------------------------------------------------------*/

#endif /* OPT_KERNEL_ENABLE */

/*===================================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/

/** @endcond *//** @} *//*************************************************************************
 * END of kernel.c
 *************************************************************************************************/
