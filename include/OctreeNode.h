#ifndef OCTREE_NODE_H
#define OCTREE_NODE_H

const int MAX_NODE_COUNT = 20000;//__UINT16_MAX__;
const int MAX_VALUE = 2;

const int FLAG_NO_FLAG =       0b00000000000000000000000000000000;
const int FLAG_HOMOGENEOUS =   0b00000000000000000000000000000001;

#include "VkUtils.h"

struct OctreeNode {
    uint32_t childrenIndex;
    int32_t value;
    int flags;
    int32_t minX, maxX, minY, maxY, minZ, maxZ;
    alignas(16) glm::vec3 color = glm::vec3(1.0f, 0.0f, 1.0f);

    OctreeNode();
    OctreeNode(int childrenIndex, int value, bool homogeneous, int minX, int maxX, int minY, int maxY, int minZ, int maxZ, int &nodeSlotsUsed);

    bool isHomogeneous(int *values, const int &WORLD_SIZE);

    void subdivide(int *values, const int &WORLD_SIZE, int &nodeSlotsUsed, OctreeNode *nodes);
};

enum GPUNodeStatus {
    NODE_STATUS_EMPTY = 0b0,
    NODE_STATUS_SOLID = 0b1,
};
struct GPUOctreeNode {
    uint32_t data;

    GPUOctreeNode();
    GPUOctreeNode(uint32_t data);

    void print();
};

#endif