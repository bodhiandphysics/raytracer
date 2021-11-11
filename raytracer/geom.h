namespace geom {
	
	struct nicefp {

		const double FPCUTOFF = .000001;

		double value;

		nicefp(double &avalue);

		nicefp operator+ (const& nicefp other);
		nicefp operator- (const& nicefp other);
		nicefp operator* (const& nicefp other);
		nicefp operator/ (const& nicefp other);
		nicefp operator+ (const& double other);
		nicefp operator- (const& double other);
		nicefp operator* (const& double other);
		nicefp operator/ (const& double other);

		bool operator< (const &nicefp other);
		bool operator== (const nicefp other);

	};

	nicefp sqrt(nicefp x);

	// calculate the negative exp of a nicecp;
	nicefp nicenexp(nicefp x);


	struct vec3 {

		const nicefp x;
		const nicefp y;
		const nicefp z;

		vec3(nicefp x, nicefp y, nicefp z);
		vec3(double x, double y, double z);
		vec3 mult(cons& vec3 other);


		nicefp norm();
		nicefp norm2();
		vec3 normalize();
		vec3 operator+ (const &vec3 other);
		vec3 operator- (const &vec3 other);
		bool operator== (const &vec3 other);
		vec3 operator*(const double mult);
		vec3 operator/(const double div);

		nicefp dot(const &vec3 other);

		vec3 cross(const &vec3 other);

		vec3 nexp(const &vec3 vec);
		vec3 reflect(const&  vec3 &normal);
		
		void translate(cont vec3 &by);

		void rotate(const mat3 &rotation);
		
	};

	struct mat3 {

		vec3 cols[3];

		mat3(const vec3 &col1,const &vec3 col2,const vec3 &col3);
		
		mat3 transpose();

		mat3 leftmult(const mat3 &other);
	};

	
	mat3 genrotz(const nicefp theta);

	mat3 genroty(const nicefp theta);

	mat3 genrotx(const nicefp theta); 

	struct ray {

		vec3 origin;
		vec3 direction; // should always be normalized;

		ray(const vec3 &anorigin, const vec3 &unnormalized);
		ray reflect(const vec3 &point,const vec3 &normal);
		bool refract(const vec3& point, const vec3& normal, const &nicefp inref_index, const &nicefp outref_index, ray& result);

	};

	struct interception {

		nicefp distance;
		vec3 position; // vector from center of object to intercept point
		vec3 normal;
		uv uv;

		interception(const nicefp &distance, const vec3 &position, const vec3 &normal, const uv &uv);
	};

	struct uv {

		nicefp u;
		nicefp v;

		uv(nicefp au, nicefp av);

	};

	struct shape { 

		virtual bool calc_interception(const &ray, const nicefp within_d, &intercetion out);
		virtual void translate(vec &by); // always translate after rotating
		virtual void rotate(vec3 &by); // always rotate before translation
		virtual void scale(double factor);
		virtual uv getuv(vec3 &position);
		virtual vec3 get_normal(vec3 &position) = 0;


	};

	struct triangle: shape {

		vec3 point_a
		vec3 point_b;
		vec3 point_c;
		uv uv_a, uv_b, uv_c;

		vec3 normal;
		nicefp area2;
		nicefp area;

		virtual triangle(vec3 aposition, vec3 aside_a, vec3 aside_b, uv uv_a, uv uv_b, uv uv_c);

		virtual void translate(const vec3 &by) override;
		virtual void rotate(const mat3 &by) override; 
		virtual void scale(double factor) override;


		virtual bool calc_interception(const &ray, const nicefp within_d, &interception out) override; 

		uv getuv(const vec3 &intersect) override;
	};
	

	

	struct sphere: shape {

		vec3 center;
		vec3 orientation;
		nicefp maxangle
		nicefp radius;


		virtual sphere(const vec3 acenter, vec3 orientationvec, nicefp amaxangle, nicefp aradius); 

		virtual vec3 get_normal(const vec3 &position) override; 
		virtual void translate(const vec3 &by) override; 

		virtual void rotate(const vec3 &by) override; 

		vec3 get_normal(const vec3 &position) override; 

		virtual bool interception calc_interception(const &ray, const nicefp within_d, &interception out) override; 
	};

	struct plane: shape {

		vec3 center;
		vec3 normal;
		vec3 u_axis;
		vec3 v_axis; 

		virtual plane(vec3 acenter, vec3 anormal, vec3 au_axis, vec3 av_axis);
		virtual bool calc_interception(const &ray, const nicefp &within_d, &interception out) override;
		virtual void translate(const vec &by) override;
		virtual void rotate(const vec3 &by) override;
		virtual void scale(double factor) override;
		virtual uv getuv(const vec3 &position) override;
		virtual vec3 get_normal(const vec3 &position) override;
	};
}
