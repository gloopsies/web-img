#include "output.h"

// If get_next_photo returns PHOTO_SKIP free unneeded memory
int skip_exit(struct file_data **file_in, char **file_out) {
  file_data_free(*file_in);
  free(*file_out);
  return PHOTO_SKIP;
}

enum photo_errors get_next_photo(struct arguments *arguments,
                                 struct file_data **file_in, char **file_out) {
  if (stack_length(arguments->in_files) == 0) return PHOTO_END;

  *file_in = stack_pop(arguments->in_files);
  char *out_name;

  out_name = strdup((*file_in)->file_name);

  if (stack_length(arguments->in_files) < stack_length(arguments->out_names)) {
    out_name = stack_pop(arguments->out_names);
  }

  *file_out = malloc(strlen(arguments->out_dir) + 2 * strlen(out_name) + 3);
  sprintf(*file_out, "%s/%s", arguments->out_dir, out_name);

  // Tests if input file exists
  if (!g_file_test((*file_in)->full_path,
                   G_FILE_TEST_EXISTS | G_FILE_TEST_IS_REGULAR)) {
    fprintf(stderr, "File %s doesn't exist. Skipping...\n", (*file_in)->full_path);
    return skip_exit(file_in, file_out);
  }

  // Check if output directory exists
  if (g_file_test(*file_out, G_FILE_TEST_EXISTS)) {
    // If output path exists check if it is directory
    if (g_file_test(*file_out, G_FILE_TEST_IS_DIR)) {
      fprintf(stdout,
              "Directory %s already exists. "
              "Overwrite? [Y-yes, N-no]: ",
              *file_out);

      char c = getchar();
      while ((getchar()) != '\n')
        ;

      // Overwrite exiting directory
      if (c != 'y' && c != 'Y') {
        return skip_exit(file_in, file_out);
      }
    } else {
      // Location is not a directory
      fprintf(stderr,
              "Output location %s already exists and is not a directory. "
              "Skipping...\n",
              *file_out);
      return skip_exit(file_in, file_out);
    }
  } else {
    // If path doesn't exist create it
    if (g_mkdir_with_parents(*file_out, 0774)) {
      fprintf(stderr, "Can't create dir %s. Skipping...\n", *file_out);
      return skip_exit(file_in, file_out);
    }
  }

  char *copy = *file_out;
  sprintf(*file_out, "%s/%s", copy, out_name);
  free(out_name);
  return PHOTO_OK;
}