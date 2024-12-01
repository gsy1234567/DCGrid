#pragma once

#include "utility.hpp"
#include <concepts>
#include <array>
#include <numeric>

namespace {
    constexpr std::array<u32, 256> _get_bitShuffleTable(u32 off) {
        std::array<u32, 256> table = {0};
        for(auto i = 0_u32 ; i < 256_u32 ; ++i) {
            for(auto bit = 0_u32 ; bit < 8_u32 ; ++bit) {
                    table[i] |= ((i >> bit) & 1_u32) << (off * bit);
            }
        }
        return table;
    }

    static constexpr auto _bitShuffleTable2 = _get_bitShuffleTable(2);
    static constexpr auto _bitShuffleTable3 = _get_bitShuffleTable(3);

    template<std::unsigned_integral T>
    constexpr T _extend_2(u8 x) {
        static_assert(std::numeric_limits<u8>::digits * 2 <= std::numeric_limits<T>::digits);
        return static_cast<T>(_bitShuffleTable2[x]);
    }

    template<std::unsigned_integral T>
    constexpr T _extend_3(u8 x) {
        static_assert(std::numeric_limits<u8>::digits * 3 <= std::numeric_limits<T>::digits);
        return static_cast<T>(_bitShuffleTable3[x]);
    }
}

namespace gsy {
    template<std::unsigned_integral T>
    constexpr T mortonCode(T x, T y) {
        T ret = 0;
        for(auto i = 0_u64 ; i < std::numeric_limits<T>::digits ; i += std::numeric_limits<u8>::digits) {
            ret |= _extend_2<T>((x >> i) & std::numeric_limits<u8>::max()) << static_cast<T>(i);
            ret |= _extend_2<T>((y >> i) & std::numeric_limits<u8>::max()) << static_cast<T>(i+1);
        }
        return ret;
    }

    template<std::unsigned_integral T>
    constexpr T mortonCode(T x, T y, T z) {
        T ret = 0;
        for(auto i = 0_u64 ; i < std::numeric_limits<T>::digits ; i += std::numeric_limits<u8>::digits) {
            ret |= _extend_3<T>((x >> i) & std::numeric_limits<u8>::max()) << static_cast<T>(i);
            ret |= _extend_3<T>((y >> i) & std::numeric_limits<u8>::max()) << static_cast<T>(i+1);
            ret |= _extend_3<T>((z >> i) & std::numeric_limits<u8>::max()) << static_cast<T>(i+2);
        }
        return ret;
    }
}
