#ifndef OCTREE_H
#define OCTREE_H

#include "OctreeNode.h"

const int32_t WORLD_SIZE = 16;
struct Octree {
    alignas(16) GPUOctreeNode nodes[MAX_NODE_COUNT];

    int createOctree(int *values);
};

#endif