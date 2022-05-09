//Carries out all thread operations for Reader Writers Project

#ifndef THREAD_WORK_H
#define THREAD_WORK_H

//Dependencies
#include "defs.h"

//create reader thread


//access shared_info
//stores -99 in read_int variable of reader structure on fialure
void readInt (reader_info *);
//spawn readers
int spawn_readers (reader_info *);
//spawn writers
int spawn_writers (writer_info *);


#endif