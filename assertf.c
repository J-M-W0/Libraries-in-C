#include "/opt/localc/cheaders/assertf.h"
#include "/opt/localc/cheaders/crayon.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void errorf(char const * filename, int const currline, char const * fmt, ...) 
{
    fprintf(stderr, "(Line " BOLD YELLOW "%d" NOCRAYON " in " BOLD YELLOW "%s" NOCRAYON "): ", currline, filename);

    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);

    exit(EXIT_FAILURE);
}

void warningf(char const * filename, int const currline, char const * fmt, ...)
{
    fprintf(stderr, "(Line " BOLD YELLOW "%d" NOCRAYON " in " BOLD YELLOW "%s" NOCRAYON "): ", currline, filename);

    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
}
