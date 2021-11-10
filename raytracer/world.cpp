#include <geom.h>
#include <surface.h>
#include <vector>

namespace world {

	static const surf::openspace OPENSPACE;

	using color = geom::vec3;


	struct surfacegroup {

		std::vector<surf::surface*> &surfaces;

		surf::material *inside = &OPENSPACE;
		surf::material *outside = &OPENSPACE;

		surfacegroup() {

			surfaces = std::vector<surf::surface>();
		}

		
		template<class SHAPE>
		void addshape(SHAPE theshape) {

			auto thesurface = new surf::surface<SHAPE>(theshape, inside, outside);
			surfaces.push_back(thesurface);

		}

		void rotate

		void translate

		void scale



	}






		std::vector<surf::light> &lights;
		color ambientlight;

		world ambientlight(color lightcolor) {

			
			lights = std::vector<surf::surfaces>();

			ambientlight = lightcolor;

		}

		virtual color bgcolor();


	};





}
