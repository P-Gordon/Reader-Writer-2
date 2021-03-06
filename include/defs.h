//All necessary "global" variables

#ifndef DEFS_H
#define DEFS_H

#include <pthread.h>
#include <semaphore.h>

//Constants
#define R_FAIL 0 //return failure code
#define R_SUCC 1 //return success code
#define BINARY_SEM 1 //binary sem value
#define STR_ERROR_BUFF_SIZE 150 //buffer size of char array for strerror_r()



//struct that holds shared data "globals"
typedef struct shared_dat shared_dat;

//structure to hold reader data 
//current number to write and thread ID
typedef struct reader_info {
    pthread_t reader_ID;
    int read_Int;
    int thread_num;
}reader_info;


//structure to hold writer thread ID and prev & current read int
typedef struct writer_info {
    pthread_t writer_ID;
    int curr_Int;
    int prev_Int;
    int thread_num;
}writer_info;

//Initialize process shared data
int init_Shared_Data(shared_dat *, const char *,const char *, char *, int *);
//Initialize #of readers and writers
//both return 0 on failure 1 on success
static int init_Reader_Cnt(const char *, int *);

static int init_Writer_Cnt(const char *, int *);

//Initialize semaphores
//Takes errno variable as a param
//Returns 0 on failure 1 on success
static int init_Sems (char *, int *);


void get_Reader_Cnt(int *);

void get_Writer_Cnt(int *);

void r_Write_To_File(reader_info *);

void w_Write_To_File(writer_info *);

int reader_Read(reader_info *);

int writer_RW(writer_info *);




/***********************************************************
 * Cleanup
 **********************************************************/
//returns 1 on success 0 on failure
int clean_Sems (char *, int *);

#endif