#include "/opt/localc/cheaders/cstring.h"
#include "/opt/localc/cheaders/cmove.h"
#include "/opt/localc/cheaders/assertf.h"

#include <_string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

copy bool containc(borrowed const char * s, copy const char c)
{
    while (*s != '\0') {
        if (*s == c) return true;
        s++;
    }
    return false;
}

owned char * trim(owned char * s, borrowed const char * const delims)
{
    if (s == nil) return nil;
    if (delims == nil) return s;

    u64 l_trimlen = 0;
    u64 r_trimlen = 0;
    u64 slen = strlen(s);

    while (containc(delims, s[l_trimlen])) {
        l_trimlen += 1;
    }

    if (l_trimlen != slen) {
        while (containc(delims, s[slen-1-r_trimlen])) {
            r_trimlen += 1;
        }
    }

    u64 newstrlen = slen - l_trimlen - r_trimlen;
    char * newstr = malloc(newstrlen + 1);
    scp(newstr);
    for (u64 i = 0; i < newstrlen; i++) {
        newstr[i] = s[l_trimlen + i];
    }
    newstr[newstrlen] = '\0';
    free(s);
    return newstr;
}

owned char * trim_left(owned char * s, borrowed const char * const delims)
{
    if (s == nil) return nil;
    if (delims == nil) return s;

    u64 trimlen = 0;
    u64 slen = strlen(s);
    while (containc(delims, s[trimlen])) {
        trimlen += 1;
    }

    u64 newstrlen = slen - trimlen;
    char * newstr = malloc(newstrlen + 1);
    scp(newstr);
    for (u64 i = 0; i < newstrlen; i++) {
        newstr[i] = s[trimlen + i];
    }
    newstr[newstrlen] = '\0';
    free(s);
    return newstr;
}

owned char * trim_right(owned char * s, borrowed const char * const delims)
{
    if (s == nil) return nil;
    if (delims == nil) return s;

    u64 trimlen = 0;
    u64 slen = strlen(s);
    while (containc(delims, s[slen-1-trimlen])) {
        trimlen += 1;
    }

    u64 newstrlen = slen - trimlen;
    char * newstr = malloc(newstrlen + 1);
    scp(newstr);
    for (u64 i = 0; i < newstrlen; i++) {
        newstr[i] = s[i];
    }
    newstr[newstrlen] = '\0';
    free(s);
    return newstr;
}

char * strtrim(char * s, char const delim) 
{
    if (s == nil) return nil;

    u64 l_trimlen = 0;
    u64 r_trimlen = 0;
    u64 slen = strlen(s);

    while (s[l_trimlen] == delim) {
        l_trimlen += 1;
    }

    if (l_trimlen != slen) {
        while (s[slen-1-r_trimlen] == delim) {
            r_trimlen += 1;
        }
    }

    u64 newstrlen = slen - l_trimlen - r_trimlen;
    char * newstr = malloc(newstrlen + 1);
    scp(newstr);
    for (u64 i = 0; i < newstrlen; i++) {
        newstr[i] = s[l_trimlen + i];
    }
    newstr[newstrlen] = '\0';
    free(s);
    return newstr;
}

char * strtrim_left(char * s, char const delim)
{
    if (s == nil) return nil;

    u64 trimlen = 0;
    u64 slen = strlen(s);
    while (s[trimlen] == delim) {
        trimlen += 1;
    }

    u64 newstrlen = slen - trimlen;
    char * newstr = malloc(newstrlen + 1);
    scp(newstr);
    for (u64 i = 0; i < newstrlen; i++) {
        newstr[i] = s[trimlen + i];
    }
    newstr[newstrlen] = '\0';
    free(s);
    return newstr;
}

char * strtrim_right(char * s, char const delim)
{
    if (s == nil) return nil;

    u64 trimlen = 0;
    u64 slen = strlen(s);
    while (s[slen-1-trimlen] == delim) {
        trimlen += 1;
    }

    u64 newstrlen = slen - trimlen;
    char * newstr = malloc(newstrlen + 1);
    scp(newstr);
    for (u64 i = 0; i < newstrlen; i++) {
        newstr[i] = s[i];
    }
    newstr[newstrlen] = '\0';
    free(s);
    return newstr;
}

Option strloc(char const * const s, char const c) 
{
    Option option;
    if (s == nil) {
        set_none(&option);
        return option;
    }

    u64 i = 0;
    while (s[i] != c) {
        if (s[i] == '\0') {
            set_none(&option);
            return option;
        }

        i += 1;
    }

    u64 * index = malloc(sizeof(u64));
    scp(index);
    deref(index, u64) = i;
    set_some(&option, index);
    return option;
}

