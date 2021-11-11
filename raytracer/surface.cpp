#include <geom.h>
#include <math.h>
#include <unistd.h>

namespace surf {

	using color as vec3;

	struct material {

		bool doesrefract = true;
		bool doesambient = true;
		bool doeslambert = true;
		bool doestransmit = true;
		bool doesfresnel = true;
		bool doesreflect = false;

		vec3 beerfactor = vec3(0,0,0);
		nicefp refract_index = 1;


		virtual color materialcolor(const uv &location) {return vec3(0,0,0);}
		virtual color ambient(const uv &location) {return vec3(0,0,0);} 
		virtual color diffuse(uv location) {return vec3(0,0,0);}
		virtual color specular(uv location) {return vec3(0,0,0);}
		virtual color bdfrfactor(const uv &uv, const vec3 &tolight, const vec3 &fromeye, const vec3 &normal) {return vec3(0,0,0);} //assume linear optics
		

	};

	struct light {

		color color;
		vec3 position
		vec3 direction;

		light(color color, vex3 position, vec3 direction) {

		this.color = color;
		this.position = position;
		this.direction = direction.normalize();
		}

	};

	struct surface {

		const shape* shape;
		material* inside;
		material* outside;

		template<class SHAPE>
		surface(const SHAPE &ashape, const material* insidemat, const material* outsidemat) {

			SHAPE* theshape = (SHAPE*) malloc(sizeof(SHAPE));

			*theshape = ashape;

			this.shape = theshape;
			inside = insidemat;
			outside = outsidemat;

		}

		~surface() {

			free(shape);
		}

	};

	struct openspace: material{

		bool doesrefract = true;
		bool doesambient = false;
		bool doeslambert = false;
		bool doestransmit = true;
		bool doesfresnel = true;
		bool doesreflect = false;

	}

	struct phong: material {

		bool doesrefract = false;
		bool doesambient = true;
		bool doeslambert = true;
		bool doestransmit = false;
		bool doesfresnel = false;
		bool doesreflect = true;

		virtual color shiny(uv uv) = 0;

		virtual color bdfrfactor(const uv &uv, const vec3 &tolight, const vec3 &fromeye, const vec3 &normal) final { // 

			vec3 diff = diffuse(uv);
			color retcolor(0, 0, 0);

			double lambfactor = max(0, (normal.dot(tolight)).value);
			retcolor.x += diff.x * lambfactor;
			retcolor.y += diff.y * lambfactor;
			retcolor.z += diff.z * lambfactor; 
			vec3 spec = specular(uv);
			vec3 shin = shiny(uv);

			vec3 halfvector = (tolight - fromeye).normalize();
			double phongfactor = max(0, normal.dot(halfvector));
			retcolor.x += nicefp(pow(phongfactor, shin.x.value)) * spec.x;
			retcolor.y += nicefp(pow(phongfactor, shin.x.value)) * spec.y;
			retcolor.z += nicefp(pow(phongfactor, shin.x.value)) * spec.z;

			return retcolor;
		}

	};


	struct lambert: material {

		bool doesrefract = false;
		bool doesambient = true;
		bool doeslambert = true;
		bool doestransmit = false;
		bool doesfresnel = false;
		bool doesreflect = false;


		virtual color bdfrfactor(const uv &uv, const vec3 &tolight, const vec3 &fromeye, const vec3 &normal) final {

			vec3 df = diffuse(uv);
			color retcolor(0, 0, 0);

			double lambfactor = max(0, (normal.dot(tolight)).value);
			retcolor.x += diff.x * lambfactor;
			retcolor.y += diff.y * lambfactor;
			retcolor.z += diff.z * lambfactor; 

			return retcolor;
		}


	};

	struct dialectric: material { //dialectrics have no lambert, but do have a shinyness from fresnel; assume all dialectrics' color comes from refraction

		bool doesrefract = true;
		bool doesambient = false;
		bool doeslambert = false;
		bool doestransmit = true;
		bool doesfresnel = true;
		bool doesreflect = true;

		virtual color shiny(uv uv) override = 0;

		virtual color bdfrfactor(const uv &uv, const vec3 &tolight, const vec3 &fromeye, const vec3 &normal) final {

			vec3 shin = shiny(uv);
			color retcolor(0,0,0);

			vec3 halfvector = (tolight - fromeye).normalize();
			double phongfactor = max(0, normal.dot(halfvector));
			retcolor.x = nicefp(pow(phongfactor, shin.x.value));
			retcolor.y = nicefp(pow(phongfactor, shin.y.value));
			retcolor.z = nicefp(pow(phongfactor, shin.z.value));

			return retcolor;

		}

	};

	struct mirror: material {

		bool doesrefract = false;
		bool doesambient = false;
		bool doeslambert = false;
		bool doestransmit = false;
		bool doesfresnel = false;
		bool doesreflect = true;


		virtual color bdfrfactor(const uv &uv, const vec3 &tolight, const vec3 &fromeye, const vec3 &normal) final {

			return specular(uv); // assume a phong exponent of infinity


		}


	};

}



