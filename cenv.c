#include "/opt/localc/cheaders/assertf.h"
#include "/opt/localc/cheaders/cstring.h"
#include "/opt/localc/cheaders/ckvmap.h"
#include "/opt/localc/cheaders/crayon.h"
#include "/opt/localc/cheaders/cenv.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

typedef struct _CEnv CEnv;
struct _CEnv {
    FILE * File;
    char const * FileName;
    KVMap * Config;
};

CEnv * cenv;

static void cenv_warning(char const * fmt, ...)
{
    fprintf(stderr, "CENV Warning: ");
    fprintf(stderr, YELLOW);

    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);

    fprintf(stderr, "\n" NOCRAYON);
}

static void cenv_free(CEnv * cenv)
{
    if (cenv == nil) return;

    if (cenv->File) {
        assertf(fclose(cenv->File) == 0, "");
    }

    if (cenv->FileName) {
        free((void *)cenv->FileName);
    }
    
    if (cenv->Config) {
        kvmap_easy_free(cenv->Config);
    }

    free(cenv);
}

static Return cenv_parse()
{
    Return fin;
    if (cenv == nil) {
        CEnvErr * err = malloc(sizeof(CEnvErr));
        deref(err, CEnvErr) = CENV_FAILED_TO_PARSE;
        set_err(&fin, err);
        return fin;
    }

    if (cenv->File == nil) {
        CEnvErr * err = malloc(sizeof(CEnvErr));
        deref(err, CEnvErr) = CENV_PARSE_NO_FILE;
        set_err(&fin, err);
        return fin;
    }

    if (cenv->FileName == nil) {
        CEnvErr * err = malloc(sizeof(CEnvErr));
        deref(err, CEnvErr) = CENV_PARSE_NO_FILE_NAME;
        set_err(&fin, err);
        return fin;
    }

    if (cenv->Config == nil) {
        CEnvErr * err = malloc(sizeof(CEnvErr));
        deref(err, CEnvErr) = CENV_PARSE_NO_CONFIG;
        set_err(&fin, err);
        return fin;
    }

    KVMap * config = cenv->Config;

    char * line = nil;
    u64 linecap = 0;
    i64 linelen;
    while ((linelen = getline(&line, &linecap, cenv->File)) > 0) {
        if (line == nil) continue;

        char * linecopy = strdup(line);
        linecopy = strtrim_right(linecopy, '\n');
        linecopy = strtrim(linecopy, ' ');
        if (strlen(linecopy) == 0) {
            free(linecopy);
            continue;
        }


        Option option = strloc(linecopy, '=');
        if (none(&option)) {
            free(linecopy);

            CEnvErr * err = malloc(sizeof(CEnvErr));
            deref(err, CEnvErr) = CENV_PARSE_BAD_FORMAT;
            set_err(&fin, err);
            return fin;
        }

        u64 eqindex = deref(Some(&option), u64); free(Some(&option));
        char * key = substr(linecopy, 0, eqindex);
        char * value = substr(linecopy, eqindex+1, strlen(line));
        if (key == nil || value == nil) {
            free(linecopy);
            sfree(key);
            sfree(value);

            CEnvErr * err = malloc(sizeof(CEnvErr));
            deref(err, CEnvErr) = CENV_PARSE_BAD_FORMAT;
            set_err(&fin, err);
            return fin;
        }

        kvmap_force_insert(config, key, (void *)value, free);
        free(key);
        free(linecopy);
    }
    if (line) free(line);

    set_ok(&fin, cenv->Config);
    return fin;
}

static Return cenv_config(char const * filename)
{
    Return result;
    if (filename == nil) {
        filename = ".env";
        cenv_warning("No environment explicitly provided, using" BOLD " .env " NOCRAYON YELLOW "by default.");
    }

    FILE * file = fopen(filename, "r");
    if (file == nil) {
        CEnvErr * err = malloc(sizeof(CEnvErr));
        deref(err, CEnvErr) = CENV_FAILED_TO_OPEN_ENVFILE;
        set_err(&result, err);
        return result;
    }

    if (cenv) cenv_free(cenv);
    cenv = malloc(sizeof(CEnv));
    if (cenv == nil) {
        CEnvErr * err = malloc(sizeof(CEnvErr));
        deref(err, CEnvErr) = CENV_FAILED_tO_ALLOC_ENV;
        set_err(&result, err);
        return result;
    }

    cenv->File = file;
    cenv->FileName = strdup(filename);
    cenv->Config = kvmap_create();


    Return parsedone= cenv_parse();
    if (err(&parsedone)) {
        return parsedone;
    }

    set_ok(&result, cenv);
    return result;
}

void cenv_display()
{
    if (cenv == nil) return;
    
    if (cenv->FileName) {
        fprintf(stdout, "Environment file name: %s\n", cenv->FileName);
    }

    if (cenv->Config == nil) return;

    KVMap * config = cenv->Config;
    u64 slots = config->Slots;
    for (u64 slot = 0; slot < slots; slot++) {
        if (config->Entries[slot] == nil) continue;
        KVMapEntry * entry = config->Entries[slot];
        while (entry) {
            const char * const key = entry->Key;
            const char * const value = entry->Value;
            fprintf(stdout, "%s=%s\n", key, value);
            entry = entry->next;
        }
    }
}

void cenv_diagnostics(CEnvErr * cenverr)
{
    if (cenverr == nil) return;
    switch (*cenverr) {
        case CENV_PARSE_NO_FILE:
        {
            cenv_warning("No file to parse.");
        } break;

        case CENV_FAILED_TO_PARSE:
        {
            cenv_warning("Failed to parse.");
        } break;

        case CENV_PARSE_NO_CONFIG:
        {
            cenv_warning("No parsing configuration.");
        } break;

        case CENV_PARSE_BAD_FORMAT:
        {
            cenv_warning("File %s has bad format.", cenv->FileName);
        } break;

        case CENV_PARSE_NO_FILE_NAME:
        {
            cenv_warning("File name not provided.");
        } break;

        case CENV_FAILED_tO_ALLOC_ENV:
        {
            cenv_warning("Failed to allocate new environment structure.");
        } break;

        case CENV_FAILED_TO_OPEN_ENVFILE:
        {
            cenv_warning("Could not find environment file %s", cenv->FileName);
        } break;

        default:
        {
            cenv_warning("Unknown error: %d", *cenverr);
        } break;
    }
}

Return cenv_init(char const * const filename)
{
    return cenv_config(filename);
}

bool cenv_exist()
{
    return cenv != nil;
}

Option cenv_get(char const * const key)
{
    Option option;

    if (cenv == nil || cenv->Config == nil) {
        set_none(&option);
        return option;
    }
    if (key == nil || strlen(key) == 0) {
        set_none(&option);
        return option;
    }

    KVMap * config = cenv->Config;
    if (!kvmap_has_key(config, key)) {
        set_none(&option);
        return option;
    }
    void * value = kvmap_get(config, key);
    set_some(&option, strdup(value));
    return option;
}

void cenv_end()
{
    cenv_free(cenv);
}
