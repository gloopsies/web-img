// Argp wrapper

#ifndef __args__
#define __args__

#include <argp.h>
#include <stdlib.h>

#include "../stack/stack.h"

#define USE_HEIGHT 1

// Program description
static char doc[] =
    "web-img - batch export for uploading photos to your website";

// Argp input description
static char args_doc[] = "photos";

// List of all argument options
static struct argp_option options[] = {
    {0, 0, 0, 0, "Output: ", 1},
    {"output", 'o', "FILE", 0, "Output directory", 1},
    {"out_files", 'f', "FILES", 0,
     "Names of output directories (if empty picture name is used)", 1},
    {"tab", 't', "TAB", 0, "String used for tab character", 1},
    {0, 0, 0, 0, "Width and height:", 2},
    {"width", 'w', "WIDTH", 0,
     "Width of output image(s)"
     "If both width and height are specified image is cropped",
     2},
    {"height", 'h', "HEIGHT", 0,
     "Height of output image(s). "
     "If both width and height are specified image is cropped",
     2},
    {"use-height", USE_HEIGHT, 0, 0,
     "Use height instead of width for default sizes", 2},
    {0, 0, 0, 0, "Quality:", 3},
    {"no-compression", 'n', 0, 0, "Disable compression for jpg files", 3},
    {0, 0, 0, 0, "Usage:", -1},
    {0}};

// Structure used for parsing command line
struct arguments {
  Stack *in_files;
  char *out_dir;
  Stack *out_names;
  char *tab;
  int width;
  int height;
  char use_height;
  char no_compression;
};

// Funcion for parsing arguments
static error_t parse_opt(int key, char *arg, struct argp_state *state);

// Return parsed data
struct arguments *arguments_get(int argc, char **argv);

// Free arguments struct
void arguments_free(struct arguments *arguments);

#endif