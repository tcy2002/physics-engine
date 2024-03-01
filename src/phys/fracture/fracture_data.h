#pragma once

#include <cstdint>
#include "phys/phys_general.h"
#include "fracture_utils.h"

#define FRAC_UINT_INIT(size) (size), uint32_hash_func, uint32_equal
#define FRAC_VEC_INIT(size) (size), vector3_hash_func, vector3_equal
#define FRAC_VERT_INIT(size) (size), vertex::hash_func, vertex::equal
#define FRAC_TRI_INIT(size) (size), triangle::hash_func, triangle::equal
#define FRAC_FACE_INIT(size) (size), polygon::hash_func, polygon::equal

namespace pe_phys_fracture {

    bool uint32_equal(uint32_t a, uint32_t b) {
        return a == b;
    }

    uint32_t uint32_hash_func(uint32_t v) {
        return v;
    }

    bool vector3_equal(const pe::Vector3& a, const pe::Vector3& b) {
        return approx_equal(a, b);
    }

    uint32_t vector3_hash_func(const pe::Vector3& v) {
        auto x = (uint32_t)std::round(v.x / PE_EPS);
        auto y = (uint32_t)std::round(v.y / PE_EPS);
        auto z = (uint32_t)std::round(v.z / PE_EPS);
        return (uint32_t)(x * 73856093 ^ y * 19349663 ^ z * 83492791);
    }

    struct vertex {
        pe::Vector3 pos;
        pe::Vector3 nor;
        vertex() = default;
        vertex(const pe::Vector3& p, const pe::Vector3& n) : pos(p), nor(n) {}
        static bool equal(const vertex& a, const vertex& b) {
            return approx_equal(a.pos, b.pos) && approx_equal(a.nor, b.nor);
        }
        static uint32_t hash_func(const vertex& v) {
            return vector3_hash_func(v.pos) ^ vector3_hash_func(v.nor) * 73856093;
        }
    };

    struct triangle {
        uint32_t vert_ids[3]{};
        triangle() = default;
        triangle(uint32_t v1, uint32_t v2, uint32_t v3) {
            vert_ids[0] = v1;
            vert_ids[1] = v2;
            vert_ids[2] = v3;
        }
        static bool equal(const triangle& a, const triangle& b) {
            return (a.vert_ids[0] == b.vert_ids[0] || a.vert_ids[0] == b.vert_ids[1] || a.vert_ids[0] == b.vert_ids[2]) &&
                   (a.vert_ids[1] == b.vert_ids[0] || a.vert_ids[1] == b.vert_ids[1] || a.vert_ids[1] == b.vert_ids[2]) &&
                   (a.vert_ids[2] == b.vert_ids[0] || a.vert_ids[2] == b.vert_ids[1] || a.vert_ids[2] == b.vert_ids[2]);
        }
        static uint32_t hash_func(const triangle& t) {
            auto id1 = t.vert_ids[0], id2 = t.vert_ids[1], id3 = t.vert_ids[2];
            sort3(id1, id2, id3);
            return id1 * 73856093 ^ id2 * 19349663 ^ id3 * 83492791;
        }
    };

    struct polygon {
        std::vector<uint32_t> vert_ids;
        pe::Vector3 nor;
        polygon() = default;
        explicit polygon(const pe::Vector3& n) : nor(n) {}
        void add_vert(uint32_t v, uint32_t idx = UINT32_MAX) {
            if (idx == -1) {
                vert_ids.push_back(v);
            }
            else {
                vert_ids.insert(vert_ids.begin() + idx, v);
            }
        }
        bool remove_vert(uint32_t idx) {
            if (idx >= vert_ids.size()) {
                return false;
            }
            vert_ids.erase(vert_ids.begin() + idx);
            return true;
        }
        static bool equal(const polygon& a, const polygon& b) {
            return approx_equal(a.nor, b.nor);
        }
        static bool hash_func(const polygon& p) {
            return vector3_hash_func(p.nor);
        }
    };

    struct tetrahedron {
        uint32_t tri_ids[4]{};
        pe::Vector3 center;
        pe::Real radius;
        tetrahedron() : radius(0) {}
        tetrahedron(uint32_t t1, uint32_t t2, uint32_t t3, uint32_t t4,
                    const pe::Vector3& c, pe::Real r) : center(c), radius(r) {
            tri_ids[0] = t1;
            tri_ids[1] = t2;
            tri_ids[2] = t3;
            tri_ids[3] = t4;
        }
    };

} // namespace pe_phys_fracture