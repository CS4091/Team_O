#ifndef ROUTER
#define ROUTER

#include "aircraft.h"
#include <vector>

enum class Moves { move_FORWARD, move_TURNLEFT, move_TURNRIGHT };

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
   * @param searchPercentage The percentage of traversable cells to be searched
   * for completion. Bounds checking ensures 0 <= searchPercentage <= 1.
   */
  RoutePlanner(Aircraft aircraft, float searchPercentage);
  /*
   * @brief A router for the Aircraft, based on a modified version of lawn
   * mowing algorithms combined with A* search. Finds a route that scans 80% of
   * traversable Cells.
   *
   * @read https://arxiv.org/pdf/2307.01092
   * @read
   * https://thescipub.com/pdf/jcssp.2012.2032.2041.pdf#:~:text=algorithms,Acar%20and%20Choset%2C%202001
   *
   * @return Nothing.
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
  /*
   * @brief Getter for the searchPercentage.
   *
   * @return The searchPercentage.
   */
  float getSearchPercentage() const { return m_searchPercentage; }

private:
  Aircraft m_aircraft; // The aircraft being routed
  std::vector<Moves> m_moveList;
  int m_totalMoves =
      0; // The number of moves the Aircraft uses to complete its route
  float m_searchPercentage =
      0.0; // The percentage of traversable Cells to be scanned
};

#endif
