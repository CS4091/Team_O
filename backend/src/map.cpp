#include "map.h"
#include <array>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <exception>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

enum { NONTRAVERSABLE = 0, TRAVERSABLE = 1 };

// These helper functions are put in an anonymous namespace which tells the
// compiler to keep them visible only in this file
namespace {

/*
 * @brief Helper function for the GridMap constructor, handling when the CSV
 * contains a number other than 0 or 1 by printing a warning and
 * constructing a non-traversable Cell.
 *
 * @param val The incorrect value from the CSV.
 * @param rowCount The row the value was found at.
 * @param colCount The column the value was found at.
 * @param cell The cell to be inserted.
 *
 * @return Nothing.
 */
void numericError(std::string const val, int const rowCount, int const colCount,
                  Cell &cell) {
  // Print a warning to the terminal
  std::cerr << "Warning: Unexpected numeric value '" << val << "' at row "
            << rowCount << ", column " << colCount
            << ". Defaulting to non-traversable cell.\n";

  // Make the Cell non-traversable
  cell.traversable = false;
}

/*
 * @brief Helper function for the GridMap constructor, handling when the CSV
 * contains a non-number by printing a warning and constructing a
 * non-traversable Cell.
 *
 * @param val The incorrect value from the CSV.
 * @param rowCount The row the value was found at.
 * @param colCount The column the value was found at.
 * @param cell The cell to be inserted.
 *
 * @return Nothing.
 */
void nonNumericError(std::string const val, int const rowCount,
                     int const colCount, Cell &cell) {
  // Print a warning to the terminal
  std::cerr << "Warning: Non-numeric value '" << val << "', at row " << rowCount
            << ", column " << colCount
            << ". Defaulting to non-traversable cell.\n";

  // Make the Cell non-traversable
  cell.traversable = false;
}

/*
 * @breif Helper function for the GridMap constructor, enforcing each row to
 * be the proper width.
 *
 * @param rowVector The row being checked.
 * @param xSize The proper width of a row.
 * @param rowCount The number of the row being checked.
 *
 * @return Nothing.
 */
void rowVectorValidation(std::vector<Cell> &rowVector, const int xSize,
                         const int rowCount) {
  // Check if the row is not the proper length
  if (rowVector.size() != static_cast<size_t>(xSize)) {
    // If not, print a warning to the terminal
    std::cerr << "Warning: Row " << rowCount << " expected " << xSize
              << " columns, but got " << rowVector.size() << ". ";
    // If it is too short...
    if (rowVector.size() < static_cast<size_t>(xSize)) {
      // Pad it with extra non-traversable Cells
      std::cerr << "Padding missing cells with non-traversable values.\n";
      rowVector.resize(xSize, Cell{false, false, false});
    } else {
      // Else, truncate the extra columns
      std::cerr << "Truncating extra columns.\n";
      rowVector.resize(xSize);
    }
  }
}

/*
 * @breif 2D array BFS that searches for the closest traversable Cell from an
 * arbitary point.
 *
 * @param startRow The row to start BFS at.
 * @param startCol The column to start BFS at.
 * @param map The GridMap being searched.
 *
 * @return The <row, column> coordinate pair of the closest traversable Cell.
 */
std::pair<int, int> closestTraversableBFS(const int &startRow,
                                          const int &startCol, GridMap &map) {
  struct BFSCell {
    int row;
    int col;
  };

  const int rows = map.getRowCount();
  const int cols = map.getColCount();

  // visited matrix ensures each Cell is visited only once
  std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));

  // BFS queue
  std::queue<BFSCell> q;
  q.push({startRow, startCol});
  visited[startRow][startCol] = true;

  // Directions for movement (up, down, left, and right)
  std::array<int, 4> dRow = {0, 0, -1, 1};
  std::array<int, 4> dCol = {-1, 1, 0, 0};

  while (!q.empty()) {
    const BFSCell curr = q.front();
    q.pop();

    // Check if current Cell is traversable
    if (map.isTraversable(curr.row, curr.col)) {
      return {curr.row, curr.col};
    }

    // If not, explore the neighboring Cells
    for (int i = 0; i < 4; i++) {
      const int newRow = curr.row + dRow[i];
      const int newCol = curr.col + dCol[i];

      // Ensure new Cell is witin bounds and not yet visited
      if (map.isWithinBounds(newRow, newCol) && !visited[newRow][newCol]) {
        visited[newRow][newCol] = true;
        q.push({newRow, newCol});
      }
    }
  }
  // No traversable Cell found
  throw std::exception();
}

/*
 * @brief Uses 2D array BFS to detect if a Cell is contiguous with at least 21%
 * of all traversable Cells.
 *
 * @param startRow The row to start BFS at.
 * @param startCol The column to start BFS at.
 * @param map The GridMap BFS is performed on.
 *
 * @return True if the Cell is contiguous with at least 21% of all traversable
 * Cells, else false.
 */
