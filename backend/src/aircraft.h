#ifndef AIRCRAFT
#define AIRCRAFT

#include "map.h"

// Defined directions for the aircraft to be facing
enum class Direction { dir_NORTH, dir_SOUTH, dir_EAST, dir_WEST };

/*
 * @brief Represents an Aircraft object as well as its direction and location on
 * a GridMap
 */
class Aircraft {
public:
  Aircraft(int startXCoord, int startYCoord, Direction startDir, GridMap &map)
      : xCoord(startXCoord), yCoord(startYCoord), dir(startDir), map(map) {}

  bool moveForward();
  void turnLeft();
  void turnRight();
  void scan();

  // TODO: whatever other functions are needed

private:
  int xCoord, yCoord;
  Direction dir;
  GridMap &map;
};

#endif
