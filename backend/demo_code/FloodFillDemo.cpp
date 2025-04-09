#include "../src/map.h"

int main() {
  /*
    Running this code will print the Flood-Fill output to the terminal and map
    stats.
   */
  GridMap map(100, "../unit_tests/test_csv/bigTestGrid.csv");
  map.mapStats();

  return 0;
}
