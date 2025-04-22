#include "router1.h"
#include "aircraft.h"
#include "map.h"
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <ostream>
#include <queue>
#include <set>
#include <utility>
#include <vector>

constexpr float PERCENT1 = 0.01;
constexpr float PERCENT100 = 1.0;

namespace {
using std::pair;

/*
 * @brief Helper function for RoutePlanner's findRoute(), verifying position for
 * forward moves.
 *
 * @param aircraft The aircraft with position being validated.
 * @param map The map the aircraft uses.
 *
 * @return true if moving forward keeps the aircraft on the map, else false.
 */
bool forwardPositionValid(const Aircraft &aircraft, const GridMap &map) {

  switch (aircraft.getDir()) {

  case Direction::dir_NORTH:
    // The row position must be > 0
    if (aircraft.getCurRow() <= 0) {
      return false;
    }
    if (!map.isTraversable(aircraft.getCurRow() - 1, aircraft.getCurCol())) {
      return false;
    }

    return true;

  case Direction::dir_SOUTH:
    // The row position must be < GridMap's m_rowCount - 1
    if (aircraft.getCurRow() >= map.getRowCount() - 1) {
      return false;
    }
    if (!map.isTraversable(aircraft.getCurRow() + 1, aircraft.getCurCol())) {
      return false;
    }

    return true;

  case Direction::dir_EAST:
    // The column position must be < GridMap's m_colCount - 1
    if (aircraft.getCurCol() >= map.getColCount() - 1) {
      return false;
    }
    if (!map.isTraversable(aircraft.getCurRow(), aircraft.getCurCol() + 1)) {
      return false;
    }

    return true;

  case Direction::dir_WEST:
    // The column position must be > 0
    if (aircraft.getCurCol() <= 0) {
      return false;
    }
    if (!map.isTraversable(aircraft.getCurRow(), aircraft.getCurCol()) - 1) {
      return false;
    }

    return true;
  }

  std::cerr << "Error: Something bad happened. Is the aircraft direction not a "
               "valid direction?\n";
  return false;
}

/*
 * @brief Helper function for RoutePlanner's findRoute() function, determines if
 * scanning at the current position will scan new Cells.
 *
 * @note This is somewhat of a hacky implementation, leveraging the scan
 * function. If >=1 new Cells are scanned, represented as a boolean it will be
 * true in the comparision where it is called.
 *
 * @brief aircraft The current aircraft, *passed by value, not reference*.
 *
 * @return True if new Cells would be scanned, else false.
 */
bool willScanNewCells(Aircraft aircraft) {
  aircraft.moveForward();
  return aircraft.scan() != 0;
}

/*
 * @brief Computes the manhatan distance between two points.
 *
 * @read https://en.wikipedia.org/wiki/Taxicab_geometry
 *
 * @param curXcoord The current column.
 * @param curYCoord The current row.
 * @param newXCoord The targeted column.
 * @param newYCoord The targeted row.
 *
 * @retrun The manhattan distance between the two coordinates pairs.
 */
int manhattanDistance(const int &curXCoord, const int &curYCoord,
                      const int &newXCoord, const int &newYCoord) {
  return std::abs(curXCoord - newXCoord) + std::abs(curYCoord - newYCoord);
}

// State struct representing a cell position and orientation
struct State {
  pair<int, int> position; // (row, col)
  Direction direction;

