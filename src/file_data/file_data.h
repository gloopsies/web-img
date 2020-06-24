#ifndef __image_types__
#define __image_types__

#include <string.h>
#include <stdlib.h>

enum img_type { JPG, PNG, WEBP, UNKNOWN };

struct file_data {
  char *full_path;
  char *parent;
  char *file_name;
  enum img_type type;
};

enum img_type file_type(char *full_name);
struct file_data *file_data_new(char *full_path);
void file_data_free(struct file_data *file_data);

#endif