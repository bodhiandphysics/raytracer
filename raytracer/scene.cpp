#include "scene.h"


namespace scene {

struct ballmaterial : phong {



	virtual color materialcolor(const geom::uv &location) override {return color(.9, 0 ,0);}
 	virtual color ambient(const geom::uv &location) override {return color(1,1,1);}
  	virtual color diffuse(const geom::uv &location) override {return color(.4,.4,.4);}
  	virtual color specular(const geom::uv &location) override {return color(.4,.4,.4);}
	virtual color shiny(uv theuv) override {

		return vec3(25,25, 25);
	}
};

static ballmaterial mat1; 



world::world *create_scene() {

	camera::camera* thecamera = new camera::camera(vec3(0,0,0), vec3(0,0,-1), vec3(1,0,0),
               vec3(0,1,0), nicefp(.5), nicefp(1),
               nicefp(1), 800, 800);
	world::world *theworld = new world::world(thecamera, vec3(1,1,1));

	vec3 translationamount(4,-4,-7);
	ball theball(nicefp(1), &mat1);
	theball.translate(translationamount);
	theworld->addsurfacegroup(theball);

	theworld->addlight(surf::light(vec3(1,1,1), vec3(0,0, 5), vec3(0,0,-1), nicefp(-1)));

	return theworld;  
}
}