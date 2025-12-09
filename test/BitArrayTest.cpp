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

// --- Edge Cases Tests ---

TEST(BitArrayTest, ZeroSizeConstructor) {
    BitArray b(0);
    EXPECT_EQ(b.size(), 0);
    EXPECT_TRUE(b.empty());
    EXPECT_EQ(b.count(), 0);
    EXPECT_TRUE(b.none());
    EXPECT_FALSE(b.any());
}

TEST(BitArrayTest, EmptyArrayOperations) {
    BitArray b1;
    BitArray b2;
    
    // Operations on empty arrays
    EXPECT_TRUE(b1 == b2);
    EXPECT_EQ(b1.count(), 0);
    EXPECT_TRUE(b1.none());
    EXPECT_FALSE(b1.any());
    
    // Bitwise operations on empty arrays
    BitArray res = b1 & b2;
    EXPECT_EQ(res.size(), 0);
    
    res = b1 | b2;
    EXPECT_EQ(res.size(), 0);
    
    res = b1 ^ b2;
    EXPECT_EQ(res.size(), 0);
}

TEST(BitArrayTest, InvalidIndexAccess) {
    BitArray b(10);
    
    // Out of range access
    EXPECT_THROW(b[-1], std::out_of_range);
    EXPECT_THROW(b[10], std::out_of_range);
    EXPECT_THROW(b[100], std::out_of_range);
    
    // Invalid set/reset
    EXPECT_THROW(b.set(-1), std::out_of_range);
    EXPECT_THROW(b.set(10), std::out_of_range);
    EXPECT_THROW(b.reset(-1), std::out_of_range);
    EXPECT_THROW(b.reset(10), std::out_of_range);
}

TEST(BitArrayTest, InvalidArgumentConstructor) {
    EXPECT_THROW(BitArray(-1), std::invalid_argument);
    EXPECT_THROW(BitArray(-10), std::invalid_argument);
}

TEST(BitArrayTest, InvalidArgumentResize) {
    BitArray b(10);
    EXPECT_THROW(b.resize(-1), std::invalid_argument);
    EXPECT_THROW(b.resize(-5), std::invalid_argument);
}

TEST(BitArrayTest, InvalidArgumentShift) {
    BitArray b(10);
    EXPECT_THROW(b <<= -1, std::invalid_argument);
    EXPECT_THROW(b >>= -1, std::invalid_argument);
    EXPECT_THROW(b << -1, std::invalid_argument);
    EXPECT_THROW(b >> -1, std::invalid_argument);
}

TEST(BitArrayTest, ResizeEdgeCases) {
    BitArray b(10);
    b.set(5);
    
    // Resize to same size
    b.resize(10);
    EXPECT_EQ(b.size(), 10);
    EXPECT_TRUE(b[5]);
    
    // Resize to smaller size
    b.resize(3);
    EXPECT_EQ(b.size(), 3);
    EXPECT_THROW(b[5], std::out_of_range);
    
    // Resize empty array
    BitArray empty;
    empty.resize(5, true);
    EXPECT_EQ(empty.size(), 5);
    EXPECT_TRUE(empty[4]);
    
    // Resize to 0
    empty.resize(0);
    EXPECT_EQ(empty.size(), 0);
    EXPECT_TRUE(empty.empty());
}

TEST(BitArrayTest, PushBackEdgeCases) {
    BitArray b;
    
    // Push back many elements (test block boundary crossing)
    for (int i = 0; i < 100; ++i) {
        b.push_back(i % 2 == 0);
    }
    EXPECT_EQ(b.size(), 100);
    EXPECT_TRUE(b[0]);
    EXPECT_FALSE(b[1]);
    EXPECT_TRUE(b[98]);
    EXPECT_FALSE(b[99]);
    
    // Push back to empty array
    BitArray empty;
    empty.push_back(true);
    EXPECT_EQ(empty.size(), 1);
    EXPECT_TRUE(empty[0]);
}

