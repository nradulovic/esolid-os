/*
 * Slede primeri slanja prosirenih dogadjaja pomocu nekoliko metoda. Konacna
 * implementacija se moze razlikovati od ove nevedene u ovoj datoteci.
 *
 * Primer 1: 
 * Treba da opišemo proširen dogadjaj:
 *
 * ID dogadjaja:
 *      SIG_PARAM_DATA
 *      SIG_GET_DATA
 *      SIG_SET_DATA
 *
 * Tip dogadjaja: PARAM_EXCHANGE.
 *
 * Struktura podataka dogadjaja:
 *      uint16_t paramID;
 *      uint16_t paramType;
 *      uint32_t paramData;
 *
 * Opis:
 * Ovakav dogadjaj se koristi u komunikaciji sa PARAM automatom. Prvi podatak u
 * strukturi, paramID, se koristi za identifikaciju pojedinacnih parametara koji
 * su dostupni u uredjaju. paramType se koristi za klasifikaciju parametara, na
 * osnovu kojih automat param i automat klijent mogu da protumace podatke 
 * sadrzane u paramData.
 */

/******************************************************************************
 * Prvi metod - metod 1 sa upotrebom makroa
 ******************************************************************************/

/*----------------------------------------------------------------------------*/
/* Ovu strukturu treba da vide svi oni koji koriste ove dogadjaje. */ 
/* Zbog toga ovaj kod ide u datoteku zaglavlja. */
/*----------------------------------------------------------------------------*/
typedef struct paramExchange {
    eot_evtExt_T    super;
    uint16_t        paramID;
    uint16_t        paramType;
    uint32_t        paramData;
} paramExchange_T;

/*----------------------------------------------------------------------------*/
/* unutar C datoteke                                                          */
/*----------------------------------------------------------------------------*/

/* slanje dogadjaja (KLIJENT) */
{
    paramExchange_T * pEvtExt;                                                  /* Pokazivac koji se koristi u procesu kreira-  */
                                                                                /* nja i slanja dogadjaja. Onog trenutka kada   */
                                                                                /* je dogadjaj poslat, ovaj pokazivac nije vise */
                                                                                /* potreban. Zbog kratke upotrebe pokazivaca,   */
                                                                                /* on se moze definisati kao auto promenljiva.  */
                                                                                /* Na taj nacin, on ce postojati samo u trenu-  */
                                                                                /* tku kreiranja dogadjaja. Zbog toga se kori-  */
                                                                                /* ste zakrivljene zagrade.                     */

    pEvtExt = (paramExchange_T *)HEP_evtExtCreate(                              /* Kreiraj dogadjaj paramExchange sa signalom   */
        sizeof(paramExchange_T),                                                /* SIG_GET_DATA.                                */
        (EOT_EVT_ID_T)SIG_GET_DATA);
    
    ((eot_evtExt_T *)pEvtExt)->type = (EOT_EVT_TYPE_T)PARAM_EXCHANGE;           /* U dogadjaj pEvtExt koji smo tek kreirali     */
                                                                                /* upisi TIP dogadjaja. Ovaj podatak primalac   */
                                                                                /* moze da koristi kako bi odredio kojeg tipa   */
                                                                                /* je primljeni dogadjaj ukoliko to ne moze da  */
                                                                                /* da se zakljuci na osnovu signala.            */
    ((eot_evtExt_T *)pEvtExt)->transmitter = (void *)aHSM;                      /* U dogadjaj koji smo tek kreirali upisi ko je */
                                                                                /* POSILJALAC poruke. Ovo moze biti korisno ako */
                                                                                /* primalac treba samo njemu da odgovori.       */
    pEvtExt->paramID = AVENT_SPEED3;                                            /* Popunjavanje dogadjaja korisnickim podacima. */
    pEvtExt->paramType = TYPE1;
    pEvtExt->paramData = (uint32_t)0;
    HEP_evtExtPost(                                                             /* Posalji dogadjaj pEvtExt automatu PARAM.     */
        pAOBJ_PARAM,
        (eot_evtExt_T *)pEvtExt);
}

