#ifndef __image_types__
#define __image_types__

#include <stdlib.h>
#include <string.h>

enum img_type { JPG, PNG, WEBP, UNKNOWN };

struct file_data {
  char *full_path;
  char *parent;
  char *file_name;
  enum img_type type;
};

enum img_type img_string_to_type(char *full_name);
char *img_type_to_string(enum img_type type);
struct file_data *file_data_new(char *full_path);
void file_data_free(struct file_data *file_data);

#endif