#pragma once

#include "SimpleGL/Utility/BVH.h"
#include "glm/glm.hpp"

namespace SGL::Utility {

    bool intersectTriangle(
        glm::vec3 const& rayOrigin,
        glm::vec3 const& rayDir,
        glm::vec3 const& v0,
        glm::vec3 const& v1,
        glm::vec3 const& v2,
        float* t = nullptr) noexcept;

    bool intersectAABB(
        glm::vec3 const& rayOrigin,
        glm::vec3 const& rayDir,
        BoundingBoxAABB const& box,
        float* t = nullptr) noexcept;

    bool intersectBVH(
        glm::vec3 const& rayOrigin,
        glm::vec3 const& rayDir,
        BVH const& bvh,
        float* t = nullptr) noexcept;

}
