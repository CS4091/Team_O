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
  Aircraft(int startRow, Direction startDir, int startCol, GridMap map)
      : m_curRow(startRow), m_curCol(startCol), m_dir(startDir), m_map(map) {}

  bool moveForward();
  void turnLeft();
  void turnRight();
  void scan();

  // TODO: whatever other functions are needed

private:
  int m_curRow, m_curCol;
  Direction m_dir;
  GridMap m_map;
};

#endif
