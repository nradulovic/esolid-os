/* Primer 1: izvodjenje strukture HSM automata od genericke strukture smp_exec */
typedef struct primer1 {
    smp_exec_T      super;                                                      /* super struktura - od nje se izvodi struktura primer1*/
    /* ovde se dodaju sve promenljive koje HSM interno koristi */
    /* ... */
    uint8_t         promenljiva1;
    uint16_t        promenljiva2;
    /* ... */
} primer1_T;

static primer1_T Primer1;                                                       /* jedna instanca automata */
