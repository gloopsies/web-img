#include <stdio.h>
#include <string.h>
#include <vips/vips.h>

#include "args/args.h"
#include "file_data/file_data.h"
#include "html_output/html_output.h"
#include "image_manipulation/image_manipuation.h"
#include "output/output.h"
#include "string_extended/string_extended.h"

// Argp constants
const char *argp_program_version = "web-img 1.0";
const char *argp_program_bug_address = "github.com/vodnikangus";

// Save the image in different formats based on input
int save_image(VipsImage *img, struct file_data file_in, char *file_out) {
  char *out_name = malloc(strlen(file_out) + 6);
  switch (file_in.type) {
    case JPG: {
      sprintf(out_name, "%s.jpg", file_out);
      if (vips_image_write_to_file(img, out_name, NULL)) {
        printf("Saving \"%s\" failed!\n", out_name);
      }
      break;
    }
    case PNG:
    case WEBP: {
      sprintf(out_name, "%s.png", file_out);
      if (vips_image_write_to_file(img, out_name, NULL)) {
        printf("Saving \"%s\" failed!\n", out_name);
      }
      break;
    }
    case UNKNOWN:
      break;
  }

  sprintf(out_name, "%s.webp", file_out);
  if (vips_image_write_to_file(img, out_name, NULL)) {
    printf("Saving \"%s\" failed!\n", out_name);
  }
  free(out_name);
  return 0;
}

int scale_image(struct arguments *arguments, struct file_data file_in,
                char *file_out, struct output **outputs) {
  VipsImage *in;

  in = vips_image_new_from_file(file_in.full_path, NULL);
  if (!in) {
    fprintf(stderr, "Cannot open file %s. Skipping...\n", file_in.full_path);
    return PHOTO_SKIP;
  }

  // If both width and height are provided crop image
  if (arguments->width && arguments->height) {
    in = image_crop_center(in, arguments->width, arguments->height);
    if (!in) return PHOTO_SKIP;
  }

  int w = vips_image_get_width(in);
  int h = vips_image_get_height(in);

  char *name = malloc(strlen(file_out) + 10);
  sprintf(name, "%s-original", file_out);
  save_image(in, file_in, name);
  free(name);

  // Provide sizes or scale based on input arguments
  int output_len;
  if (!arguments->width && !arguments->height) {
    output_len = 5;
    *outputs = calloc(output_len + 1, sizeof(**outputs));

    (*outputs[0]).scale = 0;
    if (!arguments->use_height) {
      (*outputs)[0].size = 3840;
      (*outputs)[1].size = 2560;
      (*outputs)[2].size = 1920;
      (*outputs)[3].size = 1280;
      (*outputs)[4].size = 720;
      (*outputs)[5].size = 0;
    } else {
      (*outputs)[0].size = 2160;
      (*outputs)[1].size = 1440;
      (*outputs)[2].size = 1080;
      (*outputs)[3].size = 720;
      (*outputs)[4].size = 565;
      (*outputs)[5].size = 0;
    }
  } else {
    output_len = 3;
    *outputs = calloc(output_len + 1, sizeof(**outputs));
    (*outputs)[0].size = 0;
    (*outputs)[0].scale = 2;
    (*outputs)[1].scale = 1;
    (*outputs)[2].scale = .5;
    (*outputs)[3].scale = 0;
  }

  // export image with different scales and sizes
  if (!arguments->width && !arguments->height) {
    for (int i = 0; i < output_len; i++) {
      VipsImage *out;

      out = image_scale_size(in, (arguments->use_height) ? h : w,
                             (*outputs)[i].size);

      if (!out) {
        (*outputs)[i].name = NULL;
        continue;
      }

      char *size;
      size = itoa((*outputs)[i].size, 10);

      name = malloc((strlen(file_out) + strlen(size) + 3) * sizeof(char));

      sprintf(name, "%s-%s%c", file_out, size,
              (arguments->use_height) ? 'h' : 'w');

      (*outputs)[i].name = name;
      save_image(in, file_in, name);

      free(size);
      g_object_unref(out);
    }
    if (in) g_object_unref(in);
    return 0;
  }

  int size = (arguments->width) ? arguments->width : arguments->height;
  for (int i = 0; i < output_len; i++) {
    VipsImage *out;

    out = image_scale_size(in, (arguments->width) ? w : h,
                           (*outputs)[i].scale * size);

    if (!out) {
      (*outputs)[i].name = NULL;
      continue;
    }
    name = malloc((strlen(file_out) + 6) * sizeof(char));

    sprintf(name, "%s-%gx", file_out, (*outputs)[i].scale);

    (*outputs)[i].name = name;
    save_image(in, file_in, name);

    g_object_unref(out);
  }

  if (in) g_object_unref(in);
  return 0;
}

int main(int argc, char **argv) {
  // Parses command line arguments using argp wrapper
  struct arguments *arguments = arguments_get(argc, argv);

  // Inits VIPS image library
  if (VIPS_INIT(argv[0])) vips_error_exit(NULL);

  // Loops through all input files
  struct file_data *file_in;
  char *file_out;
  char end = 0;
  int status;
  do {
    struct output *outputs = NULL;
    switch (get_next_photo(arguments, &file_in, &file_out)) {
      case PHOTO_END:
        end = 1;
        break;

      case PHOTO_SKIP:
        continue;

      case PHOTO_OK:
        status = scale_image(arguments, *file_in, file_out, &outputs);
        if (status == PHOTO_OK) html_print(arguments, outputs);
        free(file_out);
        break;

      default:
        fprintf(stderr, "Unknown error");
        vips_error_exit(NULL);
    }

  } while (!end);

  // Free arguments struct
  arguments_free(arguments);
  return 0;
}