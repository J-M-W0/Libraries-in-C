#include "/opt/localc/cheaders/assertf.h"
#include "/opt/localc/cheaders/cgeneric.h"

#include <stdlib.h>
#include <string.h>

void * move__(void * ptr, u64 size) 
{
    void * newptr = malloc(size);
    memcpy(newptr, ptr, size);
    free(ptr);
    return newptr;
}

