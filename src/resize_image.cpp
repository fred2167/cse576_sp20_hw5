#include <queue>
#include <cmath>
#include "image.h"

using namespace std;

// HW1 #1
// float x,y: inexact coordinates
// int c: channel
// returns the nearest neighbor to pixel (x,y,c)
float Image::pixel_nearest(float x, float y, int c) const
  {
  // Since you are inside class Image you can
  // use the member function pixel(a,b,c)
  
  // TODO: Your code here
  return clamped_pixel(rint(x), rint(y), c);
  }

// HW1 #1
// float x,y: inexact coordinates
// int c: channel
// returns the bilinearly interpolated pixel (x,y,c)
float Image::pixel_bilinear(float x, float y, int c) const
  {
  // Since you are inside class Image you can
  // use the member function pixel(a,b,c)
  
  
  // TODO: Your code here
  
  float high_x = ceilf(x);
  float low_x = floorf(x);
  float high_y = ceilf(y);
  float low_y = floorf(y);

  float top_left = clamped_pixel(low_x, low_y, c);
  float top_right = clamped_pixel(high_x, low_y, c);
  float bottom_left = clamped_pixel(low_x, high_y, c);
  float bottom_right = clamped_pixel(high_x, high_y, c);

  float a = x - low_x;
  float b = y - low_y;
  float w_tl = (1 - a) * (1 - b) * top_left;
  float w_bl = a * (1 - b) * bottom_left;
  float w_br = a * b * bottom_right;
  float w_tr = (1 - a) * b *top_left;
  return w_tl + w_bl + w_br + w_tr;
  }

// HW1 #1
// int w,h: size of new image
// const Image& im: input image
// return new Image of size (w,h,im.c)
Image nearest_resize(const Image& im, int w, int h)
  {
  Image ret(w,h,im.c);
  
  // TODO: Your code here
  
  float aw = (float)im.w / w;
  float bw = 0.5 * (aw-1);
  float ah = (float)im.h / h;
  float bh = 0.5 * (ah-1);
  float u,v;

  for (int x = 0; x < w; x++){
    for (int y = 0; y < h; y++){
      // map back to original image
      u = x * aw + bw;
      v = y * ah + bh;
      for (int c = 0; c < im.c; c++){
        ret(x, y, c) = im.pixel_nearest(u,v,c);
      }
    }
  }
  return ret;
  }


// HW1 #1
// int w,h: size of new image
// const Image& im: input image
// return new Image of size (w,h,im.c)
Image bilinear_resize(const Image& im, int w, int h)
  {
  Image ret(w,h,im.c);
  // TODO: Your code here
  
  float aw = (float)im.w / w;
  float bw = 0.5 * (aw-1);
  float ah = (float)im.h / h;
  float bh = 0.5 * (ah-1);
  float u,v;

  for (int x = 0; x < w; x++){
    for (int y = 0; y < h; y++){
      // map back to original image
      u = x * aw + bw;
      v = y * ah + bh;
      for (int c = 0; c < im.c; c++){
        ret(x, y, c) = im.pixel_bilinear(u,v,c);
      }
    }
  }
  return ret;
  }
