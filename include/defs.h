//All necessary "global" variables

#ifndef DEFS_H
#define DEFS_H

#include <pthread.h>

//Constants
#define R_FAIL 0
#define R_SUCCESS 1
#define BINARY_SEM 1


//structure for shared program data
typedef struct shared_data shared_data;

//structure to hold reader data 
//current number to write and thread ID
typedef struct reader_info reader_info;

//structure to hold writer thread ID and prev & current read int
typedef struct writer_info writer_info;

//Initialize #of readers and writers
//both return 0 on failure 1 on success
int initReaderCnt(char *);

int initWriterCnt(char *);

//Initialize semaphores
//Takes errno variable as a param
//Returns 0 on failure 1 on success
int initSems (char *);

/***********************************************************
 * Cleanup
 * *********************************************************/
//returns 1 on success 0 on failure
int cleanSems ();


#endif