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
 * @brief       Konfiguracija eSolid Sistema
 * @defgroup    sys_cfg System configuration
 *********************************************************************//** @{ */

#ifndef SYS_CONFIG_H_
#define SYS_CONFIG_H_

/*=========================================================  INCLUDE FILES  ==*/
/*===============================================================  DEFINES  ==*/
/** @cond */
/*
 * U ovom segmentu (DEFINES) treba definisati svoje opcije, primer:
 *
 * #define OPT_SYS_INTERRUPT_PRIO_MAX   15U
 *
 * Kada se opcije definisu u segmentu DEFINES, podrazumevane opcije ce postati
 * neaktivne. Preporucuje se da se donji kod ne modifikuje na bilo kakav nacin
 * jer to moze dovesti do lose konfiguracije eSolid-a. Donji kod se koristi samo
 * kao polazna tacka ili kao primer kako treba definisati, odnosno,
 * konfigurisati opcije.
 */

/** @endcond */
/*==============================================================  SETTINGS  ==*/

/*------------------------------------------------------------------------*//**
 * @name        Zastita kriticnih sekcija koda
 * @brief       Ovim makroima se stite kriticne sekcije koda
 * @details     Koriscenjem ovih makroa moze da se specificira da se prilikom
 *              pristupa deljenom resursu prvo zakljuca pristup na odredjeni
 *              nacin. To se moze vrsiti na nekoliko nacina:
 *              - gasenje prekida,
 *              - podizanje prioriteta scheduler-a,
 *
 * @p           Za zastitu kriticne sekcije koda se koriste interni mehanizmi
 *              eSolid-a ili RTOS funkcije. Ukoliko se koristi RTOS onda je
 *              moguce koristiti njegove mehanizme za zastitu kriticnih sekcija.
 *              Da bi se koristile RTOS funkcije potrebno je ukljuciti opciju
 *              @ref OPT_CRITICAL_EXTERN i definisati sve ostale
 *              OPT_CRITICAL_... makroe.
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Maksimalan prioritet kriticnih sekcija koda
 * @details     Ovim se ogranicava prioritet kriticnih sekcija koda koje jezgro
 *              koristi. Ovo podesavanje je izuzetno korisno kada postoji
 *              potreba da se pojedini hardverski prekidi ne prekidaju od strane
 *              jezgra.
 * @pre         Ova opcija je validna samo ako se koristi interni mehanizam
 *              eSolid-a, odnosno, ako je opcija @ref OPT_CRITICAL_EXTERN
 *              iskljucena.
 * @note        Podrazumevano podesavanje: ES_PRIO_REALTIME
 */
#if !defined(OPT_SYS_INTERRUPT_PRIO_MAX)
# define OPT_SYS_INTERRUPT_PRIO_MAX     ES_PRIO_REALTIME
#endif

/**
 * @brief       Ukljucivanje eksternog cuvara kriticne sekcije koda
 * @details     Podrazumevano ova opcija nije ukljucena i u tom slucaju eSolid
 *              koristi interne mehanizme za zastitu kriticnih sekcija koda.
 *              Interni mehanizmi za zastitu su zabrana prekida ili maskiranje
 *              prekida.
 *
 *              Moguce vrednosti:
 *              - 0 - koristi se eSolid cuvar
 *              - 1 - koristi se eksterni cuvar
 *
 * @note        Podrazumevano podesavanje: 0 (koristi se eSolid cuvar)
 */
#if !defined(OPT_CRITICAL_EXTERN) || defined(__DOXYGEN__)
# define OPT_CRITICAL_EXTERN            0U
#endif

#if (0U == OPT_CRITICAL_EXTERN) || defined(__DOXYGEN__)

/**
 * @brief       Deklaracija @c auto promenljive za @c CRITICAL makroe
 * @details     Koristi se samo ako je ostalim makroima potrebna @c auto
 *              promenljiva.
 */
# define OPT_CRITICAL_DECL()            ES_CRITICAL_DECL()

/**
 * @brief       Zakljucavanje pristupa
 */
# define OPT_CRITICAL_LOCK()            ES_CRITICAL_ENTER(OPT_SYS_INTERRUPT_PRIO_MAX)


/**
 * @brief       Otkljucavanje pristupa
 */
# define OPT_CRITICAL_UNLOCK()          ES_CRITICAL_EXIT()
#endif

