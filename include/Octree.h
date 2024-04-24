#ifndef OCTREE_H
#define OCTREE_H

#include "OctreeNode.h"

struct Octree {
    alignas(16) GPUOctreeNode nodes[MAX_NODE_COUNT];
};

#endif