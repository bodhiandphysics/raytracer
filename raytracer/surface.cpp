#include "surface.h"
#include <algorithm>

namespace surf {

light::light(color alightcolor, geom::vec3 aposition, geom::vec3 adirection) {

  lightcolor = alightcolor;
  position = aposition;
  direction = adirection.normalize();
}

template <class SHAPE>
surface::surface(const SHAPE &ashape, const material *insidemat,
                 const material *outsidemat) {

  shape = new SHAPE;

  *shape = ashape;
  inside = insidemat;
  outside = outsidemat;
}

surface::~surface() { delete shape;}

color phong::bdfrfactor(const geom::uv &uv, geom::vec3 &tolight,
                        const geom::vec3 &fromeye, geom::vec3 &normal) { //

  geom::vec3 diff = diffuse(uv);
  color retcolor(0, 0, 0);

  double lambfactor = std::max(0.0, (normal.dot(tolight)).value);
  retcolor.x += diff.x * lambfactor;
  retcolor.y += diff.y * lambfactor;
  retcolor.z += diff.z * lambfactor;
  geom::vec3 spec = specular(uv);
  geom::vec3 shin = shiny(uv);

  geom::vec3 halfvector = (tolight - fromeye).normalize();
  double phongfactor = std::max(0.0, normal.dot(halfvector).value);
  retcolor.x += geom::nicefp(pow(phongfactor, shin.x.value)) * spec.x;
  retcolor.y += geom::nicefp(pow(phongfactor, shin.x.value)) * spec.y;
  retcolor.z += geom::nicefp(pow(phongfactor, shin.x.value)) * spec.z;

  return retcolor;
}

color lambert::bdfrfactor(const geom::uv &uv, geom::vec3 &tolight,
                          const geom::vec3 &fromeye, geom::vec3 &normal) {

  geom::vec3 diff = diffuse(uv);
  color retcolor(0, 0, 0);

  double lambfactor = std::max(0.0, (normal.dot(tolight)).value);
  retcolor.x += diff.x * lambfactor;
  retcolor.y += diff.y * lambfactor;
  retcolor.z += diff.z * lambfactor;

  return retcolor;
}

color dialectric::bdfrfactor(const geom::uv &uv, geom::vec3 &tolight,
                             const geom::vec3 &fromeye, geom::vec3 &normal) {

  geom::vec3 shin = shiny(uv);
  color retcolor(0, 0, 0);

  geom::vec3 halfvector = (tolight - fromeye).normalize();
  double phongfactor = std::max(0.0, normal.dot(halfvector).value);
  retcolor.x = geom::nicefp(pow(phongfactor, shin.x.value));
  retcolor.y = geom::nicefp(pow(phongfactor, shin.y.value));
  retcolor.z = geom::nicefp(pow(phongfactor, shin.z.value));

  return retcolor;
}

color mirror::bdfrfactor(const geom::uv &uv, geom::vec3 &tolight,
                         const geom::vec3 &fromeye, geom::vec3 &normal) {

  return specular(uv); // assume a phong exponent of infinity
}

} // namespace surf
