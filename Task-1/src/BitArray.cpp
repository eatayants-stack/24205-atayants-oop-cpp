#include "BitArray.h"
#include <algorithm>
#include <cmath>
#include <sstream>
#if __cplusplus >= 202002L && defined(__cpp_lib_bitops)
#include <bit>
#endif

void BitArray::valid_argument(int arg) {
    if (arg < 0) {
        throw std::invalid_argument("Argument cannot be negative");
    }
}

void BitArray::valid_bit_index(int bit_pos, int array_size) {
    if (bit_pos < 0 || bit_pos >= array_size) {
        throw std::out_of_range("BitArray index out of range");
    }
}

void BitArray::check_size_compare(const BitArray& other) const {
    if (other.m_num_bits != m_num_bits) {
        throw std::invalid_argument("BitArray sizes must match for bitwise operations");
    }
}

void BitArray::trim_padding_bits() {
    if (m_num_bits == 0) return;
    int unused_bits = m_num_bits % bits_per_block;
    if (unused_bits > 0) {
        BlockType mask = (static_cast<BlockType>(1) << unused_bits) - 1;
        m_blocks.back() &= mask;
    }
}

int BitArray::get_block_index(int bit_index) {
    return bit_index / bits_per_block;
}

int BitArray::get_bit_offset(int bit_index) {
    return bit_index % bits_per_block;
}

BitArray::BitArray() : m_num_bits(0) {}

BitArray::~BitArray() = default;

BitArray::BitArray(int num_bits, unsigned long value) : m_num_bits(num_bits) {
    valid_argument(num_bits);
    
    int num_blocks = (num_bits + bits_per_block - 1) / bits_per_block;
    if (num_bits == 0) num_blocks = 0;
    
    m_blocks.resize(num_blocks, 0);
    
    if (num_blocks > 0) {
        m_blocks[0] = value;
    }
    trim_padding_bits();
}

BitArray::BitArray(const BitArray& b) = default;

BitArray::BitArray(BitArray&& b) noexcept 
    : m_blocks(std::move(b.m_blocks)), m_num_bits(b.m_num_bits) {
    b.m_num_bits = 0;
}

void BitArray::swap(BitArray& b) {
    std::swap(m_num_bits, b.m_num_bits);
    std::swap(m_blocks, b.m_blocks);
}

BitArray& BitArray::operator=(const BitArray& b) {
    if (this != &b) {
        BitArray temp(b);
        swap(temp);
    }
    return *this;
}

BitArray& BitArray::operator=(BitArray&& b) noexcept {
    if (this != &b) {
        m_blocks = std::move(b.m_blocks);
        m_num_bits = b.m_num_bits;
        b.m_num_bits = 0;
    }
    return *this;
}

void BitArray::resize(int num_bits, bool value) {
    valid_argument(num_bits);

    int old_num_bits = m_num_bits;
    int new_num_blocks = (num_bits + bits_per_block - 1) / bits_per_block;
    if (num_bits == 0) new_num_blocks = 0;

    m_blocks.resize(new_num_blocks, value ? ~static_cast<BlockType>(0) : 0);
    m_num_bits = num_bits;

    if (num_bits > old_num_bits && value) {
        if (old_num_bits > 0) {
            int old_block_idx = get_block_index(old_num_bits - 1);
            int unused_bits_in_block = old_num_bits % bits_per_block;
            if (unused_bits_in_block > 0) {
                BlockType mask = (static_cast<BlockType>(1) << unused_bits_in_block) - 1;
                m_blocks[old_block_idx] |= ~mask;
            }
        }
    }

    trim_padding_bits();
}

void BitArray::clear() {
    m_blocks.clear();
    m_num_bits = 0;
}

void BitArray::push_back(bool bit) {
    int bit_index = m_num_bits;
    int block_index = get_block_index(bit_index);
    int bit_offset = get_bit_offset(bit_index);
    
    if (static_cast<size_t>(block_index) >= m_blocks.size()) {
        m_blocks.push_back(0);
    }
    
    m_num_bits++;
    if (bit) {
        m_blocks[block_index] |= (static_cast<BlockType>(1) << bit_offset);
    }
}

BitArray& BitArray::operator&=(const BitArray& b) {
    check_size_compare(b);
    
    for (size_t i = 0; i < m_blocks.size(); ++i) {
        m_blocks[i] &= b.m_blocks[i];
    }
    return *this;
}

BitArray& BitArray::operator|=(const BitArray& b) {
    check_size_compare(b);

    for (size_t i = 0; i < m_blocks.size(); ++i) {
        m_blocks[i] |= b.m_blocks[i];
    }
    return *this;
}

BitArray& BitArray::operator^=(const BitArray& b) {
    check_size_compare(b);

    for (size_t i = 0; i < m_blocks.size(); ++i) {
        m_blocks[i] ^= b.m_blocks[i];
    }
    return *this;
}

