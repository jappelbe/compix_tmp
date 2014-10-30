#include "compix_helpers.h"

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

int integer_from_hash(VALUE hash, char* key, int default_value, int min_value, int max_value){
  VALUE val = rb_hash_aref(hash, RB_SYM(key));
  if( TYPE(val) != T_NIL ) {
    int c_value = NUM2INT(val);
    if ( c_value < min_value || c_value > max_value ){
      rb_raise(rb_eArgError, "argument '%s' has value %d. Allowed range is %d..%d", key, c_value, min_value, max_value);
    }
    return c_value;
  }
  return default_value;
}

double float_from_hash(VALUE hash, char* key, double default_value, double min_value, double max_value){
  VALUE val = rb_hash_aref(hash, RB_SYM(key));
  if( TYPE(val) != T_NIL ) {
    double c_value = NUM2DBL(val);
    if ( c_value < min_value || c_value > max_value ){
      rb_raise(rb_eArgError, "argument '%s' has value %d. Allowed range is %d..%d", key, c_value, min_value, max_value);
    }
    return c_value;
  }
  return default_value;
}

VALUE initialize_subimage_match_obj(int coord_x, int coord_y, int pixels_compared, int pixels_matched,
                                    int subimage_width, int subimage_height, float limit_for_match) {
  VALUE mCompixModule = rb_const_get(rb_cObject, rb_intern("Compix"));
  VALUE cSubimageMatch = rb_const_get(mCompixModule, rb_intern("SubimageMatch"));

  VALUE match_obj = rb_funcall(cSubimageMatch, rb_intern("new"), 7, INT2NUM(coord_x), INT2NUM(coord_y),
                                         INT2NUM(pixels_compared), INT2NUM(pixels_matched),
                                         INT2NUM(subimage_width), INT2NUM(subimage_height), DBL2NUM(limit_for_match));
  return match_obj;
}