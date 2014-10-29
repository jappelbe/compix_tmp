#include <ruby.h>
#include <stdbool.h>
#include <stdio.h>
#include "compix.h"

void Init_compix(){
  VALUE CompixModule = rb_define_module("Compix");
  rb_define_singleton_method(CompixModule, "find_subimage", find_subimage, -1);
}

bool compare_pixels(PIXEL pone, PIXEL ptwo, int threshold){
  BYTE ored = R_BYTE(pone);
  BYTE ogreen = G_BYTE(pone);
  BYTE oblue = B_BYTE(pone);
  BYTE tred = R_BYTE(ptwo);
  BYTE tgreen = G_BYTE(ptwo);
  BYTE tblue = B_BYTE(ptwo);
  if(ored - tred > threshold || tred - ored > threshold ||
      ogreen - tgreen > threshold || tgreen - ogreen > threshold ||
      oblue - tblue > threshold || tblue - oblue > threshold){
      return false;
  }
  return true;
}

int compare_at(PIXEL *pixelsSmall, PIXEL *pixelsBig, int color_threshold, int bigXoffset, int bigYoffset,
              int siwidth, int siheight, int sswidth, int ssheight){
  PIXEL firstPixel;
  PIXEL secPixel;
  int match = 0;
  int unmatch = 0;
  int x, y;
  int small_image_coord;
  int bigCoord = 0;
  for(y = 0; y < siheight; y++){
    for(x = 0; x < siwidth; x++){
      small_image_coord = y * siwidth + x;
      firstPixel = pixelsSmall[small_image_coord];
      bigCoord = sswidth*(y+bigYoffset)+(x+bigXoffset);
      if(bigCoord >= (sswidth)*(ssheight) ){
        break;
      }
      secPixel = pixelsBig[bigCoord];
      if(compare_pixels(firstPixel, secPixel, color_threshold)){
        match++;
      } 
      else {
        unmatch++;
      }
    }
    if(unmatch > match){
      break;
    }
  }
  return match;
}

// (VALUE self, VALUE subimage, VALUE bigimage, VALUE options)
VALUE find_subimage(int argc, VALUE* argv, VALUE self){
  VALUE subimage = argv[0];
  VALUE image = argv[1];
  VALUE options_hash;
  int color_threshold = 0;
  double pixels_threshold = 0;
  int xstart = 0;
  int ystart = 0 ;

  if(argc == 3){
    options_hash = argv[2];
    color_threshold = integer_from_hash(options_hash, "threshold", 0);
    xstart = integer_from_hash(options_hash, "start_coordinate_x", 0);
    ystart = integer_from_hash(options_hash, "start_coordinate_y", 0);
    pixels_threshold = float_from_hash(options_hash, "pixels_threshold", 0.85);
  }

  VALUE subimage_match_obj;
  
  int siwidth = NUM2INT(rb_funcall(subimage, rb_intern("width"), 0));
  int siheight = NUM2INT(rb_funcall(subimage, rb_intern("height"), 0));
  int sswidth = NUM2INT(rb_funcall(image, rb_intern("width"), 0));
  int ssheight = NUM2INT(rb_funcall(image, rb_intern("height"), 0));
  
  PIXEL *bytearray_subimg = malloc(siwidth * siheight * sizeof(PIXEL));
  PIXEL *bytearray_big_image = malloc(sswidth * ssheight * sizeof(PIXEL));

  int match = 0;
  int maxmatch = 0;
  int maxX = -1;
  int maxY = -1;

  int x, y;
  int compPix = siwidth * siheight;
  
  // Create bytearrays for pixels
  bytearray_subimg = image_to_bytearray(bytearray_subimg, subimage);
  bytearray_big_image = image_to_bytearray(bytearray_big_image, image);
  
  for(y = ystart ; y <= ssheight - siheight; y++){
    for(x = xstart ; x <= sswidth - siwidth; x++){
      match = compare_at(bytearray_subimg, bytearray_big_image, color_threshold, x, y,
                        siwidth, siheight, sswidth, ssheight);
      if(match > maxmatch){
        maxmatch = match;
        maxX = x;
        maxY = y;
      }      
    }
    xstart = 0;
  }

  free(bytearray_subimg);
  free(bytearray_big_image);

  subimage_match_obj = initialize_subimage_match_obj(maxX, maxY, compPix, maxmatch, siwidth, siheight, pixels_threshold);
  return subimage_match_obj;
}