Option strfind(char const * const s, char const * const tofind) 
{
    Option option;
    if (s == nil || tofind == nil) {
        set_none(&option);
        return option;
    }

    u64 len1 = strlen(s);
    u64 len2 = strlen(tofind);
    for (u64 i = 0; i + len2 <= len1; i++) {
        if (s[i] != tofind[0]) continue;

        u64 j;
        for (j = 0; j < len2; j++) {
            if (s[i+j] != tofind[j]) break;
        }
        if (j == len2) {
            u64 * index = malloc(sizeof(u64));
            scp(index);
            deref(index, u64) = i;
            set_some(&option, index);
            return option;
        }
    }

    set_none(&option);
    return option;
}

char * substr(char const * const s, u64 start, u64 end)
{
    if (s == nil) return nil;
    if (end <= start) return nil;

    u64 len = end - start;
    char * slice = malloc(len + 1);
    scp(slice);
    for (u64 i = 0; i < len; i++) {
        slice[i] = s[start + i];
    }
    slice[len] = '\0';
    return slice;
}

owned char * substring(borrowed char const * const s, copy u64 start, copy u64 sublen) 
{
    if (s == nil) return nil;
    if (sublen == 0) return nil;

    char * slice = malloc(sizeof(sublen + 1));
    scp(slice);
    for (u64 i = 0; i < sublen; i++) {
        slice[i] = s[start + i];
    }
    slice[sublen] = '\0';
    return slice;
}

char * prependc(char * s, char const c)
{
    if (s == nil || s[0] == '\0') {
        drop(s);
        char * newstr = malloc(2);
        newstr[0] = c;
        newstr[1] = '\0';
        return newstr;
    }

    u64 len             = strlen(s);
    owned char * newstr = malloc(len + 2);
    scp(newstr);

    memcpy(newstr + 1, s, len); drop(s);
    newstr[0]       =  c;
    newstr[len + 1] = '\0';
    return newstr;
}

owned char * appendc(owned char * s, char const c)
{
    if (s == nil || s[0] == '\0') {
        drop(s);
        char * newstr = malloc(2);
        newstr[0] = c;
        newstr[1] = '\0';
        return newstr;
    }

    u64 len             = strlen(s);
    owned char * newstr = strdup(s); drop(s);
    scp(newstr);

    newstr = realloc(newstr, len + 2);
    scp(newstr);

    newstr[len]     =  c;
    newstr[len + 1] = '\0';
    return newstr;
}

owned char * prependstr(owned char * s, borrowed char * substr)
{
    if (substr == nil || substr[0] == '\0') return s;

    if (s == nil || s[0] == '\0') {
        drop(s);
        return strdup(substr);
    }

    u64 len      = (s == nil) ? 0 : strlen(s);
    u64 sublen   = strlen(substr);
    u64 totallen = len + sublen;

    char * newstr = strdup(substr);
    scp(newstr);

    newstr = realloc(newstr, totallen + 1);
    scp(newstr);

    memcpy(newstr, s, len);
    newstr[totallen] = '\0';
    drop(s);
    return newstr;
}

owned char * appendstr(owned char * s, borrowed char * substr)
{
    if (substr == nil || substr[0] == '\0') return s;

    if (s == nil || s[0] == '\0') {
        drop(s);
        return strdup(substr);
    }

    u64 len      = (s == nil) ? 0 : strlen(s);
    u64 sublen   = strlen(substr);
    u64 totallen = len + sublen;

    owned char * newstr = strdup(s);
    scp(newstr);

    newstr = realloc(newstr, totallen + 1);
    scp(newstr);

    memcpy(newstr + len, substr, sublen);
    newstr[totallen] = '\0';
    drop(s);
    return newstr;
}

bool has_extension(const char * s, const char * ext)
{
    if (s == nil || s[0] == '\0') return false;

    u64 len = strlen(s);
    u64 dotindex = len-1;
    while (dotindex > 0) {
        if (s[dotindex] == '.') break;
        dotindex -= 1;
    }

    u64 extlen = strlen(ext);
    if (extlen == 0) return dotindex == 0;

    return strncmp(s + dotindex, ext, extlen);
}

bool isdecimal(char const c)
{
    return (c >= '0' && c <= '9');
}
bool ishexadecimal(char const c)
{
    return (c >= '0' && c <= '9')
        || (c >= 'a' && c <= 'f')
        || (c >= 'A' && c <= 'F');
}
bool isoctal(char const c)
{
    return (c >= '0' && c <= '7');
}
bool isbinary(char const c)
{
    return c == '0' || c == '1';
}
bool isletter(char const c)
{
    return islower(c)
        || isupper(c);
}
bool isident(char const c)
{
    return isletter(c)
        || isdecimal(c);
}
bool isidentifier(char const c)
{
    return isletter(c) 
        || isdecimal(c)
        || c == '_';
}

