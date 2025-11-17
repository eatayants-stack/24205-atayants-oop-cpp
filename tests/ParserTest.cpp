#include "gtest/gtest.h"
#include "Parser.h"
#include <vector>
#include <string>


TEST(ParserTest, HandlesSimpleLine) {
    std::vector<std::string> expected = {"hello", "world"};
    EXPECT_EQ(Parser::parseLine("Hello world!"), expected);
}

TEST(ParserTest, HandlesMixedSeparators) {
    std::vector<std::string> expected = {"word1", "word2", "word3"};
    EXPECT_EQ(Parser::parseLine("word1, word2; word3."), expected);
}

TEST(ParserTest, HandlesLineWithNumbers) {
    std::vector<std::string> expected = {"count", "is", "123"};
    EXPECT_EQ(Parser::parseLine("count is 123"), expected);
}

TEST(ParserTest, HandlesEmptyLine) {
    std::vector<std::string> expected = {};
    EXPECT_EQ(Parser::parseLine(""), expected);
}

