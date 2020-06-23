#ifndef __output__
#define __output__

#include <glib.h>
#include <stdio.h>
#include <string.h>

#include "../args/args.h"

// possible returns from get_next_photo()
enum photo_errors { PHOTO_OK, PHOTO_END, PHOTO_SKIP, PHOTO_COUNT };

// Returns name of a file from path (Without extensions)
char *filename(char *path);

// gets next photo from arguments struct and it's output data
enum photo_errors get_next_photo(struct arguments *arguments, char **file_in,
                                 char **file_out);

#endif