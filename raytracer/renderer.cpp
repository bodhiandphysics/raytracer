
#include "renderer.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <unistd.h>
#include "stb_image_write.h"


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
  this->height = height;
  this->width = width;

}

camera::~camera() { 
 }

void rendertask(camera *camera, std::vector<std::vector<vec3>> &theimage, int start, int stride, world::world *theworld,
                        int cutoff, double maxdistance) {

  vec3 position = camera->position;
  vec3 direction = camera->direction;
  vec3 image_udirection = camera->image_udirection;
  vec3 image_vdirection =
      camera->image_vdirection;
    nicefp focal_length =
        camera->focal_length; 
  double u_scale =
              (camera->u_size / camera->width).value; 
  double v_scale = (camera->v_size / camera->height).value;



  int height = camera->height; // half height and width
  int width = camera->width;
  
  for (int i = 0; i < width; i++ ) {

    for (int j = 0; j < height; j++) {

      // this could be optomized
      vec3 pixel_loc = position + (direction * focal_length) +
                       ((image_udirection * -1) * camera->u_size/2) +
                       (image_vdirection * camera->v_size/2) +
                       (image_udirection * i * u_scale) +
                       (image_vdirection * -j * v_scale);
      vec3 raydirection = pixel_loc - position;
      geom::ray lightray(position, raydirection);

      vec3 pixcolor = raytrace::raytrace(lightray, theworld, cutoff, nicefp(maxdistance));

      
      theimage[i][j] = pixcolor;
        
    }
  }
}

void camera::render(const char *filename, world::world *theworld, int cutoff,
                    double maxdistance) { // this is dumb multithreading, but it
                                          // should work on average
  //int numthreads = std::thread::hardware_concurrency();
  //std::vector<std::thread> threads;

  // remember to put in exceptions later
 // for (int threadnum = 0; threadnum < numthreads; threadnum++)
    //threads.emplace_back(rendertask, this, threadnum, numthreads, std::ref(theworld), cutoff,
                        // maxdistance);
  std::vector<std::vector<vec3>> theimage;

  rendertask(this, theimage, 0, 1, theworld, cutoff, maxdistance);
                        // maxdistance
  //for (int threadnum = 0; threadnum < numthreads; threadnum++)
  //  threads[threadnum].join(); 

  create_png(filename, theimage, this->height, this->width);
}

void create_png(const char *filename, std::vector<std::vector<vec3>> &pixeldata, int height,
                int width) {

  char *rgbabuffer = (char *)malloc(4 * height * width * sizeof(char));
  
  for (int i = 0; i < width; i++)
    for (int j = 0; j < height; j++) {

      double redcolorvalue = std::min(
          255.0,
              (
              (pixeldata[i][j].x)
                  .value)); // apply stupid gamma correction, 8 bit color channel
      double greencolorvalue =
          std::min(255.0, (pixeldata[i][j].y).value);
      double bluecolorvalue =
          std::min(255.0, (pixeldata[i][j]).value);
      double alphacolorvalue = 255;

      rgbabuffer[4 * (width * j + i)] = static_cast<int>(redcolorvalue);
      rgbabuffer[4 * (width * j + i) + 1] = static_cast<int>(greencolorvalue);
      rgbabuffer[4 * (width * j + i) + 2] = static_cast<int>(bluecolorvalue);
      rgbabuffer[4 * (width * j + i) + 3] = static_cast<int>(alphacolorvalue);
    }

  


  if (stbi_write_png(filename, width, height, 4, (void*) rgbabuffer, height * 4) != 0)
     std::cout << "There was an error with producing the png!\n";
  free(rgbabuffer);
}
} // namespace camera
