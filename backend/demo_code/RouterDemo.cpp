#include "../src/aircraft.h"
#include "../src/map.h"
#include "../src/router1.h"

/*
 Running this code will give a demonstration of the router.
 */
int main() {
  GridMap map(100, "../unit_tests/test_csv/bigTestGrid.csv");
  map.mapStats();

  Aircraft aircraft(0, Direction::dir_EAST, 0, map);

  RoutePlanner router(aircraft);

  router.findRoute();

  return 0;
}
