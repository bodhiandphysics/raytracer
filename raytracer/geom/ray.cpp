#include "ray.h"
#include <math.h>

namespace geom {


	ray::ray(const vec3& anorigin, vec3& unnormalized) {

			origin = anorigin;
			direction = unnormalized.normalize();

	}

	ray ray::reflect(vec3 &point, vec3 &normal) { // create a reflection ray

			vec3 newdirection = direction.reflect(normal);
			vec3 peterbation = point + newdirection * .001;
			return ray(peterbation, newdirection); // perturb the new ray a little
	}


	// this is aweful!! Create a refraction ray. 

	bool ray::refract(vec3& point, vec3& normal, nicefp& inref_index, nicefp& outref_index, ray& result) {

		if (outref_index == 0) return false; // no refraction because no light transport

		double index_ratio = inref_index.value / outref_index.value;
		double cosincedent = -(direction.dot(normal).value); // we're coming into the point, so invert direction
		double sinout2 = (1 - cosincedent*cosincedent) * index_ratio;
		double discriminant = 1 - sinout2;

		if (discriminant <= .00001) return false; // total internal reflection

		vec3 refractdir = direction * index_ratio
						+ normal*(index_ratio*cosincedent - sqrt(discriminant));

		result.origin = point + refractdir*nicefp(.0001); // perturb the new ray a little
		result.direction = refractdir;

		return true;
	}

}