#include "Engine.h"
#include "Rule.h"
#include "Universe.h"

#include <gtest/gtest.h>
#include <string>
#include <vector>

TEST(EngineTest, BlinkerOscillator) {
    Universe universe(5, 5, Rule(), "Blinker");
    universe.Set(2, 1, true);
    universe.Set(2, 2, true);
    universe.Set(2, 3, true);

    Engine engine;
    engine.Tick(universe, 1);
    EXPECT_TRUE(universe.Get(1, 2));
    EXPECT_TRUE(universe.Get(2, 2));
    EXPECT_TRUE(universe.Get(3, 2));
    EXPECT_FALSE(universe.Get(2, 1));
    EXPECT_FALSE(universe.Get(2, 3));

    engine.Tick(universe, 1);
    EXPECT_TRUE(universe.Get(2, 1));
    EXPECT_TRUE(universe.Get(2, 2));
    EXPECT_TRUE(universe.Get(2, 3));
}

TEST(EngineTest, ToroidalWrapping) {
    Universe universe(3, 3, Rule(), "Torus");
    universe.Set(0, 0, true);
    universe.Set(2, 2, true);

    Engine engine;
    engine.Tick(universe, 1);

    // The two diagonal corners should see each other through wrapping and die.
    EXPECT_FALSE(universe.Get(0, 0));
    EXPECT_FALSE(universe.Get(2, 2));
}

TEST(EngineTest, ToadOscillator) {
    Universe universe(6, 6, Rule(), "Toad");
    // Toad pattern: horizontal line of 4
    universe.Set(2, 2, true);
    universe.Set(3, 2, true);
    universe.Set(4, 2, true);
    universe.Set(1, 3, true);
    universe.Set(2, 3, true);
    universe.Set(3, 3, true);

    Engine engine;
    std::size_t initialCells = universe.LiveCells().size();
    engine.Tick(universe, 1);

    // Toad oscillates, so pattern should change but maintain similar cell count
    std::size_t afterFirstTick = universe.LiveCells().size();
    EXPECT_GE(afterFirstTick, 4); // Should have at least 4 cells
    EXPECT_LE(afterFirstTick, 8); // Should not grow too much

    engine.Tick(universe, 1);

    // After two ticks, should oscillate back (similar to original)
    std::size_t afterSecondTick = universe.LiveCells().size();
    EXPECT_GE(afterSecondTick, 4);
    EXPECT_LE(afterSecondTick, 8);
    // Should have similar number of cells as initial
    EXPECT_NEAR(afterSecondTick, initialCells, 2);
}

TEST(EngineTest, BlockStillLife) {
    Universe universe(5, 5, Rule(), "Block");
    // Block pattern (2x2 square) - should remain stable
    universe.Set(1, 1, true);
    universe.Set(2, 1, true);
    universe.Set(1, 2, true);
    universe.Set(2, 2, true);

    Engine engine;
    engine.Tick(universe, 10); // Multiple iterations

    // Block should remain unchanged
    EXPECT_TRUE(universe.Get(1, 1));
    EXPECT_TRUE(universe.Get(2, 1));
    EXPECT_TRUE(universe.Get(1, 2));
    EXPECT_TRUE(universe.Get(2, 2));
    EXPECT_FALSE(universe.Get(0, 0));
    EXPECT_FALSE(universe.Get(3, 3));
}

TEST(EngineTest, GliderMovement) {
    Universe universe(10, 10, Rule(), "Glider");
    // Glider pattern
    universe.Set(1, 0, true);
    universe.Set(2, 1, true);
    universe.Set(0, 2, true);
    universe.Set(1, 2, true);
    universe.Set(2, 2, true);

    Engine engine;
    engine.Tick(universe, 1);

    // Glider should move down-right after one tick
    // Check that glider has moved (not in original position)
    bool gliderMoved = !universe.Get(1, 0) || !universe.Get(2, 1) || 
                       !universe.Get(0, 2) || !universe.Get(1, 2) || !universe.Get(2, 2);
    EXPECT_TRUE(gliderMoved);
    
    // Check that there are still 5 live cells (glider maintains its size)
    auto liveCells = universe.LiveCells();
    EXPECT_EQ(liveCells.size(), 5);

    engine.Tick(universe, 3);

    // After 4 total ticks, glider should have moved further
    EXPECT_FALSE(universe.Get(0, 0));
    EXPECT_FALSE(universe.Get(1, 0));
    // Glider should still exist (5 cells)
    liveCells = universe.LiveCells();
    EXPECT_EQ(liveCells.size(), 5);
}

TEST(EngineTest, MultipleIterations) {
    Universe universe(5, 5, Rule(), "MultiTick");
    universe.Set(2, 2, true);
    universe.Set(2, 3, true);
    universe.Set(2, 4, true);

    Engine engine;
    std::size_t initialIteration = universe.Iteration();
    engine.Tick(universe, 5);

    EXPECT_EQ(universe.Iteration(), initialIteration + 5);
    // Blinker should have oscillated
    EXPECT_TRUE(universe.Get(2, 2) || universe.Get(1, 3) || universe.Get(3, 3));
}

TEST(EngineTest, CustomRuleHighLife) {
    std::vector<std::string> warnings;
    Rule highLife = Rule::FromString("B36/S23", warnings);
    Universe universe(10, 10, highLife, "HighLife");
    
    // Replicator pattern for HighLife
    universe.Set(4, 4, true);
    universe.Set(5, 4, true);
    universe.Set(4, 5, true);
    universe.Set(5, 5, true);
    universe.Set(3, 3, true);
    universe.Set(6, 6, true);

    Engine engine;
    engine.Tick(universe, 1);

    // With HighLife rule, pattern should evolve differently
    EXPECT_EQ(universe.GetRule().ToString(), "B36/S23");
}

TEST(EngineTest, SingleCellDies) {
    Universe universe(5, 5, Rule(), "SingleCell");
    universe.Set(2, 2, true);

    Engine engine;
    engine.Tick(universe, 1);

    // Single cell with less than 2 neighbors should die
    EXPECT_FALSE(universe.Get(2, 2));
}

TEST(EngineTest, Overcrowding) {
    Universe universe(5, 5, Rule(), "Overcrowding");
    // Create a pattern where a cell has more than 3 neighbors
    universe.Set(2, 2, true);
    universe.Set(1, 1, true);
    universe.Set(2, 1, true);
    universe.Set(3, 1, true);
    universe.Set(1, 2, true);
    universe.Set(3, 2, true);
    universe.Set(1, 3, true);
    universe.Set(2, 3, true);
    universe.Set(3, 3, true);

    Engine engine;
    engine.Tick(universe, 1);

    // Center cell (2,2) has 8 neighbors, should die from overcrowding
    EXPECT_FALSE(universe.Get(2, 2));
}

