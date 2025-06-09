#include "/opt/localc/cheaders/ckvmap.h"
#include "/opt/localc/cheaders/cgeneric.h"
#include "/opt/localc/cheaders/crayon.h"

#include <stdlib.h>
#include <string.h>



#define KVMAP_DEFAULT_SLOTS 100
#define HASH_DJB2_INITIAL_HASH 7691

static KVMapEntry * kvmap_entry_create(char const * key, void * value)
{
    KVMapEntry * entry = malloc(sizeof(KVMapEntry));
    scp(entry);

    entry->Key = strdup(key);
    entry->Value = value;
    entry->next = nil;

    return entry;
}

static void kvmap_entry_free(KVMapEntry * entry, void (*freefn)(void*))
{
    if (!entry) return;

    scp(entry->Key);
    free((void *) entry->Key);
    
    if (freefn == nil) freefn = free;
    freefn(entry->Value);

    free(entry);
}

static u64 hash_djb2(const char * key)
{
    scp(key);

    u64 hash = HASH_DJB2_INITIAL_HASH;

    char c;
    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

KVMap * kvmap_init(u64 slots) 
{
    KVMap * kvmap = malloc(sizeof(KVMap));
    scp(kvmap);
    kvmap->Entries = calloc(slots, sizeof(KVMapEntry *));
    scp(kvmap->Entries);
    kvmap->Slots = slots;
    kvmap->Size = 0;
    return kvmap;;
}

KVMap * kvmap_create()
{
    return kvmap_init(KVMAP_DEFAULT_SLOTS);
}

bool kvmap_has_key(KVMap * kvmap, const char * key)
{
    scp(kvmap);
    scp(key);

    u64 slots = kvmap->Slots;
    u64 index = hash_djb2(key) % slots;
    KVMapEntry * entry = kvmap->Entries[index];

    while (entry) {
        if (streq(entry->Key, key)) return true;
        entry = entry->next;
    }

    return false;
}

void kvmap_insert(KVMap * kvmap, char const * key, void * value)
{
    if (kvmap_has_key(kvmap, key)) return;
    scp(kvmap);
    scp(key);

    u64 slots = kvmap->Slots;
    u64 index = hash_djb2(key) % slots;
    KVMapEntry * entry = kvmap->Entries[index];

    if (entry == nil) {
        kvmap->Entries[index] = kvmap_entry_create(key, value);
    } else {
        while (entry->next) {
            entry = entry->next;
        }
        entry->next = kvmap_entry_create(key, value);
    }

    kvmap->Size += 1;
}

void kvmap_force_insert(KVMap * kvmap, char const * key, void * value, void(*freefn)(void*))
{
    scp(kvmap);
    scp(key);

    if (freefn == nil) freefn = free;

    u64 slots = kvmap->Slots;
    u64 index = hash_djb2(key) % slots;
    KVMapEntry * entry = kvmap->Entries[index];

    if (entry == nil) {
        kvmap->Entries[index] = kvmap_entry_create(key, value);
        kvmap->Size += 1;
        return;
    }

    if (streq(entry->Key, key)) {
        freefn(entry->Value);
        entry->Value = value;
        kvmap->Size += 1;
        return;
    }

    while (entry->next) {
        entry = entry->next;
        if (streq(entry->Key, key)) {
            freefn(entry->Value);
            entry->Value = value;
            kvmap->Size += 1;
            return;
        }
    }

    entry->next = kvmap_entry_create(key, value);
    kvmap->Size += 1;
}

void * kvmap_get(KVMap * kvmap, char const * key)
{
    scp(kvmap);
    scp(key);

    u64 slots = kvmap->Slots;
    u64 index = hash_djb2(key) % slots;
    KVMapEntry * entry = kvmap->Entries[index];
    while (entry) {
        if (streq(entry->Key, key)) return entry->Value;
        entry = entry->next;
    }
    return nil;
}

void * kvmap_retrieve(KVMap * kvmap, char const * key)
{
    scp(kvmap);
    scp(key);

    u64 slots = kvmap->Slots;
    u64 index = hash_djb2(key) % slots;
    KVMapEntry * entry = kvmap->Entries[index];
    if (entry == nil) return nil;

    if (streq(entry->Key, key)) {
        kvmap->Entries[index] = entry->next;
        void * value = entry->Value;
        free((void *)entry->Key);
        free(entry);
        return value;
    }

    KVMapEntry * prev = entry;
    entry = entry->next;
    while (entry) {
        if (streq(entry->Key, key)) {
            free((void *)entry->Key);
            void * value = entry->Value;
            prev->next = entry->next;
            free((void *)entry->Key);
            free(entry);
            return value;
        }
        prev = entry;
        entry = entry->next;
    }

    return nil;
}

void kvmap_remove(KVMap * kvmap, char const * key, void (*freefn)(void*))
{
    scp(kvmap);
    scp(key);
    if (freefn == nil) freefn = free;

    u64 slots = kvmap->Slots;
    u64 index = hash_djb2(key) % slots;
    KVMapEntry * entry = kvmap->Entries[index];

    if (entry == nil) return;

    if (streq(entry->Key, key)) {
        kvmap->Entries[index] = entry->next;
        KVMapEntry * temp = entry;
        kvmap_entry_free(temp, freefn);
        return;
    }

    KVMapEntry * prev = entry;
    entry = entry->next;
    while (entry) {
        if (streq(entry->Key, key)) {
            KVMapEntry * temp = entry;
            prev->next = entry->next;
            kvmap_entry_free(temp, freefn);
            return;
        }
        prev = entry;
        entry = entry->next;
    }
}

void kvmap_free(KVMap * kvmap, void (*freefn)(void*))
{
    if (kvmap == nil) return;
    if (freefn == nil) freefn = free;

    u64 slots = kvmap->Slots;
    for (u64 i = 0; i < slots; i++) {
        KVMapEntry * entry = kvmap->Entries[i];
        while (entry) {
            KVMapEntry * next = entry->next;
            freefn(entry);
            entry = next;
        }
        kvmap->Entries[i] = nil;
    }

    sfree(kvmap->Entries);
    free(kvmap);
}

void kvmap_easy_free(KVMap * kvmap)
{
    kvmap_free(kvmap, nil);
}

void kvmap_clear(KVMap * kvmap, void (*freefn)(void*))
{
    scp(kvmap);
    if (freefn == nil) freefn = free;

    u64 slots = kvmap->Slots;
    for (u64 i = 0; i < slots; i++) {
        KVMapEntry * entry = kvmap->Entries[i];
        while (entry) {
            KVMapEntry * next = entry->next;
            freefn(entry);
            entry = next;
        }
        kvmap->Entries[i] = nil;
    }
}

void kvmap_easy_clear(KVMap * kvmap)
{
    kvmap_clear(kvmap, nil);
}

u64 kvmap_size(KVMap * kvmap) 
{
    scp(kvmap);
    return kvmap->Size;
}

u64 kvmap_slots(KVMap * kvmap)
{
    scp(kvmap);
    return kvmap->Slots;
}
