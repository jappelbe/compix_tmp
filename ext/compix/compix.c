#include <ruby.h>
#include <stdbool.h>
#include <stdio.h>
#include "compix.h"

void Init_compix(){
  VALUE CompixModule = rb_define_module("Compix");
  rb_define_singleton_method(CompixModule, "find_subimage", find_subimage, 3);
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

int compare_at(PIXEL *pixelsSmall, PIXEL *pixelsBig, int thres, int bigXoffset, int bigYoffset,
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
      if(compare_pixels(firstPixel, secPixel, thres)){
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

PIXEL* image_to_bytearray(PIXEL *bytearray_ptr, VALUE image){
  VALUE pixels = rb_funcall(image, rb_intern("pixels"), 0);
  int x, y;

  int width = NUM2INT(rb_funcall(image, rb_intern("width"), 0));
  int height = NUM2INT(rb_funcall(image, rb_intern("height"), 0));

  for(y = 0 ; y < height ; y++){
    for(x = 0 ; x < width ; x++){
      bytearray_ptr[y * width + x] = NUM2UINT(rb_ary_entry(pixels, y * width + x));
    }
  }
  return bytearray_ptr;
}

VALUE find_subimage(VALUE self, VALUE subimage, VALUE bigimage, VALUE threshold){
  VALUE subimage_match_obj;
  
  int siwidth = NUM2INT(rb_funcall(subimage, rb_intern("width"), 0));
  int siheight = NUM2INT(rb_funcall(subimage, rb_intern("height"), 0));
  int sswidth = NUM2INT(rb_funcall(bigimage, rb_intern("width"), 0));
  int ssheight = NUM2INT(rb_funcall(bigimage, rb_intern("height"), 0));
  
  PIXEL *bytearray_subimg = malloc(siwidth * siheight * sizeof(PIXEL));
  PIXEL *bytearray_big_image = malloc(sswidth * ssheight * sizeof(PIXEL));
  
  int thres = NUM2UINT(threshold);
  int match = 0;
  int maxmatch = 0;
  int maxX = -1;
  int maxY = -1;
  //int xLeftLimit = NUM2INT(rb_ary_entry(minCoord, 0));
  //int yTopLimit = NUM2INT(rb_ary_entry(minCoord, 1));
  //int xRightLimit = NUM2INT(rb_ary_entry(maxCoord, 0));
  //int yBottomLimit = NUM2INT(rb_ary_entry(maxCoord, 1));
  // I didn't want to break backwards compatibility, so while this is less legible
  // and slower if old version of compix is used, it will still work.
  // The third and fourth entry in the array specifies from where to start checking the big image.
  // Used for e.g. findAllSubImages in rubypng
  int xstart = 0; //NUM2INT(rb_ary_entry(minCoord, 2));
  int ystart = 0 ; //NUM2INT(rb_ary_entry(minCoord, 3));
  int x, y;
  int compPix = siwidth * siheight;
  
  // Create bytearrays for pixels
  bytearray_subimg = image_to_bytearray(bytearray_subimg, subimage);
  bytearray_big_image = image_to_bytearray(bytearray_big_image, bigimage);
  
  for(y = ystart ; y <= ssheight - siheight; y++){
    for(x = xstart ; x <= sswidth - siwidth; x++){
      match = compare_at(bytearray_subimg, bytearray_big_image, thres, x, y,
                        siwidth, siheight, sswidth, ssheight);
      if(match > maxmatch){
        maxmatch = match;
        maxX = x;
        maxY = y;
      }      
    }
  }

  free(bytearray_subimg);
  free(bytearray_big_image);

  subimage_match_obj = initialize_subimage_match_obj(maxX, maxY, compPix, maxmatch);
  return subimage_match_obj;
}

VALUE initialize_subimage_match_obj(int coord_x, int coord_y, int pixels_compared, int pixels_matched){
  VALUE mCompixModule = rb_const_get(rb_cObject, rb_intern("Compix"));
  VALUE cSubimageMatch = rb_const_get(mCompixModule, rb_intern("SubimageMatch"));

  VALUE match_obj = rb_funcall(cSubimageMatch, rb_intern("new"), 4, INT2NUM(coord_x), INT2NUM(coord_y),
                                         INT2NUM(pixels_compared), INT2NUM(pixels_matched));
  return match_obj;
}