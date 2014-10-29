#ifndef COMPIX_H
#define COMPIX_H

#include <ruby.h>
#include <stdbool.h>



// Types and macros from OilyPNG for compatibility
typedef uint32_t PIXEL; // Pixels use 32 bits unsigned integers
typedef unsigned char BYTE; // Bytes use 8 bits unsigned integers
#define R_BYTE(pixel)  ((BYTE) (((pixel) & (PIXEL) 0xff000000) >> 24))
#define G_BYTE(pixel)  ((BYTE) (((pixel) & (PIXEL) 0x00ff0000) >> 16))
#define B_BYTE(pixel)  ((BYTE) (((pixel) & (PIXEL) 0x0000ff00) >> 8))
#define A_BYTE(pixel)  ((BYTE) (((pixel) & (PIXEL) 0x000000ff)))

void Init_compix();

VALUE find_subimage(VALUE self, VALUE subimage, VALUE bigimage, VALUE threshold);

VALUE initialize_subimage_match_obj(int coord_x, int coord_y, int pixels_compared, int pixels_matched);

int compare_at(PIXEL *pixelsSmall, PIXEL *pixelsBig, int thres, int bigXoffset, int bigYoffset,
              int siwidth, int siheight, int sswidth, int ssheight);

bool compare_pixels(PIXEL pone, PIXEL ptwo, int threshold);

// The bytearray_ptr should be allocated for the correct size.
PIXEL* image_to_bytearray(PIXEL *bytearray_ptr, VALUE image);

#endif