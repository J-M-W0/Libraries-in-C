#ifndef CENV_H
#define CENV_H

#include <stdio.h>
#include "/opt/localc/cheaders/ckvmap.h"
#include "/opt/localc/cheaders/cgeneric.h"

/** cenv - a C library for environment file parsing.
 * */

typedef enum _CEnvErr CEnvErr;
enum _CEnvErr {
    CENV_FAILED_TO_OPEN_ENVFILE,
    CENV_FAILED_tO_ALLOC_ENV,
    CENV_FAILED_TO_PARSE,
    CENV_PARSE_NO_FILE,
    CENV_PARSE_NO_FILE_NAME,
    CENV_PARSE_NO_CONFIG,
    CENV_PARSE_BAD_FORMAT,
};

Return cenv_init(char const * const filename);
Option cenv_get(char const * const key);
bool cenv_exist();
void cenv_end();
void cenv_display();
void cenv_diagnostics(CEnvErr * cenverr);

#endif // CENV_H
