//file-op.h implementation

//dependencies
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "file_op.h"
#include "defs.h"

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
void print_Err(int *errnum, const char *error_str) {

    fprintf(stderr, "\nError: %s %s\n",error_str, strerror(*errnum));

    return;
}