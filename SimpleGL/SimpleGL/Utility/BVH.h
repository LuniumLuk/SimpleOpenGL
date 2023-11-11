#pragma once

#include "glm/glm.hpp"

namespace SGL::Utility {

    struct BoundingBoxAABB {

        glm::vec3 min{  std::numeric_limits<float>::max() };
        glm::vec3 max{ -std::numeric_limits<float>::max() };

        void append(glm::vec3 const& v) noexcept;
        void append(BoundingBoxAABB const& b) noexcept;

        glm::vec3 getCenter() const noexcept;
        float getArea() const noexcept;
        float getVolumn() const noexcept;

        void reset() noexcept;

    };

    struct BVHPrimitive {

        virtual BoundingBoxAABB getBound() const noexcept = 0;

        /*
        *   Intersect with primitive
        *       if t is provided, the distance from ray origin to intersection point will be stored in t
        */
        virtual bool intersect(glm::vec3 const& rayOrigin, glm::vec3 const& rayDir, float* t = nullptr) const noexcept = 0;

    };

    struct BVHNode {

        BoundingBoxAABB bound;
        uint32_t left, right;
        uint32_t firstPrim, primCount;

        bool isLeaf() const noexcept { return primCount > 0; }

    };

    struct BVH {

        std::vector<BVHNode> nodes;

        /*
        *   Primities in BVH are inheritant of BVHPrimitive
        */
        std::vector<BVHPrimitive*> prims;

        BVH() = default;
        ~BVH() = default;

        BVH(BVH const&) = delete;
        BVH(BVH&& other) = delete;

        BVH& operator=(BVH const&) = delete;
        BVH& operator=(BVH&& other) = delete;

        static std::unique_ptr<BVH> build(std::vector<BVHPrimitive*> const& prims) noexcept;

    };

}
