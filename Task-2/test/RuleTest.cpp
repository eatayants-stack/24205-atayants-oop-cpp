#include "Rule.h"

#include <gtest/gtest.h>
#include <string>
#include <vector>

TEST(RuleTest, ParsesClassicRule) {
    std::vector<std::string> warnings;
    Rule rule = Rule::FromString("B3/S23", warnings);
    EXPECT_TRUE(warnings.empty());
    EXPECT_EQ(rule.ToString(), "B3/S23");
    EXPECT_TRUE(rule.WillLive(false, 3));
    EXPECT_TRUE(rule.WillLive(true, 2));
    EXPECT_FALSE(rule.WillLive(true, 1));
}

TEST(RuleTest, ParsesHighLifeRule) {
    std::vector<std::string> warnings;
    Rule rule = Rule::FromString("B36/S23", warnings);
    EXPECT_TRUE(warnings.empty());
    EXPECT_EQ(rule.ToString(), "B36/S23");
    EXPECT_TRUE(rule.WillLive(false, 3));
    EXPECT_TRUE(rule.WillLive(false, 6));
    EXPECT_FALSE(rule.WillLive(false, 2));
}

TEST(RuleTest, ParsesImmortalRule) {
    std::vector<std::string> warnings;
    Rule rule = Rule::FromString("B3/S01234567", warnings);
    EXPECT_TRUE(warnings.empty());
    EXPECT_TRUE(rule.WillLive(true, 0));  // Survives with 0 neighbors
    EXPECT_TRUE(rule.WillLive(true, 1));  // Survives with 1 neighbor
    EXPECT_TRUE(rule.WillLive(true, 7));  // Survives with 7 neighbors
    EXPECT_TRUE(rule.WillLive(false, 3)); // Born with 3 neighbors
    // Note: 8 is not in S01234567 (only digits 0-7)
}

TEST(RuleTest, ParsesSeedsRule) {
    std::vector<std::string> warnings;
    Rule rule = Rule::FromString("B2/S", warnings);
    EXPECT_TRUE(warnings.empty());
    EXPECT_TRUE(rule.WillLive(false, 2)); // Born with 2 neighbors
    EXPECT_FALSE(rule.WillLive(true, 2));  // Dies (no survival rules)
    EXPECT_FALSE(rule.WillLive(true, 0));
}

TEST(RuleTest, ParsesComplexRule) {
    std::vector<std::string> warnings;
    Rule rule = Rule::FromString("B1357/S02468", warnings);
    EXPECT_TRUE(warnings.empty());
    EXPECT_TRUE(rule.WillLive(false, 1));
    EXPECT_TRUE(rule.WillLive(false, 3));
    EXPECT_TRUE(rule.WillLive(true, 0));
    EXPECT_TRUE(rule.WillLive(true, 2));
    EXPECT_FALSE(rule.WillLive(false, 2));
    EXPECT_FALSE(rule.WillLive(true, 1));
}

TEST(RuleTest, DefaultRule) {
    Rule rule;
    EXPECT_EQ(rule.ToString(), "B3/S23");
    EXPECT_TRUE(rule.WillLive(false, 3));
    EXPECT_TRUE(rule.WillLive(true, 2));
    EXPECT_TRUE(rule.WillLive(true, 3));
    EXPECT_FALSE(rule.WillLive(false, 2));
    EXPECT_FALSE(rule.WillLive(true, 1));
}

