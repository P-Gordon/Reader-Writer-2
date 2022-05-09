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
#include "file_op.h"
#include "thread_work.h"

//clean dynamically created vars




//begin main
int main (int argc ,char *argv[]) {

    if (argc != 2) {
        printf("Usage: ./project_2 #READERS #WRITERS");
        exit(EXIT_FAILURE);
    }

//program Variables
    shared_dat shared_data;//shared data
    int errnum;//error number container

//Try casting and assigning readers and writers
    if (!initReaderCnt(&shared_data, argv[1])){
        printErr(&errno);
        exit(EXIT_FAILURE);
    }

    if (!initWriterCnt(&shared_data, argv[2])){
        printErr(&errno);
        exit(EXIT_FAILURE);
    }

    char *fail_Str = NULL;

//Try to initialize semaphores
    if (!initSems(&shared_data, fail_Str)) {
        fprintf(stderr, fail_Str, strerror(errno));
        exit(EXIT_FAILURE);
    }

//Thread data structures
    reader_info *r_Array;//pointer head for reader array
    writer_info *w_Array;//pointer head for writer array
    //create reader array
    r_Array = (reader_info *)calloc(atoi(argv[1]), sizeof(reader_info));
    //create writer array
    w_Array = (writer_info *)calloc(atoi(argv[2]), sizeof(writer_info));
    //check if valid
    if (r_Array == NULL || w_Array == NULL) {
        errnum = errno;
        fprintf(stderr, "\nError Creating reader/writer arrays: %s\n", strerror(errnum));
        if (r_Array){
            free(r_Array);
        }

        if (w_Array){
            free(w_Array);
        }
        exit(EXIT_FAILURE);
    }

/******************************************************
*Run thread Processes
******************************************************/
    //create reader threads
    
/******************************************************
* Cleanup
******************************************************/
    if (r_Array){
        free(r_Array);
    }

    if (w_Array){
        free(w_Array);
    }

    return 0;
}

