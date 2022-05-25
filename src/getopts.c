#include <getopts.h>

int startsWith(const char *pre, const char *str)
{
    size_t lenpre = strlen(pre),
           lenstr = strlen(str);
    return lenstr < lenpre ? 1 : memcmp(pre, str, lenpre * sizeof(char)) == 0;
}

const option_t * searchoptlong(const option_t * optarr, const char * str)
{
    for(size_t i = 0; *(int *)(optarr + i); i++)
    {
        if(optarr[i].longopt == NULL)
	    continue;

        if(strcmp(str, optarr[i].longopt) == 0)
            return optarr + i;
    }
    return NULL;
}

const option_t * searchoptshort(const option_t * optarr, const char c)
{
    for(size_t i = 0; *(int *)(optarr + i); i++)
    {
        if(optarr[i].shortopt == '\0')
	    continue;

	if(optarr[i].shortopt == c)
	    return optarr + i;
    }
    return NULL;
}

getopts_codes_t getopts(const option_t * optarr, int * optind, int argc, char ** argv)
{
    for(size_t i = 0; *(int *)(optarr + i); i++)
    {
        if(optarr[i].flag)
	    *optarr[i].flag = 0;
	if(optarr[i].arg)
	    *optarr[i].arg = "";
    }

    *optind = 1;
    while (1)
    {
        char * arg = argv[*optind];

        if(*optind >= argc)
	    return SUCCESS;

        if(strcmp(arg, ARGPARSESTOP) == 0)
	{
	    (*optind)++;
	    return SUCCESS;
	}

	if(startsWith(LONGOPTPREFIX, arg)) // long option
	{
            char * newarg = strcpy(
			    (char *) malloc((strlen(arg) + 1 - LONGOPTPREFIXLEN) * sizeof(char)),
			    arg + LONGOPTPREFIXLEN
			    );// Remove prefix

            char * value = strchr(newarg, '=') + 1;
	    char * key = (value != ((char *)1)) ?
		    strncpy((char *) malloc((value - newarg - 1) * sizeof(char)), newarg, value - newarg - 1) :
		    newarg;

           const option_t * opt = searchoptlong(optarr, key);

	   if(opt == NULL) // Invalid flag
	   {
               fprintf(stderr, "There is no long option called \"%s\"!\n", key);
	       return INVALID_LONG_OPT;
	   }

	   if(opt->argm == no_arg && value != ((char *)1)) // No arg allowed
	   {
               fprintf(stderr, "\"%s\" doesn't allow an argument, but got \"%s\"!\n", key, value);
	       return INVALID_ARGCOUNT;
	   }

	   if(opt->argm == req_arg && value == ((char *)1)) // Arg required
	   {
               fprintf(stderr, "\"%s\" requires an argument!\n", key);
	       return INVALID_ARGCOUNT;
	   }

	   if(opt->flag != NULL)
	       *opt->flag = 1;

	   if(opt->arg != NULL && value != ((char *)1))
	       *opt->arg = strcpy((char *) malloc((strlen(value) + 1) * sizeof(char)), value);

	    free(key);
	    if(value != ((char *)1))
	        free(newarg);
	}
	else if(startsWith(SHORTOPTPREFIX, arg)) // short option
	{
            char * value = strcpy(
			    (char *) malloc((strlen(arg) + 2 - SHORTOPTPREFIXLEN) * sizeof(char)),
			    arg + 1 + SHORTOPTPREFIXLEN
			    );// Remove prefix and option
	    char key = arg[SHORTOPTPREFIXLEN];

	    const option_t * opt = searchoptshort(optarr, key);
	    if(opt == NULL) // Invalid flag
	    {
                fprintf(stderr, "There is no short option called \"%c\"!\n", key);
		return INVALID_SHORT_OPT;
	    }

	    if(opt->argm == no_arg && strcmp(value, "") != 0)
	    {
                fprintf(stderr, "\"%c\" doesn't allow an argument, but got \"%s\"!\n", key, value);
		return INVALID_ARGCOUNT;
	    }

	    if(opt->argm == req_arg && strcmp(value, "") == 0)
	    {
                fprintf(stderr, "\"%c\" requires an argument!\n", key);
		return INVALID_ARGCOUNT;
	    }

	    if(opt->flag != NULL)
	        *opt->flag = 1;

	    if(opt->arg != NULL)
		*opt->arg = strcpy((char *) malloc((strlen(value) + 1) * sizeof(char)), value);

	    free(value);
	}
	else // stop parsing
	    return SUCCESS;

	(*optind)++;
    }
    return SUCCESS;
}

