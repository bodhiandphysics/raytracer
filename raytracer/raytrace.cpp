#include "raytrace.h"

namespace raytrace {

	




	surf::surface* find_next_surface(const rayoflight& theray, world::world theworld, geom::interception& intercept, nicefp maxdistance) {
		nicefp cutoff = maxdistance;
		surf::surface* retsurf = nullptr;
		bool didintercept;

		for (surf::surface &checksurf: theworld.surfaces) {

			//rmember to set intercept max distance in caller
			didintercept = checksurf->shape.calc_interception(theray.ray, cutoff, intercept); 
			if (didintercept) {
				retsuf = checksurface;
				cutoff = intercept.distance;
			}

		}
		return retsurf;
	}


	color cast_to_light(const ray& theray, const world::world& theworld, cons const surface::light& thelight, nicefp starting_n, nicefp maxdistance) {

		vec3 delta = thelight.position - theray.origin;
		nicefp light_distance = delta.norm();
		vec3 currentbeer = vec3(0,0,0);
		nicefp current_n = starting_n;

		if (maxdistance < light_distance) return color(0,0,0); // light is too far
		if ((delta / light_distance).dot(thelight.direction) < thelight.min_cos_angle) return color(0,0,0); // outside of light cone


		geom::interception intercept(light_distance, vec3(0,0,0), vec3(0,0,0));

		while ((surf::surface* next_surface = find_next_surface(theray, theworld, intercept, maxdistance)) != nullptr) {

			material* next_material;
			if (theray.ray.direction.dot(intercept.normal) < 0) {

				*next_material = &next_surface->inside;
				*current_material = &next_surface->outside;
			} else {
				*next_material = &next_surface->outside;
				*current_material = &next_surface->inside;
			}

			if (!next_material->doestransmit) return color(0,0,0); // we hit an opaque surface

			nicefp next_n = next_material->refract_index;  // we now need to check for total internal refraction

			if (next_n > currentn) {

				nicefp ddotn = theray.theray.direction.dot(intercept.normal);
				if (ddotn*ddotn < ((currentn*currentn) / (next_n*next_n)) - 1 )
					return color(0,0,0); 					// Total internal reflection
			}

			currentbeer += current_material->beerfactor * intercept.distance;
			current_n = next_n;

			if (theray.currentbeer.norm2() > nicefp(200)) return color(0,0,0); // to much in the way
		} 

		// we break out of the loop at the light

		return thelight.color.nexp(theray.currentbeer) / light_distance;  //use linear as opposed to quadratic attenuation... better for close distances

	}



	color raytrace(ray &theray, world::world theworld, int cutoff, nicefp maxdistance) {

		color retcolor = vec3(0,0,0);
		if (cutoff == 0) return retcolor;

		geom::interception intercept(maxdistance, vec3(0,0,0), vec3(0,0,0));

		surf::surface* next_surface = find_next_surface(theray, theworld, intercept, maxdistance);
		if (next_surface == nullptr) return theworld.getbgcolor(theray.theray); // we went off the map
		

		surf::material* next_material;
		surf::material* current_material;
		if (theray.ray.direction.dot(intercept.normal) < 0) {

			*next_material = &next_surface->inside;
			*current_material = &next_surface->outside;
		} else {
			*next_material = &next_surface->outside;
			*current_material = &next_surface->inside;
		}

		nicefp distance = intercept.distance;


		geom::uv uv = intercept.uv;
		// first do global illumination
		if (next_material.doesambient) {

			retcolor += next_material->material_color.mult(next_material->ambient(uv)).mult(theworld.ambientlight);
		}

		// next cast to all the light sources

		for (surface::light &thelight: theworld.lights) {

			// find if we need to transition matieral

			vec3 direction_to_light = intercept.position - thelight.position;
			nicefp currentn;
			if (direction_to_light.dot(next_surface.normal) < 0) current_n = next_material->refract_index; // going through new material
			else current_n = current_material->refract_index;  // going through the same material

			ray shadowray(intercept.position, direction_to_light);

			color shadowlightcolor = cast_to_light(shadowray, theworld, thelight, current_n, maxdistance);
			color += next_material->bdfrfactor(uv, shadowray.theray.direction, theray.theray.direction, intercept.normal).mult(shadowlightcolor); 
		}

		// next handle reflections

		ray reflectray(intercept.position, theray.direction.reflect());


		color reflectlightcolor = raytrace(reflectray, theworld, cutoff - 1, maxdistance);

		nicefp fresnelterm = 1;
		if (next_material->doesfresnel) { 
			double schlickt1 = pow(((next_material->refract_index - current_material->refract_index) / (next_material->refract_index + current_material->refract_index)).value, 2);
			double costheta = reflectray.theray.direction.dot(next_material.normal).value;
			double schlick = schlickt1 + (1-schlickt1)*pow((1- costheta), 5);
			fresnelterm = nicefp(shlick);
		}

		color += (next_material->bdfrfactor(uv, shadowray.ray.direction, theray.ray.direction, intercept.normal) * fresnelterm).mult(refelctlightcolor);

		// then handle refraction

		ray refractray;
		color refractlightcolor = vec3(0,0,0);
		bool didrefract = geom::refract(intercept.position, intercept.normal, next_material->refract_index, current_material->refract_index, refractray);

		if (didrefract) refractlightcolor = raytrace(refractray, theworld, cutoff - 1, maxdistance);
		color += refractlightcolor * (1 - fresnelterm); // try to conserve engery

		// finally put it all together

		if (theray.currentbeer == 0) return color / intercept.distance; // don't do nexp if not nescessary
		else return color.nexp(theray.currentbeer * intercept.distance) / intercept.distance; //use linear attenuation
	}
}
















