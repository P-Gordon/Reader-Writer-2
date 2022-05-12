//Thread-work.h implementation

#define DEBUG
//Dependencies
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "thread_work.h"
#include "defs.h"
#include "file_op.h"



struct shared_dat {

    int shared_int;
    int num_Readers;
    int num_Writers;
    sem_t read;//semaphore for readers
    sem_t block_Readers;//stop
    sem_t writing;//binary semaphore that allows a writer to write and also blocks readers
    sem_t file_IO;//allows only one process to access output file at a time

}shared_data;

void readInt(reader_info *reader_In) {
    
    return;
}

void *r_driver(void *reader_In){
    reader_info *reader = (reader_info *) reader_In;
    int ret = 0;
    ret = sem_getvalue();
    if (ret )
#ifdef DEBUG
    printf("\nReader_ID: %lu\n", reader->reader_ID);
#endif
    sleep(1);
    return ;
}

void *w_driver(void *writer_In){
    writer_info *writer = (writer_info *) writer_In;
    sleep(1);
#ifdef DEBUG
    printf("\nWriter_ID: %lu\n", writer->writer_ID);
#endif
    return NULL;
}
//spawn readers
int spawn_readers(reader_info *r_Array) {
    int ret = 0;
    //populate reader thread array, loops until it reaches size of the array
    for (int i = 0; i < sizeof(r_Array)/sizeof(reader_info*); i++) {
        ret = pthread_create(&r_Array[i].reader_ID, NULL, r_driver, (void *) &r_Array[i]);
        //check if creation was successful
        if (ret != 0) {
            return R_FAIL;
        }
    }

    return R_SUCC;
}

//spawn writers
int spawn_writers(writer_info * w_Array) {
    int ret = 0;
    //populate writer thread array, loops until it reaches size of the array
    for (int i = 0; i < sizeof(w_Array) / sizeof(writer_info*); i++) {
        ret = pthread_create(&w_Array[i].writer_ID, NULL, w_driver, (void *) &w_Array[i]);
        //check if creation was successful
        if (ret != 0) {
            return R_FAIL;
        }
    }

    return R_SUCC;
}


//rejoin threads
//both arrays are checked for the returning thread iD, skip is set to true if its found in the
//reader array r_Array so the 
/*
int rejoin_Threads(reader_info *r_Array, writer_info *w_Array, int total_readers, int total_writers) {
    int total_members = total_readers + total_writers;
    int ret = 0;
    int skip = FALSE;
    char *ret_status;
    pid_t ret_thread = 0;
    for (int i = 0; i < total_members; i++) {
        ret = pthread_join(ret_thread, &ret_status);
        if (0 != ret) {
            print_Err(&ret, "joining threads");
            return R_FAIL;
        }
        for (int x = 0; x < total_readers; x++) {
            if (r_Array[x].reader_ID == ret_thread) {
                r_Array[x].done = TRUE;
                skip = TRUE;
                break;
            }
        }
        if (FALSE == skip) {
            for (int x = 0; x < total_writers; x++) {
                if (w_Array[x].writer_ID == ret_thread) {
                    w_Array[x].done = TRUE;
                    break;
                }
            }
        }
        skip = FALSE;
    }

    return R_SUCC;
}*/