#pragma once

#include <array>
#include <memory>

#include "AABB.h"
#include "HitTest.h"

class svoNode {
    AABB<float> m_bounds;
    float m_size;
    svoNode* m_parent;
    svoNode* m_root;

    svoNode(AABB<float> bounds, svoNode* parent) : m_bounds(bounds), m_parent(parent) { 
        //ensure parent has a pointer to root.
        assert(parent->m_root != nullptr);
        m_root = parent->m_root;
        Vector3f s = m_bounds.getSize();
        //Ensure the size yields a cubic volume.
        assert(s.x == s.y == s.y);
        m_size = s.x;
    }

    bool contains(AABB<float> bb) { return HitTest<float>::i3D_AABB_AABB(m_bounds, bb) == HitTest<float>::hitType::INTERSECT; }
    bool contains(Vector3f p) { return HitTest<float>::i3D_AABB_Point(m_bounds, p) != HitTest<float>::hitType::NONE; }


	virtual ~svoNode() = 0;
};

class svoBranch : public svoNode {
    std::array<std::unique_ptr<svoNode>, 8> children;

    ~svoBranch() final;
};

template <class T>
struct svoLeaf : public svoNode {
    std::array<T> data{};

    ~svoLeaf() final;
};



template <class T>
class svo {
    void insert(Vector3f pos, T* data) {
        ensureSpace(pos);
        auto octreeNodeIndex = indexOf(pos);
        insert(octreeNodeIndex, data);
    }


    T* operator[](Vector3f pos) {
        ensureSpace(pos);
        auto octreeNodeIndex = indexOf(pos);
        return findOrCreate(octreeNodeIndex);
    }

    T* at(Vector3f pos) {
        u32 lim = boundsTest(pos);
        ALWAYS_ASSERT(lim == 0);
        auto octreeNodeIndex = indexOf(pos);
        auto* result = find(octreeNodeIndex);
        ALWAYS_ASSERT(result != nullptr);
        return *result;
    }

    const T* at(Vector3f pos) const {
        u32 lim = boundsTest(pos);
        ALWAYS_ASSERT(lim == 0);
        auto octreeNodeIndex = indexOf(pos);
        auto* result = find(octreeNodeIndex);
        ALWAYS_ASSERT(result != nullptr);
        return *result;
    }

private:
   
    T* findOrCreate(int octreeNodeIndex) {
        svoNode* node = root.get();
        for (size_t s = depth * 3; s != size_t(-3); s -= 3) {
            u32 octDigit = (octreeNodeIndex >> s) & 0b111;
            if (s != 0) {
                auto* branch = downcast<SVOBranch*>(node);
                if (branch->children[octDigit] != nullptr) {
                    node = branch->children[octDigit].get();
                }
                else {
                    auto* child = s == 3 ? static_cast<SVONode*>(new SVOLeaf)
                        : static_cast<SVONode*>(new SVOBranch);
                    node = child;
                    branch->children[octDigit] = std::unique_ptr<SVONode>{ child };
                }
            }
            else {
                auto* leaf = downcast<SVOLeaf*>(node);
                return leaf->data[octDigit];
            }
        }
        DEBUG_ASSERT_UNREACHABLE();
    }

    rgb32_t* find(u64 octreeNodeIndex) const {
        SVONode* node = root.get();
        for (size_t s = depth * 3; s != size_t(-3); s -= 3) {
            u32 octDigit = (octreeNodeIndex >> s) & 0b111;
            if (s != 0) {
                auto* branch = downcast<SVOBranch*>(node);
                if (branch->children[octDigit] == nullptr) {
                    return nullptr;
                }
                else {
                    node = branch->children[octDigit].get();
                }
            }
            else {
                auto* leaf = downcast<SVOLeaf*>(node);
                return &leaf->data[octDigit];
            }
        }
        DEBUG_ASSERT_UNREACHABLE();
    }

    u64 indexOf(Vec3i32 pos) const {
        Vec3u32 uPos = static_vec_cast<u32>(pos - minIncl());
        return ileave3(uPos[0], uPos[1], uPos[2]);
    }

    void ensureSpace(Vec3i32 pos) {
        if (u32 lim = boundsTest(pos); lim != 0) {
            grow(lim);
        }
    }

    void insert(u64 octreeNodeIndex, u32 color) {
        findOrCreate(octreeNodeIndex) = color;
    }

    void grow(u32 lim) {
        for (size_t size = 1 << depth; size <= lim; depth <<= 1, size = 1 << depth) {
            growOnce();
        }
    }

    void growOnce() {
        for (size_t i = 0; i < 8; ++i) {
            if (root->children[i] == nullptr) {
                continue;
            }
            auto parent = std::make_unique<SVOBranch>();
            parent->children[~i & 0b111] = std::move(root->children[i]);
            root->children[i] = std::move(parent);
        }
    }

    /**
     * Tests whether the input vector lies within this octree. The result is an unsigned integer which indicates how
     * much the octree has to be enlarged to fit the vector.
     * The dimensions of the octree in all directions need to be > this integer.
     * @param v the input position
     * @return 0 if the test passes or a maximum-like coordinate if the test fails
     */
    uint32_t boundsTest(Vec3i32 v) const
    {
        constexpr auto absForBoundsTest = [](int32_t x) -> uint32_t {
            return static_cast<uint32_t>(x < 0 ? -x - 1 : x);
        };
        static_assert (absForBoundsTest(-5) == 4);
        u32 max = absForBoundsTest(v[0]) | absForBoundsTest(v[1]) | absForBoundsTest(v[2]);
        return max >= (1u << depth) ? max : 0;
    }
};

