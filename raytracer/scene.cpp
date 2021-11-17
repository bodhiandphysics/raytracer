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

		return vec3(70,70,70);
	}
};

struct ballmaterial2 : phong {



	ballmaterial2() {

	   doesrefract = false;
	   doesambient = true;
	   doeslambert = true;
	   doestransmit = false;
	   doesfresnel = false;
	   doesreflect = true;
	}

	virtual color materialcolor(const geom::uv &location) override {
		if (int(location.u.value * 20) % 2 == 0) return color(1,0,0);
		else return color (0,1,0);
	}
 	virtual color ambient(const geom::uv &location) override {return color(1,1,1);}
  	virtual color diffuse(const geom::uv &location) override {return color(1,1,1);}
  	virtual color specular(const geom::uv &location) override {return color(1,1,1);}
	virtual color shiny(uv theuv) override {

		return vec3(70,70,70);
	}
};

static ballmaterial mat1; 
static ballmaterial2 mat2; 




world::world *create_scene() {

	camera::camera* thecamera = new camera::camera(vec3(0,0,0), vec3(0,0,-1), vec3(1,0,0),
               vec3(0,1,0), nicefp(.5), nicefp(1),
               nicefp(1), 400, 400);
	world::world *theworld = new world::world(thecamera, vec3(200,200,200));

	vec3 translationamount(0,0,-5);
	vec3 trans2(3, 2, -7);
	ball theball(nicefp(1), &mat1);
	ball anotherball(nicefp(1), &mat2);
	theball.translate(translationamount);
	anotherball.translate(trans2);
	theworld->addsurfacegroup(theball);
	theworld->addsurfacegroup(anotherball);

	theworld->addlight(surf::light(vec3(10000,10000,10000), vec3(0,0, 5), vec3(0,0,-1), nicefp(-1)));
	theworld->addlight(surf::light(vec3(100,100,100), vec3(0,0, -10), vec3(0,0,-1), nicefp(-1)));

	return theworld;  
}
}