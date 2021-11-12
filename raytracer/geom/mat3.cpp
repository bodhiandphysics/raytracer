#include "mat3.h"

namespace geom {

	mat3::mat3(const vec3 &col1,const vec3& col2,const vec3 &col3) {

			cols[0] = col1;
			cols[1] = col2;
			cols[2] = col3;
		
	}

	mat3 mat3::transpose() { // matrix transpose

			vec3 col1, col2, col3;

			col1.x = cols[0].x;
			col1.y = cols[1].x;
			col1.z = cols[2].x;

			col2.x = cols[0].y;
			col2.y = cols[1].y;
			col2.z = cols[2].y;

			col3.x = cols[0].z;
			col3.y = cols[1].z;
			col3.z = cols[2].z;

			return mat3(col1, col2, col3);

		}


	mat3 mat3::leftmult(const mat3& other) {


		vec3 buffer1, buffer2, buffer3;
			mat3 trans = other.transpose();

			buffer1.x = trans.cols[0].dot(cols[0]);
			buffer1.y = trans.cols[1].dot(cols[0]);
			buffer1.z = trans.cols[2].dot(cols[0]);

			buffer2.x = trans.cols[0].dot(cols[1]);
			buffer2.y = trans.cols[1].dot(cols[1]);
			buffer2.z = trans.cols[2].dot(cols[1]);

			buffer3.x = trans.cols[0].dot(cols[2]);
			buffer3.y = trans.cols[1].dot(cols[2]);
			buffer3.z = trans.cols[2].dot(cols[2]);

			return mat3(buffer1, buffer2, buffer3);
	}

	mat3 genrotz(const nicefp& theta) {

		double costheta = cos(theta.value);
		double sintheta = sin(theta.value);

		return mat3(vec3(costheta, sintheta, 0), 
					vec3(sintheta, costheta,  0),
					vec3(0, 	   0,         1));
	}

	mat3 genroty(const nicefp &theta) {

		double costheta = cos(theta.value);
		double sintheta = sin(theta.value);

		return mat3(vec3(costheta,  0,	sintheta ), 
					vec3(0, 	    1,	0  		 ),
					vec3(-sintheta, 0,	costheta));

	}

	mat3 genrotx(const nicefp &theta) {

		double costheta = cos(theta.value);
		double sintheta = sin(theta.value);

		return mat3(vec3(1, 0, 		  0        ), 
					vec3(0, costheta, -sintheta),
					vec3(0, sintheta, costheta));

	}
}
