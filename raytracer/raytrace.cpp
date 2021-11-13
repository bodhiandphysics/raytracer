#include "raytrace.h"

namespace raytrace {

	

	rayoflight::rayoflight(ray theray, nicefp n, nicefp beer) {

		ray = theray;
		currentn = n;
		currentbeer = beer;	
	}





	surf::surface* find_next_surface(const rayoflight& theray, world::world theworld, geom::interception& intercept) {
		nicefp current_max_distance;
		surf::surface* retsurf = nullptr;
		bool didintercept;

		for (surf::surface &checksurf: theworld.surfaces) {

			//rmember to set intercept max distance in caller
			didintercept = checksurf->shape.calc_interception(theray.ray, min(intercept.distance), intercept); 
			if (didintercept) retsuf = checksurface;

		}
		return retsurf;
	}


	color cast_to_light(const rayoflight& theray, const &world::world theworld, const &surface::light thelight) {

		vec3 delta = thelight.position - theray.origin;
		nicefp light_distance = delta.norm();
		if ((delta / light_distance).dot(thelight.direction) < thelight.min_cos_angle) return color(0,0,0); // outside of light cone

		geom::interception intercept(light_distance, vec3(0,0,0), vec3(0,0,0));

		while ((surf::surface* next_surface = find_next_surface(theray, theworld, intercept)) != nullptr) {

			material* next_material;
			if (theray.theray.direction.dot(intercept.normal) < 0) *next_material = &next_surface->inside;
			else *next_material = &next_surface->outside;

			if (!next_material->doestransmit) return color(0,0,0); // we hit an opaque surface

			nicefp nextn = next_material->refract_index;  // we now need to check for total internal refraction

			if (nextn < currentn) {

				nicefp ddotn = theray.theray.direction.dot(intercept.normal);
				if (ddotn*ddotn < ((currentn*currentn) / (nextn*nextn)) - 1 )
					return color(0,0,0); 					// Total internal reflection
			}

			theray.currentn = nextn;
			theray.currentbeer += next_material->beerfactor * intercept.distance;

			if (theray.currentbeer.norm2() > nicefp(200)) return color(0,0,0); // to much in the way
		} 

		// we break out of the loop at the light

		return thelight.color.nexp(theray.currentbeer) / light_distance;  //use linear as opposed to quadratic attenuation... better for close distances

	}



	color raytrace(rayoflight theray, world::world theworld, int cutoff, nicefp maxdistance) {

		color retcolor = vec3(0,0,0);
		if (cutoff == 0) return retcolor;

		geom::interception intercept(maxdistance, vec3(0,0,0), vec3(0,0,0));

		surf::surface* next_surface = find_next_surface(theray, theworld, intercept);
		if (next_surface == nullptr) return theworld.getbgcolor(theray.theray); // we went off the map
		

		surf::material* next_material;
		if (theray.ray.direction.dot(intercept.normal) < 0) *next_material = &next_surface->inside;
			else *next_material = &next_surface->outside;

		nicefp distance = intercept.distance;


		geom::uv uv = intercept.uv;
		// first do global illumination
		if (next_material.doesambient) {

			retcolor += next_material->material_color.mult(next_material->ambient(uv)).mult(theworld.ambientlight);
		}

		// next cast to all the light sources

		for (surface::light &thelight: theworld.lights) {

			rayoflight shadowray(
								ray(intercept.position, 
								intercept.position - thelight.position), 
								next_material->refract_index,  // we've tranitioned into a new mateiral
								nicefp(0)); // start each shadowray with new beerfactor

			color shadowlightcolor = cast_to_light(shadowray, theworld, thelight);
			color += next_material->bdfrfactor(uv, shadowray.theray.direction, theray.theray.direction, intercept.normal).mult(shadowlightcolor); 
		}

		// next handle reflections

		rayoflight reflectray(  ray(intercept.position, 
								theray.direction.reflect()), 
								theray.currentn,  // we're still in the same material
								theray.currentbeer);


		color reflectlightcolor = raytrace(reflectray, theworld, cutoff - 1, maxdistance);

		nicefp fresnelterm = 1;
		if (next_material->doesfresnel) { 
			double schlickt1 = pow(((next_material->refract_index - theray.currentn) / (next_material->refract_index + theray.currentn)).value, 2);
			double costheta = reflectray.theray.direction.dot(next_material.normal).value;
			double schlick = schlickt1 + (1-schlickt1)*pow((1- costheta), 5);
			fresnelterm = nicefp(shlick);
		}

		color += (next_material->bdfrfactor(uv, shadowray.ray.direction, theray.ray.direction, intercept.normal) * fresnelterm).mult(refelctlightcolor);


		// then handle refraction


		
		ray refractray;
		color refractlightcolor = vec3(0,0,0);

		bool didrefract = geom::refract(intercept.position, intercept.normal, next_material->refract_index, theray.currentn, refractray);

		if (didrefract){
			rayoflight refractrayoflight(refractray, 
										next_material.refract_index,  // we're still in the same material
										next_material->beerfactor);

			refractlightcolor = raytrace(refractray, theworld, cutoff - 1, maxdistance);
		}


		color += refractlightcolor * (1 - fresnelterm); // try to conserve engery

		// finally put it all together

		if (theray.currentbeer == 0) return color / intercept.distance; // don't do nexp if not nescessary
		else return color.nexp(intercept.distance * theray.currentbeer) / intercept.distance; //use linear attenuation
	}
}
















