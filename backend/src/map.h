#ifndef MAP
#define MAP

#include <string>
#include <vector>

// Representation of cells in the map
struct Cell {
  bool traversable;
  bool scanned;
  // TODO: need anything else?
};

/*
 * @brief Represents a 2D grid map loaded from a CSV file.
 *
 * The GridMap is stored as a vector of Cell vectors.
 */
class GridMap {
public:
  /*
   * @brief Constructs a GridMap object.
   *
   * Populates the grid of Cells from a given CSV file.
   * The error handling strategy of this constructor is to never crash on bad
   * data, but instead convert bad data to non-traversable Cells, or truncate
   * extra Cells in a row with too many columns.
   *
   * @param width Represents the x-dimension (number of columns) of the grid.
   * @param filePath The location of the CSV file the map is populated from.
   *
   * @return Nothing.
   */
  GridMap(int width, std::string filePath);

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
  /*
   * @brief Getter for the y-dimension (number of columns) of the grid.
   *
   * @return The y-dimension (number of columns) of the grid.
   */
  int getYSize() const { return ySize; }
  /*
   * @brief Getter for the x-dimension (number of rows) of the grid.
   *
   * @return The x-dimension (number of rows) of the grid.
   */
  int getXSize() const { return xSize; }
  /*
   * @brief Getter for a Cell at a specifc location.
   *
   * @note No error handling for out of bounds.
   *
   * @return The Cell at the requested location.
   */
  Cell getCell(int xCoord, int yCoord) { return grid[xCoord][yCoord]; }

private:
  std::vector<std::vector<Cell>> grid;
  int xSize;
  int ySize;
};

#endif
