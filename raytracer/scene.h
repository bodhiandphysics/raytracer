// This is a template for changing the scene.  In this you'll need to redifine
// material, surfaces, and set the camera. All this is done by changing the cpp
// file for create scene.

#include "world.h"

namespace scene {

using namespace world;
using namespace geom;

using color = vec3; 


static int cutoff = 5; // recursion depth of raytracer;
static nicefp maxdistance =
    nicefp(200); // default value of how far to check for objects

world::world *create_scene();
} // namespace scene