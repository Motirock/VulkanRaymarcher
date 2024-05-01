#ifndef OCTREE_H
#define OCTREE_H

#include "OctreeNode.h"

struct Octree {
    alignas(16) GPUOctreeNode nodes[MAX_NODE_COUNT];
    alignas(16) const int WORLD_SIZE = 4;

    int createOctree(int *values) {
        int nodeSlotsUsed = 1;
        nodes[0] = GPUOctreeNode(-1, -1, false, 0, WORLD_SIZE, 0, WORLD_SIZE, 0, WORLD_SIZE, nodeSlotsUsed);
        nodes[0].subdivide(values, WORLD_SIZE, nodeSlotsUsed, nodes);
        std::cout << nodeSlotsUsed << std::endl;
        return nodeSlotsUsed;
    }
};

#endif