//file-op.h implementation

//dependencies
#include <stdlib.h>
#include <stdio.h>

#include "defs.h"

void print(int *errnum) {
    fprintf(stderr, "Error:    ", stderror(*errnum));
    return;
}