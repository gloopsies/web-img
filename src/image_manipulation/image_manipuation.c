#include "image_manipuation.h"

VipsImage *image_crop_center(VipsImage *in, int out_width, int out_height) {
  double aspect_in =
      (double)vips_image_get_width(in) / vips_image_get_height(in);
  double aspect_out = (float)out_width / out_height;

  int crop_w = vips_image_get_width(in);
  int crop_h = vips_image_get_height(in);
  if (aspect_in > aspect_out)
    crop_w = crop_h * aspect_out;
  else
    crop_h = crop_w * aspect_out;

  VipsImage *out;
  if (vips_crop(in, &out, (vips_image_get_width(in) - crop_w) / 2,
                (vips_image_get_height(in) - crop_h) / 2, crop_w, crop_h,
                NULL)) {
    if (out) g_object_unref(out);
    return NULL;
  }

  return out;
}

VipsImage *image_scale_size(VipsImage *in, int size_in, int size_out) {
  if (size_in < size_out) return NULL;

  VipsImage *out;
  if (vips_resize(in, &out, (float)size_in / size_out, NULL)) {
    if (out) g_object_unref(out);
    return NULL;
  }

  return out;
}
