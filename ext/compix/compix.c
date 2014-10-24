#include <ruby.h>
#include <stdbool.h>

// Types and macros from OilyPNG for compatibility
typedef unsigned int PIXEL; // Pixels use 32 bits unsigned integers
typedef unsigned char BYTE; // Bytes use 8 bits unsigned integers
#define R_BYTE(pixel)  ((BYTE) (((pixel) & (PIXEL) 0xff000000) >> 24))
#define G_BYTE(pixel)  ((BYTE) (((pixel) & (PIXEL) 0x00ff0000) >> 16))
#define B_BYTE(pixel)  ((BYTE) (((pixel) & (PIXEL) 0x0000ff00) >> 8))
#define A_BYTE(pixel)  ((BYTE) (((pixel) & (PIXEL) 0x000000ff)))

void Init_compix();
VALUE compix_compare_pixels(VALUE self, VALUE subimage, VALUE bigimage, 
                            VALUE minCoord, VALUE maxCoord, VALUE threshold);
                            
void Init_compix(){
  VALUE klass = rb_define_class("Compix", rb_cObject);
  rb_define_singleton_method(klass, "compare_pixels", compix_compare_pixels, 5);
}

int getImgWidth(VALUE img){
  int width = NUM2INT(rb_funcall(img, rb_intern("width"), 0));
  return width;
}

int getImgHeight(VALUE img){
  int height = NUM2INT(rb_funcall(img, rb_intern("height"), 0));
  return height;
}

int comparePixels(PIXEL pone, PIXEL ptwo, int threshold){
  unsigned char ored = R_BYTE(pone);
  unsigned char ogreen = G_BYTE(pone);
  unsigned char oblue = B_BYTE(pone);
  unsigned char tred = R_BYTE(ptwo);
  unsigned char tgreen = G_BYTE(ptwo);
  unsigned char tblue = B_BYTE(ptwo);
  if(ored - tred > threshold || tred - ored > threshold ||
      ogreen - tgreen > threshold || tgreen - ogreen > threshold ||
      oblue - tblue > threshold || tblue - oblue > threshold){
      return false;
  }
  return true;
}

int compareAt(unsigned int *pixelsSmall, unsigned int *pixelsBig, int thres, int bigXoffset, int bigYoffset,
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
      if(bigCoord >= (sswidth)*(ssheight) - 2 ){
        break;
      }
      secPixel = pixelsBig[bigCoord];
      if(comparePixels(firstPixel, secPixel, thres)){
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

VALUE compix_compare_pixels(VALUE self, VALUE subimage, VALUE bigimage, 
                            VALUE minCoord, VALUE maxCoord, VALUE threshold){  
  VALUE pixelsf = rb_funcall(subimage, rb_intern("pixels"), 0);
  VALUE pixelss = rb_funcall(bigimage, rb_intern("pixels"), 0);    
  
  int siwidth = getImgWidth(subimage);
  int siheight = getImgHeight(subimage);
  int sswidth = getImgWidth(bigimage);
  int ssheight = getImgHeight(bigimage);
  
  unsigned int *bytearray_subimg = malloc(siwidth * siheight * sizeof(unsigned int));
  unsigned int *bytearray_screenshot = malloc(sswidth * ssheight * sizeof(unsigned int));
  
  int thres = NUM2UINT(threshold);
  int match = 0;
  int maxmatch = 0;
  int maxX = -1;
  int maxY = -1;
  int xLeftLimit = NUM2INT(rb_ary_entry(minCoord, 0));
  int yTopLimit = NUM2INT(rb_ary_entry(minCoord, 1));
  int xRightLimit = NUM2INT(rb_ary_entry(maxCoord, 0));
  int yBottomLimit = NUM2INT(rb_ary_entry(maxCoord, 1));
  // I didn't want to break backwards compatibility, so while this is less legible
  // and slower if old version of compix is used, it will still work.
  // The third and fourth entry in the array specifies from where to start checking the big image.
  // Used for e.g. findAllSubImages in rubypng
  int xstart = 0; //NUM2INT(rb_ary_entry(minCoord, 2));
  int ystart = 0 ; //NUM2INT(rb_ary_entry(minCoord, 3));
  int x, y;
  int pixels = 0;
  int compPix = siwidth * siheight;
  VALUE retArr;
  int matchPercent;
  
  // Create bytearrays for pixels
  for(y = 0 ; y < siheight ; y++){
	  for(x = 0 ; x < siwidth ; x++){
	    bytearray_subimg[y*siwidth + x] = NUM2UINT(rb_ary_entry(pixelsf,y*siwidth + x));
	  }
  }
  for(y = 0 ; y < ssheight ; y++){
	  for(x = 0 ; x < sswidth ; x++){
	    bytearray_screenshot[y*sswidth + x] = NUM2UINT(rb_ary_entry(pixelss,y*sswidth + x));
	  }
  }
  
  
  for(y = ystart ; y < yBottomLimit-siheight; y++){
    for(x = xstart ; x < xRightLimit-siwidth; x++){
      match = compareAt(bytearray_subimg, bytearray_screenshot, thres, x, y,
                        siwidth, siheight, sswidth, ssheight);
      if(match > maxmatch){
        maxmatch = match;
        maxX = x;
        maxY = y;
      }      
    }
  }
  free(bytearray_subimg);
  free(bytearray_screenshot);
  pixels = x * ( ssheight - siheight ) + y;
  matchPercent = (100 * maxmatch) / compPix;
  retArr = rb_ary_new();
  rb_ary_push(retArr, INT2NUM(matchPercent));
  rb_ary_push(retArr, INT2NUM(maxX));
  rb_ary_push(retArr, INT2NUM(maxY));
  rb_ary_push(retArr, INT2NUM(pixels));
  return retArr;
}
