#include <geom.h>
#include <surface.h>
#include <vector>

namespace world {

	static const surf::openspace OPENSPACE;

	using color = geom::vec3;
	using geom


	struct surfacegroup {

		std::vector<surf::surface*> surfaces;

		surf::material *inside = &OPENSPACE;
		surf::material *outside = &OPENSPACE;

		
		template<class SHAPE>
		void addshape(SHAPE theshape) {

			auto thesurface = new surf::surface<SHAPE>(theshape, inside, outside);
			surfaces.push_back(thesurface);

		}

		void rotate(mat3 &by) {

			for (surf::surface* asurface : surfaces) surface->shape.rotate(by);
		}

		void translate(vec &by) {

			for (surf::surface* asurface : surfaces) surface->shape.translate(by);
		}

		void scale(double factor) {

			for (surf::surface* asurface : surfaces) surface->shape.scale(by);

		}

		// need to add uv mapping here, but i'm not going to bother for now


		void envelop(surfacegroup& other) { // make sure other is actually inside of this surface!!

			other.outside = inside; 
		}

	};


	struct box : surfacegroup {

		box() {

			surfaces.addshape<geom::triangle>(geom::triangle(vec3(0,0,0),vec3(0,1,0), vec3(1,0,0), vec3(0, 0, 1), uv(0,0), uv(0,0) uv(0,0)));
			surfaces.addshape<geom::triangle>(geom::triangle(vec3(1,1,0),vec3(-1,0,0), vec3(0,-1,0), vec3(0, 0, 1), uv(0,0), uv(0,0) uv(0,0))); // + z triangles
			
			surfaces.addshape<geom::triangle>(geom::triangle(vec3(0,0,-1),vec3(0,1,0), vec3(1,0,0), vec3(0, 0, -1), uv(0,0), uv(0,0) uv(0,0)));
			surfaces.addshape<geom::triangle>(geom::triangle(vec3(1,1,-1),vec3(-1,0,0), vec3(0,-1,0), vec3(0, 0, -1), uv(0,0), uv(0,0) uv(0,0))); // - z triangles
			

			surfaces.addshape<geom::triangle>(geom::triangle(vec3(0,0,0), vec3(0,0,-1), vec3(0,1,0), vec3(-1, 0, 0), uv(0,0), uv(0,0) uv(0,0)));
			surfaces.addshape<geom::triangle>(geom::triangle(vec3(0,1,-1), vec3(0,0,1), vec3(0,-1,0), vec3(-1, 0, 0), uv(0,0), uv(0,0) uv(0,0))); // - x triangles
			
			surfaces.addshape<geom::triangle>(geom::triangle(vec3(1,0,0), vec3(0,0,-1), vec3(0,1,0), vec3(1, 0, 0), uv(0,0), uv(0,0) uv(0,0)));
			surfaces.addshape<geom::triangle>(geom::triangle(vec3(1,1,0), vec3(-1,0,0), vec3(0,-1,0), vec3(1, 0, 0), uv(0,0), uv(0,0) uv(0,0))); // + x triangles

			surfaces.addshape<geom::triangle>(geom::triangle(vec3(0,0,0), vec3(0,0,-1), vec3(1,0,0), vec3(0, -1, 0), uv(0,0), uv(0,0) uv(0,0)));
			surfaces.addshape<geom::triangle>(geom::triangle(vec3(1,0,-1), vec3(0,0,1), vec3(-1,0,0), vec3(0, -1, 0), uv(0,0), uv(0,0) uv(0,0))); // - y triangles
			
			surfaces.addshape<geom::triangle>(geom::triangle(vec3(0,1,0), vec3(0,0,-1), vec3(1,0,0), vec3(0, 1, 0), uv(0,0), uv(0,0) uv(0,0)));
			surfaces.addshape<geom::triangle>(geom::triangle(vec3(1,1,-1), vec3(0,0,1), vec3(-1,0,0), vec3(0, 1, 0), uv(0,0), uv(0,0) uv(0,0))); // + y triangles
	
		}


	};

	struct ball : surfacegroup {

		ball(nicefp radius) {

			surfacegroup.addshape<geom::sphere>(geom::sphere(vec3(0, 0, 0), vec3(0,1,0), nicefp(-1), radius)); 
		}
	};


	struct world {

		std::vector<surf::surface*> surfaces; 
		std::vector<surf::light*> lights;
		color ambientlight;

		world(color lightcolor) {

			surfaces = std::vector<surf::surface*>()
			lights = std::vector<surf::light*>();

			ambientlight = lightcolor;

		}

		~world() {

			for (surf::surface* surface : surfaces) free(surface);
			for (surf::light* light : lights) free(light);
		}

		virtual color bgcolor();

		void addsurfacegroup(surfacegroup& group) {

			for (surf::surface* surface : group) surfaces.push_back(surface);
		}

		void addlight(surf::light alight) {

			surf::light* lightptr = (surf::light*) malloc(sizeof(suf::light));

			*lightptr = alight;

			lights.push_back(alight);
		}




	};





}
