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
 * @brief       Interfejs za USART modul STM32F10x Low Density Value Line serija,
 *              port klasa: varijanta.
 *
 * ------------------------------------------------------------------------------------------------
 *
 * @addtogroup  hal_usart
 *
 ****************************************************************************************//** @{ */


#ifndef VARNT_USART_H_
#define VARNT_USART_H_


/*************************************************************************************************
 * INCLUDE FILES
 *************************************************************************************************/

/*-----------------------------------------------------------------------------------------------*
 * Module dependencies
 *-----------------------------------------------------------------------------------------------*/

#include "hal/hal_compiler.h"
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"


/*-----------------------------------------------------------------------------------------------*
 * Module configuration and adapter
 *-----------------------------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------------------------*
 * EXTERNS
 *-----------------------------------------------------------------------------------*//** @cond */


#ifdef VARNT_USART_H_VAR
#define VARNT_USART_H_EXT
#else
#define VARNT_USART_H_EXT extern
#endif



/** @endcond*//***********************************************************************************
 * DEFINES
 *************************************************************************************************/

/*-------------------------------------------------------------------------------------------*//**
 * @name        Izbor periferije - konstante za izbor periferije
 *
 * @brief       Konstante koje definisu izbor Usart periferije.
 * @{ *//*---------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Usart 1 periferija.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_USART1) || defined(__DOXYGEN__)
# define HAL_USART1                     0x00
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Usart 2 periferija.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_USART2) || defined(__DOXYGEN__)
# define HAL_USART2                     0x01
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Usart 1 periferija.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_USART3) || defined(__DOXYGEN__)
# define HAL_USART3                     0x02
#endif

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
# define HAL_USART_IN                   USART_Mode_Rx
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Usart je izlaz.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_USART_OUT) || defined(__DOXYGEN__)
# define HAL_USART_OUT                  USART_Mode_Tx
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Usart je bidirekcioni.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_USART_INOUT) || defined(__DOXYGEN__)
# define HAL_USART_INOUT                0xC                                     /* (USART_Mode_Rx | USART_Mode_Tx) */
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
 * @param       usart                   Usart koji se konfigurise
 * @param       baudrate                Bitska brzina,
 * @param       direction               Smer podataka,
 * @param       wordlength              Duzina podatka koji se prenosi,
 * @param       stopbits                Broj ETT_STOP bitova,
 * @param       parity                  Parity kontrola ukljicena/iskljucena,
 * @param       hardvareflowcnt         Hardwerska kontrola ukljucena/iskljucena.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_USART_INIT) || defined(__DOXYGEN__)
# define HAL_USART_INIT(usart, baudrate, direction, wordlength, stopbits, parity, hardvareflowcnt) \
    do{ USART_InitTypeDef USART_InitStruct;                                     \
        if(usart != HAL_USART1) {                                               \
            RCC_APB1PeriphClockCmd(RCC_USART_APBPeriphLkUp[usart], ENABLE);     \
        } else {                                                                \
            RCC_APB2PeriphClockCmd(RCC_USART_APBPeriphLkUp[usart], ENABLE);     \
        }                                                                       \
        HAL_GPIO_INIT(                                                          \
                      HAL_USART_IO_portLkUp[usart],                             \
                      HAL_USART_IO_txPinsLkUp[usart],                           \
                      HAL_GPIO_AF,                                              \
                      HAL_GPIO_PP,                                              \
                      0,                                                        \
                      GPIO_Speed_50MHz);                                        \
        HAL_GPIO_INIT(                                                          \
                      HAL_USART_IO_portLkUp[usart],                             \
                      HAL_USART_IO_rxPinsLkUp[usart],                           \
                      HAL_GPIO_IN,                                              \
                      0,                                                        \
                      HAL_GPIO_NP,                                              \
                      GPIO_Speed_50MHz);                                        \
        USART_InitStruct.USART_BaudRate = baudrate;                             \
        USART_InitStruct.USART_WordLength = wordlength;                         \
        USART_InitStruct.USART_StopBits = stopbits;                             \
        USART_InitStruct.USART_Parity = parity;                                 \
        USART_InitStruct.USART_Mode = direction;                                \
        USART_InitStruct.USART_HardwareFlowControl = hardvareflowcnt;           \
        USART_Init((USART_TypeDef *)usartLkUp[usart], &USART_InitStruct);       \
    } while(0)

