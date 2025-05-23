#include "../src/aircraft.h"
#include <gtest/gtest.h>

TEST(AircraftTest, ValidMoveTest) {
  GridMap gridMap(4, "test_csv/manuverableCSV.csv");
  Aircraft aircraft(0, Direction::dir_EAST, 0, gridMap);

  EXPECT_EQ(aircraft.getCurRow(), 0);
  EXPECT_EQ(aircraft.getCurCol(), 0);
  EXPECT_EQ(aircraft.getDir(), Direction::dir_EAST);

  aircraft.moveForward();
  EXPECT_EQ(aircraft.getCurRow(), 0);
  EXPECT_EQ(aircraft.getCurCol(), 1);

  aircraft.moveForward();
  EXPECT_EQ(aircraft.getCurRow(), 0);
  EXPECT_EQ(aircraft.getCurCol(), 2);

  aircraft.moveForward();
  EXPECT_EQ(aircraft.getCurRow(), 0);
  EXPECT_EQ(aircraft.getCurCol(), 3);

  aircraft.turnRight();
  EXPECT_EQ(aircraft.getDir(), Direction::dir_SOUTH);

  aircraft.moveForward();
  EXPECT_EQ(aircraft.getCurRow(), 1);
  EXPECT_EQ(aircraft.getCurCol(), 3);
}

TEST(AircraftTest, UntraversableMoveTest) {
  GridMap gridMap(4, "test_csv/manuverableCSV.csv");
  Aircraft aircraft(0, Direction::dir_EAST, 0, gridMap);

  EXPECT_EQ(aircraft.getCurRow(), 0);
  EXPECT_EQ(aircraft.getCurCol(), 0);
  EXPECT_EQ(aircraft.getDir(), Direction::dir_EAST);

  aircraft.moveForward();
  EXPECT_EQ(aircraft.getCurRow(), 0);
  EXPECT_EQ(aircraft.getCurCol(), 1);

  aircraft.turnRight();
  EXPECT_EQ(aircraft.getDir(), Direction::dir_SOUTH);

  aircraft.moveForward();
  EXPECT_EQ(aircraft.getCurRow(), 1);
  EXPECT_EQ(aircraft.getCurCol(), 1);

  aircraft.moveForward();
  EXPECT_EQ(aircraft.getCurRow(), 2);
  EXPECT_EQ(aircraft.getCurCol(), 1);

  aircraft.turnLeft();
  EXPECT_EQ(aircraft.getDir(), Direction::dir_EAST);

  // trying to move to a nontraversable cell
  EXPECT_THROW(aircraft.moveForward(), std::exception);
}

TEST(AircraftTest, OutofBoundsMoveTest) {
  GridMap gridMap(4, "test_csv/manuverableCSV.csv");
  Aircraft aircraft(0, Direction::dir_EAST, 0, gridMap);

  EXPECT_EQ(aircraft.getCurRow(), 0);
  EXPECT_EQ(aircraft.getCurCol(), 0);
  EXPECT_EQ(aircraft.getDir(), Direction::dir_EAST);

  aircraft.moveForward();
  EXPECT_EQ(aircraft.getCurRow(), 0);
  EXPECT_EQ(aircraft.getCurCol(), 1);

  aircraft.moveForward();
  EXPECT_EQ(aircraft.getCurRow(), 0);
  EXPECT_EQ(aircraft.getCurCol(), 2);

  aircraft.moveForward();
  EXPECT_EQ(aircraft.getCurRow(), 0);
  EXPECT_EQ(aircraft.getCurCol(), 3);

  EXPECT_THROW(aircraft.moveForward(), std::exception);
}

