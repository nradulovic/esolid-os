typedef struct mojDogadjaj {                                                    /* Kreiranje novog dogadjaja.            */
    eot_evt_T    super;                                                      /* Super struktura prosirenog dogadjaja. */
    uint8_t         prviElement;                                                /* Korisnicki podaci.                    */
    uint8_t         drugiElement;
} mojDogadjaj_T;

/* citanje dogadjaja u funkciji stanja */
static smp_state_T stateS1 (
    primer1_T       * aHSM,
    const eot_evt_T * evt) {

    /* ... */

    aHSM->lokalni_podatak = EOT_EXT_PARAM(evt, mojDogadjaj_T, prviElement);     /* Cita se prviElement podatak */
                                                                                /* iz strukture mojDogadjaj_T  */
                                                                                /* u dogadjaju evt.            */

    return SMP_STATE_HANDLED();

    /* ... */
}