bool contiguousTraversableBFS(const int &startRow, const int &startCol,
                              const GridMap &map) {
  struct BFSCell {
    int row;
    int col;
  };

  const int rows = map.getRowCount();
  const int cols = map.getColCount();

  const float PERCENT21 = 0.21;

  const int goalTraversablility =
      std::ceil(static_cast<float>(map.getTraversableCount()) * PERCENT21);
  int currentTraversability = 1;

  // visited matrix ensures each Cell is visited only once
  std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));

  // BFS queue
  std::queue<BFSCell> q;
  q.push({startRow, startCol});
  visited[startRow][startCol] = true;

  // Directions for movement (up, down, left, and right)
  std::array<int, 4> dRow = {0, 0, -1, 1};
  std::array<int, 4> dCol = {-1, 1, 0, 0};

  while (!q.empty()) {
    const BFSCell curr = q.front();
    q.pop();

    // Check if traverability goal has been hit
    if (currentTraversability >= goalTraversablility) {
      return true;
    }

    // Explore the neighboring Cells
    for (int i = 0; i < 4; i++) {
      const int newRow = curr.row + dRow[i];
      const int newCol = curr.col + dCol[i];

      // Ensure new Cell is witin bounds and not yet visited
      if (map.isWithinBounds(newRow, newCol) && !visited[newRow][newCol] &&
          map.isTraversable(newRow, newCol)) {
        visited[newRow][newCol] = true;
        currentTraversability++;
        q.push({newRow, newCol});
      }
    }
  }
  // No traversable Cell found
  return false;
}

/*
 * @brief Finds coordinates suitable to start a Flood-Fill on the GridMap.
 *
 * @param map The GridMap the Flood-Fill will be performed on.
 *
 * @return The coordinates of a traversable Cell in the main contiguous area.
 */
std::pair<int, int> getFloodFillCoordinates(GridMap &map) {
  // Try to find main contiguous traversable area
  // Start at the middle Cell, as it is most likely to be in the main area
  int row = std::ceil(map.getRowCount() / 2);
  int col = std::ceil(map.getColCount() / 2);

  // If the middle Cell is not traversable, BFS out to closest traversable Cell
  if (!map.isTraversable(row, col)) {
    const std::pair<int, int> closestTraversableCord =
        closestTraversableBFS(row, col, map);
    row = closestTraversableCord.first;
    col = closestTraversableCord.second;
  }

  // Do a BFS until finding at least 21% of total traversable Cells (confirming
  // that it is the main contiguous traversable area)
  if (contiguousTraversableBFS(row, col, map)) {
    return {row, col};
  }

  // If not 21%, start looping through all Cells until main contiguous area is
  // found
  for (int loopRow = 0; loopRow < map.getRowCount(); loopRow++) {
    for (int loopCol = 0; loopCol < map.getColCount(); loopCol++) {
      if (map.isTraversable(loopRow, loopCol)) {
        if (contiguousTraversableBFS(loopRow, loopCol, map)) {
          return {loopRow, loopCol};
        }
      }
    }
  }

  throw std::exception(); // means there are no traversable Cells
}

/*
 * @brief Recrusively performs a Flood-Fill to mark only the main contiguous
 * traversable Cells as traversable.
 *
 * @note This recursive implementation may be too heavy on stack usage for very
 * large GridMaps. If we end up running into a problem here, we need to switch
 * to an iterative implementation.
 *
 * @param row The row the algorithm is ran on.
 * @param col The column the algorithm is ran on.
 * @param map The GridMap the algorithm is ran on.
 *
 * @return Nothing.
 */
void floodFill(const int row, const int col, GridMap &map) {
  // Base case: If out of bounds or not traversable, stop recursing
  if (!map.isWithinBounds(row, col) || !map.isTraversable(row, col) ||
      map.isColored(row, col)) {
    return;
  }

  // mark row as colored
  map.markColored(row, col);

  // recurse to the north, south, east, and west
  floodFill(row - 1, col, map);
  floodFill(row + 1, col, map);
  floodFill(row, col + 1, map);
  floodFill(row, col - 1, map);
}

} // namespace