  // Define operator< so that State can be stored in a set
  bool operator<(const State &other) const {
    if (position.first != other.position.first) {
      return position.first < other.position.first;
    }
    if (position.second != other.position.second) {
      return position.second < other.position.second;
    }
    return static_cast<int>(direction) < static_cast<int>(other.direction);
  }
};

// A* node that stores a state and the sequence of moves that led to it
struct AStarNode {
  State state;
  std::vector<Moves> moves; // sequence of moves to reach this state
  int cost = 0;             // g: cost so far (number of moves)
  int f = 0; // f = g + h, where h is the heuristic (manhattan distance)
};

// Custom comparator for std::priority_queue (min-heap based on f)
struct AStarNodeComparator {
  bool operator()(const AStarNode &a, const AStarNode &b) const {
    return a.f > b.f; // lower f has higher priority
  }
};

/*
 * @brief Represents the aircraft position after moving forward one spot.
 *
 * @param State The State of an Aircraft (row, column, & position).
 *
 * @return The row and column after moving forward.
 */
std::pair<int, int> positionAfterMovingForward(const State &state) {
  const int row = state.position.first;
  const int col = state.position.second;
  switch (state.direction) {
  case Direction::dir_NORTH:
    return {row - 1, col};
  case Direction::dir_SOUTH:
    return {row + 1, col};
  case Direction::dir_EAST:
    return {row, col + 1};
  case Direction::dir_WEST:
    return {row, col - 1};
  }
  return {row, col}; // Should never happen
}

/*
 * @brief The direction after turning left.
 *
 * @param The current aircraft direction.
 *
 * @return The new direction after left turn.
 */
Direction directionAfterTurnLeft(Direction direction) {
  switch (direction) {
  case Direction::dir_NORTH:
    return Direction::dir_WEST;
  case Direction::dir_SOUTH:
    return Direction::dir_EAST;
  case Direction::dir_EAST:
    return Direction::dir_NORTH;
  case Direction::dir_WEST:
    return Direction::dir_SOUTH;
  }
  return direction; // Should never happen
}

/*
 * @brief The direction after turning right.
 *
 * @param The current aircraft direction.
 *
 * @return The new direction after right turn.
 */
Direction directionAfterTurnRight(Direction direction) {
  switch (direction) {
  case Direction::dir_NORTH:
    return Direction::dir_EAST;
  case Direction::dir_SOUTH:
    return Direction::dir_WEST;
  case Direction::dir_EAST:
    return Direction::dir_SOUTH;
  case Direction::dir_WEST:
    return Direction::dir_NORTH;
  }
  return direction; // Should never happen
}

/*
 * @brief Determines if moving forward after turning results in a valid
 * position.
 *
 * @param aircraft The aircraft being turned.
 * @param turn The direction of the turn.
 * @param map The map the aircraft exists on.
 *
 * @return True if moving forward after a turn lands on a valid space, else
 * false.
 */
bool isTurnValid(const Aircraft &aircraft, const Moves &turn,
                 const GridMap &map) {
  Direction simulatedDir = aircraft.getDir();
  if (turn == Moves::move_TURNLEFT) {
    simulatedDir = directionAfterTurnLeft(aircraft.getDir());
  } else if (turn == Moves::move_TURNRIGHT) {
    simulatedDir = directionAfterTurnRight(aircraft.getDir());
  }
  State simulatedPos;
  simulatedPos.direction = simulatedDir;
  simulatedPos.position.first = aircraft.getCurRow();
  simulatedPos.position.second = aircraft.getCurCol();

  const std::pair<int, int> simCoord = positionAfterMovingForward(simulatedPos);

  if (simCoord.first < 0 || simCoord.first >= map.getRowCount() ||
      simCoord.second < 0 || simCoord.second >= map.getColCount()) {
    return false;
  }
  if (!map.isTraversable(simCoord.first, simCoord.second)) {
    return false;
  }
  return true;
}

/*
 * @brief Determines if moving forward after turning in a direction would lead
 * to scanning new Cells.
 *
 * @param aircraft The aircraft being examined.
 * @param turn The direction of the turn.
 * @param map THe map the aicraft exists on.
 *
 * @return True if moving forward after turning in a direction would lead to
 * scanning new Cells, else False.
 */
bool forwardAfterTurnScansNew(Aircraft aircraft, const Moves &turn,
                              const GridMap map) {
  Direction simulatedDir = aircraft.getDir();
  if (turn == Moves::move_TURNLEFT) {
    simulatedDir = directionAfterTurnLeft(aircraft.getDir());
  } else if (turn == Moves::move_TURNRIGHT) {
    simulatedDir = directionAfterTurnRight(aircraft.getDir());
  }
  State simulatedPos;
  simulatedPos.direction = simulatedDir;
  simulatedPos.position.first = aircraft.getCurRow();
  simulatedPos.position.second = aircraft.getCurCol();

  const std::pair<int, int> simCoord = positionAfterMovingForward(simulatedPos);

  if (simCoord.first < 0 || simCoord.first >= map.getRowCount() ||
      simCoord.second < 0 || simCoord.second >= map.getColCount()) {
    return false;
  }
  if (turn == Moves::move_TURNLEFT) {
    aircraft.turnLeft();
    aircraft.moveForward();
  } else if (turn == Moves::move_TURNRIGHT) {
    aircraft.turnRight();
    aircraft.moveForward();
  }
  return aircraft.scan() > 0;
}

/*
 * @brief Simulates if moving in a direction based on action is valid and fills
 * newState with the resulting state.
 *
 * @param curState The current state (row, column, direction) of the aircraft.
 * @param move The move (left turn, right turn, forward) for the aircraft to
 * make.
 * @param map The map the aircraft exists on.
 * @param newState The new state (row, column, direction) of the aircraft.
 *
 * @return True if the simulated move is on the map and on a traversable Cell,
 * else False.
 */
bool simulateAction(const State &curState, const Moves &move,
                    const GridMap &map, State &newState) {
  newState = curState; // initialize with current state
  if (move == Moves::move_TURNLEFT) {
    newState.direction = directionAfterTurnLeft(curState.direction);
    return true; // turning in place is always valid
  } else if (move == Moves::move_TURNRIGHT) {
    newState.direction = directionAfterTurnRight(curState.direction);
    return true;
  } else if (move == Moves::move_FORWARD) {
    const std::pair<int, int> newPos = positionAfterMovingForward(curState);
    if (!map.isWithinBounds(newPos.first, newPos.second)) {
      return false;
    }
    if (!map.isTraversable(newPos.first, newPos.second)) {
      return false;
    }
    newState.position = newPos;
    // unchanged direction
    return true;
  }
  return false; // unknown move, shouldn't happen
}

/*
 * @brief If a simple rotation does not lead to a correct move, the aircaft must
 * reroute itself to a valid Cell in multiple moves. This function computes the
 * path the aircraft must take to the nearest valid move using A* search with
 * manhatan distance as the hueristic.
 *
 * @read https://en.wikipedia.org/wiki/A*_search_algorithm
 *
 * @param curPos The pair of <row, column> the Aircraft currently occupies.
 * @param curDir The current direction of the Aircaft.
 * @param targetPos The pair of <row, column> the Aircraft should move to.
 * @param map The GridMap the Aircraft exists on.
 *
 * @return The vector of moves required to reach the desired position.
 */
std::vector<Moves> computePath(const std::pair<int, int> &curPos,
                               Direction curDir,
                               const std::pair<int, int> &targetPos,
                               const GridMap &map) {
  const State startState{curPos, curDir};
  std::priority_queue<AStarNode, std::vector<AStarNode>, AStarNodeComparator>
      openSet;
  std::set<State> closedSet; // visited states

  AStarNode startNode;
  startNode.state = startState;
  startNode.moves = {};
  startNode.cost = 0;
  startNode.f =
      startNode.cost + manhattanDistance(curPos.first, curPos.second,
                                         targetPos.first, targetPos.second);

  openSet.push(startNode);

  while (!openSet.empty()) {
    const AStarNode node = openSet.top();
    openSet.pop();

    // If target position reached (direction does not matter), return moves
    if (node.state.position == targetPos) {
      return node.moves;
    }

    // Skip states that have been processed
    if (closedSet.find(node.state) != closedSet.end())
      continue;
    closedSet.insert(node.state);

    // Try each possible action
    const std::vector<Moves> actions = {
        Moves::move_FORWARD, Moves::move_TURNLEFT, Moves::move_TURNRIGHT};
    for (const Moves &action : actions) {
      State nextState;
      if (simulateAction(node.state, action, map, nextState)) {
        if (closedSet.find(nextState) != closedSet.end())
          continue;
        AStarNode nextNode;
        nextNode.state = nextState;
        nextNode.moves = node.moves;
        nextNode.moves.push_back(action);
        nextNode.cost = node.cost + 1; // Assume each move has a cost of 1
        nextNode.f = nextNode.cost +
                     manhattanDistance(nextState.position.first,
                                       nextState.position.second,
                                       targetPos.first, targetPos.second);
        openSet.push(nextNode);
      }
    }
  }
  return {}; // Return empty vector if no valid path is found
}

} // namespace

