//implementaiton of defnitions.h

//depenencies
#include <semaphore.h>
#include <errno.h>
//Custom
#include "defs.h"


//struct that holds shared data "globals"
struct shared_data {

    int shared_int;
    int num_Readers;
    int num_Writers;
    sem_t read;//semaphore for readers
    sem_t block_Readers;//stop 
    sem_t writing;//binary semaphore that allows a writer to write and also blocks readers
    sem_t file_IO;//allows only one process to access output file at a time

}shared_data;

struct reader_info {
    thread_t reader_ID = 0;
    int read_Int = 0;
}

struct writer_info {
    pthread_t writer_ID = 0;
    int curr_Int = 0;
    int prev_Int = 0;
}

int initReaderCnt (char * read_Num_In) {
    //Check if readers is numeric
    shared_data->num_Readers = atoi(*read_Num_In);
    if (shared_data.num_Readers <= 0) {//catches the case of the user entering 0 readers
        return R_FAIL;
    }
    return R_SUCCESS;
}

int initWriterCnt (char * read_Num_In) {
    //Check is writers is numeric
    shared_data->num_Writers = atoi(*read_Num_In);
    if (shared_data->num_Writer <= 0) {//catches the case of user entering 0 writers
        return R_FAIL;
    }
    return R_SUCCESS
}

int initSems (char * error_Str) {
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
    return R_SUCCESS;
}

/***********************************************************
 * Cleanup
 * *********************************************************/

int cleanSems () {
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
    return R_SUCCESS;
}