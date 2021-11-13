#pragma once
#include "surface.h"
#include <vector>

namespace world {

struct world {

  std::vector<surf::surface> surfaces;
  std::vector<surf::light> lights;
};
} // namespace world