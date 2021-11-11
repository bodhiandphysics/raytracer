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

	volatile int cutoff;


	void render(int start, int stride, world::world theworld, int cutoff, double maxdistance);

};
	
void create_png(const char* filename, double* pixeldata, int height, int width);



