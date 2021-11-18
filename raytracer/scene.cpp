#include "scene.h"


namespace scene {

struct ballmaterial : lambert {

ballmaterial() {
	doesrefract = false;
	doesambient = true;
	doeslambert = true;
	doestransmit = false;
	doesfresnel = false;
	doesreflect = false;
}

virtual color materialcolor(const geom::uv &location) override {return color(1, 0 ,0);}

}


static ballmaterial mat1();  




world::world *create_scene() {

	camera::camera* thecamera = new camera::camera(vec3(0,0,0), vec3(0,0,-1), vec3(1,0,0),
               vec3(0,1,0), nicefp(.5), nicefp(1),
               nicefp(1), 400, 400);
	world::world *theworld = new world::world(thecamera, vec3(200,200,200));

	vec3 translationamount(0,0,-5);
	vec3 trans2(3, 2, -7);
	box thebox(&mat1);
	
	thebox.translate(translationamount);
	
	theworld->addsurfacegroup(thebox);


	theworld->addlight(surf::light(vec3(10000,10000,10000), vec3(0,0, 5), vec3(0,0,-1), nicefp(-1)));

	return theworld;  
}
}