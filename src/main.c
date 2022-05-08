#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define VERSION "v1.0.2"

static bool help_f = false;
static bool version_f = false;

static struct option options[] = {
/* NAME		ARGS		FLAG			SHORT  */
  {"help",	no_argument,	(int *)&help_f,		'h'},
  {"version",	no_argument,	(int *)&version_f,	'v'},

  {NULL,	no_argument,	NULL,			0}
};

void usage(void);
void version(void);

int main(int argc, char** argv) {
  int option_index = 0;
  while(getopt_long_only(argc, argv, "", options, &option_index) != -1);

  if(help_f) // Show help page
    usage();
    
  if(version_f) // Show version
    version();

  if(optind == argc) // No futher args -> Show help page
    usage();

  size_t buffer_size = 0;
  
  // Get amount of buffer
  for(size_t i = 1; i < argc; ++i)
    buffer_size += (strlen(argv[i]) + 1) * sizeof(char);

  // Alloc buffer
  char* buffer_str = (char *)malloc(buffer_size);

  for(size_t i = 1; i < argc; ++i) {
    strcat(buffer_str, argv[i]);
    if((i + 1) < argc) strcat(buffer_str, " ");
  }

  // Exec program
  const time_t before = time(NULL);
  const int exit_code = system(buffer_str);
  const time_t after = time(NULL);

  printf("\nProcess exited with code %d (0x%X); took %lu seconds\n", exit_code, exit_code, after - before);
  
  free(buffer_str);
  return 0;
}

void usage(void)
{
  puts(
          "Usage: coderunner <command> <command-args>\n"
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

