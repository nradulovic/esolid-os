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
 * @author      nesa
 *
 * @brief       Privatni objekti SMP podmodula.
 *
 * ------------------------------------------------------------------------------------------------
 *
 * @addtogroup  smp_impl
 *
 ****************************************************************************************//** @{ */

#ifndef SMP_PRIVATE_H_
#define SMP_PRIVATE_H_


/*************************************************************************************************
 * INCLUDE FILES
 *************************************************************************************************/


/*-----------------------------------------------------------------------------------------------*
 * EXTERNS
 *-----------------------------------------------------------------------------------*//** @cond */

#ifdef SMP_PRIVATE_H_VAR
#define SMP_PRIVATE_H_EXT
#else
#define SMP_PRIVATE_H_EXT extern
#endif


/** @endcond*//***********************************************************************************
 * DEFINES
 *************************************************************************************************/


/*************************************************************************************************
 * MACRO's
 *************************************************************************************************/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Posalji predefinisan dogadjaj @c evt automatu @c hsm.
 *
 * @param       hsm                     Pokazivac na strukturu automata,
 * @param       state                   pokazivac na funkciju stanja,
 * @param       evt                     redni broj (enumerator) rezervisanog
 *                                      dogadjaj.
 */
/*-----------------------------------------------------------------------------------------------*/
#define SMP_RSRVEVT_SEND(hsm, state, evt)                                       \
    (* state)((hsm), (eot_evt_T *)&smp_controlEvt[evt])

/*-------------------------------------------------------------------------------------------*//**
 * @name        Debug podrska
 *
 * @brief       Makroi za proveru validnosti internih podataka
 *
 * @{ *//*---------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Izvrsi validaciju odgovora stanja funkcije stanja HSM automata.
 */
/*-----------------------------------------------------------------------------------------------*/
#define HSM_VALID_STATE(expr)                                                   \
    (RETN_SUPER == (expr))   ||                                                 \
    (RETN_TRAN == (expr))    ||                                                 \
    (RETN_HANDLED == (expr)) ||                                                 \
    (RETN_NOEX == (expr)) ||                                                    \
    (RETN_DEFERRED == (expr))||                                                 \
    (RETN_IGNORED == (expr))

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Izvrsi validaciju odgovora stanja funkcije stanja FSM automata.
 */
/*-----------------------------------------------------------------------------------------------*/
#define FSM_VALID_STATE(expr)                                                   \
    (RETN_TRAN == (expr))    ||                                                 \
    (RETN_HANDLED == (expr)) ||                                                 \
    (RETN_NOEX == (expr)) ||                                                    \
    (RETN_IGNORED == (expr))

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Debug podrska
 *
 * @brief       Makroi za debug podrsku. Pogledati @ref dbg_intf.
 *
 * @{ *//*---------------------------------------------------------------------------------------*/

#if defined(OPT_DBG_SMP)
# define SMP_ASSERT                     DBG_ASSERT
# define SMP_ASSERT_ALWAYS              DBG_ASSERT_ALWAYS
# define SMP_COMPILE_ASSERT             DBG_COMPILE_ASSERT
# define SMP_DBG_DECL                   DBG_DECL
# define SMP_DBG_DEFINE_MODULE          DBG_DEFINE_MODULE
# define SMP_DBG_ENTRY                  DBG_ENTRY
# define SMP_DBG_EXIT                   DBG_EXIT
# define SMP_DBG_MACRO                  DBG_MACRO
# define SMP_DBG_CHECK                  DBG_CHECK
#else
# define SMP_ASSERT(expr)               DBG_EMPTY_MACRO()
# define SMP_ASSERT_ALWAYS(expr)        DBG_EMPTY_MACRO()
# define SMP_COMPILE_ASSERT(expr)       DBG_EMPTY_DECL()
# define SMP_DBG_DECL(expr)             DBG_EMPTY_DECL()
# define SMP_DBG_DEFINE_MODULE(expr)    DBG_EMPTY_DECL()
# define SMP_DBG_ENTRY()                DBG_EMPTY_MACRO()
# define SMP_DBG_EXIT()                 DBG_EMPTY_MACRO()
# define SMP_DBG_MACRO(expr)            DBG_EMPTY_MACRO()
# define SMP_DBG_CHECK(expr)            DBG_EMPTY_MACRO()
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

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Rezervisani prosireni dogadjaji.
 *
 *              Ovaj niz cuva strukture prosirene reprezentacije
 *              (@ref eot_evtExt_T) dogadjaja i inicijalizuje se na rezervisane
 *              dogadjaje koji su nabrojeni u @ref smp_evtID. Inicijalizacija se
 *              vrsi pomocu funkcije eSsmp_rsrvEvtInit(). Ove rezervisane
 *              dogadjaje dispecer salje automatima kako bi se opsluzio dogadjaj.
 *
 * @note        Niz rezervisanih dogadjaja se koristi samo kada su ukljuceni
 *              prosireni dogadjaji.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(ES_OPTIMIZE_SPEED) && (PORT_SUPP_IDENTIC_ROM_RAM == PORT_TRUE)
extern const C_ROM eot_evt_T smp_controlEvt[];
#else
extern eot_evt_T smp_controlEvt[];
#endif


/*************************************************************************************************
 * FUNCTION PROTOTYPES
 *************************************************************************************************/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Konstruise HSM automat
 *
 * @param       aMemClass               Klasa memorije koja se koristi za
 *                                      skladistenje,
 *  @arg        memHeapClass
 *  @arg        memPoolClass
 *  @arg        memStaticClass
 * @param       aHsm                    pokazivac na tek kreiran HSM automat,
 * @param       aInitState              inicijalno stanje automata,
 * @param       aStateDepth             maksimalna hijerarhijska dubina stanja
 *                                      automata.
 * @note        Ne vrsi se verifikacija aMemClass i aHsm argumenta.
 */
/*-----------------------------------------------------------------------------------------------*/
void smp_hsmCtor(
    const C_ROM smm_memClass_T  * aMemClass,
    smp_exec_T      * aHsm,
    smp_ptrState_T  aInitState,
    uint8_t         aStateDepth);

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Dekonstruise HSM automat
 *
 * @param       aMemClass               Klasa memorije koja se koristi za
 *                                      skladistenje,
 *  @arg        memHeapClass
 *  @arg        memPoolClass
 *  @arg        memStaticClass
 * @param       aHsm                    pokazivac na kreiran HSM automat.
 * @note        Ne vrsi se verifikacija aMemClass i aHsm argumenta.
 */
/*-----------------------------------------------------------------------------------------------*/
void smp_hsmDtor(
    const C_ROM smm_memClass_T  * aMemClass,
    smp_exec_T      * aHsm);


/*-----------------------------------------------------------------------------------------------*
 * C/C++ #endif - close
 *-----------------------------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif


/*************************************************************************************************
 * CONFIGURATION ERRORS
 *************************************************************************************//** @cond */

#if defined(OPT_SMP_FVAR_STATIC)
# define SMP_FVAR                       static
#else
# define SMP_FVAR                       auto
#endif


/** @endcond *//** @} *//*************************************************************************
 * END of smp_pkg.h
 *************************************************************************************************/
#endif /* SMP_PRIVATE_H_ */
