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
 * @brief       Interfejs za USART modul.
 *
 * ------------------------------------------------------------------------------------------------
 *
 * @addtogroup  hal_usart
 *
 ****************************************************************************************//** @{ */


#ifndef HAL_USART_H_
#define HAL_USART_H_


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

#include PORT_VARIANT_HEADER(hal/usart)


/*-----------------------------------------------------------------------------------------------*
 * EXTERNS
 *-----------------------------------------------------------------------------------*//** @cond */

#ifdef HAL_USART_H_VAR
#define HAL_USART_H_EXT
#else
#define HAL_USART_H_EXT extern
#endif


/** @endcond*//***********************************************************************************
 * DEFINES
 *************************************************************************************************/

/*-------------------------------------------------------------------------------------------*//**
 * @name        Smer podataka - direction konstante
 *
 * @brief       Konstante koje definisu smer kretanja podataka na Usartu.
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Usart je ulaz.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_USART_IN) || defined(__DOXYGEN__)
# define HAL_USART_IN
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Usart je izlaz.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_USART_OUT) || defined(__DOXYGEN__)
# define HAL_USART_OUT
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Usart je bidirekcioni.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_USART_INOUT) || defined(__DOXYGEN__)
# define HAL_USART_INOUT
#endif

/** @} *//*--------------------------------------------------------------------------------------*/
/*************************************************************************************************
 * MACRO's
 *************************************************************************************************/

