#include "PCH.h"

#include "SimpleGL/Utility/BVH.h"

namespace SGL::Utility {

    void BoundingBoxAABB::append(glm::vec3 const& v) noexcept {
        min = glm::min(min, v);
        max = glm::max(max, v);
    }

    void BoundingBoxAABB::append(BoundingBoxAABB const& b) noexcept {
        min = glm::min(min, b.min);
        max = glm::max(max, b.max);
    }

    glm::vec3 BoundingBoxAABB::getCenter() const noexcept {
        return (max + min) * 0.5f;
    }

    float BoundingBoxAABB::getArea() const noexcept {
        glm::vec3 d = max - min;
        return d.x * d.y + d.y * d.z + d.z * d.x;
    }

    float BoundingBoxAABB::getVolumn() const noexcept {
        glm::vec3 d = max - min;
        return d.x * d.y * d.z;
    }

    void BoundingBoxAABB::reset() noexcept {
        min = glm::vec3( std::numeric_limits<float>::max());
        max = glm::vec3(-std::numeric_limits<float>::max());
    }

    static void updateNodeBound(std::vector<BVHPrimitive*> const& prims, std::vector<BVHNode>& nodes, uint32_t nodeIdx) noexcept {
        auto& node = nodes[nodeIdx];
        node.bound.reset();
        for (uint32_t i = 0; i < node.primCount; ++i) {
            node.bound.append(prims[i]->getBound());
        }
    }

    static float evaluateSAH(std::vector<BVHPrimitive*> const& prims, std::vector<BVHNode> const& nodes, uint32_t nodeIdx, std::vector<glm::vec3>& centers, int axis, float pivot) noexcept {
        auto const& node = nodes[nodeIdx];
        BoundingBoxAABB leftBox, rightBox;
        int leftCount = 0;
        int rightCount = 0;
        for (uint32_t i = 0; i < node.primCount; ++i) {
            if (centers[node.firstPrim + i][axis] < pivot) {
                ++leftCount;
                leftBox.append(prims[node.firstPrim + i]->getBound());
            }
            else {
                ++rightCount;
                rightBox.append(prims[node.firstPrim + i]->getBound());
            }
        }
        float cost = leftCount * leftBox.getArea() + rightCount * rightBox.getArea();
        return cost > 0 ? cost : std::numeric_limits<float>::max();
    }

    static void subdivideNode(std::vector<BVHPrimitive*>& prims, std::vector<BVHNode>& nodes, uint32_t nodeIdx, std::vector<glm::vec3>& centers) noexcept {
        auto& node = nodes[nodeIdx];
        if (node.primCount <= 2) return;

#if defined(SGL_UTILITY_BVH_SPLIT_NAIVE)
        glm::vec3 extent = node.bound.max - node.bound.min;
        int axis = 0;
        if (extent.y > extent.x) axis = 1;
        if (extent.z > extent.y) axis = 2;

        float splitPivot = node.bound.min[axis] + extent[axis] * 0.5f;
#else
        int bestAxis = -1;
        float bestPivot = 0.0f;
        float bestSAHCost = std::numeric_limits<float>::max();

        for (int a = 0; a < 3; ++a) {
            for (uint32_t i = 0; i < node.primCount; ++i) {
                float pivot = centers[i + node.firstPrim][a];
                float cost = evaluateSAH(prims, nodes, nodeIdx, centers, a, pivot);
                if (cost < bestSAHCost) {
                    bestPivot = pivot;
                    bestAxis = a;
                    bestSAHCost = cost;
                }
            }
        }

        float splitPivot = bestPivot;
        int axis = bestAxis;
#endif

        uint32_t i = node.firstPrim;
        uint32_t j = i + node.primCount - 1;
        while (i <= j) {
            if (centers[i][axis] < splitPivot) {
                ++i;
            }
            else {
                std::swap(prims[i], prims[j]);
                std::swap(centers[i], centers[j]);
                --j;
            }
        }

        uint32_t leftCount = i - node.firstPrim;
        if (leftCount == 0 || leftCount == node.primCount) return;

        BVHNode left, right;

        left.firstPrim = node.firstPrim;
        left.primCount = leftCount;
        left.left = left.right = 0;
        node.left = nodes.size();
        nodes.push_back(left);

        right.firstPrim = i;
        right.primCount = node.primCount - leftCount;
        right.left = right.right = 0;
        node.right = nodes.size();
        nodes.push_back(right);

        node.primCount = 0;
        updateNodeBound(prims, nodes, node.left);
        updateNodeBound(prims, nodes, node.right);

        subdivideNode(prims, nodes, node.left, centers);
        subdivideNode(prims, nodes, node.right, centers);
    }

    std::unique_ptr<BVH> BVH::build(std::vector<BVHPrimitive*> const& prims) noexcept {
        auto bvh = std::make_unique<BVH>();
        bvh->prims = prims;
        bvh->nodes.reserve(prims.size() * 2 - 1);

        std::vector<glm::vec3> centers(prims.size());
        for (uint32_t i = 0; i < prims.size(); ++i) {
            centers[i] = prims[i]->getBound().getCenter();
        }

        BVHNode root;
        root.left = root.right = 0;
        root.firstPrim = 0;
        root.primCount = static_cast<uint32_t>(prims.size());
        bvh->nodes.push_back(root);

        updateNodeBound(bvh->prims, bvh->nodes, 0);
        subdivideNode(bvh->prims, bvh->nodes, 0, centers);

        return bvh;
    }

}
