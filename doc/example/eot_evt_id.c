/* Primer 1: Citanje identifikatora dogadjaja */
static smp_state_T stateFunc2 (
    primer_T        * aHSM,
    const eot_evt_T * evt) {

    switch (EOT_PLN_ID(evt)) {                                                  /* citanje identifikatora dogadjaja */
        case SIG_A : {

            return SMP_STATE_HANDLED();
        }

        default : {

            return SMP_STATE_SUPER(aHSM, &eS_hsmTopState);
        }
    }
}
