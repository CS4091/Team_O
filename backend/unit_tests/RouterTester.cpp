#include "../src/router1.h"
#include <gtest/gtest.h>

TEST(RouterTest, BigCSVTest) {
  GridMap map(100, "test_csv/bigTestGrid.csv");
  map.mapStats();
  Aircraft aircraft(0, Direction::dir_EAST, 0, map);
  RoutePlanner router(aircraft);

  router.findRoute();
}

TEST(RouterTest, MediumCSVTest) {
  GridMap map(50, "test_csv/mediumTestGrid.csv");
  map.mapStats();
  Aircraft aircraft(0, Direction::dir_EAST, 0, map);
  RoutePlanner router(aircraft);

  router.findRoute();
}

TEST(RouterTest, smallCSVTest) {
  GridMap map(25, "test_csv/smallTestGrid.csv");
  map.mapStats();
  Aircraft aircraft(9, Direction::dir_SOUTH, 4, map);
  RoutePlanner router(aircraft);

  router.findRoute();
}