/** @} *//*-------------------------------------------------------------------*/
/*------------------------------------------------------------------------*//**
 * @name        Zastita od istovremenog pristupa deljenom resursu
 * @brief       Ovim makroima se sprecava istovremeni pristup deljenom resursu
 * @details     Koriscenjem ovih makroa moze da se specificira da se prilikom
 *              pristupa deljenom resursu prvo zakljuca pristup na odredjeni
 *              nacin. To se moze vrsiti na nekoliko nacina:
 *              - gasenje prekida,
 *              - podizanje prioriteta scheduler-a,
 *              - aktiviranjem mutex-a
 *              - aktiviranjem binarnog semaphore-a.
 *
 * @p           Za zastitu od istovremenog pristupa deljenom resursu se koriste
 *              interni mehanizmi eSolid-a ili RTOS funkcije. Ukoliko se koristi
 *              RTOS onda je moguce koristiti njegove mehanizme za
 *              sinhronizaciju. Da bi se koristile RTOS funkcije potrebno je
 *              ukljuciti opciju @ref OPT_GUARD_EXTERN i definisati sve ostale
 *              OPT_GUARD_... makroe.
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Ukljucivanje eksternog cuvara deljenih resursa
 * @details     Podrazumevano ova opcija nije ukljucena i u tom slucaju eSolid
 *              koristi interne mehanizme za zastitu deljenih resursa. Interni
 *              mehanizmi za zastitu su zabrana prekida ili maskiranje prekida.
 *
 *              Moguce vrednosti:
 *              - 0 - koristi se eSolid cuvar
 *              - 1 - koristi se eksterni cuvar
 *
 * @note        Podrazumevano podesavanje: 0 (koristi se eSolid cuvar)
 */
#if !defined(OPT_GUARD_EXTERN) || defined(__DOXYGEN__)
# define OPT_GUARD_EXTERN               0U
#endif

#if (0U == OPT_GUARD_EXTERN) || defined(__DOXYGEN__)
/**
 * @brief       Tip podataka za zastitu deljenog resursa
 * @details     Ovde treba postaviti tip cuvara resursa, kao na primer struktura
 *              mutex-a ili semaphore-a kada se koristi neki RTOS.
 */
# if defined(__DOXYGEN__)
#  define GUARD_T
# endif

/**
 * @brief       Deklaracija @c auto promenljive za @c GUARD makroe
 * @details     Koristi se samo ako je ostalim makroima potrebna @c auto
 *              promenljiva.
 */
# define OPT_GUARD_DECL()               ES_CRITICAL_DECL()

/**
 * @brief       Inicijalizacija cuvara deljenog resursa
 * @details     Ovde treba postaviti funkciju koja vrsi inicijalizaciju cuvara
 *              resursa. Ona se poziva u ...Init() funkcijama.
 */
# define OPT_GUARD_INIT(guard)          (void)guard

/**
 * @brief       Zakljucavanje pristupa
 */
# define OPT_GUARD_LOCK(guard)          ES_CRITICAL_ENTER(OPT_SYS_INTERRUPT_PRIO_MAX)


/**
 * @brief       Otkljucavanje pristupa
 */
# define OPT_GUARD_UNLOCK(guard)        ES_CRITICAL_EXIT()
#endif

/** @} *//*-------------------------------------------------------------------*/
/*------------------------------------------------------------------------*//**
 * @name        Odabir pool alokatora
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Ukljucivanje eksternog pool alokatora
 * @details     Moguce vrednosti:
 *              - 0 - koristi se eSolid alokator
 *              - 1 - koristi se eksterni alokator
 *
 * @note        Podrazumevano podesavanje: 0 (koristi se eSolid alokator)
 */
#if !defined(OPT_MEM_POOL_EXTERN) || defined(__DOXYGEN__)
# define OPT_MEM_POOL_EXTERN            0U
#endif

#if (0U == OPT_MEM_POOL_EXTERN)  || defined(__DOXYGEN__)

/**
 * @brief       Deskriptor Pool alokatora
 */
# define OPT_MEM_POOL_T                 esPMemHandle_T

/**
 * @brief       Dobavlja memorijski blok
 */
# define OPT_MEM_POOL_ALLOC(pool)                                               \
    esPMemAlloc(pool)

/**
 * @brief       Oslobadja memorijski blok
 */
# define OPT_MEM_POOL_DEALLOC(pool, mem)                                        \
    esPMemDeAlloc(pool, mem)
#endif

