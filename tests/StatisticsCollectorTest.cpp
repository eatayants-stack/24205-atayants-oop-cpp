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
    

    EXPECT_EQ(result[0].word, "c");
    EXPECT_EQ(result[0].frequency, 3);
    EXPECT_NEAR(result[0].percentage, 50.0, 0.01);

    EXPECT_EQ(result[1].word, "b");
    EXPECT_EQ(result[1].frequency, 2);
    EXPECT_NEAR(result[1].percentage, 33.333, 0.01);

    EXPECT_EQ(result[2].word, "a");
    EXPECT_EQ(result[2].frequency, 1);
    EXPECT_NEAR(result[2].percentage, 16.666, 0.01);
}

TEST(StatisticsCollectorTest, HandlesEmptyCounter) {
    StatisticsCollector stats;
    stats.process();
    
    const auto& result = stats.getResults();
    EXPECT_TRUE(result.empty());
}
