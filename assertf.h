#include "crayon.h"
#include <stdbool.h>

#define nil 0

#define assertf(expr, ...)                                          \
        do {                                                        \
            if (!(expr)) errorf(__FILE__, __LINE__, __VA_ARGS__);   \
        } while (0)

#define abortif(expr)                                                           \
        do {                                                                    \
            if ((expr)) errorf(__FILE__, __LINE__, BOLD RED #expr NOCRAYON );   \
        } while (0)

#define cassert(expr)                                                           \
        do {                                                                    \
            if (!(expr)) errorf(__FILE__, __LINE__, BOLD RED #expr NOCRAYON );  \
        } while (0)

#define debug(msg)                                                              \
        warningf(__FILE__, __LINE__, BOLD YELLOW #msg NOCRAYON )

// scp - Security Check for Pointers.
#define scp(ptr)                                                    \
        assertf(ptr != nil, "intolerable" BOLD " nil " NOCRAYON "pointer at" ITALIC " %s()" NOCRAYON, __FUNCTION__)

// sfree - Secure free
#define sfree(ptr)                                                  \
        do {                                                        \
            if ((ptr)) free(ptr);                                   \
            ptr = nil;                                              \
        } while (0)

#define streq(s1, s2)                                               \
        ((s1) && (s2) && (strcmp(s1, s2) == 0))

void errorf(char const * filename, int const currline, char const * fmt, ...);
void warningf(char const * filename, int const currline, char const * fmt, ...);
