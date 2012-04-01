typedef struct hsmPrimer {                                                      /* Struktura EPA objekta koji koristi time dogadjaje.       */
    epe_epaHsm_T    super;                                                      /* Super struktura automata koji se koristi u EPA objektu.  */
    tem_evtTime_T   * tmrRedovno;                                               /* Pokazivac na tajmer 1.                                   */
    tem_evtTime_T   * tmrVremeCekanja;                                          /* Pokazivac na tajmer 2.                                   */
} hsmPrimer_T;

static hsmPrimer_T hsmPrimer;                                                   /* Instaciranje strukture.                                  */

