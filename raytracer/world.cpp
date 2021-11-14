
#include "world.h"

namespace world {

template <class SHAPE> void surfacegroup::addshape(SHAPE theshape) {

  surf::surface<SHAPE>* thesurface = new surf::surface(theshape, inside, outside);
  surfaces.push_back((surf::surface<shape>*) thesurface);
}

void surfacegroup::rotate(mat3 &by) {

  for (surf::surface<shape> *asurface : surfaces)
    asurface->shape->rotate(by);
}

void surfacegroup::translate(vec3 &by) {

  for (surf::surface<shape> *asurface : surfaces)
    asurface->shape->translate(by);
}

void surfacegroup::scale(vec3 &by) {

  for (surf::surface<shape> *asurface : surfaces)
    asurface->shape->scale(by);
}

// need to add uv mapping here, but i'm not going to bother for now

void surfacegroup::envelop(surfacegroup &other) { // make sure other is actually
                                                  // inside of this surface!!

  other.outside = inside;
  for (surf::surface<shape> *asurface : other.surfaces)
    asurface->outside = inside;
}

box::box() {

  addshape<triangle>(triangle(vec3(0, 0, 0), vec3(0, 1, 0),
                                       vec3(1, 0, 0), vec3(0, 0, 1), uv(0, 0),
                                       uv(0, 0), uv(0, 0)));
  addshape<triangle>(triangle(vec3(1, 1, 0), vec3(-1, 0, 0),
                                       vec3(0, -1, 0), vec3(0, 0, 1), uv(0, 0),
                                       uv(0, 0), uv(0, 0))); // + z triangles

  addshape<triangle>(triangle(vec3(0, 0, -1), vec3(0, 1, 0),
                                       vec3(1, 0, 0), vec3(0, 0, -1), uv(0, 0),
                                       uv(0, 0), uv(0, 0)));
  addshape<triangle>(triangle(vec3(1, 1, -1), vec3(-1, 0, 0),
                                       vec3(0, -1, 0), vec3(0, 0, -1), uv(0, 0),
                                       uv(0, 0), uv(0, 0))); // - z triangles

  addshape<triangle>(triangle(vec3(0, 0, 0), vec3(0, 0, -1),
                                       vec3(0, 1, 0), vec3(-1, 0, 0), uv(0, 0),
                                       uv(0, 0), uv(0, 0)));
  addshape<triangle>(triangle(vec3(0, 1, -1), vec3(0, 0, 1),
                                       vec3(0, -1, 0), vec3(-1, 0, 0), uv(0, 0),
                                       uv(0, 0), uv(0, 0))); // - x triangles

  addshape<triangle>(triangle(vec3(1, 0, 0), vec3(0, 0, -1),
                                       vec3(0, 1, 0), vec3(1, 0, 0), uv(0, 0),
                                       uv(0, 0), uv(0, 0)));
  addshape<triangle>(triangle(vec3(1, 1, 0), vec3(-1, 0, 0),
                                       vec3(0, -1, 0), vec3(1, 0, 0), uv(0, 0),
                                       uv(0, 0), uv(0, 0))); // + x triangles

  addshape<triangle>(triangle(vec3(0, 0, 0), vec3(0, 0, -1),
                                       vec3(1, 0, 0), vec3(0, -1, 0), uv(0, 0),
                                       uv(0, 0), uv(0, 0)));
  addshape<triangle>(triangle(vec3(1, 0, -1), vec3(0, 0, 1),
                                       vec3(-1, 0, 0), vec3(0, -1, 0), uv(0, 0),
                                       uv(0, 0), uv(0, 0))); // - y triangles

  addshape<triangle>(triangle(vec3(0, 1, 0), vec3(0, 0, -1),
                                       vec3(1, 0, 0), vec3(0, 1, 0), uv(0, 0),
                                       uv(0, 0), uv(0, 0)));
  addshape<triangle>(triangle(vec3(1, 1, -1), vec3(0, 0, 1),
                                       vec3(-1, 0, 0), vec3(0, 1, 0), uv(0, 0),
                                       uv(0, 0), uv(0, 0))); // + y triangles
}

ball::ball(nicefp radius) {

  addshape<geom::sphere>(
      sphere(vec3(0, 0, 0), vec3(0, 1, 0), vec3(1,0,0), nicefp(-1), radius));
}

world::world(camera::camera *thecamera, color lightcolor) {

  surfaces = std::vector<surf::surface<shape>*>();
  lights = std::vector<surf::light>();
  this->thecamera = thecamera;
  ambientlight = lightcolor;
}

world::~world() {

  for (surf::surface<shape>* surface : surfaces)
    delete surface;
  delete thecamera;
}

void world::addsurfacegroup(surfacegroup &group) {

  for (surf::surface<shape> *surface : group.surfaces)
    surfaces.push_back(surface);
}

void world::addlight(surf::light alight) { lights.push_back(alight); }

} // namespace world
