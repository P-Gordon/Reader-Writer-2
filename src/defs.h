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
#define TRUE 1
#define FALSE 0



//struct that holds shared data "globals"
typedef struct shared_dat shared_dat;

//structure to hold reader data 
//current number to write and thread ID
typedef struct reader_info {
    pthread_t reader_ID;
    int read_Int;
    int done;
}reader_info;


//structure to hold writer thread ID and prev & current read int
typedef struct writer_info {
    pthread_t writer_ID;
    int curr_Int;
    int prev_Int;
    int done;
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

/***********************************************************
 * Cleanup
 **********************************************************/
//returns 1 on success 0 on failure
int clean_Sems (char *, int *);

#endif


int reader_Read (reader_info * reader) {
    int ret = 0;
    int value = 0;
    int errnum = 0;
#ifdef DEBUG
    printf("reader working");
#endif
    ret = sem_getvalue(&shared_data.can_read, &value);
    //If can_read is 0 (FALSE) then wait on curr_readers
    if (0 == value) {
        sem_wait(&shared_data.curr_readers);
    }

    //otherwise set reading to 1 (TRUE) and get the value of the int
    shared_data.reading = TRUE;
    reader->read_Int = shared_data.shared_int;

    //If reading can still happen (can read is 1) and there are readers leave
    //reading set otherwise clear to 0 (FALSE)
    ret = sem_getvalue(&shared_data.can_read, &value);
    if (0 != ret) {
        errnum = errno;
        print_Err(&errnum, "getting value of sem CAN_READ (reader_READ)");
        pthread_exit(NULL);
    }

    if (FALSE == value) {
        shared_data.reading = 0;
        return R_SUCC;
    }
    //checking if any readers are left
    ret = sem_getvalue(&shared_data.curr_readers, &value);
    if (0 != ret) {
        errnum = errno;
        print_Err(&errnum, "getting value of sem CURR_READERS (reader_READ)");
        pthread_exit(NULL);
    }
    //if no readers reading then set reading to 0
    if (value == shared_data.num_readers) {
        shared_data.reading = 0;
    }

    return R_SUCC;
}

static int init_Sems (char * error_str, int *errnum) {
    if (sem_init(&shared_data.curr_readers, 0, shared_data.num_readers) != 0) {
        *errnum = errno;
        error_str = "Failed init CURR_READERS sem... ";
        return R_FAIL;
    }
    if (sem_init(&shared_data.writing, 0, BINARY_SEM) !=0) {
        *errnum = errno;
        error_str = "Failed init WRITING sem... ";
        if (!sem_destroy(&shared_data.curr_readers)) {
            printf("\n\tError closing READ sem");
        }
        return R_FAIL;
    }
    if (sem_init(&shared_data.file_IO, 0, BINARY_SEM)) {
        *errnum = errno;
        error_str = "Fail init FILE_IO sem... ";
        if (!sem_destroy(&shared_data.curr_readers)) {
            printf("\n\tError closing READ sem");
        }
        if (!sem_destroy(&shared_data.writing)) {
            printf("\n\tError closing WRITING sem");
        }
        return R_FAIL;
    }
    if (sem_init(&shared_data.can_read, 0, BINARY_SEM)) {
        *errnum = errno;
        error_str = "Fail init CAN_READ sem... ";
        if (!sem_destroy(&shared_data.curr_readers)) {
            printf("\n\tError closing READ sem\n");
        }
        if (!sem_destroy(&shared_data.writing)) {
            printf("\n\tError closing WRITING sem\n");
        }
        if (!sem_destroy(&shared_data.file_IO)) {
            printf("\n\tError closing FILE_IO sem\n");
        }
    }

    pthread_mutex_init(&shared_data.leader_reader, NULL);
    pthread_mutex_init(&shared_data.reader_num_control, NULL);

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

/* void print_Err(int *errnum) {

    char *error_buffer = NULL; 
    error_buffer = (char *) malloc(STR_ERROR_BUFF_SIZE * sizeof(char));
    if (!error_buffer) {
        printf("\nERROR making strerror_r() buffer NOT FATAL"
            "(even though it should be)\n");
    }
    fprintf(stderr, "\nError %s\n", strerror_r(*errnum, error_buffer,));

    return;
}

void print_Err(const char *error_str) {

    printf("\nError: %s\n", error_str);

    return;
}
 */