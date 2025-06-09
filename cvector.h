#ifndef CVECTOR_H
#define CVECTOR_H

#include "/opt/localc/cheaders/assertf.h"
#include "/opt/localc/cheaders/cgeneric.h"

#include <stdbool.h>

typedef struct _Vector Vector;
struct _Vector {
    void ** Data;
    u64 Size;
    u64 Capacity;
    u64 ElemSize;
};

Vector * vector_init(u64 capacity, u64 elemsize);
Vector * vector_create(u64 elemsize);
void vector_pushfront(Vector * vector, void * elem);
void vector_pushback(Vector * vector, void * elem);
void * vector_peekfront(Vector * vector);
void * vector_peekback(Vector * vector);
void * vector_popfront(Vector * vector);
void * vector_popback(Vector * vector);
void * vector_remove(Vector * vector, void * target, bool (*comp)(void*, void*));
void vector_free(Vector * vector, void (*freefn)(void *));
void vector_clear(Vector * vector, void (*freefn)(void *));
void vector_easy_free(Vector * vector);
void vector_easy_clear(Vector * vector);
u64 vector_size(Vector * vector);
u64 vector_capacity(Vector * vector);
void ** vector_ptr(Vector * vector);
bool vector_in_bound(Vector * vector, u64 index);
void * vector_at(Vector * vector, u64 index);
void vector_extend_capacity(Vector * vector, u64 extra);
void vector_meta_info(Vector * vector);
bool vector_empty(Vector * vector);
void vector_apply(Vector * vector, void(*fn)(void*));

#endif // CVECTOR_H
