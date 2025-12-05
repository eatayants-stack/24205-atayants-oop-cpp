#include "gtest/gtest.h"
#include "Parser.h"
#include <vector>
#include <string>


TEST(ParserTest, HandlesSimpleLine) {
    std::vector<std::string> expected = {"hello", "world"};
    Parser parser;
    EXPECT_EQ(parser.parseLine("Hello world!"), expected);
}

TEST(ParserTest, HandlesMixedSeparators) {
    std::vector<std::string> expected = {"word1", "word2", "word3"};
    Parser parser;
    EXPECT_EQ(parser.parseLine("word1, word2; word3."), expected);
}

TEST(ParserTest, HandlesLineWithNumbers) {
    std::vector<std::string> expected = {"count", "is", "123"};
    Parser parser;
    EXPECT_EQ(parser.parseLine("count is 123"), expected);
}

TEST(ParserTest, HandlesEmptyLine) {
    std::vector<std::string> expected = {};
    Parser parser;
    EXPECT_EQ(parser.parseLine(""), expected);
}