/* prihvatanje i slanje dogadjaja (PARAM) */
    paramExchange_T * pEvtExt;                                                  /* Sa obzirom da PARAM automat stalno radi sa   */
                                                                                /* paramExchange_T pokazivacem, onda je bolje   */
                                                                                /* da se deklarise kao static tip promenljive   */
                                                                                /* i da postoji sve vreme dok je automat akti-  */
                                                                                /* van. Deklarisanje pokazivaca kao auto pro-   */
                                                                                /* menljive bi u ovom slucaju bilo kontrapro-   */
                                                                                /* duktivno sa stanovista performansi.          */
    case SIG_GET_DATA : {
    
        /* ... */
        switch (((const paramExchange_T *)evt)->paramID)) {                     /* Dobavljanje korisnickog podatka paramID i    */
                                                                                /* njegovo ispitivanje.                         */
            case AVENT_SPEED1 : {
                /* ... */
            }
            
            case AVENT_SPEED2 : {
                /* ... */
            }
            
            case AVENT_SPEED3 : {
                /* ... */
                pEvtExt = (paramExchange_T *)HEP_evtExtCreate(                  /* Kreiranje novog dogadjaja radi formiranja    */
                    sizeof(paramExchange_T),                                    /* odgovora.                                    */
                    (EOT_EVT_ID_T)SIG_PARAM_DATA);
                ((eot_evtExt_T *)pEvtExt)->type = (EOT_EVT_TYPE_T)PARAM_EXCHANGE; /* Upis tipa dogadjaja.                       */
                ((eot_evtExt_T *)pEvtExt)->transmitter = (void *)aHSM;          /* Upis adrese posiljalaca.                     */
                pEvtExt->paramID = AVENT_SPEED3;                                /* Upis korisnickog podatka paramID.            */
                pEvtExt->paramType = TYPE1;                                     /* Upis korisnickog podatka paramType.          */
                pEvtExt->paramData = data;                                      /* Upis korisnickog podatka paramData.          */
                HEP_evtExtPost(                                                 /* Slanje dogadjaja pEvtExt. Dogadjaj se salje  */
                    evt->transmitter,                                           /* onom automatu koji je i izvrsio upit u prim- */
                    (eot_evtExt_T *)pEvtExt);                                   /* ljenom dogadjaju. Njegova adresa se dobavlja */
                                                                                /* preko makroa HEP_EXT_TRANSMITTER().          */
            }
        }
    }
    
/******************************************************************************
 * Drugi metod - metod 1 sa upotrebom makroa
 ******************************************************************************/

/*----------------------------------------------------------------------------*/
/* Ovu strukturu treba da vide svi oni koji koriste ove dogadjaje.            */ 
/* Zbog toga ovaj kod ide u datoteku zaglavlja.                               */
/*----------------------------------------------------------------------------*/
typedef struct paramExchange {
    eot_evtExt_T    super;
    uint16_t        paramID;
    uint16_t        paramType;
    uint32_t        paramData;
} paramExchange_T;

/*----------------------------------------------------------------------------*/
/* unutar C datoteke                                                          */
/*----------------------------------------------------------------------------*/

/* slanje dogadjaja (KLIJENT) */
{
    paramExchange_T * pEvtExt;                                                  /* Pokazivac koji se koristi u procesu kreira-  */
                                                                                /* nja i slanja dogadjaja. Onog trenutka kada   */
                                                                                /* je dogadjaj poslat, ovaj pokazivac nije vise */
                                                                                /* potreban. Zbog kratke upotrebe pokazivaca,   */
                                                                                /* on se moze definisati kao auto promenljiva.  */
                                                                                /* Na taj nacin, on ce postojati samo u trenu-  */
                                                                                /* tku kreiranja dogadjaja. Zbog toga se kori-  */
                                                                                /* ste zakrivljene zagrade.                     */

    pEvtExt = HEP_EXT_CREATE(paramExchange_T, SIG_GET_DATA);                    /* Kreiraj dogadjaj paramExchange sa signalom   */
                                                                                /* SIG_GET_DATA.                                */
    HEP_EXT_WRTYPE(pEvtExt, PARAM_EXCHANGE);                                    /* U dogadjaj pEvtExt koji smo tek kreirali     */
                                                                                /* upisi TIP dogadjaja. Ovaj podatak primalac   */
                                                                                /* moze da koristi kako bi odredio kojeg tipa   */
                                                                                /* je primljeni dogadjaj ukoliko to ne moze da  */
                                                                                /* da se zakljuci na osnovu signala.            */
    HEP_EXT_WRTRANSMITTER(pEvtExt, aHSM);                                       /* U dogadjaj koji smo tek kreirali upisi ko je */
                                                                                /* POSILJALAC poruke. Ovo moze biti korisno ako */
                                                                                /* primalac treba samo njemu da odgovori.       */
    HEP_EXT_WRPARAM(pEvtExt, paramExchange_T, paramID, AVENT_SPEED3);           /* Popunjavanje dogadjaja korisnickim podacima. */
    HEP_EXT_WRPARAM(pEvtExt, paramExchange_T, paramType, TYPE1);
    HEP_EXT_WRPARAM(pEvtExt, paramExchange_T, paramData, 0);
    HEP_evtExtPost(                                                             /* Posalji dogadjaj pEvtExt automatu PARAM.     */
        pAOBJ_PARAM,
        (eot_evtExt_T *)pEvtExt);
}

