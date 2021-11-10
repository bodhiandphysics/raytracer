#include <math.h>

namespace geom {
	
	struct nicefp {

		const double FPCUTOFF = .000001;

		double value;

		nicefp(double &avalue) {

			this.value = value;
		}


		nicefp operator+ (const& nicefp other) {return nicepf(value + other.value)};
		nicefp operator- (const& nicefp other) {return nicefp(value - other.value)};
		nicefp operator* (const& nicefp other) {return nicefp(value * other.value)};
		nicefp operator/ (const& nicefp other) {return nicefp(value / other.value)};

		nicefp operator+ (const& double other) {return value + other};
		nicefp operator- (const& double other) {return value - other};
		nicefp operator* (const& double other) {return value * other};
		nicefp operator/ (const& double other) {return value / other};

		bool operator< (const &nicefp other) {

			double difference = this.value - other.value;
			if (difference < FPCUTOFF) return true;
			return false; 

		}
		bool operator== (const nicefp other) {

			double difference = this.value - other.value;

			if (difference < FPCUTOFF && difference > -FPCUTOFF) return true;
			return false
		}

	};

	nicefp nicesqrt(nicefp x) {

		return nicefp(sqrt(x.value));
	}


	// calculate the negative exp of a nicecp;
	nicefp nicenexp(nicefp x) {

		if (x = 0) return 1;
		else return exp(-x.vlaue);

	}
	struct vec3 {

		const nicefp x;
		const nicefp y;
		const nicefp z;

		vec3(nicefp x, nicefp y, nicefp z) {

			this.x = x;
			this.y = y;
			this.z = z;
		}

		vec3(double x, double y, double z) {

			this.x = nicefp(x);
			this.y = nicefp(y);
			this.z = nicefp(z);
		}

		vec3 mult(cons& vec3 other) { // a termwise mult

			return vec3(x*other.x, y*other.y, z*other.z);
		}

		nicefp norm() {

			return sqrt(x*x + y*y + z*z);
		}

		nicefp norm2() {

			return x*x + y*y + z*z;
		}

		vec3 normalize() {


			nicefp thenorm = this.norm;
			if (thenorm == 0) // the direction is arbitrary
				return vec3(nicefp(1), nicefp(0), nicefp(0))
			else return vec3(x/thenorm, y/thenorm, z/thenorm);
		}

		vec3 operator+ (const &vec3 other) {

			return vec3(x + other.x, y + other.y, z + other.z);
		}

		vec3 operator- (const &vec3 other) {

			return vec3(x - other.x, y - other.y, z - other.z);
		}

		bool operator== (const &vec3 other) {

			return x == other.x && y == other.y && z == other.z;

		}


		vec3 operator*(const &nicefp mult) {

			return vec3(x * mult, y * mult, z * mult);
		}

		vec3 operator/(const &nicefp div) {
			return vec3(x / div, y / div, z / div);
		}

		}

		bool iszero() {

			return return x.value == 0 && y.value == 0 && z.value == 0;

		}

		nicefp dot(const &vec3 other) {

			return x*other.x + y*other.y + z*other.z;
		}

		vec3 cross(const &vec3 other) {

			nicefp returnz = x*other.y - y*other.x;
			nicefp returny = z*other.x - x*other.z;
			nicefp returnx = y*other.z - z*other.y;

			return vec3(returnx, returny, returnz);

		}

		vec3 nexp(vec3 vec) {

			return vec3(nicenexp(vec.x),  nicenexp(vec.y), nicenexp(vec.z));
		}

		vec3 reflect(const  vec3 &normal) {

			newdirection = this - 2(this.dot(normal * normal))
			return newdirection;
		}

		void translate(vec3 &by) {

			x += by.x;
			y += by.y;
			z += by.z;
		}

