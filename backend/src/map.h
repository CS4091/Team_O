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
 * @brief Represents a 2D rectangular grid map loaded from a CSV file.
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

  /*
   * @brief Used to determine if a Cell is traversable.
   *
   * @return True if the Cell at grid[row][column] exists and is traversable,
   * else false.
   */
  bool isTraversable(int row, int column) const;
  /*
   * @brief Used to determine if a Cell has been scanned.
   *
   * @return True if the Cell at grid[row][column] exists and is scanned, else
   * false.
   */
  bool isScanned(int row, int column) const;
  /*
   * @brief Marks a Cell as scanned.
   *
   * If the Cell was already scanned, it remains scanned and a note is printed.
   * If the Cell does not exist, a warning is printed and the grid does not
   * change.
   *
   * @return Nothing.
   */
  void markScanned(int row, int column);
  // TODO: Think of other needed functions

  /*
   * @brief Prints the grid map to the terminal.
   *
   * Utility function to ensure the vector of vectors is loading properly.
   *
   * @return Nothing.
   */
  void printer();
  /*
   * @brief Getter for the y-dimension (number of rows) of the grid.
   *
   * @return The y-dimension (number of rows) of the grid.
   */
  int getYSize() const { return ySize; }
  /*
   * @brief Getter for the x-dimension (number of columns) of the grid.
   *
   * @return The x-dimension (number of columns) of the grid.
   */
  int getXSize() const { return xSize; }
  /*
   * @brief Getter for a Cell at a specifc location.
   *
   * @note If a Cell is requested out of bounds, a warning is printed and a
   * dummy Cell is returned with all false values.
   *
   * @return The Cell at grid[row][column].
   */
  Cell getCell(int row, int column);

private:
  std::vector<std::vector<Cell>> grid; // 0-indexed vector of vectors
  int xSize = 0;                       // 1-indexed grid column count
  int ySize = 0;                       // 1-indexed grid row count
};

#endif