RoutePlanner::RoutePlanner(Aircraft aircraft, float searchPercentage,
                           int moveLimit)
    : m_aircraft(aircraft), m_moveLimit(moveLimit),
      m_searchPercentage(searchPercentage) {
  // searchPercentage is a float that must be between 0.01 (1%) and 1.0 (100%)
  if (searchPercentage < PERCENT1) {
    std::cerr << "searchPercentage must be at least 0.01 (1%). Updating value "
                 "to 1%\n";
    m_searchPercentage = PERCENT1;
  } else if (searchPercentage > PERCENT100) {
    std::cerr << "searchPercentage cannot be greater than 1.0 (100%). Updating "
                 "value to 100%\n";
    m_searchPercentage = PERCENT100;
  }

  // moveLimit must be greater than zero
  if (moveLimit < 1) {
    std::cerr << "moveLimit must be greater than 0. Updating value to 1.\n";
    m_moveLimit = 1;
  }
}

std::vector<Moves> RoutePlanner::findRoute() {
  const int targetScanCount =
      std::ceil(m_searchPercentage *
                static_cast<float>(m_aircraft.getMap().getTraversableCount()));

  int scannedCount = 0;

  std::cout << "Aircraft start at [" << m_aircraft.getCurRow() << "]["
            << m_aircraft.getCurCol() << "]\n";

  // Make an initial scan at the starting location
  scannedCount = scannedCount + m_aircraft.scan();
  std::cout << "New scan count: " << scannedCount << ". Target is "
            << targetScanCount
            << ". Remaining: " << targetScanCount - scannedCount << ". Potential moves remaining: " << m_moveLimit - m_totalMoves << ".\n";

  // Continue looping until coverage requirement is hit
  while ((scannedCount < targetScanCount) && (m_totalMoves < m_moveLimit)) {
    std::cout << "Aircraft at [" << m_aircraft.getCurRow() << "]["
              << m_aircraft.getCurCol() << "]\n";
    // If moving forward is possible and beneficial
    if (forwardPositionValid(m_aircraft, m_aircraft.getMap()) &&
        willScanNewCells(m_aircraft)) {
      // Then move forward and scan
      m_aircraft.moveForward();
      m_totalMoves++;
      std::cout << "MOVE FORWARD to [" << m_aircraft.getCurRow() << "]["
                << m_aircraft.getCurCol() << "]\n";
      m_moveList.push_back(Moves::move_FORWARD);
      scannedCount += m_aircraft.scan();
      std::cout << "New scan count: " << scannedCount << ". Target is "
                << targetScanCount
            << ". Remaining: " << targetScanCount - scannedCount << ". Potential moves remaining: " << m_moveLimit - m_totalMoves << ".\n";
      // If forward move is blocked or no new Cells scanned, turn to continue
      // sweeping pattern
      // Try left move first
    } else if (isTurnValid(m_aircraft, Moves::move_TURNLEFT,
                           m_aircraft.getMap()) &&
               forwardAfterTurnScansNew(m_aircraft, Moves::move_TURNLEFT,
                                        m_aircraft.getMap())) {
      // If moving left is beneficial, turn left and scan
      m_aircraft.turnLeft();
      m_totalMoves++;
      std::cout << "TURN LEFT\n";
      m_moveList.push_back(Moves::move_TURNLEFT);
      scannedCount += m_aircraft.scan();
      std::cout << "New scan count: " << scannedCount << ". Target is "
                << targetScanCount
            << ". Remaining: " << targetScanCount - scannedCount << ". Potential moves remaining: " << m_moveLimit - m_totalMoves << ".\n";
      // verify again that forward move is possible, then do it and scan
      if (forwardPositionValid(m_aircraft, m_aircraft.getMap())) {
        m_aircraft.moveForward();
        m_totalMoves++;
        std::cout << "MOVE FORWARD TO [" << m_aircraft.getCurRow() << "]["
                  << m_aircraft.getCurCol() << "]\n";
        m_moveList.push_back(Moves::move_FORWARD);
        scannedCount += m_aircraft.scan();
        std::cout << "New scan count: " << scannedCount << ". Target is "
                  << targetScanCount
            << ". Remaining: " << targetScanCount - scannedCount << ". Potential moves remaining: " << m_moveLimit - m_totalMoves << ".\n";
      }
      // If left move doesn't work, try right move
    } else if (isTurnValid(m_aircraft, Moves::move_TURNRIGHT,
                           m_aircraft.getMap()) &&
               forwardAfterTurnScansNew(m_aircraft, Moves::move_TURNRIGHT,
                                        m_aircraft.getMap())) {
      // If moving right is beneficial, turn right and scan
      m_aircraft.turnRight();
      m_totalMoves++;
      std::cout << "TURN RIGHT\n";
      m_moveList.push_back(Moves::move_TURNRIGHT);
      scannedCount += m_aircraft.scan();
      std::cout << "New scan count: " << scannedCount << ". Target is "
                << targetScanCount
            << ". Remaining: " << targetScanCount - scannedCount << ". Potential moves remaining: " << m_moveLimit - m_totalMoves << ".\n";
      // verify again that forward move is possible, then do it and scan
      if (forwardPositionValid(m_aircraft, m_aircraft.getMap())) {
        m_aircraft.moveForward();
        m_totalMoves++;
        std::cout << "MOVE FORWARD TO [" << m_aircraft.getCurRow() << "]["
                  << m_aircraft.getCurCol() << "]\n";
        m_moveList.push_back(Moves::move_FORWARD);
        scannedCount += m_aircraft.scan();
        std::cout << "New scan count: " << scannedCount << ". Target is "
                  << targetScanCount
            << ". Remaining: " << targetScanCount - scannedCount << ". Potential moves remaining: " << m_moveLimit - m_totalMoves << ".\n";
      }

    } else {
      // If neither left nor right yields a forward move that scans new
      // Cells, the Aircraft is either at a dead-end or surrounded by
      // already scanned/blocked cells. In this case, reposition to the
      // nearest area with unscanned traversable cells.

      // Find the nearest unscanned coordinates using manhattan distance
      const int targetRow = findNearestUnscannedPosRow();
      const int targetCol = findNearestUnscannedPosCol();

      std::cout << "Found new target position at [" << targetRow << "]["
                << targetCol << "]. Must reposition...\n";

      // Using pairs here makes functions less verbose
      std::pair<int, int> curPos;
      curPos.first = m_aircraft.getCurRow();
      curPos.second = m_aircraft.getCurCol();

      std::pair<int, int> targetPos;
      targetPos.first = targetRow;
      targetPos.second = targetCol;

      // Find the path from the current position to the new one
      const std::vector<Moves> path = computePath(
          curPos, m_aircraft.getDir(), targetPos, m_aircraft.getMap());
      if (path.empty()) {
      }

      // Carry out every move required to get to the new position
      for (const auto &move : path) {
        if ((scannedCount < targetScanCount) && (m_totalMoves < m_moveLimit)) {
          if (move == Moves::move_FORWARD) {
            m_aircraft.moveForward();
            m_totalMoves++;
            std::cout << "MOVE FORWARD TO [" << m_aircraft.getCurRow() << "]["
                      << m_aircraft.getCurCol() << "]\n";
            m_moveList.push_back(Moves::move_FORWARD);
            scannedCount += m_aircraft.scan();
            std::cout << "New scan count: " << scannedCount << ". Target is "
                      << targetScanCount
            << ". Remaining: " << targetScanCount - scannedCount << ". Potential moves remaining: " << m_moveLimit - m_totalMoves << ".\n";
          } else if (move == Moves::move_TURNLEFT) {
            m_aircraft.turnLeft();
            m_totalMoves++;
            std::cout << "TURN LEFT\n";
            m_moveList.push_back(Moves::move_TURNLEFT);
            scannedCount += m_aircraft.scan();
            std::cout << "New scan count: " << scannedCount << ". Target is "
                      << targetScanCount
            << ". Remaining: " << targetScanCount - scannedCount << ". Potential moves remaining: " << m_moveLimit - m_totalMoves << ".\n";
          } else if (move == Moves::move_TURNRIGHT) {
            m_aircraft.turnRight();
            m_totalMoves++;
            std::cout << "TURN RIGHT\n";
            m_moveList.push_back(Moves::move_TURNRIGHT);
            scannedCount += m_aircraft.scan();
            std::cout << "New scan count: " << scannedCount << ". Target is "
                      << targetScanCount
            << ". Remaining: " << targetScanCount - scannedCount << ". Potential moves remaining: " << m_moveLimit - m_totalMoves << ".\n";
          }
        }
      }
    }
  }
  std::cout << "TOTAL MOVES: " << m_totalMoves << std::endl;
  return moves;
}

