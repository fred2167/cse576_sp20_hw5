#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"

#define M_PI 3.14159265358979323846

// HW1 #2.1
// Image& im: image to L1-normalize
void l1_normalize(Image& im)
  {
  
  // TODO: Normalize each channel
  for (int cha = 0; cha < im.c; cha++ ){
    float total = 0;
    for (int i = cha * im.h * im.w; i < (cha+1) * im.h * im.w; i++){
      total += im.data[i];
    }

    for (int i = cha * im.h * im.w; i < (cha+1) * im.h * im.w; i++){
      if (total > 0) im.data[i] /= total;
      else im.data[i] /= 1/im.w/im.h;
    }
  }
  
  }

// HW1 #2.1
// int w: size of filter
// returns the filter Image of size WxW
Image make_box_filter(int w)
  {
  assert(w%2); // w needs to be odd
  
  // TODO: Implement the filter
  Image ret = Image(w, w, 1);
  for (int i = 0; i < w*w; i++){
    ret.data[i] = 1;
  }
  l1_normalize(ret);
  return ret; 
  }

// HW1 #2.2
// const Image&im: input image
// const Image& filter: filter to convolve with
// bool preserve: whether to preserve number of channels
// returns the convolved image
Image convolve_image(const Image& im, const Image& filter, bool preserve)
  {
    assert(filter.c==1);
    Image ret;
    // This is the case when we need to use the function clamped_pixel(x,y,c).
    // Otherwise you'll have to manually check whether the filter goes out of bounds
    
    // TODO: Make sure you set the sizes of ret properly. Use ret=Image(w,h,c) to reset ret
    // TODO: Do the convolution operator
    if (preserve){
      ret = Image(im.w, im.h, im.c);

      for (int x = 0; x < ret.w; x++){
        for (int y = 0; y < ret.h; y++){
          for (int cha = 0; cha < ret.c; cha++){

            float total = 0;
            for (int kx = 0; kx < filter.w; kx++){
              for (int ky = 0; ky < filter.h; ky++){

              total += filter(kx, ky) * im.clamped_pixel(x - filter.w / 2 + kx, y - filter.h / 2 + ky, cha);
              }
            }  
            ret(x, y, cha) = total;
          }
        }
      }

    }else {
      ret = Image(im.w, im.h, 1);

      for (int x = 0; x < ret.w; x++){
        for (int y = 0; y < ret.h; y++){
      
            float total = 0;
            for (int kx = 0; kx < filter.w; kx++){
              for (int ky = 0; ky < filter.h; ky++){
                for (int cha = 0; cha < im.c; cha++){

                  total += filter(kx, ky) * im.clamped_pixel(x - filter.w / 2 + kx, y - filter.h / 2 + ky, cha);
                }
              }  
            }
            ret(x, y) = total;
        }
      }
    }
    // Make sure to return ret and not im. This is just a placeholder
    return ret;
  }

// HW1 #2.3
// returns basic 3x3 high-pass filter
Image make_highpass_filter()
  {
  // TODO: Implement the filter
  Image kernel = Image(3,3,1);

  kernel(0, 0) = 0;
  kernel(1, 0) = -1;
  kernel(2, 0) = 0;
  kernel(0, 1) = -1;
  kernel(1, 1) = 4;
  kernel(2, 1) = -1;
  kernel(0, 2) = 0;
  kernel(1, 2) = -1;
  kernel(2, 2) = 0;
  
  return kernel; 
  
  }

// HW1 #2.3
// returns basic 3x3 sharpen filter
Image make_sharpen_filter()
  {
  // TODO: Implement the filter
  Image kernel = Image(3,3,1);

  kernel(0, 0) = 0;
  kernel(1, 0) = -1;
  kernel(2, 0) = 0;
  kernel(0, 1) = -1;
  kernel(1, 1) = 5;
  kernel(2, 1) = -1;
  kernel(0, 2) = 0;
  kernel(1, 2) = -1;
  kernel(2, 2) = 0;
  
  return kernel;
  
  }

// HW1 #2.3
// returns basic 3x3 emboss filter
Image make_emboss_filter()
  {
  // TODO: Implement the filter
  Image kernel = Image(3,3,1);

  kernel(0, 0) = -2;
  kernel(1, 0) = -1;
  kernel(2, 0) = 0;
  kernel(0, 1) = -1;
  kernel(1, 1) = 1;
  kernel(2, 1) = 1;
  kernel(0, 2) = 0;
  kernel(1, 2) = 1;
  kernel(2, 2) = 2;
  
  return kernel;
  
  }

// HW1 #2.4
// float sigma: sigma for the gaussian filter
// returns basic gaussian filter
Image make_gaussian_filter(float sigma)
  {
  // TODO: Implement the filter
  // 2D case
  int size = sigma * 6;
  if (size % 2 == 0) size += 1;
  Image kernel = Image(size, size, 1);
  for (int x = 0; x < kernel.w; x++){
    for (int y = 0; y < kernel.h; y++){

      // x, y need to subtract mean (size / 2)
      kernel(x, y) = (1. / (2 * M_PI * pow(sigma, 2))) * exp(-(pow(x - size/2, 2) + pow(y - size/2, 2)) / (2 * pow(sigma, 2)));
    }
  }
  l1_normalize(kernel);
  return kernel;
  }


// HW1 #3
// const Image& a: input image
// const Image& b: input image
// returns their sum
Image add_image(const Image& a, const Image& b)
  {
  assert(a.w==b.w && a.h==b.h && a.c==b.c); // assure images are the same size
  
  // TODO: Implement addition
  Image result(a.w, a.h, a.c);
  for (int i =0; i < a.w*a.h*a.c; i++){
    result.data[i] = a.data[i] + b.data[i];
  }

  return result;
  
  }

