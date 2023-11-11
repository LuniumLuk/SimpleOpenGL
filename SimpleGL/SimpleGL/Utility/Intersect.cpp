#include "PCH.h"

#include "SimpleGL/Utility/Intersect.h"

namespace SGL::Utility {

    // Moller Trumbore intersection algorithm
    bool intersectTriangle(
        glm::vec3 const& rayOrigin,
        glm::vec3 const& rayDir,
        glm::vec3 const& v0,
        glm::vec3 const& v1,
        glm::vec3 const& v2,
        float* t) noexcept {
        float const eps = 1e-6f;

        auto const e1 = v1 - v0;
        auto const e2 = v2 - v0;
        auto const h = glm::cross(rayDir, e2);
        auto const a = glm::dot(e1, h);

        if (a > -eps && a < eps) {
            // parallel
            return false;
        }
        auto const f = 1.0f / a;
        auto const s = rayOrigin - v0;
        auto const u = f * glm::dot(s, h);
        if (u < -eps || u > 1 + eps) {
            // u out of bounds
            return false;
        }
        auto const q = glm::cross(s, e1);
        auto const v = f * glm::dot(rayDir, q);
        if (v < -eps || u + v > 1 + eps) {
            // v out of bounds
            return false;
        }
        auto const tt = f * glm::dot(e2, q);
        if (t) {
            if (tt > eps && tt < *t) {
                *t = tt;
                return true;
            }
        }
        else {
            return tt > eps;
        }
        return false;
    }

    bool intersectAABB(
        glm::vec3 const& rayOrigin,
        glm::vec3 const& rayDir,
        BoundingBoxAABB const& box,
        float* t) noexcept {
        const float tx1 = (box.min.x - rayOrigin.x) / rayDir.x;
        const float tx2 = (box.max.x - rayOrigin.x) / rayDir.x;
        const float ty1 = (box.min.y - rayOrigin.y) / rayDir.y;
        const float ty2 = (box.max.y - rayOrigin.y) / rayDir.y;
        const float tz1 = (box.min.z - rayOrigin.z) / rayDir.z;
        const float tz2 = (box.max.z - rayOrigin.z) / rayDir.z;
        float tmin = std::min(tx1, tx2);
        float tmax = std::max(tx1, tx2);
        tmin = std::max(tmin, std::min(ty1, ty2));
        tmax = std::min(tmax, std::max(ty1, ty2));
        tmin = std::max(tmin, std::min(tz1, tz2));
        tmax = std::min(tmax, std::max(tz1, tz2));
        if (tmin < tmax && tmax > 0) {
            if (t) {
                if (tmin < *t) {
                    *t = tmin;
                    return true;
                }
            }
            else {
                return true;
            }
        }
        return false;
    }

    bool intersectBVH(
        glm::vec3 const& rayOrigin,
        glm::vec3 const& rayDir,
        BVH const& bvh,
        float* t) noexcept {
        BVHNode const* node = &bvh.nodes[0];
        BVHNode const* stack[64] = {};
        uint32_t stackPtr = 0;
        bool hit = false;
        float tt = t ? *t : INFINITY;
        while (1) {
            if (node->isLeaf()) {
                for (size_t i = 0; i < node->primCount; ++i) {
                    if (bvh.prims[node->firstPrim + i]->intersect(rayOrigin, rayDir, &tt)) {
                        hit = true;
                    }
                }
                if (stackPtr == 0) {
                    break;
                }
                else {
                    node = stack[--stackPtr];
                }
                continue;
            }

            BVHNode const* child1 = &bvh.nodes[node->left];
            BVHNode const* child2 = &bvh.nodes[node->right];

            float t1 = INFINITY;
            float t2 = INFINITY;

            intersectAABB(rayOrigin, rayDir, child1->bound, &t1);
            intersectAABB(rayOrigin, rayDir, child2->bound, &t2);

            if (t1 > t2) {
                std::swap(t1, t2);
                std::swap(child1, child2);
            }

            if (t1 == INFINITY) {
                if (stackPtr == 0) {
                    break;
                }
                else {
                    node = stack[--stackPtr];
                }
            }
            else {
                node = child1;
                if (t2 != INFINITY) {
                    stack[stackPtr] = child2;
                    ++stackPtr;
                }
            }
        }
        return hit;
    }

}
