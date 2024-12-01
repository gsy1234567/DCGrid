#pragma once

#include <eigen3/Eigen/Dense>
#include <cassert>
#include <unordered_map>
#include <bit>
#include <numeric>
#include <optional>
#include "utility.hpp"
#include "mortonCode.hpp"

namespace gsy {
    template<std::unsigned_integral index_t, std::floating_point float_t, std::uint64_t Dim>
    struct DenseGrid {
        static_assert(Dim >= 2 && Dim <= 3, "Dim must be 2 or 3");
        float_t delta;
        Eigen::Vector<index_t, Dim> resolution;
        Eigen::Vector<float_t, Dim> origin;

        DenseGrid() = default;
        DenseGrid(float_t delta, Eigen::Vector<index_t, Dim> resolution, Eigen::Vector<float_t, Dim> origin) : 
            delta(delta), resolution(resolution), origin(origin) {
            assert(delta > 0);
        }

        const auto& get_min() const { return origin; }
        auto get_delta() const { return resolution. template cast<float_t>() * delta; }
        auto get_max() const { return origin + get_delta(); }

        auto get_index_rd(const Eigen::Vector<float_t, Dim>& point) const {
            return ((point - origin) / delta)
                .unaryExpr([](float_t x)->float_t {return std::floor(x);})
                .template cast<index_t>();
        }

        auto get_index_ru(const Eigen::Vector<float_t, Dim>& point) const {
            return ((point - origin) / delta)
            .unaryExpr([](float_t x)->float_t {return std::ceil(x);})
            .template cast<index_t>();
        }

        auto get_index_rn(const Eigen::Vector<float_t, Dim>& point) const {
            return ((point - origin) / delta)
            .unaryExpr([](float_t x)->float_t {return std::round(x);})
            .template cast<index_t>();
        }

        bool valid(const Eigen::Vector<index_t, Dim>& idx) const {
            return (0 <= idx.array()).all() && (idx.array() < resolution.array()).all();
        }

        auto get_x_uint() const { 
            if constexpr (Dim == 2) {
                return Eigen::Vector<float_t, Dim>{delta, 0};
            } else {
                return Eigen::Vector<float_t, Dim>{delta, 0, 0};
            }
        }
        auto get_y_uint() const { 
            if constexpr (Dim == 2) {
                return Eigen::Vector<float_t, Dim>{0, delta};
            } else {
                return Eigen::Vector<float_t, Dim>{0, delta, 0};
            }
        }

        auto get_z_unit() const { 
            static_assert(Dim == 3, "Dim must be 3");
            return Eigen::Vector<float_t, Dim>{0, 0, delta};
        }


    };

    template<std::unsigned_integral index_t, std::floating_point float_t, std::uint64_t Dim>
    class SpaceHash {
        private:
            DenseGrid<index_t, float_t, Dim> m_grid;
            std::unordered_map<index_t, index_t> m_map;


            index_t get_hash_key(const Eigen::Vector<index_t, Dim>& idx) const {
                assert(m_grid.valid(idx));
                if constexpr (Dim == 2) {
                    return mortonCode(idx[0], idx[1]);
                } else if constexpr (Dim == 3) {
                    return mortonCode(idx[0], idx[1], idx[2]);
                } else {
                    static_assert(0, "Dim must be 2 or 3");
                    return 0;
                }
            }
        public:
            SpaceHash(DenseGrid<index_t, float_t, Dim> grid) : m_grid(std::move(grid)) {
                auto validBits = 0_u64;
                for(auto i = 0_u64 ; i != Dim ; ++i) {
                    assert(grid.resolution[i] != 0_u64);
                    validBits += std::numeric_limits<index_t>::digits - std::countl_zero(grid.resolution[i] - 1);
                }
                assert(std::numeric_limits<index_t>::digits >= validBits);
            }

            #define insert(op) \
                void insert_##op(const Eigen::Vector<float_t, Dim>& point, index_t idx) { \
                    auto pIdx = m_grid.get_index_##op(point); \
                    assert(m_grid.valid(pIdx)); \
                    m_map[get_hash_key(pIdx)] = idx; \
                }

            #define erase(op) \
                void erase_##op(const Eigen::Vector<float_t, Dim>& point) { \
                    auto pIdx = m_grid.get_index_##op(point); \
                    assert(m_grid.valid(pIdx)); \
                    m_map.erase(get_hash_key(point)); \
                }

            #define find(op) \
                std::optional<index_t> find_##op(const Eigen::Vector<float_t, Dim>& point) const { \
                    auto pIdx = m_grid.get_index_##op(point); \
                    assert(m_grid.valid(pIdx)); \
                    auto iter = m_map.find(get_hash_key(pIdx)); \
                    return iter == m_map.end() ? std::nullopt : std::make_optional(iter->second); \
                }

            #define find_left(op) \
                std::optional<index_t> find_left_##op(const Eigen::Vector<float_t, Dim>& point) const { \
                    return find_##op(point - m_grid.get_x_uint()); \
                }
            
            #define find_right(op) \
                std::optional<index_t> find_right_##op(const Eigen::Vector<float_t, Dim>& point) const { \
                    return find_##op(point + m_grid.get_x_uint()); \
                }
            
            #define find_front(op) \
                std::optional<index_t> find_back_##op(const Eigen::Vector<float_t, Dim>& point) const { \
                    return find_##op(point - m_grid.get_y_uint()); \
                }
            
            #define find_back(op) \
                std::optional<index_t> find_front_##op(const Eigen::Vector<float_t, Dim>& point) const { \
                    return find_##op(point + m_grid.get_y_uint()); \
                }

            #define find_top(op) \
                std::optional<index_t> find_top_##op(const Eigen::Vector<float_t, Dim>& point) const { \
                    return find_##op(point + m_grid.get_z_unit()); \
                }

            #define find_bottom(op) \
                std::optional<index_t> find_bottom_##op(const Eigen::Vector<float_t, Dim>& point) const { \
                    return find_##op(point - m_grid.get_z_unit()); \
                }

            insert(rd);
            insert(rn);
            insert(ru);

            erase(rd);
            erase(rn);
            erase(ru);

            find(rd);
            find(rn);
            find(ru);

            find_left(rd);
            find_left(rn);
            find_left(ru);

            find_right(rd);
            find_right(rn);
            find_right(ru);

            find_front(rd);
            find_front(rn);
            find_front(ru);

            find_back(rd);
            find_back(rn);
            find_back(ru);

            find_top(rd);
            find_top(rn);
            find_top(ru);

            find_bottom(rd);
            find_bottom(rn);
            find_bottom(ru);

            #undef find_bottom
            #undef find_top
            #undef find_back
            #undef find_front
            #undef find_right
            #undef find_left
    };
}
