//implementaiton of defnitions.h

//depenencies
#include <stdlib.h>
#include <semaphore.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
//Custom
#include "defs.h"




struct shared_dat {

    int shared_int;
    int num_Readers;
    int num_Writers;
    int reading;
    sem_t can_read;
    sem_t writing;
    sem_t curr_readers;
    sem_t file_IO;
}shared_data;


static int init_Reader_Cnt(const char *read_Num_In, int *errnum) {
    //Check if readers is numeric
    shared_data.num_Readers = atoi(read_Num_In);
    *errnum = errno;
    if (shared_data.num_Readers <= 0) {//catches the case of the user entering 0 readers
        return R_FAIL;//return failure
    }
    return R_SUCC;//return success
}

//errnum in here must be tested to make sure that it goes back to errnum 
//in project_2.c
//dereferencing errnum here may change what the previous pointer is pointing to
// and not the value of errnum in project_2.c
static int init_Writer_Cnt (const char *write_Num_In, int *errnum) {
    //Check is writers is numeric
    shared_data.num_Writers = atoi(write_Num_In);
    *errnum = errno;
    if (shared_data.num_Writers <= 0) {//catches the case of user entering 0 writers
        //return failure
        return R_FAIL;
    }
    //return success
    return R_SUCC;
}
//Initialize semaphores in the shared_data structure
//destroys the preceding initialized sems and returns with failure code if any initialization fails 
static int init_Sems (char * error_str, int *errnum) {
    if (sem_init(&shared_data.curr_readers, 0, shared_data.num_Readers) != 0) {
        *errnum = errno;
        error_str = "Failed init CURR_READERS sem... ";
        return R_FAIL;
    }
    if (sem_init(&shared_data.writing, 0, shared_data.num_Writers) !=0) {
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
    return R_SUCC;
}

int init_Shared_Data(shared_dat *ptr_shared_data, const char *reader_num,
    const char *writer_num, char *ptr_error_str, int *errnum) {

    int ret = 0;
    ptr_shared_data = &shared_data;
    ptr_shared_data->reading = 0;
    ptr_shared_data->shared_int = 0;

    ret = init_Reader_Cnt(reader_num, errnum);
    if (R_FAIL == ret) {
        return R_FAIL;
    }

    ret = init_Writer_Cnt(writer_num, errnum);
    if (R_FAIL == ret) {
        return R_FAIL;
    }

    ret = init_Sems(ptr_error_str, errnum);
    if (R_FAIL == ret) {
        return R_FAIL;
    }

    return R_SUCC;
}


int reader_Read (reader_info * reader) {
    int ret = 0;
    int value = 0;
    int errnum = 0;
    ret = sem_getvalue(shared_data.can_read, value);
    if (!ret) {
        errnum = errno;
        print_Err(&errnum, "getting value of sem CAN_READ (reader_READ)");
    }

}
/***********************************************************
 * Cleanup
 * *********************************************************/

int clean_Sems (char *ptr_error_str, int *errnum) {
    if (!sem_destroy(&shared_data.can_read)) {
        *errnum = errno;
        ptr_error_str = "closing CAN_READ sem";
        return R_FAIL;
    }
    if (!sem_destroy(&shared_data.writing)) {
        *errnum = errno;
        ptr_error_str = "closing WRITING sem";
        return R_FAIL;
    }
    if (!sem_destroy(&shared_data.file_IO)) {
        *errnum = errno;
        ptr_error_str = "closing FILE_IO sem";
        return R_FAIL;
    }
    if (!sem_destroy(&shared_data.curr_readers)) {
        *errnum = errno;
        ptr_error_str = "closing CURR_READERS sem";
        return R_FAIL;
    }

    return R_SUCC;
}