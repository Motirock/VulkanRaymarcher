#ifndef OCTREE_H
#define OCTREE_H

#include "OctreeNode.h"

const int32_t WORLD_SIZE = 64;
struct Octree {
    alignas(16) OctreeNode nodes[MAX_NODE_COUNT];

    int createOctree(int *values);
};

struct GPUOctree {
    alignas(16) GPUOctreeNode nodes[MAX_NODE_COUNT];

    GPUOctree(Octree &octree);
};

#endif