// HW1 #3
// const Image& a: input image
// const Image& b: input image
// returns their difference res=a-b
Image sub_image(const Image& a, const Image& b)
  {
  assert(a.w==b.w && a.h==b.h && a.c==b.c); // assure images are the same size
  
  // TODO: Implement subtraction
    Image result(a.w, a.h, a.c);
  for (int i =0; i < a.w*a.h*a.c; i++){
    result.data[i] = a.data[i] - b.data[i];
  }

  return result;
  }

// HW1 #4.1
// returns basic GX filter
Image make_gx_filter()
  {
  // TODO: Implement the filter
  Image kernel = Image(3,3,1);

  kernel(0, 0) = -1;
  kernel(1, 0) = 0;
  kernel(2, 0) = 1;
  kernel(0, 1) = -2;
  kernel(1, 1) = 0;
  kernel(2, 1) = 2;
  kernel(0, 2) = -1;
  kernel(1, 2) = 0;
  kernel(2, 2) = 1;
  
  return kernel; 
  }

// HW1 #4.1
// returns basic GY filter
Image make_gy_filter()
  {
  // TODO: Implement the filter
  Image kernel = Image(3,3,1);

  kernel(0, 0) = -1;
  kernel(1, 0) = -2;
  kernel(2, 0) = -1;
  kernel(0, 1) = 0;
  kernel(1, 1) = 0;
  kernel(2, 1) = 0;
  kernel(0, 2) = 1;
  kernel(1, 2) = 2;
  kernel(2, 2) = 1;
  
  return kernel; 
  }

// HW1 #4.2
// Image& im: input image
void feature_normalize(Image& im)
  {
  assert(im.w*im.h); // assure we have non-empty image
  
  // TODO: Normalize the features for each channel
  for (int cha = 0; cha < im.c; cha++) {
    float min = MAXFLOAT;
    float max = 0;

    for (int i = cha * im.h * im.w; i < (cha + 1) * im.h * im.w; i++){

      if (im.data[i] < min) min = im.data[i];
      if (im.data[i] > max) max = im.data[i];
    }

    float range = max - min;
    if (range != 0){

      for (int i = cha * im.h * im.w; i < (cha + 1) * im.h * im.w; i++){
        im.data[i] = (im.data[i] - min) / range;
      }
    }else{

      for (int i = cha * im.h * im.w; i < (cha + 1) * im.h * im.w; i++){
        im.data[i] = 0;
      } 
    }
  }
  }


// Normalizes features across all channels
void feature_normalize_total(Image& im)
  {
  assert(im.w*im.h*im.c); // assure we have non-empty image
  
  int nc=im.c;
  im.c=1;im.w*=nc;
  
  feature_normalize(im);
  
  im.w/=nc;im.c=nc;
  
  }


// HW1 #4.3
// Image& im: input image
// return a pair of images of the same size
pair<Image,Image> sobel_image(const Image& im)
  {
  // TODO: Your code here
  Image gx_kernel = make_gx_filter();
  Image gy_kernel = make_gy_filter();

  Image gx = convolve_image(im, gx_kernel, false);
  Image gy = convolve_image(im, gy_kernel, false);

  Image magnitude(im.w, im.h);
  for (int i = 0; i < im.w*im.h*im.c; i++) magnitude.data[i] = sqrt(pow(gx.data[i], 2) + pow(gy.data[i], 2));

  Image direction(im.w, im.h);
  for (int i = 0; i < im.w*im.h*im.c; i++) direction.data[i] = atan2(gy.data[i], gx.data[i]);

  return {magnitude, direction};
  }


// HW1 #4.4
// const Image& im: input image
// returns the colorized Sobel image of the same size
Image colorize_sobel(const Image& im)
  {
  
  // TODO: Your code here
  Image smoothIm = convolve_image(im, make_gaussian_filter(4), true);
  pair<Image, Image> pair = sobel_image(smoothIm);
  Image mag = pair.first;
  Image dir = pair.second;

  feature_normalize(mag);

  for (int i = 0; i < dir.h* dir.w* dir.c; i++) dir.data[i] = dir.data[i] / (2 * M_PI) + 0.5;

  Image result(im.w, im.h, im.c);
  int chaSize = im.w * im.h;
  memcpy(result.data, dir.data, sizeof(float) * chaSize);
  memcpy(&result.data[chaSize], mag.data, sizeof(float) * chaSize);
  memcpy(&result.data[2*chaSize], mag.data, sizeof(float) * chaSize);

  hsv_to_rgb(result);
  return result;
  }


// HW1 #4.5
// const Image& im: input image
// float sigma1,sigma2: the two sigmas for bilateral filter
// returns the result of applying bilateral filtering to im
Image bilateral_filter(const Image& im, float sigma1, float sigma2)
  {
  Image bf=im;
  
  // TODO: Your bilateral code
  NOT_IMPLEMENTED();
  
  return bf;
  }



// HELPER MEMBER FXNS

void Image::feature_normalize(void) { ::feature_normalize(*this); }
void Image::feature_normalize_total(void) { ::feature_normalize_total(*this); }
void Image::l1_normalize(void) { ::l1_normalize(*this); }

Image operator-(const Image& a, const Image& b) { return sub_image(a,b); }
Image operator+(const Image& a, const Image& b) { return add_image(a,b); }
