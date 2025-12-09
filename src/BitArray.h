#ifndef INC_24205_ATAYANTS_OOP_CPP_BITARRAY_H
#define INC_24205_ATAYANTS_OOP_CPP_BITARRAY_H

#include <vector>
#include <string>
#include <stdexcept>
#include <climits>

class BitArray {
private:
    class BitReference {
    private:
        BitArray& bitArray;
        int bitPos;
    public:
        BitReference(BitArray& ba, int pos) : bitArray(ba), bitPos(pos) {}
        
        operator bool() const;
        BitReference& operator=(bool value);
        BitReference& operator=(const BitReference& other);
    };

    using BlockType = unsigned long;
    static constexpr int bits_per_block = sizeof(BlockType) * CHAR_BIT;

    std::vector<BlockType> m_blocks;
    int m_num_bits;

    void trim_padding_bits();
    static int get_block_index(int bit_index);
    static int get_bit_offset(int bit_index);
    static void valid_argument(int arg);
    static void valid_bit_index(int bit_pos, int array_size);
    void check_size_compare(const BitArray& other) const;

    friend bool operator==(const BitArray& a, const BitArray& b);

public:
    BitArray();
    ~BitArray();

    explicit BitArray(int num_bits, unsigned long value = 0);
    BitArray(const BitArray& b);
    BitArray(BitArray&& b) noexcept;

    void swap(BitArray& b);
    BitArray& operator=(const BitArray& b);
    BitArray& operator=(BitArray&& b) noexcept;

    bool operator[](int i) const;
    BitReference operator[](int i);

    int size() const;
    bool empty() const;
    void resize(int num_bits, bool value = false);
    void clear();
    void push_back(bool bit);

    BitArray& set(int n, bool val = true);
    BitArray& set();
    BitArray& reset(int n);
    BitArray& reset();

    bool any() const;
    bool none() const;
    BitArray operator~() const;
    int count() const;

    BitArray& operator<<=(int n);
    BitArray& operator>>=(int n);
    BitArray operator<<(int n) const;
    BitArray operator>>(int n) const;

    std::string to_string() const;

    BitArray& operator&=(const BitArray& b);
    BitArray& operator|=(const BitArray& b);
    BitArray& operator^=(const BitArray& b);
};

bool operator==(const BitArray& a, const BitArray& b);
bool operator!=(const BitArray& a, const BitArray& b);

void swap(BitArray& a, BitArray& b);

BitArray operator&(const BitArray& b1, const BitArray& b2);
BitArray operator|(const BitArray& b1, const BitArray& b2);
BitArray operator^(const BitArray& b1, const BitArray& b2);

#endif
