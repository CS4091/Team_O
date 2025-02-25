#ifndef ROUTER
#define ROUTER

#include "aircraft.h"
#include "map.h"
#include <string>
#include <vector>

/*
 * @brief Represents the set of functions completing the route planning
 * algorithm.
 */
class RoutePlanner {
public:
  RoutePlanner(GridMap map, Aircraft aircraft);

  std::vector<std::string> findRoute();

private:
  GridMap m_map;
  Aircraft m_aircraft;
};

#endif
