#include <stddef.h>
#include <stdint.h>
#include "kernel/mem.h"

struct dataBlock {                                          /* Some application data structure*/
    uint32_t data01;
    uint32_t data02;
    uint16_t command;
};

#define POOL_ELEMENTS                   10U                 /* Specification of pool */

int main(
    void) {

    static esPMemHandle_T myPool;                           /* myPool pool handle */
    static void * poolStorage;                              /* pointer to pool storage */
    static size_t poolSize;                                 /* size of pool */

    esSMemInit();                                           /* Initialize static memory manager */
    poolSize = esPMemCalcPoolSize(
        sizeof(struct dataBlock),
        POOL_ELEMENTS);                                     /* Calculate the required pool size which will hold */
                                                            /* POOL_ELEMENTS number of structures of type dataBlock */
    poolStorage = esSMemAllocI(
        poolSize);                                          /* Allocate pool storage memory */
    esPMemInit(
        &myPool,
        poolStorage,
        poolSize,
        sizeof(struct dataBlock));                          /* Initialize myPool pool memory */

    while (TRUE) {
        struct dataBlock * data;

        data = esPMemAllocI(
            &myPool);                                       /* Allocate a block from pool memory */

        /*
         * Do something
         */

        esPMemDeAllocI(
            &myPool,
            data);                                          /* Return back the block */
    }
}
