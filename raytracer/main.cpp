#include "scene.h"
#include <iostream>

int main(int argc, char const *argv[]) {
  if (argc != 1)
    std::cout << "You need to put in a filename as a render target\n";

  char *filename = argv[0] world::world *theworld = create_scene();

  theworld->thecamera->render(filename, theworld, scene::cutoff,
                              scene::maxdistance);
  delete theworld;

  return 0;
}