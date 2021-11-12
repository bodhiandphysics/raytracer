#pragma once
#include "vec3.h"
#include "nicefp.h"

namespace geom {
	struct ray {

		vec3 origin;
		vec3 direction; // should always be normalized;

		ray(const vec3 &anorigin, vec3 &unnormalized);
		ray reflect(vec3 &point, vec3 &normal);
		bool refract(vec3& point, vec3& normal, nicefp& inref_index, nicefp& outref_index, ray& result);

	};
}