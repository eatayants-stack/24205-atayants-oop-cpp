#include <gtest/gtest.h>
#include "BitArray.h"

// --- Constructor Tests ---

TEST(BitArrayTest, DefaultConstructor) {
    BitArray b;
    EXPECT_EQ(b.size(), 0);
    EXPECT_TRUE(b.empty());
    EXPECT_EQ(b.to_string(), "");
}

TEST(BitArrayTest, ValueConstructor) {
    // 10 bits, init with 5 (binary ...00101)
    BitArray b(10, 5); 
    EXPECT_EQ(b.size(), 10);
    EXPECT_TRUE(b[0]); // LSB is 1 (5 is 101)
    EXPECT_FALSE(b[1]);
    EXPECT_TRUE(b[2]);
    EXPECT_FALSE(b[3]);
    EXPECT_EQ(b.to_string(), "1010000000"); // index 0 is left
}

TEST(BitArrayTest, ValueConstructorOverflow) {
    // Only 2 bits, but value is larger. Should truncate.
    BitArray b(2, 7); // 7 is 111
    EXPECT_EQ(b.size(), 2);
    EXPECT_TRUE(b[0]);
    EXPECT_TRUE(b[1]);
    // Ensure padding bits don't mess up count
    EXPECT_EQ(b.count(), 2);
}

// --- Modification Tests ---

TEST(BitArrayTest, Resize) {
    BitArray b(5, 0);
    b.resize(10, true);
    EXPECT_EQ(b.size(), 10);
    EXPECT_TRUE(b[9]); // New bits should be true
    
    b.resize(3);
    EXPECT_EQ(b.size(), 3);
    EXPECT_THROW(b[4], std::out_of_range);
}

TEST(BitArrayTest, PushBack) {
    BitArray b;
    b.push_back(true);
    b.push_back(false);
    EXPECT_EQ(b.size(), 2);
    EXPECT_TRUE(b[0]);
    EXPECT_FALSE(b[1]);
}

TEST(BitArrayTest, SetReset) {
    BitArray b(10);
    b.set(5);
    EXPECT_TRUE(b[5]);
    b.reset(5);
    EXPECT_FALSE(b[5]);
    
    b.set(); // Set all
    EXPECT_EQ(b.count(), 10);
    b.reset();
    EXPECT_EQ(b.count(), 0);
}

// --- Bitwise Operations Tests ---

TEST(BitArrayTest, BitwiseAnd) {
    BitArray b1(4, 5); // 0101 (binary) -> "1010" string
    BitArray b2(4, 3); // 0011 (binary) -> "1100" string
    BitArray res = b1 & b2; // 0001 -> "1000"
    EXPECT_TRUE(res[0]);
    EXPECT_FALSE(res[1]);
}

TEST(BitArrayTest, BitwiseOr) {
    BitArray b1(4, 5); 
    BitArray b2(4, 3); 
    BitArray res = b1 | b2; // 0111 -> "1110"
    EXPECT_EQ(res.count(), 3);
}

TEST(BitArrayTest, BitwiseXor) {
    BitArray b1(4, 5); 
    BitArray b2(4, 3); 
    BitArray res = b1 ^ b2; // 0110 -> "0110"
    EXPECT_FALSE(res[0]);
    EXPECT_TRUE(res[1]);
    EXPECT_TRUE(res[2]);
    EXPECT_FALSE(res[3]);
}

TEST(BitArrayTest, SizeMismatch) {
    BitArray b1(5);
    BitArray b2(10);
    EXPECT_THROW(b1 &= b2, std::invalid_argument);
    EXPECT_THROW(b1 |= b2, std::invalid_argument);
    EXPECT_THROW(b1 ^= b2, std::invalid_argument);
}

// --- Shift Tests ---

TEST(BitArrayTest, LeftShift) {
    BitArray b(10);
    b.set(0); // 1 at index 0
    b <<= 2;
    EXPECT_FALSE(b[0]);
    EXPECT_TRUE(b[2]);
    EXPECT_EQ(b.count(), 1);
    
    b <<= 20; // Shift out of range
    EXPECT_TRUE(b.none());
}

TEST(BitArrayTest, RightShift) {
    BitArray b(10);
    b.set(9); 
    b >>= 2;
    EXPECT_TRUE(b[7]);
    EXPECT_FALSE(b[9]);
}

TEST(BitArrayTest, Inversion) {
    BitArray b(3); // 000
    b = ~b;        // 111
    EXPECT_EQ(b.count(), 3);
    // Crucial check: did inversion mess up the padding bits?
    // If padding isn't handled, resize might reveal garbage.
    b.resize(100, false);
    EXPECT_FALSE(b[50]); // Should be false (newly added)
}

// --- Equality Tests ---

TEST(BitArrayTest, Equality) {
    BitArray b1(10, 5);
    BitArray b2(10, 5);
    BitArray b3(10, 6);
    EXPECT_TRUE(b1 == b2);
    EXPECT_TRUE(b1 != b3);
    
    BitArray b4(11, 5);
    EXPECT_FALSE(b1 == b4); // diff size
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}