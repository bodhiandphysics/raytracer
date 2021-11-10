#include <geom.h>

struct camera {


	volatile vec3 position;
	volatile vec3 direction;
	volatile vec3 image_udirection;
	volatile vec3 image_vdirection
	volatile nicefp focal_length;
	volatile nicefp u_size; // half sizes of image
	volatile nicefp v_size;
	volatile double* image;

	volatile int height; // half height and length
	volatile int length;

	volatile int cutoff


	render(int stride, world::world theworld, int cutoff, double maxdistance) {

		vec3 position = this.position;
		vec3 direction = this.direction;
		vec3 image_udirection = this.image_udirection
		vec3 image_vdirection = this.image_vdirection
		nicefp focal_length = this.focal_length
		double u_scale = this.u_size / this.length;  // half sizes of image
		double v_scale = this.v_size / this.height;
		double* image = this.image;
		int height = this.height; // half height and length
		int length = this.length;

		vec3
		for (int i = -length - 1; i < length; i + stride) {

			for (int j = -height - 1; j < height; j + stride) {
				
				// this could be optomized
				vec3 pixel_loc = position + (direction * focal_length) + (image_udirection * i * u_scale) + (image_vdirection * j * v_scale);
				geom::ray theray(pixel_loc, pixel_loc - position)
				rt::rayoflight lightray(theray, nicefp(1), nicefp(0));

				image[i + length + 1][j + height + 1] = raytrace(lightray, theworld, cutoff, nicefp(maxdistance));
			}
		}
	}
};

