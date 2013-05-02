#include <stddef.h>
#include "kernel/mem.h"

struct dataBlock {                                          /* Some application data structure*/
    uint32_t data01;
    uint32_t data02;
    uint16_t command;
};

#define POOL_ELEMENTS                   10U                 /* Specification of pool */
#define POOL_SIZE                       (ES_ALIGN_UP(sizeof(struct dataBlock), sizeof(unative_T)) * POOL_ELEMENTS)

int main(
    void) {

    static esPMemHandle_T myPool;                           /* myPool pool handle */
    static void * poolStorage;                              /* pointer to pool storage */

    esSMemInit();                                           /* Initialize static memory manager */
    poolStorage = esSMemAllocI(
        POOL_SIZE);                                         /* Allocate pool storage memory */
    esPMemInit(
        &myPool,
        poolStorage,
        POOL_SIZE,
        ES_ALIGN_UP(sizeof(struct dataBlock), sizeof(unative_T))); /* Initialize myPool pool memory */

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
