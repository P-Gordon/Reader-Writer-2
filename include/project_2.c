/*
*Pete Gordon
*04/21/2021
*Second Reader Writers Problem
*/

/*Main
*Takes in two command line args 1. # of readers; 2. # of writers
*To compile run make from the src directory
*Compiled with clang 10
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


//variables only global so funciton can be called to clean
reader_info *r_Array = NULL;
writer_info *w_Array = NULL;

//clean created vars
void cleanup() {
    int num_readers = 0;
    int num_writers = 0;
    int ret = 0;
    int errnum = 0;
    char *error_str = NULL;

    ret = clean_Sems(error_str, &errnum);
    if (R_FAIL == ret) {
        print_Err(&errnum, error_str);
        return;
    }

    get_Reader_Cnt(&num_readers);
    get_Writer_Cnt(&num_writers);
    //free the elements of the arrays
    //for (int i = 0; i < num_readers; i++) {
        free(r_Array);
    //}
    //for (int i = 0; i < num_writers; i++) {
        free(w_Array);
    //}
    
    return;
}


//begin main
int main (int argc ,char *argv[]) {

    if (argc != 3) {
        printf("Usage: ./project_2 #READERS #WRITERS\n");
        exit(EXIT_FAILURE);
    }

//program Variables
    shared_dat *shared_data = NULL;//shared data
    char *ptr_error_str = NULL;//holds return error messages
    int errnum = 0;//error number container
    int ret = 0;

//initialize shared data
    ret = init_Shared_Data(shared_data, argv[1], argv[2], ptr_error_str, &errnum);
    if (R_FAIL == ret) {
        print_Err(&errnum, ptr_error_str);
        exit(EXIT_FAILURE);
    }

//Thread data structures
    //pointer head for reader array
    errnum = 0;//Clear errnum
    r_Array = (reader_info *) calloc(atoi(argv[1]), sizeof(reader_info));
    if (!r_Array) {
        errnum = errno;
        ptr_error_str = "Error Creating reader/writer arrays";
        print_Err(&errnum, ptr_error_str);
        exit(EXIT_FAILURE);
    }
    //pointer head for writer array
    w_Array = (writer_info *) calloc(atoi(argv[2]), sizeof(writer_info));
    //check if valid
    if (!w_Array) {
        errnum = errno;
        ptr_error_str = "Error Creating reader/writer arrays";
        print_Err(&errnum, ptr_error_str);
        exit(EXIT_FAILURE);
    }

/******************************************************
*Run thread Processes
******************************************************/
    //create reader threads
    ret = spawn_readers(r_Array);
    if (ret == R_FAIL) {
        print_Err(&errnum, "populating reader array");
        cleanup();
        exit(EXIT_FAILURE);
    }

    ret = spawn_writers(w_Array);
    if (ret == R_FAIL) {
        print_Err(&errnum,"populating writer array");
        cleanup();
        exit(EXIT_FAILURE);
    }

//Wait for all readers and writers to return
    int num_readers = 0;
    int num_writers = 0;

    get_Reader_Cnt(&num_readers);
    get_Writer_Cnt(&num_writers);

    for (int i = 0; i < num_readers; i++) {
        ret = pthread_join(r_Array[i].reader_ID, NULL);
        if (0 != ret) {
            print_Err(&ret, "joining reader threads");
            cleanup();
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < num_writers; i++) {
        ret = pthread_join(w_Array[i].writer_ID, NULL);
        if (0 != ret) {
            print_Err(&ret, "joining writer threads");
            cleanup();
            exit(EXIT_FAILURE);
        }
    }

/******************************************************
* Cleanup
******************************************************/

    cleanup();

    return 0;
}

