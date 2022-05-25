#include <getopts.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define VERSION "v1.1.1"

static bool autoexit_f = false;
static bool help_f = false;
static bool version_f = false;

static const option_t options[] = {
/* long		short	arg	flag			argbuffer  */
  { "autoexit",	'a',	no_arg,	(int *)&autoexit_f,	NULL },
  { "help",	'h',	no_arg,	(int *)&help_f,		NULL },
  { "version",	'v',	no_arg,	(int *)&version_f,	NULL },

  { NULL,	'\0',	no_arg,	NULL,			NULL }
};

void usage(void);
void version(void);

int main(int argc, char** argv) {
  int option_index;
  if(getopts(options, &option_index, argc, argv) != SUCCESS)
    usage();

  if(help_f) // Show help page
    usage();
    
  if(version_f) // Show version
    version();

  if(option_index == argc) // No futher args -> Show help page
    usage();

  size_t buffer_size = 0;
  
  // Get amount of buffer
  for(size_t i = option_index; i < argc; ++i)
    buffer_size += (strlen(argv[i]) + 1) * sizeof(char);

  // Alloc buffer
  char* buffer_str = (char *)malloc(buffer_size);

  for(size_t i = option_index; i < argc; ++i) {
    strcat(buffer_str, argv[i]);
    if((i + 1) < argc) strcat(buffer_str, " ");
  }

  // Exec program
  const time_t before = time(NULL);
  const int exit_code = system(buffer_str);
  const time_t after = time(NULL);

  printf("\nProcess exited with code %d (0x%X); took %lu seconds\n", exit_code, exit_code, after - before);

  if(!autoexit_f)
    getchar();
  
  free(buffer_str);
  return 0;
}

void usage(void)
{
  puts(
          "Usage: coderunner <command> <command-args>\n"
          "\n"
	  "Options:\n"
	  "-a, --autoexit\tDon't pause after <command> exits\n"
	  "-h, --help\tShow this help page\n"
	  "-v, --version\tprint out version"
	  "\n"
          "Coderunner " VERSION "\n"
          "Contribute at https://github.com/sudo200/CodeRunner.git"
        );
  exit(-1);
}

void version(void)
{
  puts("Coderunner " VERSION);
  exit(-1);
}

