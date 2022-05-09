//All necessary "global" variables

#ifndef DEFS_H
#define DEFS_H

#include <pthread.h>
#include <semaphore.h>

//Constants
#define R_FAIL 0 //return failure code
#define R_SUCC 1 //return success code
#define BINARY_SEM 1 //binary sem value




//struct that holds shared data "globals"
typedef struct shared_dat {

    int shared_int;
    int num_Readers;
    int num_Writers;
    sem_t read;//semaphore for readers
    sem_t block_Readers;//stop
    sem_t writing;//binary semaphore that allows a writer to write and also blocks readers
    sem_t file_IO;//allows only one process to access output file at a time

}shared_dat;


//structure to hold reader data 
//current number to write and thread ID
typedef struct reader_info {
    pthread_t reader_ID;
    int read_Int;
}reader_info;


//structure to hold writer thread ID and prev & current read int
typedef struct writer_info {
    pthread_t writer_ID;
    int curr_Int;
    int prev_Int;
}writer_info;

//Initialize #of readers and writers
//both return 0 on failure 1 on success
int initReaderCnt(shared_dat *,char *);

int initWriterCnt(shared_dat *,char *);

//Initialize semaphores
//Takes errno variable as a param
//Returns 0 on failure 1 on success
int initSems (shared_dat *,char *);

/***********************************************************
 * Cleanup
 **********************************************************/
//returns 1 on success 0 on failure
int cleanSems (shared_dat *);


#endif