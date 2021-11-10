namespace geom {
	
	struct nicefp {

		const double FPCUTOFF = .000001;

		double value;

		nicefp(double &avalue);

		nicefp operator+ (const& nicefp other);
		nicefp operator- (const& nicefp other);
		nicefp operator* (const& nicefp other);
		nicefp operator/ (const& nicefp other);

		bool operator< (const &nicefp other);
		bool operator== (const nicefp other);

	};

	nicefp nicesqrt(nicefp x);

	// calculate the negative exp of a nicecp;
	nicefp nicenexp(nicefp x)l;


	struct vec3 {

		const nicefp x;
		const nicefp y;
		const nicefp z;

		nicefp norm();

		nicefp norm2();
		vec3 operator+ (const &vec3 other);
		vec3 operator- (const &vec3 other);
		bool operator== (const &vec3 other);
		vec3 operator*(double mult);
		vec3 operator/(double div);

		nicefp dot(const &vec3 other);

		vec3 cross(const &vec3 other);

		vec3 nexp(vec3 vec);
		void translate(vec3 &by);

		void rotate(mat3 &rotation);
		
	};

	struct mat3 {

		vec3 cols[3];

		mat3(vec3 row1, vec3 row2, vec3 row3);
		mat3 transpose();

		mat3 leftmult(mat3 &other);
	};

	
	mat3 genrotz(nicefp theta);

	mat3 genroty(nicefp theta);

	mat3 genrotx(nicefp theta); 

	struct ray {

		vec3 origin;
		vec3 direction; // should always be normalized;

		ray(vec3 anorigin, vec3 unnormalized);
	};

	struct interception {

		nicefp distance;
		vec3 position; // vector from center of object to intercept point
		vec3 normal;
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
		virtual uv getuv(vec3 &position);

	};

	struct triangle: shape {

		vec3 point_a
		vec3 point_b;
		vec3 point_c;
		uv uv_a, uv_b, uv_c;

		vec3 normal;

		virtual triangle(vec3 aposition, vec3 aside_a, vec3 aside_b, uv uv_a, uv uv_b, uv uv_c);
		virtual ~triangle() = default;

		

		virtual void translate(vec &by);


		virtual void rotate(mat3 &by); 


		virtual bool calc_interception(const &ray, const nicefp within_d, &interception out); 

		uv getuv(vec3 &intersect);
	};
	

	

	struct sphere: shape {

		vec3 center;
		vec3 orientation;
		nicefp maxangle
		nicefp radius;


		virtual sphere(vec3 acenter, vec3 orientationvec, nicefp amaxangle); 

		virtual sphere(vec3 acenter, nicefp radius); 
		virtual ~sphere() = default;

		virtual vec3 get_normal(vec3 position); 
		virtual void translate(vec &by); 

		virtual void rotate(vec3 &by); 

		vec3 get_normal(vec3 position); 

		virtual bool interception calc_interception(const &ray, const nicefp within_d, &interception out); 
	};
}