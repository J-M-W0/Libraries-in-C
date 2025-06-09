#ifndef CMOVE_H
#define CMOVE_H

#include "/opt/localc/cheaders/assertf.h"
#include "/opt/localc/cheaders/cgeneric.h"

#define move_(ptr, size) move__(ptr, size)
#define move(...) move_(__VA_ARGS__)

void * move__(void * ptr, u64 size);

#endif // CMOVE_H
