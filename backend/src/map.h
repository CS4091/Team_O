#ifndef MAP
#define MAP

#include <vector>

// Representation of cells in the map
struct Cell {
  bool traversable;
  bool scanned;
  // need anything else?
};

// Class for the map
class GridMap {
 public:
  GridMap(int width, int height); // TODO: This needs to be able to handle input from a CSV file
  bool isTraversable(int xCoord, int yCoord) const;
  void markScanned(int xCoord, int yCoord);
  // Think of other needed functions

private:
  std::vector<std::vector<Cell>> grid;
};

#endif
