#include "Parser.h"
#include "Rule.h"
#include "Universe.h"

#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <string>
#include <vector>

namespace {
std::filesystem::path WriteTempFile(const std::string& content) {
    auto path = std::filesystem::temp_directory_path() / "life_test.lif";
    std::ofstream out(path);
    out << content;
    return path;
}
} // namespace

TEST(ParserTest, LoadsUniverseWithNameAndRule) {
    const std::string fileContent =
        "Life 1.06\n"
        "#N Sample\n"
        "#R B36/S23\n"
        "0 0\n"
        "1 0\n";

    auto path = WriteTempFile(fileContent);
    std::vector<std::string> warnings;
    Universe universe = Parser::Load(path.string(), warnings);

    EXPECT_EQ(universe.Name(), "Sample");
    EXPECT_EQ(universe.GetRule().ToString(), "B36/S23");
    EXPECT_TRUE(universe.Get(0, 0));
    EXPECT_TRUE(universe.Get(1, 0));
    EXPECT_TRUE(warnings.empty());
}

TEST(ParserTest, LoadsUniverseWithCustomSize) {
    const std::string fileContent =
        "Life 1.06\n"
        "#N Large Universe\n"
        "#R B3/S23\n"
        "#S 50 50\n"
        "10 10\n"
        "11 11\n";

    auto path = WriteTempFile(fileContent);
    std::vector<std::string> warnings;
    Universe universe = Parser::Load(path.string(), warnings);

    EXPECT_EQ(universe.Name(), "Large Universe");
    EXPECT_EQ(universe.Width(), 50);
    EXPECT_EQ(universe.Height(), 50);
    EXPECT_TRUE(universe.Get(10, 10));
    EXPECT_TRUE(universe.Get(11, 11));
}

TEST(ParserTest, LoadsUniverseWithoutSizeHint) {
    const std::string fileContent =
        "Life 1.06\n"
        "#N AutoSize\n"
        "#R B3/S23\n"
        "5 5\n"
        "10 10\n"
        "15 15\n";

    auto path = WriteTempFile(fileContent);
    std::vector<std::string> warnings;
    Universe universe = Parser::Load(path.string(), warnings);

    EXPECT_EQ(universe.Name(), "AutoSize");
    EXPECT_GE(universe.Width(), 5);
    EXPECT_GE(universe.Height(), 5);
    EXPECT_TRUE(universe.Get(5, 5));
    EXPECT_TRUE(universe.Get(10, 10));
    EXPECT_TRUE(universe.Get(15, 15));
}

TEST(ParserTest, HandlesDuplicateCoordinates) {
    const std::string fileContent =
        "Life 1.06\n"
        "#N Duplicates\n"
        "#R B3/S23\n"
        "1 1\n"
        "2 2\n"
        "1 1\n"
        "3 3\n";

    auto path = WriteTempFile(fileContent);
    std::vector<std::string> warnings;
    Universe universe = Parser::Load(path.string(), warnings);

    // Should have warnings about duplicates
    EXPECT_FALSE(warnings.empty());
    // But should still load unique cells
    EXPECT_TRUE(universe.Get(1, 1));
    EXPECT_TRUE(universe.Get(2, 2));
    EXPECT_TRUE(universe.Get(3, 3));
}

TEST(ParserTest, HandlesMissingNameAndRule) {
    const std::string fileContent =
        "Life 1.06\n"
        "0 0\n"
        "1 1\n";

    auto path = WriteTempFile(fileContent);
    std::vector<std::string> warnings;
    Universe universe = Parser::Load(path.string(), warnings);

    EXPECT_FALSE(warnings.empty()); // Should warn about missing name/rule
    EXPECT_EQ(universe.GetRule().ToString(), "B3/S23"); // Default rule
    EXPECT_TRUE(universe.Get(0, 0));
    EXPECT_TRUE(universe.Get(1, 1));
}

TEST(ParserTest, SaveAndReloadUniverse) {
    Rule rule;
    Universe original(10, 10, rule, "Test Save");
    original.Set(3, 3, true);
    original.Set(4, 4, true);
    original.Set(5, 5, true);

    auto path = WriteTempFile("");
    std::filesystem::remove(path); // Remove empty file

    Parser::Save(original, path.string());

    std::vector<std::string> warnings;
    Universe loaded = Parser::Load(path.string(), warnings);

    EXPECT_EQ(loaded.Name(), "Test Save");
    EXPECT_EQ(loaded.Width(), 10);
    EXPECT_EQ(loaded.Height(), 10);
    EXPECT_TRUE(loaded.Get(3, 3));
    EXPECT_TRUE(loaded.Get(4, 4));
    EXPECT_TRUE(loaded.Get(5, 5));
}

TEST(ParserTest, HandlesInvalidCoordinates) {
    const std::string fileContent =
        "Life 1.06\n"
        "#N Invalid\n"
        "#R B3/S23\n"
        "1 1\n"
        "invalid line\n"
        "2 2\n";

    auto path = WriteTempFile(fileContent);
    std::vector<std::string> warnings;
    Universe universe = Parser::Load(path.string(), warnings);

    EXPECT_FALSE(warnings.empty()); // Should warn about invalid line
    EXPECT_TRUE(universe.Get(1, 1));
    EXPECT_TRUE(universe.Get(2, 2));
}

TEST(ParserTest, HandlesEmptyFile) {
    const std::string fileContent = "Life 1.06\n";

    auto path = WriteTempFile(fileContent);
    std::vector<std::string> warnings;
    Universe universe = Parser::Load(path.string(), warnings);

    EXPECT_FALSE(warnings.empty()); // Should warn about missing data
    EXPECT_EQ(universe.GetRule().ToString(), "B3/S23"); // Default rule
}