		void rotate(mat3 &rotation) {

			xbuff = this.dot(rotation[1]);
			ybuff = this.dot(rotation[2]);
			zbuff = this.dot(rotation[3]);

			x = xbuff;
			y = ybuff;
			z = zbuff;
		}
		
	};

	struct mat3 {

		vec3 cols[3];

		mat3(vec3 col1, vec3 col2, vec3 col3) {


			cols[1] = col1;
			cols[2] = col2;
			cols[3] = col3;
		}

		mat3 transpose() {

			vec3 col1, col2, col3;

			col1.x = cols[1].x;
			col1.y = cols[2].x;
			col1.z = cols[3].x;

			col2.x = cols[1].y;
			col2.y = cols[2].y;
			col2.z = cols[3].y;

			col3.x = cols[1].z;
			col3.y = cols[2].z;
			col3.z = cols[3].z;

			return mat3(col1, col2, col3);

		}

		mat3 leftmult(mat3 &other) { /// all this is slow!!!!! But i'm not animating, so this will be used very rarely

			vec3 buffer1, buffer2, buffer3;
			mat3 trans = other.transpose;

			buffer1.x = trans.cols[1].dot(cols[1]);
			buffer1.y = trans.cols[2].dot(cols[1]);
			buffer1.z = trans.cols[3].dot(cols[1]);

			buffer2.x = trans.cols[1].dot(cols[2]);
			buffer2.y = trans.cols[2].dot(cols[2]);
			buffer2.z = trans.cols[3].dot(cols[2]);

			buffer3.x = trans.cols[1].dot(cols[3]);
			buffer3.y = trans.cols[2].dot(cols[3]);
			buffer3.z = trans.cols[3].dot(cols[3]);

			return mat3(buffer1, buffer2, buffer3);
		}
	};

	mat3 genrotz(nicefp theta) {

		nicefp costheta = cos(theta);
		nicefp sintheta = sin(theta);

		return mat3(vec3(costheta, -sintheta, 0), 
					vec3(sintheta, costheta,  0)
					vec3(0, 	   0,         1));

	}

	mat3 genroty(nicefp theta) {

		nicefp costheta = cos(theta);
		nicefp sintheta = sin(theta);

		return mat3(vec3(costheta,  0,	sintheta ), 
					vec3(0, 	    1,	0  		 ),
					vec3(-sintheta, 0	costheta));

	}

	mat3 genrotx(nicefp theta) {

		nicefp costheta = cos(theta);
		nicefp sintheta = sin(theta);

		return mat3(vec3(1, 0, 		  0        ), 
					vec3(0, costheta, -sintheta),
					vec3(0, sintheta, costheta));

	}
	struct ray {

		vec3 origin;
		vec3 direction; // should always be normalize; but can be passed in unnormalized

		ray(const vec3 &anorigin, const vec3 &unnormalized) {

			origin = anorigin;
			direction = unnormalized.normalize();

		}

		ray reflect(const vec3 &point,const  vec3 &normal) {

			vec3 newdirection = direction.reflect(normal);
			return ray(point + newdirection*nicefp(.0001), newdirection); // perturb the new ray a little
		}

	};

	bool refract(const vec3& point, const vec3& normal, const &nicefp inref_index, const &nicefp outref_index, ray& result) {

		if (outref_index == 0) return false // no refraction because no light transport;

		double index_ratio = inref_index.value / outref_index.value;
		double cosincedent = -ray.direction.dot(normal).value; 
		double sinout2 = (1 - cosincedent*cosincedent) * index_ratio;
		double discriminant = 1 - sinout2;

		if (discriminant <= .00001) return false // total internal reflection

		vec3 refractdir = nicefp(index_ratio)*ray.direction
								 + nicefp(index_ratio*cosincedent - sqrt(discriminant))*normal;

		result.origin = point + refractdir*nicefp(.0001); // perturn the new ray a little
		result.direction = refractdir;

		return true;
	}

	struct interception {

		nicefp distance;
		vec3 position; // vector from center of object to intercept point
		vec3 normal;
		uv uv;

		interception(nicefp distance, vec3 position, vec3 normal, uv uv) {

			this.distance = distance;
			this.postition = position;
			this.normal = normal;
			this.uv = uv;
		}
	
	};

	struct uv {

		nicefp u;
		nicefp v;

		uv(nicefp au, nicefp av) {

			u = au;
			v = av;
		}

		uv(double au, double av) {

			uv(nicefp(au), nicefp(av));
		}

	};

	struct shape { 

		virtual bool calc_interception(const &ray, const nicefp within_d, &intercetion out) = 0;
		virtual void translate(vec &by) = 0; // always translate after rotating
		virtual void rotate(vec3 &by) = 0; // always rotate before translation
		virtual void scale(double factor) = 0;

		virtual uv getuv(vec3 &position) = 0;
		virtual vec3 get_normal(vec3 &position) = 0;
		
	};

	struct triangle: shape {

		vec3 position;
		vec3 side_a;
		vec3 side_b
		uv uv_a, uv_b, uv_c;

		vec3 normal; // not normalized!!!! for optimization
		nicefp area2;
		nicefp area;

		triangle(vec3 aposition, vec3 aside_a, vec3 aside_b, vec3 anormal, uv uv_a, uv uv_b, uv uv_c) {

			position = aposition;
			side_a = aside_a;
			side-b = aside_b;
			this.uv_a = uv_a;
			this.uv_b = uv_b;
			this.uv_c = uv_c;


			normal = anormal;
			area2 = normal.norm2();
			area = sqrt(area2);

		}

		

		virtual void translate(vec &by) {

			position+= by;
		}


		virtual void rotate(mat3 &by) { // don't rotate triangles unless to absolutely must 

			position.rotate(by);
			point_b.rotate(by);
			point_c.rotate(by);

			normal = normal.rotate(by); // update normal!!! (could rotate, but this prevents errors?)

		}

		virtual void scale(double factor) {

			position *= factor;
			side_a *= factor;
			side_b *= factor;
			area2 *= factor * factor;
			area *= factor;

		}


		virtual bool calc_interception(const &ray, const nicefp within_d, &interception out) override {

			// this uses the Moller-Trumbore algorithm
			// note i'm not doing backface culling, since I'll need to intersect with backfaces for refraction
			// algorithm was mostly copied from the original article, with some mods 

			if (ray.direction.dot(normal) == 0) return false;

			nicefp t, u, v;

	

			vec3 edge1 = side_a;
			vec3 edge2 = side_b;
			vec3 origin_to_tri = ray.origin - position;

			nicefp determinant = ray.direction.cross(edge2).dot(edge1);

			if (determinant == 0) return false;

			vec3 p = ray.direction.cross(edge2);

			vec3 q = origin_to_tri.cross(edge1);

			u = p.dot(origin_to_tri);

			if (u < 0 || determinant < u) return false;

			v = ray.direction.dot(q);

			if (v < 0 || determinant < u + v) return false;


			t = q.dot(edge2);

			out.distance = t;
			out.position = position + ((u * edge1) + (v * edge2)) / sqrt(determinant);
			out.normal = normal;
			out.uv = getuv(out.position);
			return true;
		}

		uv getuv(vec3 &intersect) { //using barycentric cooredinate
			
			if (area2 = 0) return uv(uv_a, 0, 0); // cut out early for really small triangles

			vec3 delta_p = intersect - position;

			nicefp APBcross = side_a.cross(delta_p);
			nicefp APCcross = sibe_b.cross(delta_p);

			nicefp beta = APBcross.dot(normal)/area2;
			nicefp gamma = APCcross.dot(normal)/area2;
			nicefp alpha = 1 - beta - gamma;

			nicefp u = uv_a.u * alpha + uv_b.u * beta + uv_c.u * gamma;
			nicefp v = uv_a.v * alpha + uv_b.v * beta + uv_c.v * gamma;

			return uv(u, v);
		}

		vec3 get_normal(vec3 &position) {

			return normal / area; 
		}
	};
	

	

	struct sphere: shape {

		vec3 center;
		vec3 orientation;
		nicefp maxangle
		nicefp radius;


		virtual sphere(vec3 acenter, vec3 orientationvec, nicefp amaxangle, nicefp aradius) { // maxangle in cos nicefps

			center = acenter;
			orientation = orientationvec;
			maxangle = amaxangle;
			radius = aradius;
		}


		virtual vec3 get_normal(vec3 position) override {

			return (position - center) / radius; 

		}

		virtual void translate(vec &by) {

			center.translate(by);
		}


		virtual void rotate(vec3 &by) { 

			center.rotate(by);
			orientationvec.rotate(by);
		}

		virtual void scale(double factor) {

			center *= scale;
			radius *= scale;
		}

		vec3 get_normal(vec3 &position) { //centered on origin of sphere!!!

			return (position - center) / radius; 
		}

		virtual bool interception calc_interception(const &ray, const nicefp within_d, &interception out) {

			// uses the algorithm on page 76 of Shirley and Marschner

			const vec3 delta_o_c = ray.origin - center;
			const nicefp d_dot_d = ray.direction.dot(ray.direction);
			const nicefp d_dot_o_c = ray.direction.dot(delta_o_c);
			
			const nicefp descriminant = (d_dot_o_c * d_dot_o_c) - (d_dot_d *(delta_o_c.dot(delta_o_c) - radius*radius));

			if (descriminant < 0) return false;

			const nicefp sd = sqrt(descriminant);

			const nicefp distance_p = sd - d_dot_o_c; // note direction is always normalized!

			const nicefp distance_m = -1*sd - d_dot_o_c;

	
			if (distance_m < distance_p && distance_m.value > 0 && distance_m < within_d) {
					out.distance = distance_m;
					out.position = delta_o_c + direction * distance_m ; // i.e. orign + direction* distance - center
					if ((out.position - center).dot(orientation) > amaxangle) return true;
				}
			

			if (distance_p.value > 0 && distance_p < within_d)
				if (distance_p.value > 0 && distance_p < within_d) {
					out.distance = distance_m;
					out.position = delta_o_c + direction * distance_p ; // i.e. orign + direction* distance - center
					out.normal = getnormal(out.position);
					out.uv = getuv(out.position);
					if ((out.position - center).dot(orientation) > amaxangle) return true;
				}

			return false;
			
		}
	};

	struct plane: shape {

		vec3 center;
		vec3 normal;
		vec3 u_axis;
		vec3 v_axis; 

		virtual plane(vec3 acenter, vec3 anormal, vec3 au_axis, vec3 av_axis) {

			center = acenter;
			normal = anormal / anormal.norm();; // should be normalized!
			u_axis = au_axis / au_axis.norm();
			v_axis = av_axis / au_axis.norm();
		}

		virtual bool calc_interception(const &ray, const nicefp within_d, &interception out) {

			nicefp denom =ray.direction.dot(normal);
			if (denom == 0) return false; //parralel to plane

			interception.distance = (center - ray.origin).dot(normal) / denom;
			interception.position = ray.origin + ray.direction * interception.distance;
			interception.normal = normal;

			return true;

		}


		virtual void translate(vec &by) { // always translate after rotating

			center.translate(by);

		}
		virtual void rotate(vec3 &by) { // always rotate before translation

			normal.rotate(by);
			u_axis.rotate(by);
			v_axis.rotate(by);
		}

		virtual uv getuv(vec3 &position) {

			vec3 deltap = position - center;

			nicefp u = delta_p.dot(u_axis);
			nicefp v = delta_p.dot(v_axis);

			return uv(u, v);
		}

		virtual vec3 get_normal(vec3 &position) {

			return  normal;

		}
	};
}



	

