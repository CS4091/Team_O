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
  /*
   * @brief Constructs an Aircraft object.
   *
   * @param startRow The 0-indexed row the Aircraft starts at.
   * @param startDir The Direction the Aircraft starts facing.
   * @param startCol The 0-indexed column the Aircraft starts at.
   * @param map The GridMap the Aircraft exists on.
   *
   * @return Nothing.
   */
  Aircraft(int startRow, Direction startDir, int startCol, GridMap map)
      : m_curRow(startRow), m_curCol(startCol), m_dir(startDir), m_map(map) {}

  /*
   * @brief Moves the aircraft forward one Cell in the current direction if
   * possible, else throws exception.
   *
   * @return Nothing.
   */
  void moveForward();
  /*
   * @brief Rotates the aircraft's current direction to the left.
   *
   * @return Nothing.
   */
  void turnLeft();
  /*
   * @brief Rotates the aircraft's current direction to the right.
   *
   * @return Nothing.
   */
  void turnRight();
  /*
   * @brief Scans Cells in a 2x3 area in front of the aircraft.
   *
   * @return The number of Cells scanned. 
   */
  int scan();

  // TODO: whatever other functions are needed

  /*
   * @breif Getter for the aircraft's current row.
   *
   * @return The aircraft's current row.
   */
  int getCurRow() const { return m_curRow; }

  /*
   * @breif Getter for the aircraft's 0-indexed current column.
   *
   * @return The aircraft's current column.
   */
  int getCurCol() const { return m_curCol; }

  /*
   * @brief Getter for the aircraft's 0-indexed current direction.
   *
   * @return The aircraft's current direction.
   */
  Direction getDir() const { return m_dir; }

  /*
   * @brief Getter for the aircraft's GridMap.
   *
   * @return The aircraft's GridMap.
   */
  GridMap getMap() const { return m_map; }

private:
  int m_curRow = -1, m_curCol = -1; // 0-indexed row and column values
  Direction m_dir;
  GridMap m_map;
};

#endif