TEST(BitArrayTest, ShiftEdgeCases) {
    BitArray b(10);
    b.set(0);
    b.set(5);
    b.set(9);
    
    // Shift by 0
    BitArray b_copy = b;
    b_copy <<= 0;
    EXPECT_TRUE(b_copy == b);
    
    b_copy = b;
    b_copy >>= 0;
    EXPECT_TRUE(b_copy == b);
    
    // Shift by size (should clear)
    b_copy = b;
    b_copy <<= 10;
    EXPECT_TRUE(b_copy.none());
    
    b_copy = b;
    b_copy >>= 10;
    EXPECT_TRUE(b_copy.none());
    
    // Shift by more than size
    b_copy = b;
    b_copy <<= 20;
    EXPECT_TRUE(b_copy.none());
    
    b_copy = b;
    b_copy >>= 20;
    EXPECT_TRUE(b_copy.none());
}

TEST(BitArrayTest, ShiftBlockBoundary) {
    // Test shifting across block boundaries
    const int bits_per_block = sizeof(unsigned long) * 8;
    BitArray b(bits_per_block * 2 + 10);
    
    // Set bits at block boundaries
    b.set(0);
    b.set(bits_per_block - 1);
    b.set(bits_per_block);
    b.set(bits_per_block + 1);
    b.set(bits_per_block * 2);
    
    // Left shift by bits_per_block (exactly one block)
    BitArray b_copy = b;
    b_copy <<= bits_per_block;
    // After shift: bit at position bits_per_block moves to 2*bits_per_block
    EXPECT_FALSE(b_copy[0]); // Original bit at 0 is shifted out
    EXPECT_FALSE(b_copy[bits_per_block - 1]); // Original bit at bits_per_block-1 is shifted out
    EXPECT_TRUE(b_copy[bits_per_block * 2]); // Bit from position bits_per_block moved here
    EXPECT_TRUE(b_copy[bits_per_block * 2 + 1]); // Bit from position bits_per_block+1 moved here
    
    // Right shift by bits_per_block
    b_copy = b;
    b_copy >>= bits_per_block;
    // After shift: bit at position bits_per_block moves to 0
    EXPECT_TRUE(b_copy[0]); // Bit from position bits_per_block moved here
    EXPECT_TRUE(b_copy[1]); // Bit from position bits_per_block+1 moved here
    EXPECT_FALSE(b_copy[bits_per_block - 1]); // This should be empty after shift
}

TEST(BitArrayTest, CountEdgeCases) {
    // Empty array
    BitArray empty;
    EXPECT_EQ(empty.count(), 0);
    
    // All zeros
    BitArray zeros(100);
    EXPECT_EQ(zeros.count(), 0);
    
    // All ones
    BitArray ones(100);
    ones.set();
    EXPECT_EQ(ones.count(), 100);
    
    // Single bit
    BitArray single(1);
    EXPECT_EQ(single.count(), 0);
    single.set(0);
    EXPECT_EQ(single.count(), 1);
}

TEST(BitArrayTest, AnyNoneEdgeCases) {
    BitArray empty;
    EXPECT_FALSE(empty.any());
    EXPECT_TRUE(empty.none());
    
    BitArray zeros(100);
    EXPECT_FALSE(zeros.any());
    EXPECT_TRUE(zeros.none());
    
    BitArray ones(100);
    ones.set();
    EXPECT_TRUE(ones.any());
    EXPECT_FALSE(ones.none());
    
    BitArray single(1);
    EXPECT_FALSE(single.any());
    single.set(0);
    EXPECT_TRUE(single.any());
    EXPECT_FALSE(single.none());
}

TEST(BitArrayTest, CopyAndAssignment) {
    BitArray b1(10, 5);
    b1.set(7);
    
    // Copy constructor
    BitArray b2(b1);
    EXPECT_TRUE(b1 == b2);
    EXPECT_TRUE(b2[0]);
    EXPECT_TRUE(b2[7]);
    
    // Assignment operator
    BitArray b3;
    b3 = b1;
    EXPECT_TRUE(b1 == b3);
    
    // Self-assignment
    b3 = b3;
    EXPECT_TRUE(b1 == b3);
    
    // Assignment to different size
    BitArray b4(5);
    b4 = b1;
    EXPECT_TRUE(b1 == b4);
}

TEST(BitArrayTest, Swap) {
    BitArray b1(10, 5);
    BitArray b2(15, 7);
    b1.set(3);
    b2.set(10);
    
    BitArray b1_copy = b1;
    BitArray b2_copy = b2;
    
    b1.swap(b2);
    
    EXPECT_TRUE(b1 == b2_copy);
    EXPECT_TRUE(b2 == b1_copy);
    
    // Test external swap function
    swap(b1, b2);
    EXPECT_TRUE(b1 == b1_copy);
    EXPECT_TRUE(b2 == b2_copy);
}

