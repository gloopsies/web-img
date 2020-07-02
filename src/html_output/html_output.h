#ifndef __html_output__
#define __html_output__

#include "../args/args.h"
#include "../file_data/file_data.h"

struct output {
  char *name;
  float scale;
  int size;
};

void html_print(struct arguments *arguments, struct file_data file_in,
                struct output *outputs);

#endif