#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#define VERSION "1.0"

int main(int argc, char **argv) {
  int opt;
  int verbose = 0;
  char *output_file = NULL;
  char *path = NULL;

  static struct option long_options[] = {
      {"verbose" , no_argument       , NULL , 'v'},
      {"output"  , required_argument , NULL , 'o'},
      {"path"    , required_argument , NULL , 'p'},
      {"help"    , no_argument       , NULL , 'h'},
      {"version" , no_argument       , NULL , 'V'},
      {0, 0, 0, 0}
  };

  while ((opt = getopt_long(argc, argv, "vho:p:", long_options, NULL)) != -1) {
    switch (opt) {
      case 'v':
        verbose = 1;
        break;
      case 'h':
        printf("Usage: %s [options]\n", argv[0]);
        printf("Options:\n");
        printf("  -v, --verbose\t\tVerbose mode\n");
        printf("  -h, --help\t\t\tHelp\n");
        printf("  -o, --output\t\tOutput file\n");
        printf("  -p, --path\t\t\tPath\n");
        printf("  -V, --version\t\tVersion\n");
        exit(0);
      case 'o':
        output_file = optarg;
        break;
      case 'p':
        path = optarg;
        break;
      case 'V':
        printf("Version %s\n", VERSION);
        exit(0);
      case '?':
        fprintf(stderr, "Unknown option `%c'\n", optopt);
        exit(1);
      default:
        fprintf(stderr, "Unknown option `%c'\n", opt);
        exit(1);
    }
  }
  if (verbose) printf("Verbose mode enabled\n");
  if (output_file) printf("Output file: %s\n", output_file);
  if (path) printf("Path: %s\n", path);
  return 0;
}
