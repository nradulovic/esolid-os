#include "kernel/mem.h"

#define BUFFER_SIZE                     1024

int main (
    void) {

    static esDMemHandle_T myHeap;
    static char * buffer;

    esSMemInit();                                           /* Initialize the static memory */
                                                            /* The amount of static memory is */
                                                            /* specified in mem_config.h file */
    buffer = esSMemAllocI(
        BUFFER_SIZE);                                       /* Create a buffer of 1024 bytes */

    esDMemInit(
        &myHeap,
        buffer,
        BUFFER_SIZE);                                       /* Initialize the dynamic memory */

    while (TRUE) {
        int * myArray;

        myArray = esDMemAllocI(
            &myHeap,
            sizeof(int) * 10U);                             /* Allocate an array of 10 integers */

        /*
         * Do some stuff
         */

        esDMemDeAllocI(
            &myHeap,
            myArray);                                       /* Delete the array */
    }
}
