#ifndef COMPIX_H
#define COMPIX_H

#include <ruby.h>
#include <stdbool.h>
#include "compix_helpers.h"


void Init_compix();

VALUE find_subimage(int argc, VALUE* argv, VALUE self);

int compare_at(PIXEL *pixelsSmall, PIXEL *pixelsBig, int thres, int bigXoffset, int bigYoffset,
              int siwidth, int siheight, int sswidth, int ssheight);

bool compare_pixels(PIXEL pone, PIXEL ptwo, int threshold);

#endif