/*
*Pete Gordon
*04/21/2021
*Second Reader Writers Problem
*/

/*Main
*Takes in two command line args 1. # of readers; 2. # of writers
*/

//Includes
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <semaphore.h>
#include <pthread.h>

#include "defs.h"
#include "file-io.h"





//begin main
int main (int argc ,char *argv[]) {

    if (argc != 2) {
        printf("Usage: ./main #READERS #WRITERS");
        exit(EXIT_FAILURE);
    }
//Try casting and assigning readers and writers
    if (!initReaderCnt(argv[1])){
        printErr(&errno);
        exit(EXIT_FAILURE);
    }

    if (!initWriterCnt(argv[2])){
        printErr(&errno);
        exit(EXIT_FAILURE);
    }

    char *fail_Str;

//Try to initialize semaphores
    if (!initSems(fail_Str)) {
        fprintf(stderr, *fail_Str, strerror(errno));
        exit(EXIT_FAILURE);
    }

//Thread data structures
    reader_info *r_Array;//pointer head for reader array
    writer_info *w_Array;//pointer head for writer array
    //create reader array
    r_Array = (reader *) calloc (atoi(argv[1]), sizeof(reader_info));
    //create writer array
    w_Array = (writer *) calloc (atoi(argv[2]), sizeof(writer_info));
    //check if valid
    if (r_Array == NULL || w_Array == NULL) {
        fprintf(stderr, "\nError Creating reader/writer arrays...\n", stderror(errno));
        exit(EXIT_FAILURE);
    }
    

/******************************************************
*Run thread Processes
******************************************************/

    for (int i = 0; i < )

    return 0;
}

void cleanup (){


}