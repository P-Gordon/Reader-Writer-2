//implementaiton of defnitions.h

//depenencies
#include <semaphore.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
//Custom
#include "defs.h"



int initReaderCnt (shared_dat * shared_data,const char * read_Num_In) {
    //Check if readers is numeric
    shared_data->num_Readers = atoi(*read_Num_In);
    if (shared_data->num_Readers <= 0) {//catches the case of the user entering 0 readers
        return R_FAIL;//return failure
    }
    return R_SUCC;//return success
}

int init_Writer_Cnt (shared_dat * shared_data,const char * write_Num_In) {
    //Check is writers is numeric
    shared_data->num_Writers = atoi(*write_Num_In);
    if (shared_data->num_Writers <= 0) {//catches the case of user entering 0 writers
        //return failure
        return R_FAIL;
    }
    //return success
    return R_SUCC;
}
//Initialize semaphores in the shared_data structure
//destroys the preceding initialized sems and returns with failure code if any initialization fails 
int init_Sems (shared_dat *shared_data, char * error_Str) {
    if (sem_init(&shared_data->read, 0, shared_data->num_Readers) != 0) {
        error_Str = "Fail init READ sem... ";
        return R_FAIL;
    }
    if (sem_init(&shared_data->writing, 0, shared_data->num_Writers) !=0) {
        error_Str = "Fail init WRITING sem... ";
        if (!sem_destroy(&shared_data->read)) {
            printf("\nError closing READ sem");
        }
        return R_FAIL;
    }
    if (sem_init(&shared_data->file_IO, 0, BINARY_SEM)) {
        error_Str = "Fail init FILE_IO sem... ";
        if (!sem_destroy(&shared_data->read)) {
            printf("\nError closing READ sem");
        }
        if (!sem_destroy(&shared_data->writing)) {
            printf("\nError closing WRITING sem");
        }
        return R_FAIL;
    }
    return R_SUCC;
}

/***********************************************************
 * Cleanup
 * *********************************************************/

int cleanSems (shared_dat * shared_data) {
    if (!sem_destroy(&shared_data->read)) {
        printf("\nError closing READ sem");
        return R_FAIL;
    }
    if (!sem_destroy(&shared_data->writing)) {
        printf("\nError closing WRITING sem");
        return R_FAIL;
    }
    if (!sem_destroy(&shared_data->file_IO)) {
        printf("\nError closing WRITING sem");
        return R_FAIL;
    }
    return R_SUCC;
}