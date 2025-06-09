#ifndef CKVMAP_H
#define CKVMAP_H

#include "/opt/localc/cheaders/assertf.h"
#include "/opt/localc/cheaders/cgeneric.h"

typedef struct _KVMapEntry KVMapEntry;
typedef struct _KVMap KVMap;
struct _KVMapEntry {
    char const *  Key;
    void *      Value;
    KVMapEntry * next;
};
struct _KVMap {
    KVMapEntry ** Entries;
    u64             Slots;
    u64              Size;
};

KVMap * kvmap_create();
KVMap * kvmap_init(u64 slots);
bool kvmap_has_key(KVMap * kvmap, const char * key);
void kvmap_insert(KVMap * kvmap, char const * key, void * value);
void kvmap_force_insert(KVMap * kvmap, char const * key, void * value, void(*freefn)(void*));
void * kvmap_get(KVMap * kvmap, char const * key);
void * kvmap_retrieve(KVMap * kvmap, char const * key);
void kvmap_remove(KVMap * kvmap, char const * key, void (*freefn)(void*));
void kvmap_free(KVMap * kvmap, void (*freefn)(void*));
void kvmap_easy_free(KVMap * kvmap);
void kvmap_clear(KVMap * kvmap, void (*freefn)(void*));
void kvmap_easy_clear(KVMap * kvmap);
u64 kvmap_size(KVMap * kvmap); 
u64 kvmap_slots(KVMap * kvmap);

#endif // CKVMAP_H