#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Izvrsava deinicijalizaciju uarta.
 *
 * @param       uart                    Uart koji se deinicijalizuje.
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
# define HAL_USART_SEND(usart, data)                                            \
    ((USART_TypeDef *)usartLkUp[usart])->DR = (data & (uint16_t)0x01FF)

#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Primanje podatka @c preko usarta.
 *
 * @param       usart                   Usart koji prima podatke,
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_USART_RECEIVE) || defined(__DOXYGEN__)
# define HAL_USART_RECEIVE(usart)                                               \
    (uint8_t)(((USART_TypeDef *)usartLkUp[usart])->DR & (uint16_t)0x01FF)
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
 * @param       command                 Komanda ENABLE/DISABLE.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_USART_CMD) || defined(__DOXYGEN__)
# define HAL_USART_CMD(usart, command)                                          \
     USART_Cmd((USART_TypeDef *)usartLkUp[usart], command)
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Konfiguracija Usart interapta.
 *
 * @param       usart                    Usart kome se setuje interapt.
 * @param       usartit                  Zeljeni interapt,
 *                                       moze imati sledece vrednosti:
 *        @arg USART_IT_CTS:  CTS change interrupt (not available for UART4 and UART5)
 *        @arg USART_IT_LBD:  LIN Break detection interrupt
 *        @arg USART_IT_TXE:  Transmit Data Register empty interrupt
 *        @arg USART_IT_TC:   Transmission complete interrupt
 *        @arg USART_IT_RXNE: Receive Data register not empty interrupt
 *        @arg USART_IT_IDLE: Idle line detection interrupt
 *        @arg USART_IT_PE:   Parity Error interrupt
 *        @arg USART_IT_ERR:  Error interrupt(Frame error, noise error, overrun error)
 * @param       itpriority                  Interupt priority
 *        @arg 1 to 255, 1 The Higest prority 255 the Lovest priority.
 * @param       command                     Komanda moze biti ENABLE/DISABLE.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_USART_IT_CONFIG) || defined(__DOXYGEN__)
# define HAL_USART_IT_CONFIG(usart, usartit, itpriority, command)               \
    do{                                                                         \
        NVIC_EnableIRQ((IRQn_Type)HAL_USART_NVIC_IRQ_ChanelLkUp[usart]);        \
        NVIC_SetPriority((IRQn_Type)HAL_USART_NVIC_IRQ_ChanelLkUp[usart],       \
                        (uint32_t)itpriority);                                  \
        USART_ITConfig(((USART_TypeDef *)usartLkUp[usart]), usartit, command);  \
    }while(0)
#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Proverava dali je neki od Usart flagova setovan ili ne.
 *
 * @param       usart                    Selektovani Usart.
 * @param       usartflag                Fleg koji se zeli proveriti,
 *                                       moze imati sledece vrednosti:
 *        @arg USART_FLAG_CTS:  CTS Change flag (not available for UART4 and UART5)
 *        @arg USART_FLAG_LBD:  LIN Break detection flag
 *        @arg USART_FLAG_TXE:  Transmit data register empty flag
 *        @arg USART_FLAG_TC:   Transmission Complete flag
 *        @arg USART_FLAG_RXNE: Receive data register not empty flag
 *        @arg USART_FLAG_IDLE: Idle Line detection flag
 *        @arg USART_FLAG_ORE:  OverRun Error flag
 *        @arg USART_FLAG_NE:   Noise Error flag
 *        @arg USART_FLAG_FE:   Framing Error flag
 *        @arg USART_FLAG_PE:   Parity Error flag
 * @return      Vrednost flega.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_USART_GET_FLAG) || defined(__DOXYGEN__)
# define HAL_USART_GET_FLAG(usart, usartflag)                                   \
    (FlagStatus) USART_GetFlagStatus(((USART_TypeDef *)usartLkUp[usart]),       \
    usartflag);

