#pragma once

#include "world.h"
#include <stb_image_write.h>
#include <stdio.h>
#include <thread>
#include <vector>

namespace camera {

camera::camera(vec3 position, vec3 direction, vec3 image_udirection,
               vec3 image_vdirection, nicefp focal_length, nicefp u_size,
               nicefp v_size, int height, int width) {

  this->position = position;
  this->direction = direction;
  this->image_udirection = image_udirection;
  this->image_vdirection = image_vdirection;
  this->focal_length = focal_length;
  this->u_size = u_size;
  this->v_size = v_size;

  this->image = (double *)malloc(height * width * sizeof(double));
}

camera::~camera() { free(this->image); }

void camera::rendertask(int start, int stride, world::world theworld,
                        int cutoff, double maxdistance) {

  vec3 position = this.position;
  vec3 direction = this.direction;
  vec3 image_udirection = this.image_udirection vec3 image_vdirection =
      this.image_vdirection nicefp focal_length =
          this.focal_length double u_scale =
              this.u_size / this.width; // half sizes of image
  double v_scale = this.v_size / this.height;
  vec3 *image = this.image;
  int height = this.height; // half height and width
  int width = this.width;

  for (int i = -width - 1 + start; i < width; i += stride) {

    for (int j = -height - 1; j < height; j++) {

      // this could be optomized
      vec3 pixel_loc = position + (direction * focal_length) +
                       (image_udirection * i * u_scale) +
                       (image_vdirection * j * v_scale);
      geom::ray theray(pixel_loc, pixel_loc - position)
          rt::rayoflight lightray(theray, nicefp(1), nicefp(0));

      image[i + width + 1][j + height + 1] =
          raytrace(lightray, theworld, cutoff, nicefp(maxdistance));
    }
  }
}

void camera::render(const char *filename, world::world &theworld, int cutoff,
                    double maxdistance) { // this is dumb multithreading, but it
                                          // should work on average
  int numthreads = std::thread::hardware_concurrency();
  std::vector<thread> threads;

  // remember to put in exceptions later
  for (int threadnum = 0; threadnum < numthreads; threadnum++)
    threads.emplace_back(rendertask, threadnum, numthreads, theworld, cutoff,
                         maxdistance);

  for (int threadnum = 0; threadnum < numthreads; threadnum++)
    threads[i].join();

  create_png(finelname, this->image, this.height, this.width);
}

void create_png(const char *filename, double *pixeldata, int height,
                int width) {

  char *rgbabuffer = (char *)malloc(4 * height * width * sizeof(char));

  for (int i = 0; i < width i++)
    for (int j = 0; j < height; j++) {

      double redcolorvalue = max(
          255,
          256 *
              sqrt(pixeldata[width * j + i].x)
                  .value); // apply stupid gamma correction, 8 bit color channel
      double greencolorvalue =
          max(255, 256 * sqrt(pixeldata[width * j + i].y).value);
      double bluecolorvalue =
          max(255, 256 * sqrt(pixeldata[width * j + i].z).value);
      double aplphacolorvalue = 255;

      rgbabuff[4 * (width * j + i)] = floor(redcolorvalue);
      rgbabuff[4 * (width * j + i) + 1] = floor(greencolorvalue);
      rgbabuff[4 * (width * j + i) + 2] = floor(bluecolorvalue);
      rgbabuff[4 * (width * j + i) + 3] = floor(alphacolorvalue);
    }

  if (stbi_write_png(filename, width, height, 4, width * 4) != 0)
    std::cout << "There was an error with producing the png!\n";
  free(rgbabuffer);
}
} // namespace camera
