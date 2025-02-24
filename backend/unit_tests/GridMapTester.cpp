#include "../src/map.h"
#include <gtest/gtest.h>

TEST(GridMapTest, ValidCSVTest) {
  // Create a GridMap with width 3 and expected height 2.
  GridMap gridMap(3, "test_csv/validCSV.csv");

  // Check that the grid has the correct dimensions.
  ASSERT_EQ(gridMap.getYSize(), 2);
  ASSERT_EQ(gridMap.getXSize(), 3);

  // Verify cell values.
  EXPECT_TRUE(gridMap.getCell(0, 0).traversable);  // 1
  EXPECT_FALSE(gridMap.getCell(0, 1).traversable); // 0
  EXPECT_TRUE(gridMap.getCell(0, 2).traversable);  // 1
  EXPECT_FALSE(gridMap.getCell(1, 0).traversable); // 0
  EXPECT_TRUE(gridMap.getCell(1, 1).traversable);  // 1
  EXPECT_FALSE(gridMap.getCell(1, 2).traversable); // 0

  EXPECT_FALSE(gridMap.getCell(0, 0).scanned);
  EXPECT_FALSE(gridMap.getCell(0, 1).scanned);
  EXPECT_FALSE(gridMap.getCell(0, 2).scanned);
  EXPECT_FALSE(gridMap.getCell(1, 0).scanned);
  EXPECT_FALSE(gridMap.getCell(1, 1).scanned);
  EXPECT_FALSE(gridMap.getCell(1, 2).scanned);
}

TEST(GridMapTest, CSVWithBadNumericValueTest) {
  // Create a GridMap with width 3 and expected height 1.
  GridMap gridMap(3, "test_csv/badNumericCSV.csv");

  // Check that the grid has the correct dimensions.
  ASSERT_EQ(gridMap.getYSize(), 1);
  ASSERT_EQ(gridMap.getXSize(), 3);

  // verify Cell values.
  EXPECT_TRUE(gridMap.getCell(0, 0).traversable);  // 1
  EXPECT_FALSE(gridMap.getCell(0, 1).traversable); // 2 -> error -> false
  EXPECT_TRUE(gridMap.getCell(0, 2).traversable);  // 1

  EXPECT_FALSE(gridMap.getCell(0, 0).scanned);
  EXPECT_FALSE(gridMap.getCell(0, 1).scanned);
  EXPECT_FALSE(gridMap.getCell(0, 2).scanned);
}

TEST(GridMapTest, CSVWithNonNumericValueTest) {
  // Create a GridMap with width 3 and expected height 1.
  GridMap gridMap(3, "test_csv/badNonNumericCSV.csv");

  // Check that the grid has the correct dimensions.
  ASSERT_EQ(gridMap.getYSize(), 1);
  ASSERT_EQ(gridMap.getXSize(), 3);

  // verify Cell values.
  EXPECT_TRUE(gridMap.getCell(0, 0).traversable);  // 1
  EXPECT_FALSE(gridMap.getCell(0, 1).traversable); // "foo" -> error -> false
  EXPECT_FALSE(gridMap.getCell(0, 2).traversable); // 0

  EXPECT_FALSE(gridMap.getCell(0, 0).scanned);
  EXPECT_FALSE(gridMap.getCell(0, 1).scanned);
  EXPECT_FALSE(gridMap.getCell(0, 2).scanned);
}

TEST(GridMapTest, RowVectorPaddingTest) {
  // Create a GridMap with width 3 and expected height 1.
  GridMap gridMap(3, "test_csv/CSVNeedingPadding.csv");

  // Check that the grid has the correct dimensions.
  ASSERT_EQ(gridMap.getYSize(), 1);
  ASSERT_EQ(gridMap.getXSize(), 3);

  // verify Cell values.
  EXPECT_TRUE(gridMap.getCell(0, 0).traversable);  // 1
  EXPECT_FALSE(gridMap.getCell(0, 1).traversable); // 0
  EXPECT_FALSE(gridMap.getCell(0, 2).traversable); // padded cell -> false

  EXPECT_FALSE(gridMap.getCell(0, 0).scanned);
  EXPECT_FALSE(gridMap.getCell(0, 1).scanned);
  EXPECT_FALSE(gridMap.getCell(0, 2).scanned);
}

TEST(GridMapTest, RowVectorTruncationTest) {
  // Create a GridMap with width 3 and expected height 1.
  GridMap gridMap(3, "test_csv/CSVNeedingTruncation.csv");

  // Check that the grid has the correct dimensions.
  ASSERT_EQ(gridMap.getYSize(), 1);
  ASSERT_EQ(gridMap.getXSize(), 3);

  // verify Cell values.
  EXPECT_TRUE(gridMap.getCell(0, 0).traversable);  // 1
  EXPECT_FALSE(gridMap.getCell(0, 1).traversable); // 0
  EXPECT_TRUE(gridMap.getCell(0, 2).traversable);  // 1
  // The extra value "0" is ignored.

  EXPECT_FALSE(gridMap.getCell(0, 0).scanned);
  EXPECT_FALSE(gridMap.getCell(0, 1).scanned);
  EXPECT_FALSE(gridMap.getCell(0, 2).scanned);
}

TEST(GridMapTest, gridGetterTest) {
  // Create a GridMap with width 3 and expected height 2.
  GridMap gridMap(3, "test_csv/validCSV.csv");

  // Verify cell values.
  EXPECT_TRUE(gridMap.getCell(0, 0).traversable);    // 1
  EXPECT_FALSE(gridMap.getCell(0, 1).traversable);   // 0
  EXPECT_FALSE(gridMap.getCell(99, 99).traversable); // Cell doesn't exist

  EXPECT_FALSE(gridMap.getCell(0, 0).scanned);
  EXPECT_FALSE(gridMap.getCell(0, 1).scanned);
  EXPECT_FALSE(gridMap.getCell(99, 99).scanned);
}

TEST(GridMapTest, gridTraversabilityTest) {
  // Create a GridMap with width 3 and expected height 2.
  GridMap gridMap(3, "test_csv/validCSV.csv");

  // Verify cell values.
  EXPECT_TRUE(gridMap.isTraversable(0, 0));    // 1
  EXPECT_FALSE(gridMap.isTraversable(0, 1));   // 0
  EXPECT_FALSE(gridMap.isTraversable(99, 99)); // Cell doesn't exist
}

TEST(GridMapTest, gridScannedTest) {
  // Create a GridMap with width 3 and expected height 2.
  GridMap gridMap(3, "test_csv/validCSV.csv");

  // Verify cell values.
  EXPECT_FALSE(gridMap.isScanned(0, 0)); // unscanned by default
  EXPECT_FALSE(
      gridMap.isScanned(99, 99)); // doesnt exist, should return unscanned

  gridMap.markScanned(0, 0);
  EXPECT_TRUE(gridMap.isScanned(0, 0));

  gridMap.markScanned(
      99, 99); // doesnt exist, should print a warning rather than crashing

  gridMap.markScanned(0, 0); // already scanned, should print a note to terminal
  EXPECT_TRUE(gridMap.isScanned(0, 0)); // scanned value should not change
}
