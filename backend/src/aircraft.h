#ifndef AIRCRAFT
#define AIRCRAFT

#include "map.h"
#include <iostream>

// Defined directions for the aircraft to be facing
enum class Direction {
  dir_NORTH = 0,
  dir_SOUTH = 1,
  dir_EAST = 2,
  dir_WEST = 3
};

// Used for debugging
inline std::ostream &operator<<(std::ostream &os, const Direction dir) {
  switch (dir) {
  case Direction::dir_NORTH:
    os << "NORTH";
    break;
  case Direction::dir_SOUTH:
    os << "SOUTH";
    break;
  case Direction::dir_EAST:
    os << "EAST";
    break;
  case Direction::dir_WEST:
    os << "WEST";
    break;
  }
  return os;
}

/*
 * @brief Represents an Aircraft object as well as its direction and location on
 * a GridMap
 */
class Aircraft {
public:
  /*
   * @brief Constructs an Aircraft object, repositions Aircraft to traversable
   * Cell if applicable.
   *
   * @param startRow The 0-indexed row the Aircraft starts at.
   * @param startDir The Direction the Aircraft starts facing.
   * @param startCol The 0-indexed column the Aircraft starts at.
   * @param map The GridMap the Aircraft exists on.
   *
   * @return Nothing.
   */
  Aircraft(int startRow, Direction startDir, int startCol, GridMap map);

  /*
   * @brief Moves the Aircraft forward one Cell in the current direction if
   * possible, else throws exception.
   *
   * @return Nothing.
   */
  void moveForward();
  /*
   * @brief Rotates the Aircraft's current direction to the left.
   *
   * @return Nothing.
   */
  void turnLeft();
  /*
   * @brief Rotates the Aircraft's current direction to the right.
   *
   * @return Nothing.
   */
  void turnRight();
  /*
   * @brief Scans Cells in a 2x3 area in front of the Aircraft.
   *
   * @return The number of Cells scanned.
   */
  int scan();

  /*
   * @breif Getter for the Aircraft's 0-indexed current row.
   *
   * @return The Aircraft's current row.
   */
  int getCurRow() const { return m_curRow; }

  /*
   * @brief Setter for the Aircraft's 0-indexed current row.
   *
   * @return Nothing.
   */
  void setRow(const int row) { m_curRow = row; }

  /*
   * @breif Getter for the Aircraft's 0-indexed current column.
   *
   * @return The Aircraft's current column.
   */
  int getCurCol() const { return m_curCol; }

  /*
   * @brief Setter for the Aircraft's 0-indexed current column.
   *
   * @return Nothing.
   */
  void setCol(const int col) { m_curCol = col; }

  /*
   * @brief Getter for the Aircraft's current direction.
   *
   * @return The Aircraft's current direction.
   */
  Direction getDir() const { return m_dir; }

  /*
   * @brief Getter for the Aircraft's GridMap.
   *
   * @return The Aircraft's GridMap.
   */
  GridMap getMap() const { return m_map; }

private:
  int m_curRow = -1, m_curCol = -1; // 0-indexed row and column values
  Direction m_dir;
  GridMap m_map;
};

#endif
