#include "aircraft.h"
#include "map.h"
#include <cassert>
#include <iostream>

namespace {

/*
 * @brief Helper function for Aircraft's moveForward(), verifying position for
 * forward moves.
 *
 * @param aircraft The aircraft with position being validated.
 * @param map The map the aircraft uses.
 *
 * @return true if moving forward keeps the aircraft on the map, else false.
 */
bool forwardPositionValid(Aircraft &aircraft, GridMap &map) {

  switch (aircraft.getDir()) {

  case Direction::dir_NORTH:
    // The row position must be > 0
    if (aircraft.getCurRow() <= 0) {
      std::cerr << "Error: attempted to move NORTH, but currently at row "
                << aircraft.getCurRow() << std::endl;
      return false;
    }

    return true;

    break;

  case Direction::dir_SOUTH:
    // The row position must be < GridMap's m_rowCount - 1
    if (aircraft.getCurRow() >= map.getRowCount() - 1) {
      std::cerr << "Error: attempted to move SOUTH, but currently at row "
                << aircraft.getCurRow() << " and map only contains "
                << map.getRowCount() - 1 << " rows.\n";
      return false;
    }

    return true;

    break;

  case Direction::dir_EAST:
    // The column position must be < GridMap's m_colCount - 1
    if (aircraft.getCurCol() >= map.getColCount() - 1) {
      std::cerr << "Error: attempted to move EAST, but currently at column "
                << aircraft.getCurCol() << " and map only contains "
                << map.getColCount() - 1 << " columns.\n";
      return false;
    }

    return true;

    break;

  case Direction::dir_WEST:
    // The column position must be > 0
    if (aircraft.getCurCol() <= 0) {
      std::cerr << "Error: attempted to move WEST, but currently at column "
                << aircraft.getCurCol() << std::endl;
      return false;
    }

    return true;

    break;
  }

  std::cerr << "Error: Something bad happened. Is the aircraft direction not a "
               "valid direction?\n";
  return false;
}

/*
 * @brief Helper function for Aircraft's moveForward(), verifying traversability
 * for forward moves.
 *
 * @param aircraft The aircraft with position being validated.
 * @param map The map the aircraft uses.
 *
 * @return true if moving forward lands on traversable Cell, else false.
 */
bool forwardTraverabilityValid(Aircraft &aircraft, GridMap &map) {
  switch (aircraft.getDir()) {
  case Direction::dir_NORTH:
    // The cell one row above the aircarft must be traversable
    if (map.isTraversable(aircraft.getCurRow() - 1, aircraft.getCurCol())) {
      return true;
    }

    std::cerr << "Error: moving NORTH lands on untraversable Cell\n";
    return false;

    break;

  case Direction::dir_SOUTH:
    // The Cell one row below the aircraft must be traversable
    if (map.isTraversable(aircraft.getCurRow() + 1, aircraft.getCurCol())) {
      return true;
    }

    std::cerr << "Error: moving SOUTH lands on untraverable Cell\n";
    return false;

    break;

  case Direction::dir_EAST:
    // The Cell one column right of the aircraft must be traversable
    if (map.isTraversable(aircraft.getCurRow(), aircraft.getCurCol() + 1)) {
      return true;
    }

    std::cerr << "Error: moving EAST lands on untraversable Cell\n";
    return false;

    break;

  case Direction::dir_WEST:
    // The Cell one column left of the aircraft must be traversable
    if (map.isTraversable(aircraft.getCurRow(), aircraft.getCurCol() - 1)) {
      return true;
    }

    std::cerr << "Error: moving WEST lands on untraverable Cell\n";
    return false;

    break;
  }

  std::cerr << "Error: Something bad happened. Is the aircraft direction not a "
               "valid direction?\n";
  return false;
}
} // namespace

void Aircraft::moveForward() {
  // For each direcion, determine if moving forward is a valid move and the
  // space is traversable, if not throw an exception, if it is, move forward
  switch (m_dir) {

  case Direction::dir_NORTH:
    if (forwardPositionValid(*this, m_map) &&
        forwardTraverabilityValid(*this, m_map)) {
      // Move NORTH by decreasing row position
      m_curRow--;
    } else {
      // Tried to move to invalid spot, throw exception
      throw std::exception();
      return;
    }
    break;

  case Direction::dir_SOUTH:
    if (forwardPositionValid(*this, m_map) &&
        forwardTraverabilityValid(*this, m_map)) {
      // Move SOUTH by increasing row position
      m_curRow++;
    } else {
      // Tried to move to invalid spot, throw exception
      throw std::exception();
      return;
    }
    break;

  case Direction::dir_EAST:
    if (forwardPositionValid(*this, m_map) &&
        forwardTraverabilityValid(*this, m_map)) {
      // Move EAST by increasing column position
      m_curCol++;
    } else {
      // Tried to move to invalid spot, throw exception
      throw std::exception();
      return;
    }
    break;

  case Direction::dir_WEST:
    if (forwardPositionValid(*this, m_map) &&
        forwardTraverabilityValid(*this, m_map)) {
      // Move WEST by increasing column position
      m_curCol--;
    } else {
      // tried to move to invalid spot, throw exception
      throw std::exception();
      return;
    }
    break;
  }

  assert(m_curRow > -1 && m_curRow < m_map.getRowCount());
  assert(m_curCol > -1 && m_curCol < m_map.getColCount());
}

void Aircraft::turnLeft() {
  // North --> West
  // South --> East
  // East --> North
  // West --> South

  switch (m_dir) {

  case Direction::dir_NORTH:
    m_dir = Direction::dir_WEST;
    return;
    break;

  case Direction::dir_SOUTH:
    m_dir = Direction::dir_EAST;
    return;
    break;

  case Direction::dir_EAST:
    m_dir = Direction::dir_NORTH;
    return;
    break;

  case Direction::dir_WEST:
    m_dir = Direction::dir_SOUTH;
    return;
    break;
  }
}

void Aircraft::turnRight() {
  // North --> East
  // South --> West
  // East --> South
  // West --> North

  switch (m_dir) {

  case Direction::dir_NORTH:
    m_dir = Direction::dir_EAST;
    return;
    break;

  case Direction::dir_SOUTH:
    m_dir = Direction::dir_WEST;
    return;
    break;

  case Direction::dir_EAST:
    m_dir = Direction::dir_SOUTH;
    return;
    break;

  case Direction::dir_WEST:
    m_dir = Direction::dir_NORTH;
    return;
    break;
  }
}