/*-------------------------------------------------------------------------------------------*//**
 * @name        Inicijalizacija Usarta.
 *
 * @brief       Makroi za inicijalizaciju USART modula.
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Izvrsi inicijalizaciju @c uarta.
 *
 * @param       usart                   Usart koji se konfigurise,
 * @param       baudrate                Bitska brzina,
 * @param       direction               Smer podataka,
 * @param       wordlength              Duzina podatka koji se prenosi,
 * @param       stopbits                Broj ETT_STOP bitova,
 * @param       parity                  Parity kontrola ukljicena,iskljucena,
 * @param       hardvareflowcnt         Hardwerska kontrola ukljucena,iskljucena.
 *
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_USART_INIT) || defined(__DOXYGEN__)
# define HAL_USART_INIT(usart, baudrate, direction, wordlength,                 \
                        stopbits, parity, hardvareflowcnt)
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Izvrsava deinicijalizaciju uarta.
 *
 * @param       usart                   Usart koji se deinicijalizuje.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_USART_DEINIT) || defined(__DOXYGEN__)
# define HAL_USART_DEINIT(usart)
#endif

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Osnovni makroi za rad sa slanjem i primanjem podataka.
 *
 * @brief       Slanje i primanje podataka, bez ispitivanja dali je prethodno,
 *              slanje ili prijem zavrsen.
 *
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Slanje podatak @c preko usarta.
 *
 *              Pre pozivanja ovog makroa potrebno je izvrsiti inicijalizaciju
 *              hardvera.
 *
 * @param       usart                   Usart koji salje podatak,
 * @param       data                    podatak koji se salje.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_USART_SEND) || defined(__DOXYGEN__)
# define HAL_USART_SEND(usart, data)
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Primanje podatka @c preko usarta.
 *
 * @param       usart                   Usart koji prima podatke,
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_USART_RECEIVE) || defined(__DOXYGEN__)
# define HAL_USART_RECEIVE(usart)
#endif

/** @} *//*--------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*//**
 * @name        Konfiguracija usarta za interakciju sa softverom i,
 *              ostalom periferijom mikrokontrolera,
 *
 * @brief       Citanje, brisanje interapta, flagova, DMA requesta.
 * @{ *//*---------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Komanda za ukljucivanje ili iskljucivanje Usarta.
 *
 * @param       usart                   Usart nad kome se izvrsava komanda,
 * @param       command                 Komanda ENABLE,DISABLE.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_USART_CMD) || defined(__DOXYGEN__)
# define HAL_USART_CMD(usart, command)
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Konfiguracija Usart interapta.
 *
 * @param       usart                    Usart kome se setuje interapt.
 * @param       usartit                  Zeljeni interapt,
 *                                       moze imati sledece vrednosti:
 *  @arg        USART_IT_CTS - CTS change interrupt (not available for UART4 and UART5)
 *  @arg        USART_IT_LBD - LIN Break detection interrupt
 *  @arg        USART_IT_TXE - Transmit Data Register empty interrupt
 *  @arg        USART_IT_TC - Transmission complete interrupt
 *  @arg        USART_IT_RXNE - Receive Data register not empty interrupt
 *  @arg        USART_IT_IDLE - Idle line detection interrupt
 *  @arg        USART_IT_PE - Parity Error interrupt
 *  @arg        USART_IT_ERR - Error interrupt(Frame error, noise error, overrun error)
 * @param       command                     Komanda moze biti ENABLE/DISABLE.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_USART_IT_CONFIG) || defined(__DOXYGEN__)
# define HAL_USART_IT_CONFIG(usart, usartit, command)
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Proverava dali je neki od Usart flagova setovan ili ne.
 *
 * @param       usart                    Selektovani Usart.
 * @param       usartflag                Fleg koji se zeli proveriti,
 *                                       moze imati sledece vrednosti:
 *  @arg        USART_FLAG_CTS - CTS Change flag (not available for UART4 and UART5)
 *  @arg        USART_FLAG_LBD - LIN Break detection flag
 *  @arg        USART_FLAG_TXE - Transmit data register empty flag
 *  @arg        USART_FLAG_TC - Transmission Complete flag
 *  @arg        USART_FLAG_RXNE - Receive data register not empty flag
 *  @arg        USART_FLAG_IDLE - Idle Line detection flag
 *  @arg        USART_FLAG_ORE - OverRun Error flag
 *  @arg        USART_FLAG_NE - Noise Error flag
 *  @arg        USART_FLAG_FE - Framing Error flag
 *  @arg        USART_FLAG_PE - Parity Error flag
 * @return      Vrednost markera.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_USART_GET_FLAG) || defined(__DOXYGEN__)
# define HAL_USART_GET_FLAG(usart, usartflag)
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Brisanje USART markera.
 *
 * @param       usart                    Usart kome se brise skup markera,
 * @param       usartflag                markeri mogu biti sledeci:
 *  @arg        USART_FLAG_CTS - CTS Change flag (not available for UART4 and UART5).
 *  @arg        USART_FLAG_LBD - LIN Break detection flag.
 *  @arg        USART_FLAG_TC - Transmission Complete flag.
 *  @arg        USART_FLAG_RXNE - Receive data register not empty flag.
 *
 * @note
 *   - PE (Parity error), FE (Framing error), NE (Noise error), ORE (OverRun
 *     error) and IDLE (Idle line detected) flags are cleared by software
 *     sequence: a read operation to USART_SR register (USART_GetFlagStatus())
 *     followed by a read operation to USART_DR register (USART_ReceiveData()).
 *   - RXNE flag can be also cleared by a read to the USART_DR register
 *     (USART_ReceiveData()).
 *   - TC flag can be also cleared by software sequence: a read operation to
 *     USART_SR register (USART_GetFlagStatus()) followed by a write operation
 *     to USART_DR register (USART_SendData()).
 *   - TXE flag is cleared only by a write to the USART_DR
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_USART_CLEAR_FLAG) || defined(__DOXYGEN__)
# define HAL_USART_CLEAR_FLAG(usart, usartflag)
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Proverava dali se dati interapt dogodio ili ne.
 *
 * @param       usart                    Usart kome se proverava interapt.
 * @param       usartit                  Zeljeni interapt,
 *  @arg        USART_FLAG_CTS - CTS Change flag (not available for UART4 and UART5)
 *  @arg        USART_FLAG_LBD - LIN Break detection flag
 *  @arg        USART_FLAG_TXE - Transmit data register empty flag
 *  @arg        USART_FLAG_TC -  Transmission Complete flag
 *  @arg        USART_FLAG_RXNE -Receive data register not empty flag
 *  @arg        USART_FLAG_IDLE -Idle Line detection flag
 *  @arg        USART_FLAG_ORE - OverRun Error flag
 *  @arg        USART_FLAG_NE -  Noise Error flag
 *  @arg        USART_FLAG_FE -  Framing Error flag
 *  @arg        USART_FLAG_PE -  Parity Error flag
 * @return      value                   vrednost it flega.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_USART_GET_IT_STATUS) || defined(__DOXYGEN__)
# define HAL_USART_GET_IT_STATUS(usart, usartit)
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Brisanje interapt flega.
 *
 * @param       usart                    Usart kome se setuje interapt.
 * @param       usartit                  Zeljeni interapt fleg,
 *  @arg        USART_IT_CTS - CTS change interrupt (not available for UART4 and UART5)
 *  @arg        USART_IT_LBD - LIN Break detection interrupt
 *  @arg        USART_IT_TC - Transmission complete interrupt.
 *  @arg        USART_IT_RXNE - Receive Data register not empty interrupt.
 * @note
 *   - PE (Parity error), FE (Framing error), NE (Noise error), ORE (OverRun
 *     error) and IDLE (Idle line detected) pending bits are cleared by
 *     software sequence: a read operation to USART_SR register
 *     (USART_GetITStatus()) followed by a read operation to USART_DR register
 *     (USART_ReceiveData()).
 *   - RXNE pending bit can be also cleared by a read to the USART_DR register
 *     (USART_ReceiveData()).
 *   - TC pending bit can be also cleared by software sequence: a read
 *     operation to USART_SR register (USART_GetITStatus()) followed by a write
 *     operation to USART_DR register (USART_SendData()).
 *   - TXE pending bit is cleared only by a write to the USART_DR register
 *     (USART_SendData()).
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_USART_CLEAR_IT_PENDING_BIT) || defined(__DOXYGEN__)
# define HAL_USART_CLEAR_IT_PENDING_BIT(usart, usartit)
#endif
/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Setuje IT pending bit.
 *
 * @param       usart                    Usart kome se setuje it pending bit.
 * @param       usartit                  Zeljeni interapt,
 *  @arg        USART_FLAG_CTS - CTS Change flag (not available for UART4 and UART5)
 *  @arg        USART_FLAG_LBD - LIN Break detection flag
 *  @arg        USART_FLAG_TXE - Transmit data register empty flag
 *  @arg        USART_FLAG_TC - Transmission Complete flag
 *  @arg        USART_FLAG_RXNE - Receive data register not empty flag
 *  @arg        USART_FLAG_IDLE - Idle Line detection flag
 *  @arg        USART_FLAG_ORE -  OverRun Error flag
 *  @arg        USART_FLAG_NE -   Noise Error flag
 *  @arg        USART_FLAG_FE -   Framing Error flag
 *  @arg        USART_FLAG_PE -   Parity Error flag
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_USART_SET_IT_PENDING_BIT) || defined(__DOXYGEN__)
# define HAL_USART_SET_IT_PENDING_BIT(usart, usartit)
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Ukljucivanje ili iskljucivanje UART-DMA interfejsa.
 *
 * @param       usart                   Selektovani Uart,
 * @param       dmarequest              DMA zahtev, moze biti bilo koja
 *  @arg        USART_DMAReq_Tx - USART DMA transmit request
 *  @arg        USART_DMAReq_Rx - USART DMA receive request
 * @param       command                 Moze biti ENABLE,DISABLE
 * @note        The DMA mode is not available for UART5 except in the STM32
 *              High density value line devices(STM32F10X_HD_VL).
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_USART_DMA_CMD) || defined(__DOXYGEN__)
# define HAL_USART_DMA_CMD(usart, dmarequest, command)
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
 * END of hal_usart.h
 *************************************************************************************************/
#endif /* HAL_USART_H_ */
