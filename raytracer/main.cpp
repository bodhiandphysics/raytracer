#include "scene.h"
#include <iostream>

int main(int argc, char const *argv[]) {
  if (argc < 2)
    std::cout << "You need to put in a filename as a render target\n";

  const char *filename = argv[1]; 

  world::world *theworld = scene::create_scene();

  theworld->thecamera->render(filename, theworld, scene::cutoff,
                              scene::maxdistance.value);

  delete theworld;

  return 0;
}