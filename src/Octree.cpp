#include "Octree.h"

int Octree::createOctree(int *values) {
    int nodeSlotsUsed = 1;
    nodes[0] = GPUOctreeNode(-1, -1, false, 0, WORLD_SIZE, 0, WORLD_SIZE, 0, WORLD_SIZE, nodeSlotsUsed);
    nodes[0].subdivide(values, WORLD_SIZE, nodeSlotsUsed, nodes);
    return nodeSlotsUsed;
}