#include "geom/surface"
#include "theworld.h"
namespace camera {
using namespace geom struct camera {

  volatile vec3 position;
  volatile vec3 direction;
  volatile vec3 image_udirection;
  volatile vec3 image_vdirection volatile nicefp focal_length;
  volatile nicefp u_size; // half sizes of image
  volatile nicefp v_size;
  volatile double *image;

  volatile int height; // half height and length
  volatile int width;

  volatile int cutoff;

  camera(vec3 position, vec3 direction, vec3 image_udirection,
         vec3 image_vdirection, nicefp focal_length, nicefp u_size,
         nicefp v_size, int height, int width);
  ~camera();

  void render(const char *filename, world::world &theworld, int cutoff,
              double maxdistance);

private:
  void redertask(int start, int stride, world::world &theworld, int cutoff,
                 double maxdistance);
};

void create_png(const char *filename, double *pixeldata, int height, int width);
} // namespace camera
