#include "../src/router1.h"
#include <gtest/gtest.h>

TEST(RouterTest, tooLowSearchPercentTest) {
  GridMap map(25, "test_csv/smallTestGrid.csv");
  map.mapStats();
  Aircraft aircraft(9, Direction::dir_SOUTH, 4, map);
  RoutePlanner router(aircraft, -1, 4000); // Should default up to 1%
  ASSERT_FLOAT_EQ(router.getSearchPercentage(), 0.01);
  router.findRoute();
}

TEST(RouterTest, tooHighSearchPercentTest) {
  GridMap map(25, "test_csv/smallTestGrid.csv");
  map.mapStats();
  Aircraft aircraft(9, Direction::dir_SOUTH, 4, map);
  RoutePlanner router(aircraft, 2, 4000); // Should default down to 100%
  ASSERT_FLOAT_EQ(router.getSearchPercentage(), 1.0);
  router.findRoute();
}

TEST(RouterTest, negativeMoveLimit) {
  GridMap map(25, "test_csv/smallTestGrid.csv");
  map.mapStats();
  Aircraft aircraft(9, Direction::dir_SOUTH, 4, map);
  RoutePlanner router(aircraft, 1, -10); // Should default up to 1 move
  ASSERT_FLOAT_EQ(router.getSearchPercentage(), 1.0);
  router.findRoute();
}

TEST(RouterTest, MediumCSVTest) {
  GridMap map(50, "test_csv/mediumTestGrid.csv");
  map.mapStats();
  Aircraft aircraft(0, Direction::dir_EAST, 0, map);
  RoutePlanner router(aircraft, 0.5, 4000);

  router.findRoute();
}

TEST(RouterTest, BigCSVTest) {
  GridMap map(100, "test_csv/bigTestGrid.csv");
  map.mapStats();
  Aircraft aircraft(0, Direction::dir_EAST, 0, map);
  RoutePlanner router(aircraft, 0.01, 4000);

  router.findRoute();
}