/* prihvatanje i slanje dogadjaja  (PARAM) */
    paramExchange_T * pEvtExt;                                                  /* Sa obzirom da PARAM automat stalno radi sa   */
                                                                                /* paramExchange_T pokazivacem, onda je bolje   */
                                                                                /* da se deklarise kao static tip promenljive   */
                                                                                /* i da postoji sve vreme dok je automat akti-  */
                                                                                /* van. Deklarisanje pokazivaca kao auto pro-   */
                                                                                /* menljive bi u ovom slucaju bilo kontrapro-   */
                                                                                /* duktivno sa stanovista performansi.          */
    case SIG_GET_DATA : {
    
        /* ... */
        switch (HEP_EXT_PARAM(evt, paramExchange_T, paramID)) {                 /* Dobavljanje korisnickog podatka paramID i    */
                                                                                /* njegovo ispitivanje.                         */
            case AVENT_SPEED1 : {
                /* ... */
            }
            
            case AVENT_SPEED2 : {
                /* ... */
            }
            
            case AVENT_SPEED3 : {
                /* ... */
                pEvtExt = HEP_EXT_CREATE(paramExchange_T, SIG_PARAM_DATA);      /* Kreiranje novog dogadjaja radi formiranja    */
                                                                                /* odgovora.                                    */
                HEP_EXT_WRTYPE(pEvtExt, PARAM_EXCHANGE);                        /* Upis tipa dogadjaja.                         */
                HEP_EXT_WRTRANSMITTER(pEvtExt, aHSM);                           /* Upis adrese posiljalaca.                     */
                HEP_EXT_WRPARAM(pEvtExt, paramExchange_T, paramID, AVENT_SPEED3); /* Upis korisnickog podatka paramID.          */
                HEP_EXT_WRPARAM(pEvtExt, paramExchange_T, paramType, TYPE1);    /* Upis korisnickog podatka paramType.          */
                HEP_EXT_WRPARAM(pEvtExt, paramExchange_T, paramData, data);     /* Upis korisnickog podatka paramData.          */
                HEP_evtExtPost(                                                 /* Slanje dogadjaja pEvtExt. Dogadjaj se salje  */
                    HEP_EXT_TRANSMITTER(evt),                                   /* onom automatu koji je i izvrsio upit u prim- */
                    (eot_evtExt_T *)pEvtExt);                                   /* ljenom dogadjaju. Njegova adresa se dobavlja */
                                                                                /* preko makroa HEP_EXT_TRANSMITTER().          */
            }
        }
    }
    
/******************************************************************************
 * Treci metod - kombinacija metoda 1 i 2
 ******************************************************************************/

/*
 * Ovaj metod meni najvise odgovara. On kombinuje najpogodnije elemente metoda
 * 1 i 2. Metod 1 pruza nejvecu fleksibilnost i tacno se vidi sta se radi. U 
 * metodu 2 zbog koriscenja makroa akcije se sakrivaju od programera, ali unose
 * potrebu za malo vecom kolicinom kucanja. Meni je najprirodnije da se za
 * pristup sistemskim podacima (podaci zaglavlja dogadjaja) koriste makroi 
 * HEP_EXT_{TYPE, TRANSMITTER, WRTYPE, WRTRANSMITTER}, a da se pristup 
 * korisnickim podacima obavalja direktno, preko pokazivaca "->". 
 *
 * Primer koda sa kombinacijom direktnog pristupa i pristupa preko makroa:
 */
 
    pEvtExt = HEP_EXT_CREATE(paramExchange_T, SIG_GET_DATA);
    HEP_EXT_WRTYPE(pEvtExt, PARAM_EXCHANGE);
    HEP_EXT_WRTRANSMITTER(pEvtExt, aHSM);
    pEvtExt->paramID = AVENT_SPEED3;                                            /* Upis korisnickog podatka paramID.            */
    pEvtExt->paramType = TYPE1;                                                 /* Upis korisnickog podatka paramType.          */
    pEvtExt->paramData = data;                                                  /* Upis korisnickog podatka paramData.          */
    HEP_evtExtPost(
        HEP_EXT_TRANSMITTER(evt),
        (eot_evtExt_T *)pEvtExt);
        
