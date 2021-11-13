
#include <surface.h>
#include <vector>

namespace world {

	
	template<class SHAPE>
	void surfacegroup::addshape(SHAPE theshape) {

		auto thesurface = new surf::surface<SHAPE>(theshape, inside, outside);
		surfaces.push_back(thesurface);

	}

	void surfacegroup::rotate(mat3 &by) {

		for (surf::surface* asurface : surfaces) surface->shape.rotate(by);
	}

	void surfacegroup::translate(vec &by) {

		for (surf::surface* asurface : surfaces) surface->shape.translate(by);
	}

	void surfacegroup::scale(double factor) {

		for (surf::surface* asurface : surfaces) surface->shape.scale(by);

	}

	// need to add uv mapping here, but i'm not going to bother for now


	void surfacegroup::envelop(surfacegroup& other) { // make sure other is actually inside of this surface!!

		other.outside = inside; 
		for (surf::surface* asurface : other.surfaces) asurface->outside = inside;
	}





	box::box() {

		surfaces.addshape<triangle>(triangle(vec3(0,0,0),vec3(0,1,0), vec3(1,0,0), vec3(0, 0, 1), uv(0,0), uv(0,0) uv(0,0)));
		surfaces.addshape<triangle>(triangle(vec3(1,1,0),vec3(-1,0,0), vec3(0,-1,0), vec3(0, 0, 1), uv(0,0), uv(0,0) uv(0,0))); // + z triangles
		
		surfaces.addshape<triangle>(triangle(vec3(0,0,-1),vec3(0,1,0), vec3(1,0,0), vec3(0, 0, -1), uv(0,0), uv(0,0) uv(0,0)));
		surfaces.addshape<triangle>(triangle(vec3(1,1,-1),vec3(-1,0,0), vec3(0,-1,0), vec3(0, 0, -1), uv(0,0), uv(0,0) uv(0,0))); // - z triangles
		

		surfaces.addshape<triangle>(triangle(vec3(0,0,0), vec3(0,0,-1), vec3(0,1,0), vec3(-1, 0, 0), uv(0,0), uv(0,0) uv(0,0)));
		surfaces.addshape<triangle>(triangle(vec3(0,1,-1), vec3(0,0,1), vec3(0,-1,0), vec3(-1, 0, 0), uv(0,0), uv(0,0) uv(0,0))); // - x triangles
		
		surfaces.addshape<triangle>(triangle(vec3(1,0,0), vec3(0,0,-1), vec3(0,1,0), vec3(1, 0, 0), uv(0,0), uv(0,0) uv(0,0)));
		surfaces.addshape<triangle>(triangle(vec3(1,1,0), vec3(-1,0,0), vec3(0,-1,0), vec3(1, 0, 0), uv(0,0), uv(0,0) uv(0,0))); // + x triangles

		surfaces.addshape<triangle>(triangle(vec3(0,0,0), vec3(0,0,-1), vec3(1,0,0), vec3(0, -1, 0), uv(0,0), uv(0,0) uv(0,0)));
		surfaces.addshape<triangle>(triangle(vec3(1,0,-1), vec3(0,0,1), vec3(-1,0,0), vec3(0, -1, 0), uv(0,0), uv(0,0) uv(0,0))); // - y triangles
		
		surfaces.addshape<triangle>(triangle(vec3(0,1,0), vec3(0,0,-1), vec3(1,0,0), vec3(0, 1, 0), uv(0,0), uv(0,0) uv(0,0)));
		surfaces.addshape<triangle>(triangle(vec3(1,1,-1), vec3(0,0,1), vec3(-1,0,0), vec3(0, 1, 0), uv(0,0), uv(0,0) uv(0,0))); // + y triangles

	}



	

	bal::ball(nicefp radius) {

		surfacegroup.addshape<geom::sphere>(sphere(vec3(0, 0, 0), vec3(0,1,0), nicefp(-1), radius)); 
	}


	

	world::world(camera::camera* thecamera, color lightcolor) {

		surfaces = std::vector<surf::surface*>()
		lights = std::vector<surf::light>();
		this->thecamera = thecamera;
		ambientlight = lightcolor;

	}

	world::~world() {

		for (surf::surace* surface: surfaces) delete surface;
		delete thecamera;
	}


		

	void world::addsurfacegroup(surfacegroup& group) {

		for (surf::surface* surface : group) surfaces.push_back(surface);
	}

	void world::addlight(surf::light alight) {

		lights.push_back(alight);
	}




}






