#include <stdio.h>
#include <string.h>
#include <vips/vips.h>

#include "args/args.h"
#include "html_output/html_output.h"
#include "output/output.h"

// Argp constants
const char *argp_program_version = "web-img 1.0";
const char *argp_program_bug_address = "github.com/vodnikangus";

enum img_type { JPG, PNG, WEBP, UNKNOWN };

// TODO: case sensitive
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

// Save the image in different formats based on input
int save_image(VipsImage *img, char *file_in, char *file_out) {
  char *out_name = malloc(strlen(file_out) + 6);
  switch (file_type(file_in)) {
    case JPG: {
      sprintf(out_name, "%s.jpg", file_out);
      if (vips_image_write_to_file(img, out_name, NULL)) {
        printf("Saving \"%s\" failed!\n", out_name);
      }
      break;
    }
    case PNG: {
      sprintf(out_name, "%s.png", file_out);
      if (vips_image_write_to_file(img, out_name, NULL)) {
        printf("Saving \"%s\" failed!\n", out_name);
      }
      break;
    }
  }

  sprintf(out_name, "%s.webp", file_out);
  if (vips_image_write_to_file(img, out_name, NULL)) {
    printf("Saving \"%s\" failed!\n", out_name);
  }
  free(out_name);
  return 0;
}

int crop_center(struct arguments *arguments, VipsImage **image, int *w,
                int *h) {
  double aspect_in = (double)*w / *h;
  double aspect_out = (float)arguments->width / arguments->height;

  int crop_w = *w;
  int crop_h = *h;
  if (aspect_in > aspect_out)
    crop_w = *h * aspect_out;
  else
    crop_h = *w * aspect_out;

  if (vips_crop(*image, image, (*w - crop_w) / 2, (*h - crop_h) / 2, crop_w,
                crop_h, NULL)) {
    if (*image) g_object_unref(*image);
    return PHOTO_SKIP;
  }

  *w = crop_w;
  *h = crop_h;
  return 0;
}

char *resize_width(char *file_in, char *file_out, VipsImage *in, int w,
                   int width) {
  VipsImage *out;
  if (w >= width) {
    char *name = malloc(strlen(file_out) + 7);
    vips_resize(in, &out, (float)width / w, NULL);
    sprintf(name, "%s-%dw", file_out, width);
    if (out) {
      save_image(out, file_in, name);
      g_object_unref(out);
    }
    return name;
  }
  return NULL;
}

char *resize_height(char *file_in, char *file_out, VipsImage *in, int h,
                    int height) {
  VipsImage *out;
  if (h >= height) {
    char *name = malloc(strlen(file_out) + 7);
    vips_resize(in, &out, (float)height / h, NULL);
    sprintf(name, "%s-%dh", file_out, height);
    if (out) {
      save_image(out, file_in, name);
      g_object_unref(out);
    }
    return name;
  }
  return NULL;
}

char *resize_factor(char *file_in, char *file_out, VipsImage *in, int x,
                    int dest_x, float factor) {
  VipsImage *out;
  if (x > factor * dest_x) {
    char *name = malloc(strlen(file_out) + 5);
    vips_resize(in, &out, (float)(factor * dest_x) / x, NULL);
    sprintf(name, "%s-%gx", file_out, factor);
    if (out) {
      save_image(out, file_in, name);
      g_object_unref(out);
    }
    return name;
  }
  return NULL;
}

int scale_image(struct arguments *arguments, char *file_in, char *file_out,
                struct output **outputs) {
  VipsImage *in, *out;
  in = vips_image_new_from_file(file_in, NULL);
  if (!in) {
    fprintf(stderr, "Cannot open file %s. Skipping...\n", file_in);
    return PHOTO_SKIP;
  }

  int w = vips_image_get_width(in);
  int h = vips_image_get_height(in);

  // If both width and height are provided crop image
  if (arguments->width && arguments->height)
    if (crop_center(arguments, &in, &w, &h)) return PHOTO_SKIP;

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
    if (!arguments->use_height) {
      for (int i = 0; i < output_len; i++) {
        (*outputs)[i].name =
            resize_width(file_in, file_out, in, w, (*outputs)[i].size);
      }
    } else {
      for (int i = 0; i < output_len; i++) {
        (*outputs)[i].name =
            resize_height(file_in, file_out, in, h, (*outputs)[i].size);
      }
    }
  }

  if (arguments->width) {
    for (int i = 0; i < output_len; i++) {
      (*outputs)[i].name = resize_factor(file_in, file_out, in, w,
                                         arguments->width, (*outputs)[i].scale);
    }
  } else if (arguments->height) {
    for (int i = 0; i < output_len; i++) {
      (*outputs)[i].name = resize_factor(
          file_in, file_out, in, h, arguments->height, (*outputs)[i].scale);
    }
  }

  if (in) g_object_unref(in);
}

int main(int argc, char **argv) {
  // Parses command line arguments using argp wrapper
  struct arguments *arguments = arguments_get(argc, argv);

  // Inits VIPS image library
  if (VIPS_INIT(argv[0])) vips_error_exit(NULL);

  // Loops through all input files
  char *file_in;
  char *file_out;
  int end = 0;
  do {
    struct output *outputs;
    switch (get_next_photo(arguments, &file_in, &file_out)) {
      case PHOTO_END:

        end = 1;
        break;
      case PHOTO_SKIP:
        continue;
      case PHOTO_OK:
        scale_image(arguments, file_in, file_out, &outputs);
        html_print(arguments, outputs);
        free(file_in);
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