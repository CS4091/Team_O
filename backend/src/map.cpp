#include "map.h"
#include <cassert>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
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
 * @param rowCount The number of the row being checked
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
      rowVector.resize(xSize, Cell{false, false});
    } else {
      // Else, truncate the extra columns
      std::cerr << "Truncating extra columns.\n";
      rowVector.resize(xSize);
    }
  }
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
          cell.traversable = (cellValue == TRAVERSABLE);
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

  // add the number of rows to the ySize private member
  m_rowCount = rowCount;

// For each row...
  for (int row = 0; row < m_rowCount; row++) {
    // For each column...
    for (int column = 0; column < m_colCount; column++) {
      if (isTraversable(row, column)) {
	m_totalTraversable++;
      }
    }

}
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
      // Construct a Cell from the grid
      totalCellCount++;
      if (isTraversable(row, column)) {
        totalTraversableCount++;
      } else {
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
