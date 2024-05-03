#ifndef OCTREE_NODE_H
#define OCTREE_NODE_H

const int MAX_NODE_COUNT = 5000;//__UINT16_MAX__;
const int MAX_VALUE = 2;

#include "VkUtils.h"

struct GPUOctreeNode {
    int32_t childrenIndex;
    int32_t value;
    int32_t homogenous;
    int32_t minX, maxX, minY, maxY, minZ, maxZ;
    alignas(16) glm::vec3 color = glm::vec3(1.0f, 0.0f, 1.0f);

    GPUOctreeNode();

    GPUOctreeNode(int childrenIndex, int value, bool homogenous, int minX, int maxX, int minY, int maxY, int minZ, int maxZ, int &nodeSlotsUsed);

    void checkIfHomogenous(int *values, const int &WORLD_SIZE);

    void subdivide(int *values, const int &WORLD_SIZE, int &nodeSlotsUsed, GPUOctreeNode *nodes);
};

#endif