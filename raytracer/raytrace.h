#pragma once

#include "world.h"
#include <math.h>

namespace raytrace {

	using namespace geom; 
	using color = vec3;

		
	surf::surface* find_next_surface(const ray& theray, world::world &theworld, geom::interception& intercept);
	color cast_to_light(const ray& theray,  world::world &theworld, const surface::light &thelight);
	color raytrace(ray &theray, world::world &theworld, int cutoff, nicefp maxdistance);
}