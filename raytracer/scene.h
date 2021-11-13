// This is a template for changing the scene.  In this you'll need to redifine
// material, surfaces, and set the camera. All this is done by changing the cpp
// file for create scene.

#include "world.h"

namespace scene {

using namespace world;

static int cutoff; // recursion depth of raytracer;
static nicefp maxdistance =
    nicefp(200); // default value of how far to check for objects

world *create_scene();
} // namespace scene