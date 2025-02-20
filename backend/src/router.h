#ifndef ROUTER
#define ROUTER

#include <string>
#include <vector>
#include "aircraft.h"
#include "map.h"

// Route planner class
class RoutePlanner {
 public:
  RoutePlanner(GridMap& map, Aircraft& aircraft);
  std::vector<std::string> findRoute();

 private:
  GridMap& map;
  Aircraft& aircraft;
};

#endif
