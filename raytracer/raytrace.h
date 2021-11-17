#pragma once

#include "world.h"
#include <math.h>
#include "surface.h"
#include "geom/nicefp.h"

namespace world {

  struct world;
}

namespace raytrace {

using namespace geom;
using color = vec3;

surf::surface *find_next_surface(ray &theray, world::world *theworld,
                                 geom::interception &intercept);
color cast_to_light(ray &theray, world::world *theworld,
                    const surf::light &thelight);
color raytrace(ray &theray, world::world *theworld, int cutoff,
               nicefp maxdistance);
} // namespace raytrace