/******************************************************************************
 * Cetvrti metod - koriscenje klasa
 ******************************************************************************/
 
/*----------------------------------------------------------------------------*/
/* Ovu strukturu treba da vide svi oni koji koriste ove dogadjaje.            */ 
/* Zbog toga ovaj kod ide u datoteku zaglavlja.                               */
/*----------------------------------------------------------------------------*/
typedef struct paramExchange {
    eot_evtExt_T    super;
    uint16_t        paramID;
    uint16_t        paramType;
    uint32_t        paramData;
} paramExchange_T;

/* Pokazivac na klasu (videti dole)                                           */
extern const E_ROM eot_evtClass_T * const E_ROM_VAR pParamExchange;

/*----------------------------------------------------------------------------*/
/* unutar C datoteke objekta koji definise ovaj tip dogadjaja.                */
/*----------------------------------------------------------------------------*/

/* deklaracije konstruktora i destruktora */
void * paramExchange_ctor (
    void * aMe);
void * paramExchange_dtor (
    void * aMe);

/* deklaracija i definicija klase paramExchange                               */
/*
 * Struktura klase sadrzi sledece elemente:
 * - 1. Velicinu CELOG dogadjaja (zaglavlje + korisnicki podaci),
 * - 2. enumerator tipa dogadjaja,
 * - 3. pokazivac na f-ju konstruktor,
 * - 4. pokazivac na f-ju desttruktor,
 * - 5. pokazivac na f-ju kloniranja dogadjaja,
 * - 6. pokazivac na f-ju za poredjenje dogadjaja.
 *
 * Pored navedenih elemenata, struktura klase se moze prosiriti i sa elementima
 * kao sto su:
 * - I pokazivac na znakovni niz koji opisuje tip dogadjaja,
 * - II pokazivac na znakovni niz koji opisuje strukturu dogadjaja (Deki, 
 *      trazio si ovo, sada moze lako da se uradi),
 * - III dodatne f-je za manipulacijom sa dogadjajem, primer: 
 *      pakovanje/raspakivanje podataka, kompresija/dekompresija i slicno.
 */
const E_ROM eot_evtClass_T paramExchange = {
    (EOT_EVT_SIZE_T)sizeof(paramExchange_T),
    (EOT_EVT_TYPE_T)PARAM_EXCHANGE,
    &paramExchange_ctor,
    &paramExchange_dtor,
    NULL,
    NULL};
    
/* Konstantan pokazivac na konstantnu klasu paramExchange, ovaj pokazivac     */
/* mora da bude javan.                                                        */
const E_ROM eot_evtClass_T * const E_ROM_VAR pParamExchange = &paramExchange;

/* definicija konstruktora                                                    */
/*
 * On treba da pripremi memorijski prostor za koriscenje. Pored toga, on se
 * moze koristiti i za bilo koje druge akcije. U ovom primeru konstruktor vrsi
 * inicijalizaciju paramID = 0, postavlja se tip parametara na DEFAULT_TYPE
 * koji je na primer: TYPE1 (Dekijeva klasifikacija) i postavlja paramData na
 * nulu.
 */
void * paramExchange_ctor (
    void * aMe) {
    
    ((paramExchange_T *)aME)->paramID = (uint16_t)0;
    ((paramExchange_T *)aME)->paramType = (uint16_t)DEFAULT_TYPE;
    ((paramExchange_T *)aME)->paramData = (uint32_t)0;
    
    return aME;
}

/* definicija destruktora                                                     */
/*
 * Destruktor nema jasno definisanu ulogu. Njegova svrha je da izvrsi ciscenje
 * dogadjaja nakon sto je dogadjaj eksploatisan. U njemu se moze obaviti 
 * katalogizacija dogadjaja, procesiranje statistike i brisanje podataka radi
 * povecane sigurnosti privatnih podataka.
 */