GridMap::GridMap(int width, std::string filePath) : m_colCount(width) {
  // Holds the input CSV
  std::ifstream inputFile;

  // TODO: Figure out how to properly handle this when python is calling the
  // constructor
  inputFile.open(filePath); // flawfinder: ignore

  // If the file isn't opened, exit with uncaught exception
  if (!inputFile.is_open()) {
    throw std::exception();
  }

  std::string line;
  std::string val;
  int rowCount = 0;

  // Iterate over each line of the csv file
  while (std::getline(inputFile, line)) {
    std::vector<Cell> rowVector;
    std::stringstream curLine(line);
    int colCount = 0;

    // For each value in the line...
    while (getline(curLine, val, ',')) {
      // Convert the value to a Cell that is unscanned
      Cell cell{};
      cell.scanned = false;
      // Then determine if it is travesable or not
      try {
        const int cellValue = std::stoi(val);
        // If it isn't a 1 or 0, make it nontraversable
        if (cellValue != TRAVERSABLE && cellValue != NONTRAVERSABLE) {
          numericError(val, rowCount, colCount, cell);
        } else {
          // Else assign the proper traverability value
          if (cellValue == TRAVERSABLE) {
            cell.traversable = true;
            m_totalTraversable++;
          } else
            cell.traversable = false;
        }
        // If it isn't a number, also make it non-traversable
      } catch (const std::invalid_argument &) {
        nonNumericError(val, rowCount, colCount, cell);
      }
      // Append it to the current rowVector
      rowVector.push_back(cell);
      colCount++;
    }

    // Before adding the row, validate its length to enforce rectangularity
    rowVectorValidation(rowVector, m_colCount, rowCount);

    // Once the line is scanned, add the associated rowVector to the main vector
    GridMap::m_grid.push_back(rowVector);
    rowCount++;
  }

  // Add the number of rows to the ySize private member
  m_rowCount = rowCount;

  // Now a flood-fill will be performed to capture areas of traversable Cells
  // that can not be accessed by the main contiguous area of traverable Cells
  const std::pair<int, int> floodFillCoord = getFloodFillCoordinates(*this);
  floodFill(floodFillCoord.first, floodFillCoord.second, *this);

  // Traverse every Cell. Only Cells that are colored should be traversable
  int convertedCells = 0; // num Cells converted to untraversable
  for (int row = 0; row < m_rowCount; row++) {
    for (int col = 0; col < m_colCount; col++) {
      // If a traverable Cell isn't colored, it isn't accessible
      if (isTraversable(row, col) && !isColored(row, col)) {
        // print a log message and mark it as untraversable
        std::cerr << "The Cell at [" << row << "][" << col
                  << "] was marked as traversable, but is not accessible. It "
                     "will be marked as untraversable\n";
        markUntraversable(row, col);
        convertedCells++;
      }
    }
  }
  // Print a statistic
  std::cerr << convertedCells << " were altered to being untraversable\n";

  m_totalTraversable -= convertedCells;
}

void GridMap::printer() {
  // For each row...
  for (int row = 0; row < m_rowCount; row++) {
    std::cout << std::endl;
    // For each column...
    for (int column = 0; column < m_colCount; column++) {
      // Construct a Cell from the grid
      const Cell outcell = m_grid[row][column];
      // Print whether the Cell is traversable (1 for yes, 0 for no)
      std::cout << outcell.traversable;
    }
  }
}

void GridMap::mapStats() {
  int totalCellCount = 0;
  int totalTraversableCount = 0;
  int totalUntraversableCount = 0;

  // For each row...
  for (int row = 0; row < m_rowCount; row++) {
    // For each column...
    for (int column = 0; column < m_colCount; column++) {
      // Increment total Cell count
      totalCellCount++;
      if (isTraversable(row, column)) {
        // If traverable, increment traversable count
        totalTraversableCount++;
      } else {
        // Else, increment untraversable count
        totalUntraversableCount++;
      }
    }
  }
  std::cout << "Total cell count: " << totalCellCount << std::endl;
  std::cout << "Total traversable count: " << totalTraversableCount
            << std::endl;
  std::cout << "Total untraversable count: " << totalUntraversableCount
            << std::endl;
}

bool GridMap::isTraversable(int row, int column) const {
  assert(row > -1 && column > -1 && "Attempted to access negative index");

  if (row > m_rowCount || column > m_colCount) {
    std::cerr << "Out of bounds move is not valid\n";
  }
  // Return the Cell's traverability value
  return m_grid[row][column].traversable;
}

bool GridMap::isScanned(int row, int column) const {
  assert(row > -1 && column > -1 && "Attempted to access negative index");
  assert(row < m_rowCount && column < m_colCount);

  // Return the Cell's scanned value
  return m_grid[row][column].scanned;
}

void GridMap::markScanned(int row, int column) {
  assert(row > -1 && column > -1 && "Attempted to access negative index");
  assert(row < m_rowCount && column < m_colCount &&
         "Attempted out of bounds access");

  // Determine if the Cell was already scanned
  if (m_grid[row][column].scanned) {
    // If so, print a note and leave it be
    std::cerr << "Note: Requested a Cell at [" << row << "][" << column
              << "] is already scanned\n";
    return;
  }

  // Else, mark it as scanned
  m_grid[row][column].scanned = true;
}

void GridMap::markColored(int row, int column) {
  assert(row > -1 && column > -1 && "Attempted to access negative index");
  assert(row < m_rowCount && column < m_colCount &&
         "Attempted out of bounds access");

  m_grid[row][column].colored = true;
}

bool GridMap::isWithinBounds(int row, int col) const {
  return (row >= 0 && row < m_rowCount && col >= 0 && col < m_colCount);
}

Cell GridMap::getCell(int row, int column) {
  assert(row > -1 && column > -1 && "Attempted to access negative index");
  assert(row < m_rowCount && column < m_colCount &&
         "Attempted out of bounds access");

  // Return the Cell
  return m_grid[row][column];
}
