#pragma once
#include "geom/shape.h"
#include "geom/vec3.h"
#include <math.h>
#include <unistd.h>

namespace surf {

using color = geom::vec3;
using namespace geom;

struct material {

  bool doesrefract = true;
  bool doesambient = true;
  bool doeslambert = true;
  bool doestransmit = true;
  bool doesfresnel = true;
  bool doesreflect = false;

  geom::vec3 beerfactor = geom::vec3(0, 0, 0);
  geom::nicefp refract_index = 1;

  virtual color materialcolor(const geom::uv &location) {return color(0,0,0);}
  virtual color ambient(const geom::uv &location) {return color(0,0,0);}
  virtual color diffuse(const geom::uv &location) {return color(0,0,0);}
  virtual color specular(const geom::uv &location) {return color(0,0,0);}
  virtual color bdfrfactor(const geom::uv &uv, geom::vec3 &tolight,
                           const geom::vec3 &fromeye, geom::vec3 &normal) {return color(0,0,0);}
};

struct openspace : material {

};

struct light {

  color lightcolor;
  geom::vec3 position;
  geom::vec3 direction;
  geom::nicefp min_cos_angle;

  light(color lightcolor, geom::vec3 position, geom::vec3 direction, nicefp min_cos_angle);
};

struct surface {

  geom::shape *shape;
  material *inside;
  material *outside;

  surface(geom::shape *ashape,  material *insidemat,
          material *outsidemat);
  ~surface();
};

struct phong : material {

  bool doesrefract = false;
  bool doesambient = true;
  bool doeslambert = true;
  bool doestransmit = false;
  bool doesfresnel = false;
  bool doesreflect = true;

  virtual color shiny(geom::uv uv) {return vec3(0,0,0);}

  virtual color bdfrfactor(const geom::uv &uv, geom::vec3 &tolight,
                           const geom::vec3 &fromeye, geom::vec3 &normal) final;
};

struct lambert : material {

  bool doesrefract = false;
  bool doesambient = true;
  bool doeslambert = true;
  bool doestransmit = false;
  bool doesfresnel = false;
  bool doesreflect = false;

  virtual color bdfrfactor(const geom::uv &uv, geom::vec3 &tolight,
                           const geom::vec3 &fromeye, geom::vec3 &normal) final;
};

struct dialectric
    : material { // dialectrics have no lambert, but do have a shinyness from
                 // fresnel; assume all dialectrics' color comes from refraction

  bool doesrefract = true;
  bool doesambient = false;
  bool doeslambert = false;
  bool doestransmit = true;
  bool doesfresnel = true;
  bool doesreflect = true;

  virtual color shiny(geom::uv uv);

  virtual color bdfrfactor(const geom::uv &uv, geom::vec3 &tolight,
                           const geom::vec3 &fromeye, geom::vec3 &normal) final;
};

struct mirror : material {

  bool doesrefract = false;
  bool doesambient = false;
  bool doeslambert = false;
  bool doestransmit = false;
  bool doesfresnel = false;
  bool doesreflect = true;

  virtual color bdfrfactor(const geom::uv &uv, geom::vec3 &tolight,
                           const geom::vec3 &fromeye, geom::vec3 &normal) final;
};
} // namespace surf
