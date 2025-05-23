#ifndef MAP
#define MAP

#include <string>
#include <vector>

// Representation of cells in the map
struct Cell {
  bool traversable;
  bool scanned;
  bool colored;
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
   * @param width Represents the 1-indexed x-dimension (number of columns) of
   * the grid.
   * @param filePath The location of the CSV file the map is populated from.
   *
   * @return Nothing.
   */
  GridMap(int width, std::string filePath);

  /*
   * @brief Used to determine if a Cell is traversable.
   *
   * @param row The row (y coordinate) being accessed.
   * @param column the column (x coordinate) being accessed.
   *
   * @return True if the Cell at grid[row][column] is traversable
   */
  bool isTraversable(int row, int column) const;
  /*
   * @brief Used to determine if a Cell has been scanned.
   *
   * @param row The row (y coordinate) being accessed.
   * @param column the column (x coordinate) being accessed.
   *
   * @return True if the Cell at grid[row][column] is scanned
   */
  bool isScanned(int row, int column) const;
  /*
   * @brief Marks a Cell as scanned.
   *
   * If the Cell was already scanned, it remains scanned and a note is printed.
   *
   * @param row The row (y coordinate) being accessed.
   * @param column The column (x coordinate) being accessed.
   *
   * @return Nothing.
   */
  void markScanned(int row, int column);

  /*
   * @breif Marks a Cell as colored.
   *
   * @param row The row (y coordinate) being accessed.
   * @param column The column (x coordinate) being accessed.
   *
   * @return Nothing.
   */
  void markColored(int row, int column);

  /*
   * @brief Marks a Cell as untraversable.
   *
   * @param row The row (y coordinate) being marked as untraversable.
   * @param column The column (x coordinate) being makred as untraversable.
   *
   * @return Nothing.
   */
  void markUntraversable(int row, int column) {
    m_grid[row][column].traversable = false;
  }

  /*
   * @brief Determines if a Cell is colored.
   *
   * @param row The row (y coodinate) being examined.
   * @param column (x coordinate) being examined.
   *
   * @return True if a Cell is colored, else False.
   */
  bool isColored(int row, int column) const {
    return m_grid[row][column].colored;
  }

  /*
   * @brief Determines if a position is within bounds of the map.
   *
   * @row The row whose position is being determined.
   * @col The column whose position is being determined.
   *
   * @return True if the position is valid, else false.
   */
  bool isWithinBounds(int row, int col) const;
  /*
   * @brief Prints the grid map to the terminal.
   *
   * Utility function to ensure the vector of vectors is loading properly.
   *
   * @return Nothing.
   */
  void printer();

  /*
   *
   *
   *
   */
  void mapStats();
  /*
   * @brief Getter for the y-dimension (number of rows) of the grid.
   *
   * @return The y-dimension (number of rows) of the grid.
   */
  int getRowCount() const { return m_rowCount; }
  /*
   * @brief Getter for the x-dimension (number of columns) of the grid.
   *
   * @return The x-dimension (number of columns) of the grid.
   */
  int getColCount() const { return m_colCount; }
  /*
   * @brief Getter for a Cell at a specifc location.
   *
   * @param row The row (y coordinate) being accessed.
   * @param column the column (x coordinate) being accessed.
   *
   * @return The Cell at grid[row][column].
   */
  Cell getCell(int row, int column);
  /*
   * @brief Getter for the number of traversable Cells.
   *
   * @return The number of traversable Cells.
   */
  int getTraversableCount() const { return m_totalTraversable; }

private:
  std::vector<std::vector<Cell>> m_grid; // 0-indexed vector of vectors
  int m_colCount = 0;                    // 1-indexed grid column count
  int m_rowCount = 0;                    // 1-indexed grid row count
  int m_totalTraversable = 0; // The total number of traversable Cells
};

#endif
