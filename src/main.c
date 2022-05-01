#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define	EQUALS(x, y)	(strcmp(x, y) == 0)

#define VERSION "v1.0.1"

int main(int argc, char** argv) {
  if(argc < 2 || (argc >= 2 && (EQUALS(argv[1], "-h") || EQUALS(argv[1], "--help")))) { // Show help page
    puts(
	  "Usage: coderunner <command> <command-args>\n"
	  "\n"
	  "Coderunner " VERSION "\n"
	  "Contribute at https://github.com/sudo200/CodeRunner.git"
	);
    return -1;
  }
  if(argc >= 2 && (EQUALS(argv[1], "-v") || EQUALS(argv[1], "--version"))) { // Show version
    puts("Coderunner " VERSION);
    return -1;
  }

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