TEST(BitArrayTest, InversionEdgeCases) {
    // Empty array
    BitArray empty;
    BitArray inverted_empty = ~empty;
    EXPECT_EQ(inverted_empty.size(), 0);
    
    // All zeros
    BitArray zeros(10);
    BitArray inverted_zeros = ~zeros;
    EXPECT_EQ(inverted_zeros.count(), 10);
    
    // All ones
    BitArray ones(10);
    ones.set();
    BitArray inverted_ones = ~ones;
    EXPECT_EQ(inverted_ones.count(), 0);
    
    // Double inversion should return original
    BitArray b(10, 5);
    BitArray double_inverted = ~(~b);
    EXPECT_TRUE(b == double_inverted);
}

TEST(BitArrayTest, BitReferenceAssignment) {
    BitArray arr1(10);
    BitArray arr2(10);
    
    arr1.set(0);
    arr1.set(5);
    arr2.set(3);
    arr2.set(7);
    
    // Test arr1[x] = arr2[y]
    arr1[2] = arr2[3];
    EXPECT_TRUE(arr1[2]); // Should be true (copied from arr2[3])
    
    arr1[4] = arr2[7];
    EXPECT_TRUE(arr1[4]); // Should be true (copied from arr2[7])
    
    arr1[6] = arr2[0];
    EXPECT_FALSE(arr1[6]); // Should be false (copied from arr2[0])
    
    // Test chained assignment
    arr1[8] = arr2[3];
    arr1[9] = arr1[8];
    EXPECT_TRUE(arr1[8]);
    EXPECT_TRUE(arr1[9]);
    
    // Test arr[x] = true/false
    arr1[1] = true;
    EXPECT_TRUE(arr1[1]);
    
    arr1[1] = false;
    EXPECT_FALSE(arr1[1]);
    
    // Test multiple assignments
    arr1[0] = arr2[7];
    arr1[5] = arr2[0];
    EXPECT_TRUE(arr1[0]);
    EXPECT_FALSE(arr1[5]);
}

TEST(BitArrayTest, BitReferenceChaining) {
    BitArray arr1(10);
    BitArray arr2(10);
    
    arr2.set(0);
    arr2.set(5);
    
    // Chain: arr1[x] = arr2[y] = arr2[z]
    arr1[1] = arr2[0];
    arr1[2] = arr1[1];
    EXPECT_TRUE(arr1[1]);
    EXPECT_TRUE(arr1[2]);
    
    // Multiple assignments from same source
    arr1[3] = arr2[5];
    arr1[4] = arr2[5];
    arr1[5] = arr2[5];
    EXPECT_TRUE(arr1[3]);
    EXPECT_TRUE(arr1[4]);
    EXPECT_TRUE(arr1[5]);
}

TEST(BitArrayTest, BitReferenceReading) {
    BitArray arr(10);
    arr.set(0);
    arr.set(5);
    arr.set(9);
    
    // Test reading through BitReference
    bool val0 = arr[0];
    bool val1 = arr[1];
    bool val5 = arr[5];
    bool val9 = arr[9];
    
    EXPECT_TRUE(val0);
    EXPECT_FALSE(val1);
    EXPECT_TRUE(val5);
    EXPECT_TRUE(val9);
    
    // Test reading after modification
    arr[1] = true;
    EXPECT_TRUE(arr[1]);
    
    arr[1] = false;
    EXPECT_FALSE(arr[1]);
}

TEST(BitArrayTest, SizeNotMultipleOfBlock) {
    // Test with size that's not a multiple of bits_per_block
    const int bits_per_block = sizeof(unsigned long) * 8;
    int size = bits_per_block * 2 + 5; // Not a multiple
    
    BitArray b(size);
    b.set();
    
    // All bits should be set
    EXPECT_EQ(b.count(), size);
    
    // Padding bits should be cleared
    BitArray b_copy = b;
    b_copy.resize(size + 10, false);
    EXPECT_EQ(b_copy.count(), size); // Only original bits should be set
    
    // Test inversion with non-multiple size
    BitArray inverted = ~b;
    EXPECT_EQ(inverted.count(), 0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}