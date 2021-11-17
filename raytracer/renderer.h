#pragma once

#include "surface.h"
#include "world.h"
#include <stdio.h>
#include <thread>
#include <vector>
#include <iostream>
#include "raytrace.h"



namespace world {

  struct world;
}
namespace camera {
using namespace geom; 
struct camera {

   vec3 position;
   vec3 direction;
   vec3 image_udirection;
   vec3 image_vdirection; 
   nicefp focal_length;
   nicefp u_size; // half sizes of image
   nicefp v_size;
   vec3 *image;

   int height; // half height and length
   int width;

   int cutoff;

  camera(vec3 position, vec3 direction, vec3 image_udirection,
         vec3 image_vdirection, nicefp focal_length, nicefp u_size,
         nicefp v_size, int height, int width);
  ~camera();

  void render(const char *filename, world::world *theworld, int cutoff,
              double maxdistance);
};

void create_png(const char *filename, vec3 *pixeldata, int height, int width);
} // namespace camera
