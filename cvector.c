#include "/opt/localc/cheaders/cvector.h"
#include "/opt/localc/cheaders/crayon.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VECTOR_ERROR            "Vector Fatal Error: "
#define VECTOR_WARN             "Vector Warning: "
#define VECTOR_DEFAULT_CAPACITY 20

static void vector_extend_cap(Vector * vector)
{
    vector_extend_capacity(vector, VECTOR_DEFAULT_CAPACITY);
}

Vector * vector_init(u64 capacity, u64 elemsize)
{
    assertf(
        capacity > 0, 
        "Vector Fatal Error: vector initial capacity must be bigger than ZERO!"
    );

    Vector * vector = malloc(sizeof(Vector));
    scp(vector);

    vector->Data = malloc(sizeof(void *) * capacity);
    scp(vector->Data);
    for (u64 i = 0; i < capacity; i++) {
        vector->Data[i] = nil;
    }

    vector->Size = 0;
    vector->Capacity = capacity;
    vector->ElemSize = elemsize;

    return vector;
}

Vector * vector_create(u64 elemsize)
{
    return vector_init(VECTOR_DEFAULT_CAPACITY, elemsize);
}

void vector_pushfront(Vector * vector, void * elem)
{
    scp(vector);

    u64 size = vector->Size;
    u64 capacity = vector->Capacity;

    if (size >= capacity) {
        // When vector is full.
        vector_extend_cap(vector);
    };

    for (int i = size; i > 0; i--) {
        vector->Data[i] = vector->Data[i-1];
    }

    if (elem) {
        vector->Data[0] = elem;
    } else {
        u64 elemsize = vector->ElemSize;
        vector->Data[0] = malloc(elemsize);
    }

    vector->Size += 1;
}

void vector_pushback(Vector * vector, void * elem)
{
    scp(vector);

    u64 size = vector->Size;
    u64 capacity = vector->Capacity;

    if (size >= capacity) {
        // When vector is full.
        vector_extend_cap(vector);
    }

    if (elem) {
        vector->Data[size] = elem;
    } else {
        u64 elemsize = vector->ElemSize;
        vector->Data[size] = malloc(elemsize);
    }

    vector->Size += 1;
}

void * vector_peekfront(Vector * vector)
{
    scp(vector);
    u64 size = vector->Size;
    if (size == 0) return nil;

    void * target = vector->Data[0];
    return target;
}

void * vector_peekback(Vector * vector)
{
    scp(vector);
    u64 size = vector->Size;
    if (size == 0) return nil;

    void * target = vector->Data[size-1];
    return target;
}

void * vector_popfront(Vector * vector)
{
    scp(vector);
    u64 size = vector->Size;
    if (size == 0) return nil;

    void * target = vector->Data[0];
    for (int i = 0; i < size; i++) {
        vector->Data[i] = vector->Data[i+1];
    }
    vector->Size -= 1;
    return target;
}

void * vector_popback(Vector * vector)
{
    scp(vector);
    u64 size = vector->Size;
    if (size == 0) return nil;

    void * target = vector->Data[size-1];
    vector->Data[size-1] = nil;
    vector->Size -= 1;
    return target;
}

void * vector_remove(Vector * vector, void * target, bool (*comp)(void*, void*))
{
    scp(vector);
    scp(target);
    u64 size = vector->Size;
    if (size == 0) return nil;

    for (u64 i = 0; i < size; i++) {
        void * elem = vector->Data[i];
        if (!comp(target, elem)) continue;

        for (int j = i; j < size; j++) {
            vector->Data[j] = vector->Data[j + 1];
        }

        return elem;
    }

    return nil;
}

void vector_free(Vector * vector, void (*freefn)(void *))
{
    if (vector == nil) return;

    u64 size = vector->Size;
    if (size > 0 && freefn != nil) {
        for (int i = 0; i < size; i++) {
            void * target = vector->Data[i];
            freefn(target);
        }
    }

    vector->Size = 0;
    vector->Capacity = 0;
    sfree(vector->Data);
    free(vector);
}

void vector_clear(Vector * vector, void (*freefn)(void *))
{
    if (vector == nil) return;

    u64 size = vector->Size;
    if (size > 0 && freefn != nil) {
        for (int i = 0; i < size; i++) {
            void * target = vector->Data[i];
            freefn(target);
        }
    }

    vector->Size = 0;
    vector->Capacity = 0;
    sfree(vector->Data);
}

void vector_easy_free(Vector * vector)
{
    if (vector == nil) return;
    
    u64 size = vector->Size;
    for (int i = 0; i < size; i++) {
        sfree(vector->Data[i]);
    }

    sfree(vector->Data);
    free(vector);
}

void vector_easy_clear(Vector * vector)
{
    if (vector == nil) return;
    
    u64 size = vector->Size;
    for (int i = 0; i < size; i++) {
        sfree(vector->Data[i]);
    }

    vector->Size = 0;
    vector->Capacity = 0;
    sfree(vector->Data);
}

u64 vector_size(Vector * vector)
{
    scp(vector);
    return vector->Size;
}
u64 vector_capacity(Vector * vector)
{
    scp(vector);
    return vector->Capacity;
}

void ** vector_ptr(Vector * vector)
{
    scp(vector);
    return vector->Data;
}

bool vector_in_bound(Vector * vector, u64 index)
{
    scp(vector);
    u64 size = vector->Size;
    return index < size;
}

void * vector_at(Vector * vector, u64 index)
{
    scp(vector);
    assertf(vector_in_bound(vector, index), VECTOR_ERROR "The index %lu is out of bound, vector size: %lu", index, vector->Size);
    return vector->Data[index];
}

void vector_extend_capacity(Vector * vector, u64 extra)
{
    scp(vector);
    vector->Capacity += extra;
    vector->Data = realloc(vector->Data, vector->Capacity * sizeof(void *));
    scp(vector->Data);
}

void vector_meta_info(Vector * vector)
{
    fprintf(stdout, "Vector Size: " BOLD "%lu\n" NOCRAYON, vector->Size);
    fprintf(stdout, "Vector Capacity: " BOLD "%lu\n" NOCRAYON, vector->Capacity);
    fprintf(stdout, "Vector's Element Size: " BOLD "%lu\n" NOCRAYON, vector->ElemSize);
}

bool vector_empty(Vector * vector)
{
    scp(vector);
    return vector->Size == 0;
}

void vector_apply(Vector * vector, void(*fn)(void*))
{
    scp(vector);
    scp(fn);
    u64 size = vector->Size;
    for (u64 i = 0; i < size; i++) {
        fn(vector->Data[i]);
    }
}
