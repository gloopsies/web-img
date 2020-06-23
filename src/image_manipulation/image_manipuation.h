#ifndef __image_manipulation__
#define __image_manipulation__

#include <stdio.h>
#include <vips/vips.h>

VipsImage *image_crop_center(VipsImage *in, int out_width, int out_height);
VipsImage *image_scale_factor(VipsImage *in, float factor);
VipsImage *image_scale_size(VipsImage *in, int size_in, int size_out);

#endif