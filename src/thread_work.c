//Thread-work.h implementation


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

//spawn readers
int spawn_readers(reader_info * r_Array) {

    for (int i = 0; i < sizeof(r_Array)/sizeof(reader_info*); i++) {
        pthread_create(&r_Array[i].reader_ID, NULL, r_driver, &r_Array[i]);
        if (r_Array[i] == NULL) {
            return R_FAIL;
        }
    }

    return R_SUCC;
}

//spawn writers
