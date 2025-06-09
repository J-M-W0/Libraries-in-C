#ifndef CGENERIC_H
#define CGENERIC_H

#include <stdbool.h>

#define drop(ptr)                                                   \
        do {                                                        \
            if ((ptr)) free(ptr);                                   \
        } while (0)

#define deref(ptr, type)                                            \
        (*((type *)(ptr)))

#define cast(ptr, type)                                             \
        ((type) ptr)

// Means you have to free the memory on your own.
#define owned
// Means you do not have to free the memory on your own.
#define borrowed
// Copy
#define copy

#define WHITESPACES " \t\r\n"
#define SPACE " "
#define TAB "\t"
#define CR "\r"
#define LF "\n"
#define CRLF "\r\n"
#define NEWLINE "\n"

#define new(type)                                                   \
        (malloc(sizeof(type)))

#define to_string(x)                                                \
        #x


typedef char *    string;

typedef struct _Return {
    void * Ok;
    void * Err;
} Return;
typedef struct _Option {
    bool None;
    void * Some;
} Option;



typedef char            i8;
typedef unsigned char   u8;
typedef short           i16;
typedef unsigned short  u16;
typedef int             i32;
typedef unsigned int    u32;
typedef long            i64;
typedef unsigned long   u64;
typedef unsigned long   usize;

typedef float           real32;
typedef double          real64;
typedef double          real;


bool ok(Return * res);
bool err(Return * res);
bool some(Option * option);
bool none(Option * option);

void * Ok(Return * res);
void * Err(Return * res);
void * Some(Option * option);
void * unwrap(void * p);

void set_ok(Return * res, void * value);
void set_err(Return * res, void * value);
void set_some(Option * option, void * value);
void set_none(Option * option);

#endif // CGENERIC_H
