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
