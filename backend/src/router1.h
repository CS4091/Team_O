#ifndef ROUTER
#define ROUTER

#include "aircraft.h"
#include <vector>

enum class Moves { move_FORWARD, move_TURNLEFT, move_TURNRIGHT };

inline std::ostream &operator<<(std::ostream &os, const Moves move) {
  switch (move) {
  case Moves::move_FORWARD:
    os << "MOVE FORWARD (REROUTE)";
    break;
  case Moves::move_TURNLEFT:
    os << "TURN LEFT (REROUTE)";
    break;
  case Moves::move_TURNRIGHT:
    os << "TURN RIGHT (REROUTE)";
    break;
    }
  return os;
}


/*
 * @brief Represents the set of functions completing the route planning
 * algorithm.
 */
class RoutePlanner {
public:
  /*
   * @brief Constructor for the RoutePlanner class. Adds Aircraft as member
   * variable.
   *
   * @param aircraft The Aircraft the RoutePlanner will be routing.
   */
  RoutePlanner(Aircraft aircraft) : m_aircraft(aircraft) {}

  /*
   * @brief A router for the Aircraft, based on a modified version of lawn
   * mowing algorithms combined with A* search.
   *
   * @return Nothin.
   */
  std::vector<Moves> findRoute();

  /*
   * @brief Finds the nearest unscanned row based on manhattan distance.
   *
   * @return The 0-indexed location of the nearest unscanned row.
   */
  int findNearestUnscannedPosRow();
  /*
   * @brief Finds the nearest unscanned column based on manhattan distance.
   *
   * @return The 0-indexed location of the nearest unscanned column.
   */
  int findNearestUnscannedPosCol();

private:
  Aircraft m_aircraft;
  std::vector<Moves> m_moveList;
};

#endif
