

namespace raytrace {

	using color = vec3;


	struct rayoflight {

		geom::ray ray;
		nicefp currentn;
		vec3 currentbeer;

		rayoflight(ray theray, nicefp n, nicefp beer);
	};
		
	surf::surface* find_next_surface(const rayoflight& theray, world::world theworld, geom::interception& intercept);
	color cast_to_light(const rayoflight& theray, const &world::world theworld, const &surface::light thelight);
	color raytrace(rayoflight theray, world::world theworld, int cutoff, nicefp maxdistance);
}