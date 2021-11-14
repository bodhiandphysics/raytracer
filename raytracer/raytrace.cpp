#include "raytrace.h"



namespace raytrace {

surf::surface<shape> *find_next_surface(geom::ray &theray,
                                 world::world theworld,
                                 geom::interception &intercept,
                                 nicefp maxdistance) {
  nicefp cutoff = maxdistance;
  surf::surface<shape> *retsurf = nullptr;
  bool didintercept;

  for (surf::surface<shape> *checksurf : theworld.surfaces) {

    // rmember to set intercept max distance in caller
    didintercept =
        checksurf->shape->calc_interception(theray, cutoff, intercept);
    if (didintercept) {
      retsurf = checksurf;
      cutoff = intercept.distance;
    }
  }
  return retsurf;
}

color cast_to_light(ray &theray, const world::world &theworld,
                    surf::light &thelight, nicefp starting_n,
                    nicefp maxdistance) {

  vec3 delta = thelight.position - theray.origin;
  nicefp light_distance = delta.norm();
  vec3 currentbeer = vec3(0, 0, 0);
  nicefp current_n = starting_n;

  if (maxdistance < light_distance)
    return color(0, 0, 0); // light is too far
  if ((delta / light_distance).dot(thelight.direction) < thelight.min_cos_angle)
    return color(0, 0, 0); // outside of light cone

  geom::interception intercept(light_distance, vec3(0, 0, 0), vec3(0, 0, 0), uv(nicefp(0),nicefp(0)));

  surf::surface<shape>* next_surface;
  while ((next_surface= find_next_surface(
              theray, theworld, intercept, maxdistance)) != nullptr) {

    surf::material *next_material;
    surf::material *current_material;
    if (theray.direction.dot(intercept.normal) < 0) {

      next_material = next_surface->inside;
      current_material = next_surface->outside;
    } else {
      next_material = next_surface->outside;
      current_material = next_surface->inside;
    }

    if (!next_material->doestransmit)
      return color(0, 0, 0); // we hit an opaque surface

    nicefp next_n = next_material->refract_index; // we now need to check for
                                                  // total internal refraction

    if (current_n < next_n) {

      nicefp ddotn = theray.direction.dot(intercept.normal);
      if (ddotn * ddotn < ((current_n * current_n) / (next_n * next_n)) - 1)
        return color(0, 0, 0); // Total internal reflection
    }

    currentbeer = currentbeer + current_material->beerfactor * intercept.distance;
    current_n = next_n;

    if (currentbeer.norm2() > nicefp(200))
      return color(0, 0, 0); // to much in the way
  }

  // we break out of the loop at the light

  return thelight.lightcolor.nexp(currentbeer) /
         light_distance; // use linear as opposed to quadratic attenuation...
                         // better for close distances
}

color raytrace(ray &theray, world::world &theworld, int cutoff,
               nicefp maxdistance) {

  color retcolor = vec3(0, 0, 0);
  if (cutoff == 0)
    return retcolor;

  geom::interception intercept(maxdistance, vec3(0, 0, 0), vec3(0, 0, 0), uv(nicefp(0), nicefp(0)));

  surf::surface<shape> *next_surface =
      find_next_surface(theray, theworld, intercept, maxdistance);
  if (next_surface == nullptr)
    return theworld.bgcolor(); // we went off the map

  surf::material *next_material;
  surf::material *current_material;
  if (theray.direction.dot(intercept.normal) < 0) {

    next_material = next_surface->inside;
    current_material = next_surface->outside;
  } else {
    next_material = next_surface->outside;
    current_material = next_surface->inside;
  }

  nicefp distance = intercept.distance;

  geom::uv uv = intercept.atuv;
  // first do global illumination
  if (next_material->doesambient) {

    retcolor = retcolor + next_material->materialcolor(uv).mult(next_material->ambient(uv))
                    .mult(theworld.ambientlight);
  }

  // next cast to all the light sources

  for (surf::light &thelight : theworld.lights) {

    // find if we need to transition matieral

    vec3 direction_to_light = intercept.position - thelight.position;
    nicefp currentn;
    if (direction_to_light.dot(next_surface->shape->get_normal(intercept.position)) < 0)
      currentn = next_material->refract_index; // going through new material
    else
      currentn =
          current_material->refract_index; // going through the same material

    ray shadowray(intercept.position, direction_to_light);

    color shadowlightcolor =
        cast_to_light(shadowray, theworld, thelight, currentn, maxdistance);
    retcolor = retcolor + next_material
                 ->bdfrfactor(uv, shadowray.direction,
                              theray.direction, intercept.normal)
                 .mult(shadowlightcolor);
  }

  // next handle reflections

  vec3 reflectdirection = theray.direction.reflect(intercept.normal);
  ray reflectray(intercept.position, reflectdirection);

  color reflectlightcolor =
      raytrace(reflectray, theworld, cutoff - 1, maxdistance);

  nicefp fresnelterm = 1;
  if (next_material->doesfresnel) {
    double schlickt1 =
        pow(((next_material->refract_index - current_material->refract_index) /
             (next_material->refract_index + current_material->refract_index))
                .value,
            2);
    double costheta =
        reflectray.direction.dot(intercept.normal).value;
    double schlick = schlickt1 + (1 - schlickt1) * pow((1 - costheta), 5);
    fresnelterm = nicefp(schlick);
  }

  retcolor = retcolor + (next_material->bdfrfactor(uv, reflectray.direction,
                                      theray.direction, intercept.normal) *
            fresnelterm)
               .mult(reflectlightcolor);

  // then handle refraction

  ray refractray;
  color refractlightcolor = vec3(0, 0, 0);
  bool didrefract = theray.refract(intercept.position, intercept.normal,
                                  next_material->refract_index,
                                  current_material->refract_index, refractray);

  if (didrefract)
    refractlightcolor = raytrace(refractray, theworld, cutoff - 1, maxdistance);
  retcolor = retcolor + refractlightcolor * (nicefp(1) - fresnelterm); // try to conserve engery

  // finally put it all together

  vec3 currentbeer = current_material->beerfactor;
  if (currentbeer == vec3(0,0,0))
    return retcolor / intercept.distance; // don't do nexp if not nescessary
  else 
    return retcolor.nexp(currentbeer * intercept.distance) /
           intercept.distance; // use linear attenuation
  
}
} // namespace raytrace