/** @} *//*-------------------------------------------------------------------*/
/*------------------------------------------------------------------------*//**
 * @name        Odabir dinamickog alokatora
 * @brief       Odabiranje zeljenog dinamickog alokatora
 * @details     Ukoliko sistem u koji se integrise eSolid vec poseduje dinamicke
 *              alokatore memorije ovim makroima se moze izvrsiti odabir
 *              postojecg dinamickog alokatora.
 *
 * @p           Primer kada je potrebno odabrati bibliotecke @c malloc/free
 *              funkcije:
 *              - ukljuciti opciju @ref OPT_MEM_DYN_EXTERN
 *              - definisati makro @ref OPT_MEM_DYN_T kao prazan makro
 *              - definisati makro @ref OPT_MEM_DYN_ALLOC kao:
 *              <c>OPT_MEM_DYN_ALLOC(handle, size) malloc(size)</c>
 *              - definisati makro @ref OPT_MEM_DYN_DEALLOC kao:
 *              <c>OPT_MEM_DYN_DEALLOC(handle, mem) free(mem)</c>
 *              - ukljuciti bibliotecku datoteku: @c stdlib
 * @{ *//*--------------------------------------------------------------------*/

/**
 * @brief       Ukljucivanje eksternog dinamickog alokatora
 * @details     Moguce vrednosti:
 *              - 0 - koristi se eSolid alokator
 *              - 1 - koristi se eksterni alokator
 *
 * @note        Podrazumevano podesavanje: 0 (koristi se eSolid alokator)
 */
#if !defined(OPT_MEM_DYN_EXTERN) || defined(__DOXYGEN__)
# define OPT_MEM_DYN_EXTERN             0U
#endif

#if (0U == OPT_MEM_DYN_EXTERN)  || defined(__DOXYGEN__)

/**
 * @brief       Deskriptor dinamickog alokatora
 * @details     Ukoliko alokator ne koristi deskriptor onda ovaj makro treba
 *              definisati kao prazan makro
 */
# define OPT_MEM_DYN_T                  esDMemHandle_T

/**
 * @brief       Dobavlja memorijski blok
 */
# define OPT_MEM_DYN_ALLOC(handle, size)                                        \
    esDMemAlloc(handle, size)

/**
 * @brief       Oslobadja memorijski blok
 */
# define OPT_MEM_DYN_DEALLOC(handle, mem)                                       \
    esDMemDeAlloc(handle, mem)
#endif

/** @} *//*-------------------------------------------------------------------*/

/**
 * @brief       Omogucavanje LOGer-a
 */
#if defined(__DOXYGEN__)
# define OPT_SYS_ENABLE_LOG
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/

#if (1U == OPT_GUARD_EXTERN) &&                                                 \
    (!defined(OPT_GUARD_DECL) || !defined(OPT_GUARD_INIT) ||                    \
     !defined(OPT_GUARD_LOCK) || !defined(OPT_GUARD_UNLOCK))
# error "eSolid: When option OPT_GUARD_EXTERN is active you have to define all OPT_GUARD_... macros"
#endif

#if (1U == OPT_CRITICAL_EXTERN) &&                                              \
    (!defined(OPT_CRITICAL_DECL) || !defined(OPT_CRITICAL_LOCK) ||              \
     !defined(OPT_CRITICAL_UNLOCK))
# error "eSolid: When option OPT_CRITICAL_EXTERN is active you have to define all OPT_CRITICAL_... macros"
#endif

#if (1U == OPT_MEM_DYN_EXTERN) &&                                               \
    (!defined(OPT_MEM_DYN_T) || !defined(OPT_MEM_DYN_ALLOC) ||                  \
     !defined(OPT_MEM_DYN_DEALLOC))
# error "eSolid: When option OPT_MEM_DYN_EXTERN is active you have to define all OPT_MEM_DYN_... macros"
#endif

#if (1U == OPT_MEM_POOL_EXTERN) &&                                              \
    (!defined(OPT_MEM_POOL_T) || !defined(OPT_MEM_POOL_ALLOC) ||                \
     !defined(OPT_MEM_POOL_DEALLOC))
# error "eSolid: When option OPT_MEM_POOL_EXTERN is active you have to define all OPT_MEM_POOL_... macros"
#endif

/** @endcond *//** @} *//******************************************************
 * END of sys_config.h
 ******************************************************************************/
#endif /* SYS_CONFIG_H_ */
