#pragma once
#include <surface.h>
#include <vector>
#include <renderer.h>

namespace world {

	static const surf::openspace OPENSPACE;

	using color = geom::vec3;
	using namespace geom
	using namespace surf


	struct surfacegroup {

		std::vector<surf::surface*> surfaces;

		surf::material *inside = &OPENSPACE;
		surf::material *outside = &OPENSPACE;

		
		template<class SHAPE>
		void addshape(SHAPE theshape);
		void rotate(mat3 &by);
		void translate(vec &by);
		void scale(vec3 &by);
		void envelop(surfacegroup& other);

	};


	struct box : surfacegroup {
		box();
	};

	struct ball : surfacegroup {

		ball(nicefp radius);
	};


	struct world {

		std::vector<surf::surface*> surfaces; 
		std::vector<surf::light*> lights;
		camera::camera *thecamera;
		
		color ambientlight;

		world(camera::camera* thecamera, color lightcolor);

		~world();


		virtual color bgcolor();

		void addsurfacegroup(surfacegroup& group);
		void addlight(surf::light alight);

	};
}


