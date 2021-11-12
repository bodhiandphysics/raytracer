
#pragma once
#include "nicefp.h"


namespace geom {

	struct mat3;
	struct vec3 {

		nicefp x;
		nicefp y;
		nicefp z;

		vec3(nicefp x, nicefp y, nicefp z);
		vec3(double x, double y, double z);
		vec3() = default;
		vec3 mult(const vec3& other);


		nicefp norm();
		nicefp norm2();
		vec3 normalize();
		vec3 operator+ (const vec3& other);
		vec3 operator- (const vec3& other);
		bool operator== (const vec3& other);
		vec3 operator*(double mult);
		vec3 operator*(const nicefp& other);
		vec3 operator/(double div);
		vec3 operator/(const nicefp& div);

		nicefp dot(const vec3& other);

		vec3 cross(const vec3& other);

		vec3 nexp(const vec3& vec);
		vec3 reflect(vec3 &normal);
		
		void translate(const vec3 &by);

		void rotate(const mat3 &rotation);

		void scale(const vec3& by);
		
	};
}