#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Brisanje Usart flegova.
 *
 * @param       usart                    Usart kome se brise fleg ili skup flegova.
 * @param       usartflag                Zeljeni fleg ili skup flegova,
 *                                       mogu biti sledeci flegovi:
 *        @arg USART_FLAG_CTS:  CTS Change flag (not available for UART4 and UART5).
 *        @arg USART_FLAG_LBD:  LIN Break detection flag.
 *        @arg USART_FLAG_TC:   Transmission Complete flag.
 *        @arg USART_FLAG_RXNE: Receive data register not empty flag.
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
# define HAL_USART_CLEAR_FLAG(usart, usartflag)                                 \
   ((USART_TypeDef *)usartLkUp[usart])->SR = (uint16_t)~                        \
   ((USART_TypeDef *)usartLkUp[usart]);

#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Proverava dali se dati interapt dogodio ili ne.
 *
 * @param       usart                    Usart kome se proverava interapt.
 * @param       usartit                  Zeljeni interapt,
 *        @arg USART_FLAG_CTS:  CTS Change flag (not available for UART4 and UART5)
 *        @arg USART_FLAG_LBD:  LIN Break detection flag
 *        @arg USART_FLAG_TXE:  Transmit data register empty flag
 *        @arg USART_FLAG_TC:   Transmission Complete flag
 *        @arg USART_FLAG_RXNE: Receive data register not empty flag
 *        @arg USART_FLAG_IDLE: Idle Line detection flag
 *        @arg USART_FLAG_ORE:  OverRun Error flag
 *        @arg USART_FLAG_NE:   Noise Error flag
 *        @arg USART_FLAG_FE:   Framing Error flag
 *        @arg USART_FLAG_PE:   Parity Error flag

 * @return       value                   vrednost it flega.
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_USART_GET_IT_STATUS) || defined(__DOXYGEN__)
# define HAL_USART_GET_IT_STATUS(usart, usartit)                                \
    (ITStatus) HAL_USART_GetItPendingBitStatus(usart, usartit)

#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Brisanje interapt flega.
 *
 * @param       usart                    Usart kome se setuje interapt.
 * @param       usartit                  Zeljeni interapt fleg,
 *        @arg USART_IT_CTS:  CTS change interrupt (not available for UART4 and UART5)
 *        @arg USART_IT_LBD:  LIN Break detection interrupt
 *        @arg USART_IT_TC:   Transmission complete interrupt.
 *        @arg USART_IT_RXNE: Receive Data register not empty interrupt.
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
# define HAL_USART_CLEAR_IT_PENDING_BIT(usart, usartit)                         \
    do{                                                                         \
       uint16_t bitpos = 0x00, itmask = 0x00;                                   \
       bitpos = usartit >> 0x08;                                                \
       itmask = ((uint16_t)0x01 << (uint16_t)bitpos);                           \
       ((USART_TypeDef *)usartLkUp[usart])->SR = (uint16_t)~itmask;             \
       softwareItPendingMaskLkUp[usart] = (uint32_t)0x0;                        \
    }while(0)

#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Setuje IT pending bit.
 *
 * @param       usart                    Usart kome se setuje it pending bit.
 * @param       usartit                  Zeljeni interapt,
 *        @arg USART_FLAG_CTS:  CTS Change flag (not available for UART4 and UART5)
 *        @arg USART_FLAG_LBD:  LIN Break detection flag
 *        @arg USART_FLAG_TXE:  Transmit data register empty flag
 *        @arg USART_FLAG_TC:   Transmission Complete flag
 *        @arg USART_FLAG_RXNE: Receive data register not empty flag
 *        @arg USART_FLAG_IDLE: Idle Line detection flag
 *        @arg USART_FLAG_ORE:  OverRun Error flag
 *        @arg USART_FLAG_NE:   Noise Error flag
 *        @arg USART_FLAG_FE:   Framing Error flag
 *        @arg USART_FLAG_PE:   Parity Error flag
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_USART_SET_IT_PENDING_BIT) || defined(__DOXYGEN__)
# define HAL_USART_SET_IT_PENDING_BIT(usart, usartit)                           \
    softwareItPendingMaskLkUp[usart] = usartit;                                 \
    NVIC_SetPendingIRQ(HAL_USART_NVIC_IRQ_ChanelLkUp[usart]);

