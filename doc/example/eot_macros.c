static smp_state_T stateS1 (
    primer1_T       * aHSM,
    const eot_evt_T * evt) {

    switch (EOT_EVT_ID(evt)) {

        case SIG_ENTRY: {

            return SMP_STATE_HANDLED();
        }

        case SIG_EXIT: {

            return SMP_STATE_HANDLED();
        }

        case SIG_C1: {
            aHSM->promenljiva1 = (uint8_t)0;                                    /* pristup promenljivoj u HSM-u */
            aHSM->promenljiva2 = (uint16_t)E_PTRCAST(eot_evt_T, mojDogadjaj_T, evt).podatak2;                /* cuvanje dobijenog parametara u HSM promenljivu */

            if ((uint8_t)E_PTRCAST(eot_evt_T, mojDogadjaj_T, evt).podatak == (uint8_t)uslov) {              /* ponekad je potrebno uraditi cast parametara koji je dobiven */
                return SMP_STATE_TRAN(&stateS2);                                /* SMP_STATE_TRAN == makro koji kazuje dispečeru da treba da se pređe u novo stanje*/
            }

            return SMP_STATE_HANDLED();
        }

        default : {

            return SMP_STATE_SUPER(&eS_hsmTopState);
        }
    }
}
