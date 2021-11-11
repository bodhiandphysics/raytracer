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

		