int RoutePlanner::findNearestUnscannedPosRow() {
  int bestTargetRow = -1;
  int bestDistance = std::numeric_limits<int>::max(); // "infinity"

  // For each Cell in the GridMap
  for (int row = 0; row < m_aircraft.getMap().getRowCount(); row++) {
    for (int column = 0; column < m_aircraft.getMap().getColCount(); column++) {
      // If Cell is traversable and not scanned
      if (m_aircraft.getMap().isTraversable(row, column) &&
          !m_aircraft.getMap().isScanned(row, column)) {
        // Calculate Manhattan distance
        const int distance = manhattanDistance(
            m_aircraft.getCurRow(), m_aircraft.getCurCol(), row, column);
        if (distance < bestDistance) {
          bestDistance = distance;
          bestTargetRow = row;
        }
      }
    }
  }
  return bestTargetRow;
}

int RoutePlanner::findNearestUnscannedPosCol() {
  int bestTargetCol = -1;
  int bestDistance = std::numeric_limits<int>::max(); // "infinity"

  // For each Cell in the GridMap
  for (int row = 0; row < m_aircraft.getMap().getRowCount(); row++) {
    for (int column = 0; column < m_aircraft.getMap().getColCount(); column++) {
      // If Cell is traversable and not scanned
      if (m_aircraft.getMap().isTraversable(row, column) &&
          !m_aircraft.getMap().isScanned(row, column)) {
        // Calculate Manhattan distance
        const int distance = manhattanDistance(
            m_aircraft.getCurRow(), m_aircraft.getCurCol(), row, column);
        if (distance < bestDistance) {
          bestDistance = distance;
          bestTargetCol = column;
        }
      }
    }
  }
  return bestTargetCol;
}
