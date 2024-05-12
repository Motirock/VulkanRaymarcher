#ifndef OCTREE_H
#define OCTREE_H

#include "OctreeNode.h"

const int32_t WORLD_SIZE = 64;
struct Octree {
    OctreeNode worldNode = OctreeNode(0, -1, false, 0, WORLD_SIZE, 0, WORLD_SIZE, 0, WORLD_SIZE);
    OctreeNode nodes[MAX_NODE_COUNT];

    int createOctree(int *values);
};

struct GPUOctree {
    GPUOctreeNode worldNode;
    GPUOctreeNode nodes[MAX_NODE_COUNT];

    GPUOctree(Octree &octree, const int &nodeSlotsUsed);
};

#endif