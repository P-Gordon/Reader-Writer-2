//Thread-work.h implementation

//Dependencies
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "thread_work.h"
#include "defs.h"
#include "file_op.h"





void *r_driver(void *reader_In){
    reader_info *reader = (reader_info *) reader_In;

#ifdef DEBUG
    printf("\nReader_ID: %lu\n", reader->reader_ID);
#endif
    for (int i = 0; i < 10; i++) {
        reader_Read(reader);
        r_Write_To_File(reader);

        sleep(1);
    }
    pthread_exit(NULL);
}

void *w_driver(void *writer_In){
    writer_info *writer = (writer_info *) writer_In;

#ifdef DEBUG
    printf("\nWriter_ID: %lu\n", writer->writer_ID);
#endif
    for (int i = 0; i < 10; i++) {
        writer_RW(writer);
        w_Write_To_File(writer);

        sleep(1);
    }
    pthread_exit(NULL);
}
//spawn readers
int spawn_readers(reader_info *r_Array) {
    int ret = 0;
    int num_readers = 0;
    get_Reader_Cnt(&num_readers);
    //populate reader thread array, loops until it reaches size of the array
    for (int i = 0; i < num_readers; i++) {
        r_Array[i].thread_num = i + 1;
        r_Array[i].read_Int = 0;
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
    int num_writers = 0;
    get_Writer_Cnt(&num_writers);
    //populate writer thread array, loops until it reaches size of the array
    for (int i = 0; i < num_writers; i++) {
        w_Array[i].thread_num = i + 1;
        w_Array[i].curr_Int = 0;
        w_Array[i].prev_Int = 0;
        ret = pthread_create(&w_Array[i].writer_ID, NULL, w_driver, (void *) &w_Array[i]);
        //check if creation was successful
        if (ret != 0) {
            return R_FAIL;
        }
    }

    return R_SUCC;
}


