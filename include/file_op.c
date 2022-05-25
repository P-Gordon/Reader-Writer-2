//file-op.h implementation

//dependencies
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "file_op.h"
#include "defs.h"


void print_Err(int *errnum, const char *error_str) {

    fprintf(stderr, "\nError: %s %s\n",error_str, strerror(*errnum));

    return;
}