void * paramExchange_dtor (
    void * aMe) {
    
    ((paramExchange_T *)aME)->paramID = (uint16_t)0;
    ((paramExchange_T *)aME)->paramType = (uint16_t)DEFAULT_TYPE;
    ((paramExchange_T *)aME)->paramData = (uint32_t)0;
    
    return aME;
}

/*----------------------------------------------------------------------------*/
/* unutar C datoteke KLIJENTA i PARAM                                         */
/*----------------------------------------------------------------------------*/

/* Slanje dogadjaja (KLIJENT) */
{
    paramExchange_T * pEvtExt;                                                  /* Kreiranje pokazivaca kao u prethodnim        */
                                                                                /* primerima.                                   */
    pEvtExt = HEP_EXT_CREATE(pParamExchange, SIG_GET_DATA);                     /* Kreiranje dogadjaja. Primetiti razliku da se */
                                                                                /* makrou sada predaje pokazivac na strukturu   */
                                                                                /* klase koja opisuje dogadjaj. Funkcija koja   */
                                                                                /* poziva unutar makroa sada na osnovu opisa    */
                                                                                /* klase sama popunjava tip dogadjaja.          */
    HEP_EXT_WRTRANSMITTER(pEvtExt, aHSM);                                       /* Upis posiljalaca dogadjaja.                  */
    pEvtExt->paramID = AVENT_SPEED3;                                            /* Popunjavanje dogadjaja korisnickim podacima. */
    /* pEvtExt->paramType = TYPE1; */                                           /* Korisnik ne mora da popuni paramType jer je  */
                                                                                /* to uradio konstruktor.                       */
    /* pEvtExt->paramData = (uint32_t)0; */                                     /* Korisnik, takodje, ne mora da popuni         */
                                                                                /* paramData jer ga je konstruktor inicijali-   */
                                                                                /* zovao na podrazumevanu vrednost.             */
    HEP_evtExtPost(                                                             /* Posalji dogadjaj pEvtExt automatu PARAM.     */
        pAOBJ_PARAM,
        (eot_evtExt_T *)pEvtExt);
}

/* prihvatanje i slanje dogadjaja  (PARAM) */
    paramExchange_T * pEvtExt;                                                  /* Sa obzirom da PARAM automat stalno radi sa   */
                                                                                /* paramExchange_T pokazivacem, onda je bolje   */
                                                                                /* da se deklarise kao static tip promenljive   */
                                                                                /* i da postoji sve vreme dok je automat akti-  */
                                                                                /* van. Deklarisanje pokazivaca kao auto pro-   */
                                                                                /* menljive bi u ovom slucaju bilo kontrapro-   */
                                                                                /* duktivno sa stanovista performansi.          */
    case SIG_GET_DATA : {
    
        /* ... */
        switch (HEP_EXT_PARAM(evt, paramExchange_T, paramID)) {                 /* Dobavljanje korisnickog podatka paramID i    */
                                                                                /* njegovo ispitivanje.                         */
            case AVENT_SPEED1 : {
                /* ... */
            }
            
            case AVENT_SPEED2 : {
                /* ... */
            }
            
            case AVENT_SPEED3 : {
                /* ... */
                pEvtExt = HEP_EXT_CREATE(pParamExchange, SIG_PARAM_DATA);       /* Kreiranje novog dogadjaja radi formiranja    */
                                                                                /* odgovora.                                    */
                /*HEP_EXT_WRTYPE(pEvtExt, PARAM_EXCHANGE);*/                    /* Upis tipa dogadjaja.                         */
                HEP_EXT_WRTRANSMITTER(pEvtExt, aHSM);                           /* Upis adrese posiljalaca.                     */
                pEvtExt->paramID = AVENT_SPEED3;                                /* Upis korisnickog podatka paramID.            */
                /* pEvtExt->paramType = TYPE1; */                               /* Upis korisnickog podatka paramType nije      */
                                                                                /* potreban.                                    */
                pEvtExt->paramData = data;                                      /* Upis korisnickog podatka paramData.          */
                HEP_evtExtPost(                                                 /* Slanje dogadjaja pEvtExt. Dogadjaj se salje  */
                    HEP_EXT_TRANSMITTER(evt),                                   /* onom automatu koji je i izvrsio upit u prim- */
                    (eot_evtExt_T *)pEvtExt);                                   /* ljenom dogadjaju. Njegova adresa se dobavlja */
                                                                                /* preko makroa HEP_EXT_TRANSMITTER().          */
            }
        }
    }
 
