//Thread-work.h implementation

#define DEBUG
//Dependencies
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#include "thread_work.h"
#include "defs.h"
#include "file_op.h"

void readInt(reader_info * reader_In) {
    
    return;
}

void * r_driver(void * reader_In){
    reader_info * reader = (reader_info *) reader_In;
#ifdef DEBUG
    printf("\nReader_ID: %d\n", reader.reader_ID);
#endif
    return;
}

void * w_driver(void * writer_In){
    writer_info * writer = (writer_info *) writer_In;
#ifdef DEBUG
    printf("\nWriter_ID: %d\n", writer.writer_ID);
#endif
    return;
}
//spawn readers
int spawn_readers(reader_info * r_Array) {
    int ret = NULL;
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
    int ret = NULL;
    //populate writer thread array, loops until it reaches size of the array
    for (int i = 0; i < sizeof(w_Array)/sizeof(writer_info*); i++) {
        ret = pthread_create(&w_Array[i].writer_ID, NULL, w_driver, (void *) &w_Array[i]);
        //check if creation was successful
        if (ret != 0) {
            return R_FAIL;
        }
    }

    return R_SUCC;
}