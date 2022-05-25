#ifndef  GETOPTS_H
#define  GETOPTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARGPARSESTOP "--"

#define LONGOPTPREFIX "--"
#define SHORTOPTPREFIX  "-"

#define LONGOPTPREFIXLEN 2
#define SHORTOPTPREFIXLEN 1

typedef enum
{
    no_arg=0, // Flag doesn't accept an argument
    opt_arg,  // Flag accepts an argument
    req_arg,  // Flag requires an argument
} argmode_t;

typedef struct
{
    const char * longopt; // Can be NULL, if no long opt desired
    const char shortopt;  // Can be '\0', if no short opt desired
    const argmode_t argm;
    int * flag;           // Can be NULL, if no flag feedback desired
    char ** arg;          // Can be NULL, if arg is not of any use
} option_t;



typedef enum
{
    SUCCESS,           // Worked as expected
    INVALID_LONG_OPT,  // An invalid long option was encountered
    INVALID_SHORT_OPT, // An invalid short option was encountered
    INVALID_ARGCOUNT,  // A flag requiring an arg didn't get one, or a flag not accepting one got one
} getopts_codes_t;

getopts_codes_t getopts(const option_t * optarr, int * optind, int argc, char ** argv);

#endif// GETOPTS_H

