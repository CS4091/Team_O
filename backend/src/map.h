#ifndef MAP
#define MAP

#include <vector>

// Representation of cells in the map
struct Cell {
  bool traversable;
  bool scanned;
  // TODO: need anything else?
};

/*
 * @brief Represents a 2D grid map loaded from a CSV file.
 */
class GridMap {
public:
  /*
   * @brief Constructs a GridMap object.
   *
   * Populates the grid of Cells from a CSV file (currently hard-coded).
   * The error handling strategy of this constructor is never crash on bad data,
   * but instead convert bad data to non-traversable Cells, or truncate extra
   * Cells in a row with too many columns.
   *
   * @param width Represents the x-dimension (number of columns) of the grid.
   *
   * @return Nothing.
   */
  GridMap(int width);

  bool isTraversable(int xCoord, int yCoord) const;
  void markScanned(int xCoord, int yCoord);
  // Think of other needed functions

  /*
   * @brief Prints the grid map to the terminal.
   *
   * Utility function to ensure the vector of vectors is loading properly.
   *
   * @return Nothing.
   */
  void printer();

private:
  std::vector<std::vector<Cell>> grid;
  int xSize;
  int ySize;
};

#endif
