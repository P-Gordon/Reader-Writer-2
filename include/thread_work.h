//Carries out all thread operations for Reader Writers Project

#ifndef THREAD_WORK_H
#define THREAD_WORK_H

//Dependencies
#include "defs.h"




//spawn readers
int spawn_readers(reader_info *);
//spawn writers
int spawn_writers(writer_info *);
//reader driver, primary routine for readers
void * r_driver(void *);
//writer driver, primary routine for writers
void * w_driver(void *);




#endif