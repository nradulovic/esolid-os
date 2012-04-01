ES_IRQ_HANDLER(myIRQ) {
    ES_IRQ_PROLOGUE();

    /*
     * Kod prekidne rutine, mogucnost istiskivanja od strane druge prekidne
     * rutine sa visim prioritetom ukoliko arhitektura to dozvoljava.
     */

    ES_IRQ_ENTER_CRITICAL();

    /*
     * Pozivi API funkcija koje ne smeju da se istisnu.
     */

    ES_IRQ_EXIT_CRITICAL();

    /*
     * Kod prekidne rutine, mogucnost istiskivanja od strane druge prekidne
     * rutine sa visim prioritetom ukoliko arhitektura to dozvoljava.
     */

    ES_IRQ_EPILOGUE();
}
