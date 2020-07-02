// Argp wrapper

#ifndef __args__
#define __args__

#include <argp.h>
#include <stdlib.h>

#include "../file_data/file_data.h"
#include "../stack/stack.h"
#include "../stack/stack_file_data.h"

#define USE_HEIGHT 1
#define LOSSY_COMPRESSION 2
#define LOSSLESS_COMPRESSION 3
#define WEBP_COMPRESSION_TYPE 4
#define WEBP_COMPRESSION_QUALITY 5

enum WEBP_COMPRESSION { WEBP_DEFAULT, WEBP_LOSSY, WEBP_LOSSLESS };

// Structure used for parsing command line
struct arguments {
  Stack *in_files;
  char *out_dir;
  Stack *out_names;
  char *tab;
  int width;
  int height;
  char use_height;
  short lossy_compression;
  short lossless_compression;
  enum WEBP_COMPRESSION webp_compression_type;
};

// Funcion for parsing arguments
error_t parse_opt(int key, char *arg, struct argp_state *state);

// Return parsed data
struct arguments *arguments_get(int argc, char **argv);

// Free arguments struct
void arguments_free(struct arguments *arguments);

#endif