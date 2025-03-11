#include "../src/router1.h"
#include <gtest/gtest.h>

TEST(RouterTest, ValidCSVTest) {
  GridMap map(100, "test_csv/bigTestGrid.csv");
  map.mapStats();
  Aircraft aircraft(0, Direction::dir_EAST, 0, map);
  RoutePlanner router(aircraft);

  router.findRoute();
}
