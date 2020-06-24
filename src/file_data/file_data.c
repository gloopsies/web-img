#include "file_data.h"

int find_file_name(char *full_path) {
  char *name = full_path;
  char *slash = full_path;
  do {
    char *uslash = strchr(slash, '/');
    slash = (uslash) ? uslash : strchr(slash, '\\');
    name = (slash) ? ++slash : name;
  } while (slash);

  if(full_path == name) return 0;

  return (name - full_path) / sizeof(char);
}

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

struct file_data *file_data_new(char *full_path) {
  struct file_data *new = malloc(sizeof(*new));
  new->full_path = strdup(full_path);

  if (find_file_name(new->full_path)) {
    new->parent = malloc(find_file_name(new->full_path) * sizeof(char));
    strncpy(new->parent, new->full_path, find_file_name(new->full_path) - 1);
    new->parent[find_file_name(new->full_path) - 1] = '\0';
  } else {
    new->parent = NULL;
  }

  if (strchr(new->full_path + find_file_name(new->full_path), '.')) {
    int file_name_len =
        strchr(new->full_path + find_file_name(new->full_path), '.') -
        new->full_path - 3;
    new->file_name = malloc(file_name_len * sizeof(char));
    strncpy(new->file_name, new->full_path + find_file_name(new->full_path),
            file_name_len);
    new->file_name[file_name_len] = '\0';
  } else {
    new->file_name =
        malloc(strlen(new->full_path + find_file_name(new->full_path)));
    strcpy(new->file_name, new->full_path + find_file_name(new->full_path));
  }

  new->type = file_type(new->full_path);

  return new;
}

void file_data_free(struct file_data *file_data) {
  if(!file_data) return;

  if(file_data->full_path) free(file_data->full_path);
  if(file_data->parent) free(file_data->parent);
  if(file_data->file_name) free(file_data->file_name);

  free(file_data);
}
