#include "Rule.h"
#include "Universe.h"

#include <gtest/gtest.h>
#include <string>
#include <vector>

TEST(UniverseTest, ToroidalBoundaries) {
    Universe universe(5, 5, Rule(), "Torus");
    
    universe.Set(0, 0, true);
    EXPECT_TRUE(universe.Get(5, 5));    // Wraps to (0,0)
    EXPECT_TRUE(universe.Get(10, 10));  // Wraps to (0,0)
    EXPECT_TRUE(universe.Get(-5, -5));  // Wraps to (0,0)

    universe.Set(4, 4, true);
    EXPECT_TRUE(universe.Get(-1, -1));   // Wraps to (4,4)
    EXPECT_TRUE(universe.Get(9, 9));    // Wraps to (4,4)
}

TEST(UniverseTest, LiveCellsCollection) {
    Universe universe(5, 5, Rule(), "LiveCells");
    
    universe.Set(1, 1, true);
    universe.Set(2, 2, true);
    universe.Set(3, 3, true);

    auto liveCells = universe.LiveCells();
    EXPECT_EQ(liveCells.size(), 3);
    
    // Check that all set cells are in the collection
    bool found1 = false, found2 = false, found3 = false;
    for (const auto& [x, y] : liveCells) {
        if (x == 1 && y == 1) found1 = true;
        if (x == 2 && y == 2) found2 = true;
        if (x == 3 && y == 3) found3 = true;
    }
    EXPECT_TRUE(found1 && found2 && found3);
}

TEST(UniverseTest, IterationCounter) {
    Universe universe(5, 5, Rule(), "Iterations");
    
    EXPECT_EQ(universe.Iteration(), 0);
    universe.IncrementIteration();
    EXPECT_EQ(universe.Iteration(), 1);
    universe.SetIteration(10);
    EXPECT_EQ(universe.Iteration(), 10);
}

TEST(UniverseTest, NameAndRuleManagement) {
    Rule rule1;
    std::vector<std::string> warnings;
    Rule rule2 = Rule::FromString("B36/S23", warnings);
    
    Universe universe(5, 5, rule1, "Test");
    
    EXPECT_EQ(universe.Name(), "Test");
    EXPECT_EQ(universe.GetRule().ToString(), "B3/S23");
    
    universe.SetName("NewName");
    universe.SetRule(rule2);
    
    EXPECT_EQ(universe.Name(), "NewName");
    EXPECT_EQ(universe.GetRule().ToString(), "B36/S23");
}

TEST(UniverseTest, Dimensions) {
    Universe universe(10, 20, Rule(), "Dimensions");
    
    EXPECT_EQ(universe.Width(), 10);
    EXPECT_EQ(universe.Height(), 20);
}

TEST(UniverseTest, CellStateManagement) {
    Universe universe(5, 5, Rule(), "CellState");
    
    EXPECT_FALSE(universe.Get(2, 2));
    universe.Set(2, 2, true);
    EXPECT_TRUE(universe.Get(2, 2));
    universe.Set(2, 2, false);
    EXPECT_FALSE(universe.Get(2, 2));
}

TEST(UniverseTest, NegativeCoordinatesWrap) {
    Universe universe(5, 5, Rule(), "NegativeWrap");
    
    universe.Set(-1, -1, true);
    EXPECT_TRUE(universe.Get(4, 4));
    EXPECT_TRUE(universe.Get(-6, -6));
    
    universe.Set(-10, -10, true);
    EXPECT_TRUE(universe.Get(0, 0));
}

TEST(UniverseTest, LargeCoordinatesWrap) {
    Universe universe(3, 3, Rule(), "LargeWrap");
    
    universe.Set(100, 100, true);
    EXPECT_TRUE(universe.Get(1, 1));
    
    universe.Set(200, 200, true);
    EXPECT_TRUE(universe.Get(2, 2));
}

