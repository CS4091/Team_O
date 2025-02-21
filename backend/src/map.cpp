#include "map.h"
#include <fstream>
#include <iostream>
#include <sstream>

#define TRAVERSABLE 1

GridMap::GridMap(int width, int height) {
  // Assign grid height and width to class private members
  // TODO: Could probably change this to automatically calculate these values from the CSV 
  xSize = width;
  ySize = height;

  // Holds the input CSV
  std::ifstream inputFile;

  // TODO: This hard-coded file path will need to be updated to account for
  // however the file will be passed in from python. I'm guessing it will be a
  // parameter of the constructor.
  inputFile.open("/home/ethan/Code/Team_O/grid_world.csv");

  std::string line, val;

  // Iterate over each line of the csv file
  while (std::getline(inputFile, line)) {
    std::vector<Cell> rowVector;
    std::stringstream s(line);
    // For each value in the line...
    while (getline(s, val, ',')) {
      // Convert the value to a Cell that is unscanned
      Cell cell;
      cell.traversable = (std::stoi(val) == TRAVERSABLE);
      cell.scanned = false;
      // Append it to the current rowVector
      rowVector.push_back(cell);
    }
    // Once the line is scanned, add the associated rowVector to the main vector
    GridMap::grid.push_back(rowVector);
  }
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
