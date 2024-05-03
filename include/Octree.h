#ifndef OCTREE_H
#define OCTREE_H

#include "OctreeNode.h"

struct Octree {
    alignas(16) GPUOctreeNode nodes[MAX_NODE_COUNT];
    alignas(16) const int32_t WORLD_SIZE = 16;

    int createOctree(int *values);
};

#endif