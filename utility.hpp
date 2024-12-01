#pragma once

#include <cstdint>

constexpr auto operator""_u64(unsigned long long v) -> std::uint64_t {
    return static_cast<std::uint64_t>(v);
}

constexpr auto operator""_u32(unsigned long long v) -> std::uint32_t {
    return static_cast<std::uint32_t>(v);
}

constexpr auto operator""_u16(unsigned long long v) -> std::uint16_t {
    return static_cast<std::uint16_t>(v);
}

constexpr auto operator""_u8(unsigned long long v) -> std::uint8_t {
    return static_cast<std::uint8_t>(v);
}

using u64 = std::uint64_t;
using u32 = std::uint32_t;
using u16 = std::uint16_t;
using u8 = std::uint8_t;

using i64 = std::int64_t;
using i32 = std::int32_t;
using i16 = std::int16_t;
using i8 = std::int8_t;