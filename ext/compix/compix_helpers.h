#ifndef COMPIX_HELPERS_H
#define COMPIX_HELPERS_H

#include <ruby.h>
#include <stdbool.h>


// Types and macros from OilyPNG for compatibility
typedef uint32_t PIXEL; // Pixels use 32 bits unsigned integers
typedef unsigned char BYTE; // Bytes use 8 bits unsigned integers
#define R_BYTE(pixel)  ((BYTE) (((pixel) & (PIXEL) 0xff000000) >> 24))
#define G_BYTE(pixel)  ((BYTE) (((pixel) & (PIXEL) 0x00ff0000) >> 16))
#define B_BYTE(pixel)  ((BYTE) (((pixel) & (PIXEL) 0x0000ff00) >> 8))
#define A_BYTE(pixel)  ((BYTE) (((pixel) & (PIXEL) 0x000000ff)))

// Symbols for options for method find_subimage
#define RB_SYM(string) ID2SYM(rb_intern(string))



// The bytearray_ptr should be allocated for the correct size.
PIXEL* image_to_bytearray(PIXEL *bytearray_ptr, VALUE image);

int integer_from_hash(VALUE hash, char* key, int default_value);

double float_from_hash(VALUE hash, char* key, double default_value);

VALUE initialize_subimage_match_obj(int coord_x, int coord_y, int pixels_compared, int pixels_matched,
                                    int subimage_width, int subimage_height, float limit_for_match);


#endif