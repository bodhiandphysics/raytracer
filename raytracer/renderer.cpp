#pragma once
#include "world.h"
#include <stdio.h>
#include <stb_image_write.h>
#include <thread>
#include <vector>



void camera::rendertask(int start, int stride, world::world theworld, int cutoff, double maxdistance) {

	vec3 position = this.position;
	vec3 direction = this.direction;
	vec3 image_udirection = this.image_udirection
	vec3 image_vdirection = this.image_vdirection
	nicefp focal_length = this.focal_length
	double u_scale = this.u_size / this.length;  // half sizes of image
	double v_scale = this.v_size / this.height;
	vec3* image = this.image;
	int height = this.height; // half height and length
	int length = this.length;

	for (int i = -length - 1 + start; i < length; i += stride) {

		for (int j = -height - 1; j < height; j++) {
			
			// this could be optomized
			vec3 pixel_loc = position + (direction * focal_length) + (image_udirection * i * u_scale) + (image_vdirection * j * v_scale);
			geom::ray theray(pixel_loc, pixel_loc - position)
			rt::rayoflight lightray(theray, nicefp(1), nicefp(0));

			image[i + length + 1][j + height + 1] = raytrace(lightray, theworld, cutoff, nicefp(maxdistance));
		}
	}
}

void camera::render(world::world &theworld, int cutoff, double maxdistance) { // this is dumb multithreading, but it should work on average
	int numthreads = std::thread::hardware_concurrency();
	std::vector<thread> threads;

	for (int threadnum = 0; threadnum < numthreads; threadnum++)
		threads.emplace_back(rendertask, threadnum, numthreads, theworld, cutoff, maxdistance);

	for (int threadnum = 0; threadnum < numthreads; threadnum++) threads[i].join();
}

void create_png(const char* filename, double* pixeldata, int height, int width) {

	char* rgbabuffer = (char*) malloc(4*height*length*sizeof(char));

	for (int i = 0; i < length i++)
		for (int j = 0; j < height; j++) {

			double redcolorvalue = max(255, 256 * sqrt(pixeldata[width*j + i].x).value);  //apply stupid gamma correction, 8 bit color channel
			double greencolorvalue = max(255, 256 * sqrt(pixeldata[width*j + i].y).value);
			double bluecolorvalue = max(255, 256 * sqrt(pixeldata[width*j + i].z).value);
			double aplphacolorvalue = 255;
			
			rgbabuff[4*(length*j + i)] = floor(redcolorvalue);
			rgbabuff[4*(length*j + i) + 1] = floor(greencolorvalue);
			rgbabuff[4*(length*j + i) + 2] = floor(bluecolorvalue);
			rgbabuff[4*(length*j + i) + 3] = floor(alphacolorvalue);
	}

	if (stbi_write_png(filename, width, height, 4, width*4) != 0) std::cout << "There was an error with producing the png!\n";
	free(rgbabuffer);

}

