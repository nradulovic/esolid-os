#include "kernel/mem.h"

#define DATA_BUFFER_SIZE                2048
#define COMMAND_BUFFER_SIZE             512

int main (
    void) {

    static esDMemDesc_T dataHeap;
    static esDMemDesc_T commandHeap;
    static char * dataBuffer;
    static char * commandBuffer;

    esSMemInit();                                           /* Initialize the static memory */
                                                            /* The amount of static memory is */
                                                            /* specified in mem_config.h file */
    dataBuffer = esSMemAlloc(
        DATA_BUFFER_SIZE);                                  /* Create a data buffer of 2048 bytes */
    commandBuffer = esSMemAlloc(
        COMMAND_BUFFER_SIZE);                               /* Create additional buffer of 512 bytes */

    esDMemInit(
        &dataHeap,
        dataBuffer,
        DATA_BUFFER_SIZE);                                  /* Initialize the dynamic memory for data */

    esDMemInit(
        &commandHeap,
        commandBuffer,
        COMMAND_BUFFER_SIZE);                               /* Initialize the dynamic memory for commands */

    while (TRUE) {
        int * data;
        int * command;

        data = esDMemAlloc(
            &dataHeap,
            sizeof(int) * 10U);                             /* Allocate an array of 10 integers */

        command = esDMemAlloc(
            &commandHeap,
            sizeof(int));

        /*
         * Do some stuff
         */

        esDMemDeAlloc(
            &commandHeap,
            command);

        esDMemDeAlloc(
            &dataHeap,
            data);                                          /* Delete the array */
    }
}
