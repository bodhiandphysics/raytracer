
#include "renderer.h"


namespace camera {

camera::camera(vec3 position, vec3 direction, vec3 image_udirection,
               vec3 image_vdirection, nicefp focal_length, nicefp u_size,
               nicefp v_size, int height, int width) {

  this->position = position;
  this->direction = direction;
  this->image_udirection = image_udirection;
  this->image_vdirection = image_vdirection;
  this->focal_length = focal_length;
  this->u_size = u_size;
  this->v_size = v_size;

  this->image = (vec3 *)malloc(height * width * sizeof(double));
}

camera::~camera() { free(this->image); }

void rendertask(camera *camera, int start, int stride, world::world &theworld,
                        int cutoff, double maxdistance) {

  vec3 position = camera->position;
  vec3 direction = camera->direction;
  vec3 image_udirection = camera->image_udirection;
  vec3 image_vdirection =
      camera->image_vdirection;
    nicefp focal_length =
        camera->focal_length; 
  double u_scale =
              (camera->u_size / camera->width).value; // half sizes of image
  double v_scale = (camera->v_size / camera->height).value;

  vec3 *image = camera->image;
  int height = camera->height; // half height and width
  int width = camera->width;

  for (int i = -width - 1 + start; i < width; i += stride) {

    for (int j = 0; j < height; j++) {

      // this could be optomized
      vec3 pixel_loc = position + (direction * focal_length) +
                       (image_udirection * i * u_scale) +
                       (image_vdirection * j * v_scale);
      vec3 startingpoint = pixel_loc - position;
      geom::ray lightray(pixel_loc, startingpoint);


      image[(i + width + 1) + width *j] =
          raytrace::raytrace(lightray, theworld, cutoff, nicefp(maxdistance));
    }
  }
}

void camera::render(const char *filename, world::world &theworld, int cutoff,
                    double maxdistance) { // this is dumb multithreading, but it
                                          // should work on average
  int numthreads = std::thread::hardware_concurrency();
  std::vector<std::thread> threads;

  // remember to put in exceptions later
  for (int threadnum = 0; threadnum < numthreads; threadnum++)
    threads.emplace_back(rendertask, this, threadnum, numthreads, std::ref(theworld), cutoff,
                         maxdistance);

  for (int threadnum = 0; threadnum < numthreads; threadnum++)
    threads[threadnum].join();  

  create_png(filename, this->image, this->height, this->width);
}

void create_png(const char *filename, vec3 *pixeldata, int height,
                int width) {

  char *rgbabuffer = (char *)malloc(4 * height * width * sizeof(char));

  for (int i = 0; i < width; i++)
    for (int j = 0; j < height; j++) {

      double redcolorvalue = std::max(
          255.0,
              (256 *
              nicesqrt(pixeldata[width * j + i].x)
                  .value)); // apply stupid gamma correction, 8 bit color channel
      double greencolorvalue =
          std::max(255.0, 256 * nicesqrt(pixeldata[width * j + i].y).value);
      double bluecolorvalue =
          std::max(255.0, 256 * nicesqrt(pixeldata[width * j + i].z).value);
      double alphacolorvalue = 255;

      rgbabuffer[4 * (width * j + i)] = static_cast<int>(redcolorvalue);
      rgbabuffer[4 * (width * j + i) + 1] = static_cast<int>(greencolorvalue);
      rgbabuffer[4 * (width * j + i) + 2] = static_cast<int>(bluecolorvalue);
      rgbabuffer[4 * (width * j + i) + 3] = static_cast<int>(alphacolorvalue);
    }

  if (stbi_write_png(filename, width, height, 4, (void*) rgbabuffer, width * 4) != 0)
    std::cout << "There was an error with producing the png!\n";
  free(rgbabuffer);
}
} // namespace camera
