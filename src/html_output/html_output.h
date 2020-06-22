#ifndef __html_output__
#define __html_output__

#include "../args/args.h"

struct output {
  char *name;
  float scale;
  int size;
};

void html_print(struct arguments *arguments, struct output *outputs);

#endif