bool is_valid_number(char const * s)
{
    if (s == nil) return false;
    if (s[0] == '0') {
    }
    else {
    }
    return true;
}
bool is_strictly_valid_number(char const * s)
{
    if (s == nil || !isdigit(s[0])) return false;

    if (s[0] == '0') {
        switch (s[1]) 
        {
            case 'x':
            case 'X':
            {
                s += 2;
                while (ishexadecimal(*s)) s++;
            } break;

            case 'o':
            case 'O':
            {
                s += 2;
                while (isoctal(*s)) s++;
            } break;

            case 'b':
            case 'B':
            {
                s += 2;
                while (isbinary(*s)) s++;
            } break;

            case '\0':
            {
                return true;
            } break;

            default:
            {
                return false;
            } break;
        }
    }
    else {
        while (isdecimal(*s)) s++;
    }
    return *s == '\0';
}

static u64 tonumber_helper(const char * s, NumberBase base)
{
    u64 number = 0;
    switch (base) {
        case NUMBER_BASE_2:
        {
            while (isbinary(*s)) {
                number *= 2;
                number += (*s - '0');
                s++;
            }
        } break;

        case NUMBER_BASE_8:
        {
            while (isoctal(*s)) {
                number *= 8;
                number += (*s - '0');
                s++;
            }
        } break;

        case NUMBER_BASE_10:
        {
            while (isdecimal(*s)) {
                number *= 10;
                number += (*s - '0');
                s++;
            }
        } break;

        case NUMBER_BASE_16:
        {
            while (ishexadecimal(*s)) {
                number *= 16;
                if (*s >= 'a' && *s <= 'f') {
                    number += (*s - 'a' + 10);
                }
                else if (*s >= 'A' && *s <= 'F'){
                    number += (*s - 'A' + 10);
                }
                else {
                    number += (*s - '0');
                }
                s++;
            }
        } break;
    }
    return number;
}
Option tonumber(char const * s)
{
    Option option;
    if (s == nil) {
        set_none(&option);
        return option;
    }

    if (!isdigit(s[0])) {
        set_none(&option);
        return option;
    }

    bool negative = false;
    while (*s == '-' || *s == *SPACE) {
        if (*s == '-') negative = !negative;
        s++;
    }
    i64 number = 0;
    if (s[0] == '0') {
        switch (s[1]) 
        {
            case 'x':
            case 'X':
            {
                number = tonumber_helper(s + 2, NUMBER_BASE_16);
            } break;

            case 'o':
            case 'O':
            {
                number = tonumber_helper(s + 2, NUMBER_BASE_8);
            } break;

            case 'b':
            case 'B':
            {
                number = tonumber_helper(s + 2, NUMBER_BASE_2);
            } break;

            case '\0':
            {
                number = 0;
            } break;

            default:
            {
                assertf(0, "Un-recognized number base: %c!", s[1]);
            } break;
        }
    } else {
        number = tonumber_helper(s, NUMBER_BASE_10);
    }

    if (negative) number = -number;
    u64 * value = new(u64);
    deref(value, u64) = number;
    set_some(&option, move(value, sizeof(u64)));
    return option;
}

char * n2str(i64 number, NumberBase base)
{
    if (number == 0) return strdup("0");

    char * numstr = nil;

    if (number < 0) {
        number = -number;
        numstr = strdup("-");
    }
    switch (base) {
        case NUMBER_BASE_2:
        {
        } break;

        case NUMBER_BASE_8:
        {
        } break;

        case NUMBER_BASE_10:
        {
        } break;

        case NUMBER_BASE_16:
        {
        } break;
    }

    return numstr;
}

// it will also stop if reaches '\0'
owned char * cut_before_delim(borrowed char const * s, char const delim)
{
    if (s == nil || strlen(s) == 0) return strdup("");
    u64 length = 0;
    while (s[length] != '\0' && s[length] != delim) {
        length += 1;
    }

    char * substr = malloc(length + 1);
    for (u64 i = 0; i < length; i++) {
        substr[i] = s[i];
    }
    substr[length] = '\0';
    return substr;
}
// it will also stop if reaches '\0'
owned char * cut_before_delims(borrowed char const * s, char const * const delims)
{
    if (s == nil || strlen(s) == 0) return strdup("");
    u64 length = 0;
    while (s[length] != '\0' && !containc(delims, s[length])) {
        length += 1;
    }

    char * substr = malloc(length + 1);
    for (u64 i = 0; i < length; i++) {
        substr[i] = s[i];
    }
    substr[length] = '\0';
    return substr;
}

owned char * truncate_from_left(owned char * s, copy u64 count)
{
    if (count == 0) return s;
    

    if (strlen(s) <= count) {
        free(s);
        return nil;
    }


    u64 length = strlen(s) - count;
    char * newstr = malloc(length + 1); scp(newstr);
    memcpy(newstr, s + count, length);
    newstr[length] = '\0';
    

    free(s); return newstr;
}
