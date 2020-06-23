#include "output.h"

// Returns name of a file from path (Without extensions)
char *filename(char *path) {
  char *name = path;
  char *slash = path;
  do {
    char *uslash = strchr(slash, '/');
    slash = (uslash) ? uslash : strchr(slash, '\\');
    name = (slash) ? ++slash : name;
  } while (slash);

  char *temp1 = strchr(name, '.');
  if (!temp1) return strdup(name);

  char *temp2 = strchr(temp1, '.');
  while (temp2) {
    temp1 = temp2 + 1;
    temp2 = strchr(temp1, '.');
  }

  char *ret = malloc((temp1 - name + 1) * sizeof(char));
  strncpy(ret, name, temp1 - name - 1);
  ret[temp1 - name - 1] = '\0';

  return ret;
}

// If get_next_photo returns PHOTO_SKIP free unneeded memory
int skip_exit(char **file_in, char **file_out, char *out_name) {
  free(*file_in);
  free(*file_out);
  free(out_name);
  return PHOTO_SKIP;
}

enum photo_errors get_next_photo(struct arguments *arguments, char **file_in,
                                 char **file_out) {
  if (stack_length(arguments->in_files) == 0) return PHOTO_END;

  *file_in = stack_pop(arguments->in_files);
  char *out_name;
  if (stack_length(arguments->in_files) < stack_length(arguments->out_names)) {
    out_name = stack_pop(arguments->out_names);
  } else {
    out_name = filename(strdup(*file_in));
  }

  *file_out = malloc(strlen(arguments->out_dir) + 2 * strlen(out_name) + 3);
  sprintf(*file_out, "%s/%s", arguments->out_dir, out_name);

  // Tests if input file exists
  if (!g_file_test(*file_in, G_FILE_TEST_EXISTS | G_FILE_TEST_IS_REGULAR)) {
    fprintf(stderr, "Cannot open file %s. Skipping...\n", *file_in);
    return skip_exit(file_in, file_out, out_name);
  }

  // Check if output directory exists
  if (g_file_test(*file_out, G_FILE_TEST_EXISTS)) {
    // If output path exists check if it is directory
    if (g_file_test(*file_out, G_FILE_TEST_IS_DIR)) {
      fprintf(stdout, "Directory %s already exists. Overwrite? [Y-yes, N-no]: ",
              *file_out);
      char c = getchar();
      while ((getchar()) != '\n');
      
      // Overwrite exiting directory
      if (c != 'y' && c != 'Y') {
        return skip_exit(file_in, file_out, out_name);
      }
    } else {
      // Location is not a directory
      fprintf(stderr,
              "Output location %s already exists and is not a directory. "
              "Skipping...\n",
              *file_out);
      return skip_exit(file_in, file_out, out_name);
    }
  } else {
    // If path doesn't exist create it
    if (g_mkdir_with_parents(*file_out, 0774)) {
      fprintf(stderr, "Can't create dir %s. Skipping...\n", *file_out);
      return skip_exit(file_in, file_out, out_name);
    }
  }

  sprintf(*file_out, "%s/%s", *file_out, out_name);
  free(out_name);
  return PHOTO_OK;
}