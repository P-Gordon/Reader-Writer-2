//implementaiton of defnitions.h

//depenencies
#include <stdlib.h>
#include <semaphore.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
//Custom
#include "defs.h"
#include "file_op.h"





struct shared_dat {

    int shared_int;
    int num_readers;
    int num_writers;
    int reading;
    sem_t can_read;
    sem_t writing;
    sem_t curr_readers;
    sem_t file_IO;
    FILE *file_io_ptr;
    sem_t leader_reader;
    sem_t reader_num_control;

}shared_data;


static int init_Reader_Cnt(const char *read_Num_In, int *errnum) {
    //Check if readers is numeric
    shared_data.num_readers = atoi(read_Num_In);
    *errnum = errno;
    if (shared_data.num_readers <= 0) {//catches the case of the user entering 0 readers
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
    shared_data.num_writers = atoi(write_Num_In);
    *errnum = errno;
    if (shared_data.num_writers <= 0) {//catches the case of user entering 0 writers
        //return failure
        return R_FAIL;
    }
    //return success
    return R_SUCC;
}
//Initialize semaphores in the shared_data structure
//destroys the preceding initialized sems and returns with failure code if any initialization fails 
static int init_Sems (char * error_str, int *errnum) {
    sem_init(&shared_data.curr_readers, 0, shared_data.num_readers);
    sem_init(&shared_data.writing, 0, BINARY_SEM);
    sem_init(&shared_data.file_IO, 0, BINARY_SEM);
    sem_init(&shared_data.can_read, 0, BINARY_SEM);
    sem_init(&shared_data.leader_reader, 0, BINARY_SEM);
    sem_init(&shared_data.reader_num_control, 0, BINARY_SEM);

    return R_SUCC;
}

int init_Shared_Data(shared_dat *ptr_shared_data, const char *reader_num,
    const char *writer_num, char *ptr_error_str, int *errnum) {

    int ret = 0;
    ptr_shared_data = &shared_data;
    ptr_shared_data->reading = 0;
    ptr_shared_data->shared_int = 0;
    ptr_shared_data->file_io_ptr = fopen("Output.txt", "w+");

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

    sem_wait(&shared_data.curr_readers);
    sem_getvalue(&shared_data.can_read, &value);
    if (value == 0) {
        sem_wait(&shared_data.can_read);

        if (!sem_trywait(&shared_data.leader_reader)) {
            sem_getvalue(&shared_data.curr_readers, &value);
            for (int i = 0; i <= value; i++) {
                sem_post(&shared_data.can_read);
            }
            sem_post(&shared_data.leader_reader);
        }
    }

    sem_wait(&shared_data.reader_num_control);
    shared_data.reading++;
    sem_post(&shared_data.reader_num_control);
    reader->read_Int = shared_data.shared_int;
    sem_wait(&shared_data.reader_num_control);
    shared_data.reading--;
    sem_post(&shared_data.reader_num_control);


    sem_post(&shared_data.curr_readers);
    
    return R_SUCC;
}

int writer_RW (writer_info * writer) {

    int value = 0; //used to hold values of sems from sem_getvalue()
    int ret = 0; //used to check return errors

    sem_wait(&shared_data.writing);

    sem_trywait(&shared_data.can_read);
    //If can read is 1 (True) set it to false by doing sem_wait
    //make timer structer for sleeping until readers done

    while (0 != shared_data.reading) {}

    writer->prev_Int = writer->curr_Int;
    writer->curr_Int = shared_data.shared_int;
    shared_data.shared_int++;

    sem_getvalue(&shared_data.writing, &value);
    if (value == 0) {
        sem_getvalue(&shared_data.can_read, &value);
        if (value == 0) {
            sem_post(&shared_data.can_read);
        }
    }
    sem_post(&shared_data.writing);

    return R_SUCC;
}


void get_Reader_Cnt (int * reader_num) {
    *reader_num = shared_data.num_readers;
    return;
}

void get_Writer_Cnt (int * writer_num) {
    *writer_num = shared_data.num_writers;
    return;
}

void r_Write_To_File (reader_info * reader) {

    sem_wait(&shared_data.file_IO);

    fprintf(shared_data.file_io_ptr, "Reader %d: %lu, Read Num: %d\n",
        reader->thread_num, reader->reader_ID, reader->read_Int);
    //printf("Reader: %lu, Read Num: %d\n",reader->reader_ID, reader->read_Int);

    sem_post(&shared_data.file_IO);

    return;
}

void w_Write_To_File (writer_info * writer) {

    sem_wait(&shared_data.file_IO);

    fprintf(shared_data.file_io_ptr, "Writer %d: %lu, Prev Num: %d, Curr Num: %d\n",
        writer->thread_num, writer->writer_ID, writer->prev_Int, writer->curr_Int);

    sem_post(&shared_data.file_IO);

    return;
}
/***********************************************************
 * Cleanup
 * *********************************************************/

int clean_Sems (char *ptr_error_str, int *errnum) {
    if (0 != sem_destroy(&shared_data.can_read)) {
        *errnum = errno;
        ptr_error_str = "closing CAN_READ sem";
        return R_FAIL;
    }
    if (0 != sem_destroy(&shared_data.writing)) {
        *errnum = errno;
        ptr_error_str = "closing WRITING sem";
        return R_FAIL;
    }
    if (0 != sem_destroy(&shared_data.file_IO)) {
        *errnum = errno;
        ptr_error_str = "closing FILE_IO sem";
        return R_FAIL;
    }
    if (0 != sem_destroy(&shared_data.curr_readers)) {
        *errnum = errno;
        ptr_error_str = "closing CURR_READERS sem";
        return R_FAIL;
    }

    fclose(shared_data.file_io_ptr);

    sem_destroy(&shared_data.leader_reader);
    sem_destroy(&shared_data.reader_num_control);

    return R_SUCC;
}