TEST(AircraftTest, ScanningTest) {
  GridMap gridMap(6, "test_csv/scanTestCSV.csv");
  Aircraft aircraft(0, Direction::dir_EAST, 0, gridMap);
  int counter = 0;

  counter = aircraft.scan();
  EXPECT_EQ(counter, 4);
  EXPECT_TRUE(aircraft.getMap().isScanned(0, 1));
  EXPECT_TRUE(aircraft.getMap().isScanned(0, 2));
  EXPECT_TRUE(aircraft.getMap().isScanned(1, 1));
  EXPECT_TRUE(aircraft.getMap().isScanned(1, 2));
  EXPECT_FALSE(aircraft.getMap().isScanned(0, 3));
  EXPECT_FALSE(aircraft.getMap().isScanned(1, 3));
  EXPECT_FALSE(aircraft.getMap().isScanned(1, 0));

  aircraft.moveForward();
  aircraft.moveForward();
  aircraft.turnRight();
  aircraft.moveForward();
  EXPECT_EQ(aircraft.getCurCol(), 2);
  EXPECT_EQ(aircraft.getCurRow(), 1);
  EXPECT_EQ(aircraft.getDir(), Direction::dir_SOUTH);
  counter = aircraft.scan();

  EXPECT_EQ(counter, 6);
  EXPECT_TRUE(aircraft.getMap().isScanned(2, 1));
  EXPECT_TRUE(aircraft.getMap().isScanned(2, 2));
  EXPECT_TRUE(aircraft.getMap().isScanned(2, 3));
  EXPECT_TRUE(aircraft.getMap().isScanned(2, 1));
  EXPECT_TRUE(aircraft.getMap().isScanned(3, 2));
  EXPECT_TRUE(aircraft.getMap().isScanned(3, 3));
  EXPECT_FALSE(aircraft.getMap().isScanned(1, 0));
  EXPECT_FALSE(aircraft.getMap().isScanned(2, 0));
  EXPECT_FALSE(aircraft.getMap().isScanned(1, 3));
  EXPECT_FALSE(aircraft.getMap().isScanned(4, 1));
  EXPECT_FALSE(aircraft.getMap().isScanned(4, 2));
  EXPECT_FALSE(aircraft.getMap().isScanned(4, 3));

  aircraft.turnLeft();
  aircraft.moveForward();
  aircraft.turnLeft();
  EXPECT_EQ(aircraft.getCurCol(), 3);
  EXPECT_EQ(aircraft.getCurRow(), 1);
  EXPECT_EQ(aircraft.getDir(), Direction::dir_NORTH);
  counter = aircraft.scan();

  EXPECT_EQ(counter, 2);
  EXPECT_TRUE(aircraft.getMap().isScanned(0, 2));
  EXPECT_TRUE(aircraft.getMap().isScanned(0, 3));
  EXPECT_TRUE(aircraft.getMap().isScanned(0, 4));
  EXPECT_FALSE(aircraft.getMap().isScanned(0, 5));
  EXPECT_FALSE(aircraft.getMap().isScanned(1, 4));
  EXPECT_FALSE(aircraft.getMap().isScanned(1, 5));

  aircraft.turnLeft();
  aircraft.turnLeft();
  aircraft.moveForward();
  aircraft.moveForward();
  aircraft.moveForward();
  aircraft.turnRight();
  EXPECT_EQ(aircraft.getCurCol(), 3);
  EXPECT_EQ(aircraft.getCurRow(), 4);
  EXPECT_EQ(aircraft.getDir(), Direction::dir_WEST);
  counter = aircraft.scan();

  EXPECT_EQ(counter, 4);
  EXPECT_TRUE(aircraft.getMap().isScanned(4, 2));
  EXPECT_TRUE(aircraft.getMap().isScanned(5, 2));
  EXPECT_TRUE(aircraft.getMap().isScanned(5, 1));
  EXPECT_TRUE(aircraft.getMap().isScanned(5, 2));
  EXPECT_FALSE(aircraft.getMap().isScanned(4, 0));
  EXPECT_FALSE(aircraft.getMap().isScanned(5, 0));
  EXPECT_FALSE(aircraft.getMap().isScanned(5, 3));
}

TEST(AircraftTest, TraversabilityTest) {
  GridMap gridMap(25, "test_csv/smallTestGrid.csv");
  Aircraft aircraft(4, Direction::dir_EAST, 8, gridMap);
  EXPECT_TRUE(
      gridMap.isTraversable(aircraft.getCurRow(), aircraft.getCurCol()));

  Aircraft aircraft2(9, Direction::dir_EAST, 2, gridMap);
  EXPECT_EQ(aircraft2.getCurRow(), 9);
  EXPECT_EQ(aircraft2.getCurCol(), 2);
  EXPECT_TRUE(
      gridMap.isTraversable(aircraft2.getCurRow(), aircraft2.getCurCol()));
}
