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
 * @brief       Implementacija Core modula, ARM Cortex-M3 port.
 * ------------------------------------------------------------------------------------------------
 * @addtogroup  core_impl
 ****************************************************************************************//** @{ */

/*============================================================================  INCLUDE FILES  ==*/
#include "../src/core/core_private.h"

/*============================================================================  LOCAL DEFINES  ==*/
/*============================================================================  LOCAL MACRO's  ==*/
/*=========================================================================  LOCAL DATA TYPES  ==*/
/*================================================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*==========================================================================  LOCAL VARIABLES  ==*/
/*=========================================================================  GLOBAL VARIABLES  ==*/
/*===============================================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*==============================================================  GLOBAL FUNCTION DEFINITIONS  ==*/
/*-----------------------------------------------------------------------------------------------*/
void portSchedInit(
    void) {

    uint32_t primask;

    primask = __get_PRIMASK();
    NVIC_SetPriority(
        PendSV_IRQn,
        (uint32_t)0xFF);                                                        /* Postavi prioritet PendSV na najnizi nivo.                */
    NVIC_SetPriority(
        SVCall_IRQn,
        (uint32_t)0xFF);                                                        /* Postavi prioritet SVCall na najnizi nivo.                */
    __set_PRIMASK(primask);
}

/*-----------------------------------------------------------------------------------------------*/
#if defined(OPT_KERNEL_ENABLE) || defined(__DOXYGEN__)
void PendSV_Handler(
    void) {

    __asm volatile(
        "   mrs     r0, basepri         \n"                                     /* Snimi BASEPRI u R0 da bi ga predali scheduleru.          */
        "   movs    r1, %0              \n"                                     /* Dobavi maksimalni sistemski prioritet.                   */
        "   msr     basepri, r1         \n"                                     /* Postavi na maksimalni sistemski prioritet.               */
        "   movs    r1, #0x01           \n"                                     /* Stvaranje konstante za xPSR registar, 1                  */
        "   lsls    r1, r1, #24         \n"                                     /* Stvaranje konstante za xPSR registar, 2                  */
        "   ldr     r2, =schedule       \n"                                     /* Dobavi adresu schedule labele.                           */
        "   push    {r1-r2}             \n"                                     /* Snimi u fiktivnom interrupt frame-u xPSR i PC.           */
        "   sub     sp, sp, #(5*4)      \n"                                     /* Ostatak frame-a nije bitan.                              */
        "   push    {r0}                \n"                                     /* Sacuvaj BASEPRI vrednost.                                */
        "   bx      lr                  \n"                                     /* Vrati se iz prekida, vratice se u schedule funkciju.     */
        "schedule:                      \n"
        "   bl      scheduleI           \n"                                     /* Pozovi schedulerI funkciju sa parametrom BASEPRI.        */
        "   movs    r1, #0              \n"                                     /* Priprema za izlazak iz kriticne sekcije.                 */
        "   msr     basepri, r1         \n"                                     /* Izlaz iz kriticne sekcije koda.                          */
        "   svc     0                   \n"                                     /* Pozovi SVCall prekid da se vrati prvobitnom EPA objektu. */
        :                                                                       /* Output lista.                                            */
        :"i"(OPT_MAX_INTERRUPT_PRIO)                                            /* Ulazna lista.                                            */
        :
    );
}
#endif

/*-----------------------------------------------------------------------------------------------*/
#if defined(OPT_KERNEL_ENABLE) || defined(__DOXYGEN__)
void SVC_Handler(
    void) {

    __asm volatile(
        "   add     sp, sp, #(8*4)      \n"                                     /* Vrati SP na pravi stack frame.                           */
        "   bx      lr                  \n"                                     /* Vrati se iz prekida.                                     */
        :
        :
        :
    );
}
#endif

/*===================================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/

/** @endcond *//** @} *//*************************************************************************
 * END of port_epe.c
 *************************************************************************************************/
