/* Primer 1: Upis identifikatora dogadjaja */
void servisnaFunkcija2 (
    primer_T    * aHSM,
    eot_evt_T   * evt) {

    /* ... */

    EOT_EVT_WRID(evt, SIG_A);                                                   /* ((eot_evtExt_T *)evt)->ID = (uint8_t)SIG_A */

    /* ... */
}
