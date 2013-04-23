#include "kernel/mem.h"

#define BUFFER_SIZE                     1024

int main (
    void) {

    static char buffer[BUFFER_SIZE];
    static esDMemDesc_T myHeap;

    esDMemInit(
        &myHeap,
        &buffer[0],
        sizeof(buffer));                                    /* Initialize the dynamic memory */

    while (TRUE) {
        int * myArray;

        myArray = esDMemAlloc(
            &myHeap,
            sizeof(int) * 10U);                             /* Allocate an array of 10 integers */

        /*
         * Do some stuff
         */

        esDMemDeAlloc(
            &myHeap,
            myArray);                                       /* Delete the array */
    }
}
