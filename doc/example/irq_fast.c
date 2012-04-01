ES_FAST_IRQ_HANDLER(myIRQ) {
    /*
     * Kod prekidne rutine, moze da bude istisnut ukoliko arhitektura to
     * dozvoljava. Ovde ne smeju da se pozivaju API funkcije zato sto brze
     * prekidne rutine mogu da istisnu eSolid i u kriticnim sekcijama koda.
     */
}
