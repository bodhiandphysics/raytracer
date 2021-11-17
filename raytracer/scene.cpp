#include "scene.h"


namespace scene {

struct ballmaterial : phong {



	ballmaterial() {

	   doesrefract = false;
	   doesambient = true;
	   doeslambert = true;
	   doestransmit = false;
	   doesfresnel = false;
	   doesreflect = true;
	}

	virtual color materialcolor(const geom::uv &location) override {return color(1, 0 ,0);}
 	virtual color ambient(const geom::uv &location) override {return color(1,1,1);}
  	virtual color diffuse(const geom::uv &location) override {return color(1,1,1);}
  	virtual color specular(const geom::uv &location) override {return color(1,1,1);}
	virtual color shiny(uv theuv) override {

		return vec3(20,20,20);
	}
};

static ballmaterial mat1; 



world::world *create_scene() {

	camera::camera* thecamera = new camera::camera(vec3(0,0,0), vec3(0,0,-1), vec3(1,0,0),
               vec3(0,1,0), nicefp(.5), nicefp(1),
               nicefp(1), 400, 400);
	world::world *theworld = new world::world(thecamera, vec3(0,0,0));

	vec3 translationamount(0,0,-5);
	ball theball(nicefp(1), &mat1);
	theball.translate(translationamount);
	theworld->addsurfacegroup(theball);

	theworld->addlight(surf::light(vec3(10000,10000,10000), vec3(0,0, 5), vec3(0,0,-1), nicefp(-1)));

	return theworld;  
}
}