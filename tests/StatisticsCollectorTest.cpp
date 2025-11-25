#include "gtest/gtest.h"
#include "StatisticsCollector.h"
#include <vector>
#include <string>


TEST(StatisticsCollectorTest, ProcessesAndSortsCorrectly) {
    StatisticsCollector stats;
    stats.addWords({"b", "c", "a", "b", "c", "c"});

    stats.process();

    const auto& result = stats.getResults();

    ASSERT_EQ(result.size(), 3);
    

    EXPECT_EQ(result[0].getWord(), "c");
    EXPECT_EQ(result[0].getFrequency(), 3);
    EXPECT_NEAR(result[0].getPercentage(), 50.0, 0.01);

    EXPECT_EQ(result[1].getWord(), "b");
    EXPECT_EQ(result[1].getFrequency(), 2);
    EXPECT_NEAR(result[1].getPercentage(), 33.333, 0.01);

    EXPECT_EQ(result[2].getWord(), "a");
    EXPECT_EQ(result[2].getFrequency(), 1);
    EXPECT_NEAR(result[2].getPercentage(), 16.666, 0.01);
}

TEST(StatisticsCollectorTest, HandlesEmptyCounter) {
    StatisticsCollector stats;
    stats.process();
    
    const auto& result = stats.getResults();
    EXPECT_TRUE(result.empty());
}
