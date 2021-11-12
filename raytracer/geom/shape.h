namespace geom {	

	struct uv {

		nicefp u;
		nicefp v;

		uv(nicefp au, nicefp av);

	};

	struct interception {

		nicefp distance;
		vec3 position; // vector from center of object to intercept point
		vec3 normal;
		uv atuv;

		interception(const nicefp &distance, const vec3 &position, const vec3 &normal, const uv &uv);
	};

	struct shape { 

		virtual bool calc_interception(const ray& theray, const nicefp within_d, interception& out);
		virtual void translate(vec3 &by); // always translate after rotating
		virtual void rotate(vec3 &by); // always rotate before translation
		virtual void scale(double factor);
		virtual uv getuv(vec3 &position);
		virtual vec3 get_normal(vec3 &position) = 0;


	};

	struct triangle: shape {

		vec3 point_a;
		vec3 point_b;
		vec3 point_c;
		uv uv_a, uv_b, uv_c;

		vec3 normal;
		nicefp area2;
		nicefp area;

		triangle(vec3 aposition, vec3 aside_a, vec3 aside_b, uv uv_a, uv uv_b, uv uv_c);

		virtual void translate(const vec3 &by);
		virtual void rotate(const mat3 &by); 
		virtual void scale(double factor);


		virtual bool calc_interception(const ray& theray, const nicefp within_d, interception& out); 

		uv getuv(const vec3 &intersect);
	};
	

	

	struct sphere: shape {

		vec3 center;
		vec3 orientation;
		vec3 orientation_equator;
		nicefp maxangle;
		nicefp radius;


		sphere(const vec3 acenter, vec3 orientationvec, vec3 orientation_equatorvec, nicefp amaxangle, nicefp aradius); 

		virtual vec3 get_normal(const vec3 &position); 
		virtual void translate(const vec3 &by); 

		virtual void rotate(const vec3 &by); 

		virtual bool calc_interception(const ray& theray, const nicefp within_d, interception& out); 
	};

	struct plane: shape {

		vec3 center;
		vec3 normal;
		vec3 u_axis;
		vec3 v_axis; 

		plane(vec3 acenter, vec3 anormal, vec3 au_axis, vec3 av_axis);
		virtual bool calc_interception(const ray& theray, const nicefp &within_d, interception& out);
		virtual void translate(const vec3 &by);
		virtual void rotate(const vec3 &by);
		virtual void scale(double factor);
		virtual uv getuv(const vec3 &position);
		virtual vec3 get_normal(const vec3 &position);
	};

}