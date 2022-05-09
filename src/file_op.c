//file-op.h implementation

//dependencies
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "file_op.h"
#include "defs.h"

void printErr(int *errnum) {
    fprintf(stderr, "Error: ", strerror(*errnum));
    return;
}