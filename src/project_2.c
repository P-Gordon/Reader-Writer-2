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
    //pointer head for reader array
    reader_info * r_Array = (reader_info *)calloc(atoi(argv[1]), sizeof(reader_info));
    if (!r_Array) {
        fprintf(stderr, "\nError Creating reader/writer arrays: %s\n", strerror(errnum));
        exit(EXIT_FAILURE);
    }
    //pointer head for writer array
    writer_info * w_Array = (writer_info *)calloc(atoi(argv[2]), sizeof(writer_info));
    //check if valid
    if (!w_Array) {
        fprintf(stderr, "\nError Creating reader/writer arrays: %s\n", strerror(errnum));
        free(r_Array);
        exit(EXIT_FAILURE);
    }

/******************************************************
*Run thread Processes
******************************************************/
    //create reader threads
    int ret = spawn_readers(r_Array);
    if (ret == R_FAIL) {
        fprintf(stderr, "\nError populating reader array: %s\n", strerror(errnum));
        if (r_Array){
            free(r_Array);
        }

        if (w_Array){
            free(w_Array);
        }
        exit(EXIT_FAILURE);
    }

    ret = spawn_writers(w_Array);
    if (ret == R_FAIL){
        fprintf(stderr, "\nError populating writer array: %s\n", strerror(errnum));
        if (r_Array){
            free(r_Array);
        }

        if (w_Array){
            free(w_Array);
        }
        exit(EXIT_FAILURE);
    }


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

