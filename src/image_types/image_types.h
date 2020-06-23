#ifndef __image_types__
#define __image_types__

#include <string.h>

enum img_type { JPG, PNG, WEBP, UNKNOWN };

enum img_type file_type(char *full_name);

#endif