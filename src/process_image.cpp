#include <cstdio>
#include <iostream>
#include <cstring>
#include <cassert>
#include <cmath>

#include "image.h"

using namespace std;

// HW0 #3
// const Image& im: input image
// return the corresponding grayscale image
Image rgb_to_grayscale(const Image& im)
  {
  assert(im.c == 3); // only accept RGB images
  Image gray(im.w,im.h,1); // create a new grayscale image (note: 1 channel)
  
  // TODO: calculate the pixels of 'gray'
  for (int r = 0; r < gray.h; r++) {
    for (int c = 0; c < gray.w; c++){
      gray(c, r) = 0.299 * im(c, r, 0) +  0.587 * im(c, r, 1) + 0.114  * im(c, r, 2);
    }
  }
  return gray;
  }



// Example function that changes the color of a grayscale image
Image grayscale_to_rgb(const Image& im, float r, float g, float b)
  {
  assert(im.c == 1);
  Image rgb(im.w,im.h,3);
  
  for(int q2=0;q2<im.h;q2++)for(int q1=0;q1<im.w;q1++)
    {
    rgb(q1,q2,0)=r*im(q1,q2);
    rgb(q1,q2,1)=g*im(q1,q2);
    rgb(q1,q2,2)=b*im(q1,q2);
    }
  
  return rgb;
  }




// HW0 #4
// Image& im: input image to be modified in-place
// int c: which channel to shift
// float v: how much to shift
void shift_image(Image& im, int cha, float v)
  {
  assert(cha>=0 && cha<im.c); // needs to be a valid channel
  
  // TODO: shift all the pixels at the specified channel
  for (int r = 0; r < im.h; r++) {
    for (int c = 0; c < im.w; c++){
      im(c, r, cha) += v; 
    }
  }
  }

// HW0 #8
// Image& im: input image to be modified in-place
// int c: which channel to scale
// float v: how much to scale
void scale_image(Image& im, int c, float v)
  {
  assert(c>=0 && c<im.c); // needs to be a valid channel
  
  // TODO: scale all the pixels at the specified channel
  
  NOT_IMPLEMENTED();
  
  }


// HW0 #5
// Image& im: input image to be modified in-place
void clamp_image(Image& im)
  {
  // TODO: clamp all the pixels in all channel to be between 0 and 1
  
  for (int i = 0; i < im.h*im.w*im.c; i++) {
    if (im.data[i] > 1){
      im.data[i] = 1;
    }else if (im.data[i] < 0){
      im.data[i] = 0;
    }
  }
  }

// These might be handy
float max(float a, float b, float c)
  {
  return max({a,b,c});
  }

float min(float a, float b, float c)
  {
  return min({a,b,c});
  }


// HW0 #6
// Image& im: input image to be modified in-place
void rgb_to_hsv(Image& im)
  {
  assert(im.c==3 && "only works for 3-channels images");
  
  // TODO: Convert all pixels from RGB format to HSV format
  
    for (int r = 0; r < im.h; r++){
      for (int c = 0; c < im.w; c++){
        float values[3];
        int maxIdx = 0;
        float maxPixelValue = -1;
        for (int cha = 0; cha < im.c; cha++) {
          values[cha] = im(c, r, cha);
          if (values[cha] > maxPixelValue){
            maxPixelValue = values[cha];
            maxIdx = cha;
          }
        }

        float maxVal = max(values[0], values[1], values[2]);
        float minVal = min(values[0], values[1], values[2]);

        float diff = maxVal - minVal;
        float sat = 0;
        if (maxVal != 0){
          sat = diff / maxVal;
        }
        float hue = 0;
        if (diff != 0){
          if (maxIdx == 0){
            hue =(values[1] - values[2]) / diff;
          }else if (maxIdx == 1){
            hue = (values[2] - values[0]) / diff + 2;
          }else{
            hue = (values[0] - values[1]) / diff + 4;
          }
        }
       if (hue < 0) {
         hue  = hue / 6 + 1;
       }else{
         hue = hue / 6;
       }
       im(c, r, 0) = hue;
       im(c, r, 1) = sat;
       im(c, r, 2) = maxVal;
    }
  }
  }

// HW0 #7
// Image& im: input image to be modified in-place
void hsv_to_rgb(Image& im)
  {
  assert(im.c==3 && "only works for 3-channels images");
  
  // TODO: Convert all pixels from HSV format to RGB format
  
  for (int r = 0; r < im.h; r++) {
    for (int c = 0; c < im.w; c++){
      float values[3];
      for (int cha = 0; cha < im.c; cha++){
       values[cha] = im(c, r, cha);
      }
      float C = values[1] * values[2];
      float X = C * (1 - abs( fmod(6* values[0], 2) - 1));
      float m = values[2] - C;

      float H = values[0];
      if (H < 1./6){
        im(c, r, 0) = C + m;
        im(c, r, 1) = X + m;
        im(c, r, 2) = 0 + m;
      } else if (H < 2./6){
        im(c, r, 0) = X + m;
        im(c, r, 1) = C + m;
        im(c, r, 2) = 0 + m;
      }else if (H < 3./6){
        im(c, r, 0) = 0 + m;
        im(c, r, 1) = C + m;
        im(c, r, 2) = X + m;
      }else if (H < 4./6){
        im(c, r, 0) = 0 + m;
        im(c, r, 1) = X + m;
        im(c, r, 2) = C + m;
      }else if (H < 5./6){
        im(c, r, 0) = X + m;
        im(c, r, 1) = 0 + m;
        im(c, r, 2) = C + m;
      }else{
        im(c, r, 0) = C + m;
        im(c, r, 1) = 0 + m;
        im(c, r, 2) = X + m;
      }

    }
  }
  }

// HW0 #9
// Image& im: input image to be modified in-place
void rgb_to_lch(Image& im)
  {
  assert(im.c==3 && "only works for 3-channels images");
  
  // TODO: Convert all pixels from RGB format to LCH format
  
  
  NOT_IMPLEMENTED();
  
  }

// HW0 #9
// Image& im: input image to be modified in-place
void lch_to_rgb(Image& im)
  {
  assert(im.c==3 && "only works for 3-channels images");
  
  // TODO: Convert all pixels from LCH format to RGB format
  
  NOT_IMPLEMENTED();
  
  }



// Implementation of member functions
void Image::clamp(void) { clamp_image(*this); }
void Image::shift(int c, float v) { shift_image(*this,c,v); }
void Image::scale(int c, float v) { scale_image(*this,c,v); }

void Image::HSVtoRGB(void) { hsv_to_rgb(*this); }
void Image::RGBtoHSV(void) { rgb_to_hsv(*this); }
void Image::LCHtoRGB(void) { lch_to_rgb(*this); }
void Image::RGBtoLCH(void) { rgb_to_lch(*this); }