#endif

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Ukljucivanje ili iskljucivanje UART-DMA interfejsa.
 *
 * @param       usart                   Selektovani Uart,
 * @param       dmarequest              DMA zahtev, moze biti bilo koja
 *                                      kombinacija sledecih vrednosti:
 *        @arg USART_DMAReq_Tx: USART DMA transmit request
 *        @arg USART_DMAReq_Rx: USART DMA receive request
 * @param       command                 Moze biti ENABLE/DISABLE
 * @note The DMA mode is not available for UART5 except in the STM32
 *       High density value line devices(STM32F10X_HD_VL).
 */
/*-----------------------------------------------------------------------------------------------*/
#if !defined(HAL_USART_DMA_CMD) || defined(__DOXYGEN__)
# define HAL_USART_DMA_CMD(usart, dmarequest, command)                          \
    do{                                                                         \
        if (NewState != DISABLE) {                                              \
            ((USART_TypeDef *)usartLkUp[usart])->CR3 |= USART_DMAReq;           \
   } else {                                                                     \
       ((USART_TypeDef *)usartLkUp[usart])->CR3 &= (uint16_t)~USART_DMAReq;     \
   }

#endif

/**
 *  @brief Vrti se u petlji sve dok taskflag ne postane jedinica.
 */
#if !defined(HAL_USART_WAIT_FOR_LAST_TASK) || defined(__DOXYGEN__)
# define HAL_USART_WAIT_FOR_LAST_TASK(usart, taskflag)                          \
    while (USART_GetFlagStatus(((USART_TypeDef *)usartLkUp[usart]),             \
        taskflag) == RESET) {                                                   \
        ;                                                                       \
    }

#endif

/** @} *//*--------------------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------------------------*
 * C/C++ #ifdef - open
 *-----------------------------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

/*************************************************************************************************
 * DATA TYPES
 *************************************************************************************************/


/*************************************************************************************************
 * GLOBAL VARIABLES
 *************************************************************************************************/

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Niz za konverziju rednog broja usarta u njegov pokazivac koji se
 *              predaje funkcijama.
 */
/*-----------------------------------------------------------------------------------------------*/
extern const uint32_t usartLkUp[];

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Niz za konverziju rednog broja Usarta u odgovarajuci RCC registar.
 *
 */
/*-----------------------------------------------------------------------------------------------*/
extern const uint32_t RCC_USART_APBPeriphLkUp[];

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Niz za selektovasnje Usart Tx pinova za dati Usart,
 *              predaje se funkciji za inicijalizaciju pinova.
 */
/*-----------------------------------------------------------------------------------------------*/
extern const uint32_t HAL_USART_IO_txPinsLkUp[];

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Niz za selektovasnje Usart Rx pinova za dati Usart,
 *              predaje se funkciji za inicijalizaciju pinova.
 */
/*-----------------------------------------------------------------------------------------------*/
extern const uint32_t HAL_USART_IO_rxPinsLkUp[];

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Niz za selektovasnje pripadajuceg GPIO porta za dati Usart,
 *              predaje se funkciji za inicijalizaciju pinova.
 */
/*-----------------------------------------------------------------------------------------------*/
extern const uint8_t HAL_USART_IO_portLkUp[];

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Niz za selektovasnje pripadajuceg Interapt kanala IRQn
 *              za dati Usart, predaje se funkciji za konfiguraciju interapta.
 */
/*-----------------------------------------------------------------------------------------------*/
extern const uint32_t HAL_USART_NVIC_IRQ_ChanelLkUp[];

/*-----------------------------------------------------------------------------------------------*/
/**
 * @brief       Niz za softversko maskiranje (trigerovanje) datih Usart
 *              interapt pending flegova.
 */
/*-----------------------------------------------------------------------------------------------*/
extern volatile uint16_t softwareItPendingMaskLkUp[];


/*************************************************************************************************
 * FUNCTION PROTOTYPES
 *************************************************************************************************/

ITStatus HAL_USART_GetItPendingBitStatus(uint16_t usart, uint16_t usartItFlag);


/*************************************************************************************************
 * CONFIGURATION ERRORS
 *************************************************************************************//** @cond */


/** @endcond *//** @} *//*************************************************************************
 * END of usart.h
 *************************************************************************************************/
#endif /* VARNT_USART_H_ */
