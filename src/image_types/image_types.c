#include "image_types.h"

enum img_type file_type(char *full_name) {
  if (!strcmp(&full_name[strlen(full_name) - 3], "jpg") ||
      !strcmp(&full_name[strlen(full_name) - 4], "jpeg")) {
    return JPG;
  }
  if (!strcmp(&full_name[strlen(full_name) - 3], "png")) {
    return PNG;
  }
  if (!strcmp(&full_name[strlen(full_name) - 4], "webp")) {
    return WEBP;
  }
  return UNKNOWN;
}