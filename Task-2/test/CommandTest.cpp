#include "Command.h"

#include <gtest/gtest.h>
#include <sstream>
#include <string>

TEST(CommandTest, ParseSimpleCommands) {
    auto cmd1 = CommandFactory::Create("exit");
    EXPECT_NE(cmd1, nullptr);
    
    auto cmd2 = CommandFactory::Create("help");
    EXPECT_NE(cmd2, nullptr);
    
    auto cmd3 = CommandFactory::Create("tick");
    EXPECT_NE(cmd3, nullptr);
}

TEST(CommandTest, ParseCommandsWithSpaces) {
    auto cmd1 = CommandFactory::Create("dump filename");
    EXPECT_NE(cmd1, nullptr);
    
    auto cmd2 = CommandFactory::Create("tick 5");
    EXPECT_NE(cmd2, nullptr);
    
    auto cmd3 = CommandFactory::Create("animate 10");
    EXPECT_NE(cmd3, nullptr);
}

TEST(CommandTest, ParseCommandsWithMultipleSpaces) {
    auto cmd1 = CommandFactory::Create("dump  my file with spaces ");
    EXPECT_NE(cmd1, nullptr);
    
    auto cmd2 = CommandFactory::Create("tick  10");
    EXPECT_NE(cmd2, nullptr);
}

TEST(CommandTest, ParseShortCommands) {
    auto cmd1 = CommandFactory::Create("t");
    EXPECT_NE(cmd1, nullptr);
    
    auto cmd2 = CommandFactory::Create("a");
    EXPECT_NE(cmd2, nullptr);
    
    auto cmd3 = CommandFactory::Create("q");
    EXPECT_NE(cmd3, nullptr);
    
    auto cmd4 = CommandFactory::Create("h");
    EXPECT_NE(cmd4, nullptr);
}

TEST(CommandTest, ParseCommandsWithNumbers) {
    auto cmd1 = CommandFactory::Create("tick 1");
    EXPECT_NE(cmd1, nullptr);
    
    auto cmd2 = CommandFactory::Create("tick 100");
    EXPECT_NE(cmd2, nullptr);
    
    auto cmd3 = CommandFactory::Create("animate 5 300");
    EXPECT_NE(cmd3, nullptr);
}

TEST(CommandTest, ParseInvalidCommands) {
    auto cmd1 = CommandFactory::Create("unknown");
    EXPECT_EQ(cmd1, nullptr);
    
    auto cmd2 = CommandFactory::Create("");
    EXPECT_EQ(cmd2, nullptr);
    
    auto cmd3 = CommandFactory::Create("   ");
    EXPECT_EQ(cmd3, nullptr);
}

TEST(CommandTest, ParseQuitVariants) {
    auto cmd1 = CommandFactory::Create("exit");
    EXPECT_NE(cmd1, nullptr);
    
    auto cmd2 = CommandFactory::Create("quit");
    EXPECT_NE(cmd2, nullptr);
    
    auto cmd3 = CommandFactory::Create("q");
    EXPECT_NE(cmd3, nullptr);
}

