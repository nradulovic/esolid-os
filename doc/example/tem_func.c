static smp_state_t nekoStanje (                                                 /* Stanje "nekoStanje" koristi vremenske dogadjaje.         */
    hsmPrimer_T     * aHSM,
    eot_evt_T    * aEvt) {

    switch (EOT_EVT_ID(aEvt)) {

        case SIG_ENTRY: {
            tmrRedovno = eS_tmrCreate(
                SIG_TMR_PERIODIC);                                              /* Tajmer "Redovno" ce generisati SIG_TMR_PERIODIC signal.  */
            tmrVremeCekanja = eS_tmrCreate(
                SIG_TMR_WAIT);                                                  /* Tajmer "VremeCekanja" ce generisati SIG_TMR_WAIT signal. */
            eS_tmrSet(
                tmrRedovno,                                                     /* Namesti tajmeru "Redovno"                                */
                aHSM,                                                           /* da pošalje ovom EPA objektu                              */
                200);                                                           /* predefinisan signal za 200 tick ciklusa.                 */
            eS_tmrSet(
                tmrVremeCekanja,                                                /* Namesti tajmeru "VremeCekanja"                           */
                aHSM,                                                           /* da pošalje ovom EPA objektu                              */
                30000);                                                         /* predefinisan signal za 30000 tick ciklusa.               */
            eS_tmrEnable(
                tmrRedovno);                                                    /* Ukljuci tajmer "Redovno".                                */

            return SMP_STATE_HANDLED();
        }

        case SIG_PERIODIC: {
            eS_tmrReset(
                tmrRedovno);                                                    /* Resetuj tajmer "Redovno".                                */

            /* ... */

            return SMP_STATE_HANDLED();
        }

        case SIG_NEBITNO: {
            eS_tmrEnable(
                tmrVremeCekanja);                                               /* Omoguci odbrojavanje tajmera "VremeCekanja".             */

            /* ... */

            return SMP_STATE_HANDLED();
        }

        case SIG_TMR_WAIT: {

            /* ... */

            return SMP_STATE_HANDLED();
        }

        case SIG_EXIT: {
            eS_tmrDestroy(
                tmrRedovno);                                           /* Obriši tajmere kada nam više nisu potrebni.              */
            eS_tmrDestroy(
                tmrVremeCekanja);

            return SMP_STATE_HANDLED();
        }
    }
}