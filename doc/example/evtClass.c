/*
 * U jednu datoteku zaglavlja definišu se sve klase događaja.
 *
 * Primer:
 * Klasa osnovnih događaja, događaji koji imaju samo signal.
 */

/*
 * Prvo se napravi struktura koja opisuje jednu klasu događaja. U ovom primeru
 * se uzima da je događaj u suštini samo signal i da nema dodatne elemente na
 * strukturu super. Ako u jednu klasu događaja spada samo jedan događaj onda se
 * ne treba pisati ova struktura već se primenjuje način kako je objašnjeno u
 * extEvent.c
 */
typedef struct prazan {
    HEP_evtExt_T super;
} prazan_T;

/*
 * Klasa koja opisuje sve događaje osnovniDogadjaj tipa.
 */
const HEP_evtClass_T classPrazan = {
    /*
     * Prvo se navodi kolika je veličina strukture koja definiše događaj. To je 
     * struktura osnovniDogadjaj_T. Koristi se kastovanje (eot_evtSize_T). U 
     * prethodnom primeru to je bila pretprocesorska promenjiva EOT_EVT_SIZE_T, 
     * koja je sada promenjena u typedef.
     */
    (eot_evtSize_T)sizeof(prazan_T),
    
    /*
     * Navodi se enumerator tipa događaja. On se koristi kako bi prijemnik 
     * odredio koji tip događaj je primljen. Izbrojali smo oko 8 različitih 
     * tipova tako da ovde treba da se nađe jedan od tih osam enumeratora. 
     * Enumeratori moraju da budu definisani pre ovih struktura. Kao i kod
     * velicine događaja ovde je kast promenjen sa pretprocesorska promenjive
     * u typedef.
     */
    (eot_evtType_T)EMPTY,
    
    /*
     * Navodi se adresa funkcije konstruktora ovog tipa. Ovaj tip nema 
     * konstruktor funkciju pa se navodi nula pokazivač. Tip eot_evtCtor_T je
     * typedef pokazivaca na funkciju, otuda odsustvo * karaktera.
     */
    (eot_evtCtorPtr_T)0,
    
    /*
     * Navodi se adresa funkcije destruktora ovog tipa. Ovaj tip nema 
     * destruktor funkciju pa se navodi nula pokazivač. Tip eot_evtDtor_T je
     * typedef pokazivaca na funkciju, otuda odsustvo * karaktera.
     */
    (eot_evtDtorPtr_T)0,
};

/*
 * Opis događaja nekiKonkretanDogadjaj koji je tipa osnovniDogadjaj.
 */
typedef struct nekiKonkretanDogadjaj {
    osnovniDogadjaj_T super;
} nekiKonkretanDogadjaj_T;

/*
 * Opis dogadjaja nekiDrugiDogadjaj koji je tipa osnovniDogadjaj.
 */
typedef struct nekiDrugiDogadjaj {
    osnovniDogadjaj_T super;
} nekiDrugiDogadjaj_T;

