#include "../src/map.h"
#include <gtest/gtest.h>

TEST(GridMapTest, ValidCSVTest) {
  // Create a GridMap with width 3 and expected height 2.
  GridMap gridMap(3, "test_csv/validCSV.csv");

  // Check that the grid has the correct dimensions.
  ASSERT_EQ(gridMap.getRowCount(), 2);
  ASSERT_EQ(gridMap.getColCount(), 3);

  // Verify cell values.
  EXPECT_FALSE(gridMap.getCell(0, 0)
                   .traversable); // 1, but untraversable because of flood-fill
  EXPECT_FALSE(gridMap.getCell(0, 1).traversable); // 0
  EXPECT_FALSE(gridMap.getCell(0, 2)
                   .traversable); // 1, but untraversable because of flood-fill
  EXPECT_FALSE(gridMap.getCell(1, 0).traversable); // 0
  EXPECT_TRUE(gridMap.getCell(1, 1).traversable);  // 1
  EXPECT_FALSE(gridMap.getCell(1, 2).traversable); // 0

  EXPECT_FALSE(gridMap.getCell(0, 0).scanned);
  EXPECT_FALSE(gridMap.getCell(0, 1).scanned);
  EXPECT_FALSE(gridMap.getCell(0, 2).scanned);
  EXPECT_FALSE(gridMap.getCell(1, 0).scanned);
  EXPECT_FALSE(gridMap.getCell(1, 1).scanned);
  EXPECT_FALSE(gridMap.getCell(1, 2).scanned);

  // Fail assertion on attempting to access non existent cell
  EXPECT_DEATH(gridMap.getCell(99, 99), "^.*$");
}

TEST(GridMapTest, NoCSV) {
  // Try to construct GridMap from nonexistent CSV
  EXPECT_THROW(GridMap gridMap(1, "IDontExist.csv"), std::exception);
}

TEST(GridMapTest, CSVWithBadNumericValueTest) {
  // Create a GridMap with width 3 and expected height 1.
  GridMap gridMap(3, "test_csv/badNumericCSV.csv");

  // Check that the grid has the correct dimensions.
  ASSERT_EQ(gridMap.getRowCount(), 1);
  ASSERT_EQ(gridMap.getColCount(), 3);

  // verify Cell values.
  EXPECT_TRUE(gridMap.getCell(0, 0).traversable);  // 1
  EXPECT_FALSE(gridMap.getCell(0, 1).traversable); // 2 -> error -> false
  EXPECT_FALSE(gridMap.getCell(0, 2)
                   .traversable); // 1, but untraversable beacuse of flood-fill

  EXPECT_FALSE(gridMap.getCell(0, 0).scanned);
  EXPECT_FALSE(gridMap.getCell(0, 1).scanned);
  EXPECT_FALSE(gridMap.getCell(0, 2).scanned);
}

TEST(GridMapTest, CSVWithNonNumericValueTest) {
  // Create a GridMap with width 3 and expected height 1.
  GridMap gridMap(3, "test_csv/badNonNumericCSV.csv");

  // Check that the grid has the correct dimensions.
  ASSERT_EQ(gridMap.getRowCount(), 1);
  ASSERT_EQ(gridMap.getColCount(), 3);

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
  ASSERT_EQ(gridMap.getRowCount(), 1);
  ASSERT_EQ(gridMap.getColCount(), 3);

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
  ASSERT_EQ(gridMap.getRowCount(), 1);
  ASSERT_EQ(gridMap.getColCount(), 3);

  // verify Cell values.
  EXPECT_TRUE(gridMap.getCell(0, 0).traversable);  // 1
  EXPECT_FALSE(gridMap.getCell(0, 1).traversable); // 0
  EXPECT_FALSE(gridMap.getCell(0, 2)
                   .traversable); // 1, but untraversable because of flood-fill
  // The extra value "0" is ignored.

  EXPECT_FALSE(gridMap.getCell(0, 0).scanned);
  EXPECT_FALSE(gridMap.getCell(0, 1).scanned);
  EXPECT_FALSE(gridMap.getCell(0, 2).scanned);
}

TEST(GridMapTest, gridScannedTest) {
  // Create a GridMap with width 3 and expected height 2.
  GridMap gridMap(3, "test_csv/validCSV.csv");

  // Verify cell values.
  EXPECT_FALSE(gridMap.isScanned(0, 0)); // unscanned by default

  gridMap.markScanned(0, 0);
  EXPECT_TRUE(gridMap.isScanned(0, 0));

  gridMap.markScanned(0, 0); // already scanned, should print a note to terminal
  EXPECT_TRUE(gridMap.isScanned(0, 0)); // scanned value should not change

  // Fail assertion on attempting to access non existent cell
  EXPECT_DEATH(gridMap.markScanned(99, 99), "^.*$");
}

TEST(GridMapTest, floodFillTest) {
  // Create a GridMap with width 3 and expected height 2.
  GridMap gridMap(100, "test_csv/bigTestGrid.csv");
  EXPECT_FALSE(gridMap.isTraversable(11, 99));
  EXPECT_TRUE(gridMap.isTraversable(23, 34));
}