BitArray& BitArray::operator<<=(int n) {
    valid_argument(n);
    if (n >= m_num_bits) {
        reset();
        return *this;
    }
    
    int block_shift = n / bits_per_block;
    int bit_offset = n % bits_per_block;

    if (block_shift > 0) {
        if (m_blocks.size() > 0) {
            for (size_t i = m_blocks.size(); i-- > 0; ) {
                if (i >= static_cast<size_t>(block_shift)) {
                    m_blocks[i] = m_blocks[i - block_shift];
                } else {
                    m_blocks[i] = 0;
                }
            }
        }
    }

    if (bit_offset > 0) {
        if (m_blocks.size() > 0) {
            for (size_t i = m_blocks.size(); i-- > 0; ) {
                m_blocks[i] <<= bit_offset;
                if (i > 0) {
                    m_blocks[i] |= (m_blocks[i-1] >> (bits_per_block - bit_offset));
                }
            }
        }
    }
    
    trim_padding_bits();
    return *this;
}

BitArray& BitArray::operator>>=(int n) {
    valid_argument(n); 
    if (n >= m_num_bits) {
        reset();
        return *this;
    }

    int block_shift = n / bits_per_block;
    int bit_offset = n % bits_per_block;

    if (block_shift > 0) {
        for (size_t i = 0; i < m_blocks.size(); ++i) {
            if (i + block_shift < m_blocks.size()) {
                m_blocks[i] = m_blocks[i + block_shift];
            } else {
                m_blocks[i] = 0;
            }
        }
    }

    if (bit_offset > 0) {
        for (size_t i = 0; i < m_blocks.size(); ++i) {
            m_blocks[i] >>= bit_offset;
            if (i + 1 < m_blocks.size()) {
                m_blocks[i] |= (m_blocks[i+1] << (bits_per_block - bit_offset));
            }
        }
    }
    
    trim_padding_bits();
    return *this;
}

BitArray BitArray::operator<<(int n) const {
    valid_argument(n);
    BitArray temp(*this);
    temp <<= n;
    return temp;
}

BitArray BitArray::operator>>(int n) const {
    valid_argument(n);
    BitArray temp(*this);
    temp >>= n;
    return temp;
}

BitArray& BitArray::set(int n, bool val) {
    valid_bit_index(n, m_num_bits);
    
    int blk = get_block_index(n);
    int off = get_bit_offset(n);

    if (val) {
        m_blocks[blk] |= (static_cast<BlockType>(1) << off);
    } else {
        m_blocks[blk] &= ~(static_cast<BlockType>(1) << off);
    }
    return *this;
}

BitArray& BitArray::set() {
    for (auto& block : m_blocks) {
        block = ~static_cast<BlockType>(0);
    }
    trim_padding_bits();
    return *this;
}

BitArray& BitArray::reset(int n) {
    return set(n, false);
}

BitArray& BitArray::reset() {
    for (auto& block : m_blocks) {
        block = 0;
    }
    return *this;
}

bool BitArray::any() const {
    for (const auto& block : m_blocks) {
        if (block != 0) return true;
    }
    return false;
}

bool BitArray::none() const {
    return !any();
}

BitArray BitArray::operator~() const {
    BitArray result(*this);
    for (auto& block : result.m_blocks) {
        block = ~block;
    }
    result.trim_padding_bits();
    return result;
}

int BitArray::count() const {
    int cnt = 0;
    for (const auto& block : m_blocks) {
        #if __cplusplus >= 202002L && defined(__cpp_lib_bitops)
            cnt += std::popcount(block);
        #else
            BlockType n = block;
            while (n > 0) {
                n &= (n - 1);
                cnt++;
            }
        #endif
    }
    return cnt;
}

bool BitArray::operator[](int i) const {
    valid_bit_index(i, m_num_bits);
    int blk = get_block_index(i);
    int off = get_bit_offset(i);
    return (m_blocks[blk] >> off) & 1;
}

BitArray::BitReference BitArray::operator[](int i) {
    valid_bit_index(i, m_num_bits);
    return BitReference(*this, i);
}

BitArray::BitReference::operator bool() const {
    return static_cast<const BitArray&>(bitArray).operator[](bitPos);
}

BitArray::BitReference& BitArray::BitReference::operator=(bool value) {
    bitArray.set(bitPos, value);
    return *this;
}

BitArray::BitReference& BitArray::BitReference::operator=(const BitReference& other) {
    bool value = static_cast<bool>(other);
    bitArray.set(bitPos, value);
    return *this;
}

int BitArray::size() const {
    return m_num_bits;
}

bool BitArray::empty() const {
    return m_num_bits == 0;
}

std::string BitArray::to_string() const {
    std::string s;
    s.reserve(m_num_bits);
    for (int i = 0; i < m_num_bits; ++i) {
        s.push_back((*this)[i] ? '1' : '0');
    }
    return s;
}

bool operator==(const BitArray & a, const BitArray & b) {
    if (a.m_num_bits != b.m_num_bits) return false;
    if (a.m_blocks.size() != b.m_blocks.size()) return false;
    
    for (size_t i = 0; i < a.m_blocks.size(); ++i) {
        if (a.m_blocks[i] != b.m_blocks[i]) return false;
    }
    return true;
}

bool operator!=(const BitArray & a, const BitArray & b) {
    return !(a == b);
}

void swap(BitArray& a, BitArray& b) {
    a.swap(b);
}

BitArray operator&(const BitArray& b1, const BitArray& b2) {
    BitArray temp(b1);
    temp &= b2;
    return temp;
}

BitArray operator|(const BitArray& b1, const BitArray& b2) {
    BitArray temp(b1);
    temp |= b2;
    return temp;
}

BitArray operator^(const BitArray& b1, const BitArray& b2) {
    BitArray temp(b1);
    temp ^= b2;
    return temp;
}