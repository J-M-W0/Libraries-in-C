#ifndef CSTRING_H
#define CSTRING_H

#include "/opt/localc/cheaders/assertf.h"
#include "/opt/localc/cheaders/cgeneric.h"

typedef enum
{
    NUMBER_BASE_2 = 2,
    NUMBER_BASE_8 = 8,
    NUMBER_BASE_10 = 10,
    NUMBER_BASE_16 = 16,
} NumberBase;

typedef struct _String {
    char const * Body;
    u64 Capacity;
    u64 Size;
} String;

copy bool containc(borrowed const char * s, copy const char c);
owned char * trim(owned char * s, borrowed const char * const delims);
owned char * trim_left(owned char * s, borrowed const char * const delims);
owned char * trim_right(owned char * s, borrowed const char * const delims);

char * strtrim(char * s, char const delim);
char * strtrim_left(char * s, char const delim);
char * strtrim_right(char * s, char const delim);
Option strloc(char const * const s, char const c);
Option strfind(char const * const s, char const * const tofind);

/**
 * @name substr
 * @parm s          The original string to get a substring.
 *                  Cannot be nil string.
 * @param start     The start index (inclusive).
 * @param end       The end index (exclusive).
 * @return          The from heap allocated slice of string.
 *                  nil string on error.
 *
 * @restrict        The {s} cannot be a nil string.
 *                  The {end} must be strictly greater than the {start}.
 * */

char * substr(char const * const s, u64 start, u64 end);

/** 
 * @name substring
 * @parm s          The original string to get a substring.
 *                  Cannot be nil string.
 * @param start     The start index.
 * @param sublen    The length of the substring.
 *                  Cannot be 0.
 * @return          The from heap allocated slice of string.
 *                  nil string on error.
 * */
char * substring(char const * const s, u64 start, u64 sublen);

bool isdecimal(char const c);
bool ishexadecimal(char const c);
bool isoctal(char const c);
bool isbinary(char const c);
bool isletter(char const c);
bool isident(char const c);
bool isidentifier(char const c);
bool is_valid_number(char const * s);
bool is_strictly_valid_number(char const * s);
Option tonumber(char const * s);
char * n2str(i64 number, NumberBase base);
char * prependc(char * s, char const c);
char * appendc(char * s, char const c);
char * prependstr(char * s, char * substr);
char * appendstr(char * s, char * substr);
bool has_extension(const char * s, const char * ext);

// Reminder: {s} here will not be freed.
char * cut_before_delim(char const * s, char const delim);
char * cut_before_delims(borrowed char const * s, char const * const delims);

owned char * truncate_from_left(owned char * s, copy u64 count);

#endif // CSTRING_H
