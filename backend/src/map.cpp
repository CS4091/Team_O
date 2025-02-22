#include "map.h"
#include <fstream>
#include <iostream>
#include <sstream>

#define NONTRAVERSABLE 0
#define TRAVERSABLE 1

// These helper functions are put in an anonymous namespace tells the compiler
// to keep them visible only in this file
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
            << ". Defaulting to non-traversable cell." << std::endl;

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
            << ". Defaulting to non-traversable cell." << std::endl;

  // Make the Cell non-traversable
  cell.traversable = false;
}

/*
 * @breif Helper function for the GirdMap constructor, enforcing each row to be
 * the proper width.
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
      std::cerr << "Padding missing cells with non-traversable values."
                << std::endl;
      rowVector.resize(xSize, Cell{false, false});
    } else {
      // Else, truncate the extra columns
      std::cerr << "Truncating extra columns." << std::endl;
      rowVector.resize(xSize);
    }
  }
}
} // namespace

GridMap::GridMap(int width) {
  // Assign grid width to class private member
  xSize = width;

  // Holds the input CSV
  std::ifstream inputFile;

  // TODO: This hard-coded file path will need to be updated to account for
  // however the file will be passed in from python. I'm guessing it will be a
  // parameter of the constructor.
  inputFile.open("/home/ethan/Code/Team_O/grid_world.csv");

  std::string line, val;
  int rowCount = 0;

  // Iterate over each line of the csv file
  while (std::getline(inputFile, line)) {
    std::vector<Cell> rowVector;
    std::stringstream s(line);
    int colCount = 0;

    // For each value in the line...
    while (getline(s, val, ',')) {
      // Convert the value to a Cell that is unscanned
      Cell cell;
      try {
        int cellValue = std::stoi(val);
        // If it isn't a 1 or 0, make it non-traversable
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
      cell.scanned = false;
      // Append it to the current rowVector
      rowVector.push_back(cell);
      colCount++;
    }

    // Before adding the row, validate its length to enforce rectangularity
    rowVectorValidation(rowVector, xSize, rowCount);

    // Once the line is scanned, add the associated rowVector to the main vector
    GridMap::grid.push_back(rowVector);
    rowCount++;
  }

  // add the number of rows to the ySize private member
  ySize = rowCount;
}

void GridMap::printer() {
  // For each row...
  for (int row = 0; row < xSize; row++) {
    std::cout << std::endl;
    // For each column...
    for (int column = 0; column < ySize; column++) {
      // Construct a Cell from the grid
      Cell outcell = grid[row][column];
      // Print whether the Cell is traversable (1 for yes, 0 for no)
      std::cout << outcell.traversable;
    }
  }
}
