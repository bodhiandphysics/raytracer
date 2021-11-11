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

		virtual color materialcolor(const uv &location);
		virtual color ambient(const uv &location);
		virtual color diffuse(const uv &location);
		virtual color specular(const uv &location);
		virtual color bdfrfactor(const uv &uv, const vec3 &tolight, const vec3 &fromeye, const vec3 &normal);



	};

	struct light {

		color color;
		vec3 position
		vec3 direction;

		light(color color, vex3 position, vec3 direction);

	};

	struct surface {

		const shape* shape;
		material* inside;
		material* outside;

		template<class SHAPE>
		surface(const SHAPE &ashape, const material* insidemat, const material* outsidemat);
		~surface();
	};

	struct phong: material {

		bool doesrefract = false;
		bool doesambient = true;
		bool doeslambert = true;
		bool doestransmit = false;
		bool doesfresnel = false;
		bool doesreflect = true;

		virtual color shiny(uv uv);

		virtual color bdfrfactor(const uv &uv, const vec3 &tolight, const vec3 &fromeye, const vec3 &normal) final;

	};

	struct lambert: material {

		bool doesrefract = false;
		bool doesambient = true;
		bool doeslambert = true;
		bool doestransmit = false;
		bool doesfresnel = false;
		bool doesreflect = false;


		virtual color bdfrfactor(const uv &uv, const vec3 &tolight, const vec3 &fromeye, const vec3 &normal) final;

	};

	struct dialectric: material { //dialectrics have no lambert, but do have a shinyness from fresnel; assume all dialectrics' color comes from refraction

		bool doesrefract = true;
		bool doesambient = false;
		bool doeslambert = false;
		bool doestransmit = true;
		bool doesfresnel = true;
		bool doesreflect = true;

		virtual color shiny(uv uv);

		virtual color bdfrfactor(const uv &uv, const vec3 &tolight, const vec3 &fromeye, const vec3 &normal) final;

	};

	struct mirror: material {

		bool doesrefract = false;
		bool doesambient = false;
		bool doeslambert = false;
		bool doestransmit = false;
		bool doesfresnel = false;
		bool doesreflect = true;


		virtual color bdfrfactor(const uv &uv, const vec3 &tolight, const vec3 &fromeye, const vec3 &normal) final;
	};
}













