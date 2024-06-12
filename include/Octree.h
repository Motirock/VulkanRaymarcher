#ifndef OCTREE_H
#define OCTREE_H

#include "OctreeNode.h"

const int32_t CHUNK_SIZE = 64;
const uint32_t WORLD_CHUNK_SIZE_X = 10, WORLD_CHUNK_SIZE_Y = 10, WORLD_CHUNK_SIZE_Z = 10;
const glm::ivec3 WORLD_DIMENSIONS = glm::ivec3(WORLD_CHUNK_SIZE_X, WORLD_CHUNK_SIZE_Y, WORLD_CHUNK_SIZE_Z)*CHUNK_SIZE;

struct Octree {
    OctreeNode baseNode = OctreeNode(0, -1, false, 0, CHUNK_SIZE, 0, CHUNK_SIZE, 0, CHUNK_SIZE);
    OctreeNode *nodes;
    glm::ivec3 position;

    int createOctree(int *values, glm::ivec3 position);

    ~Octree();
};

struct GPUOctree {
    GPUOctreeNode baseNode;
    GPUOctreeNode nodes[MAX_NODE_COUNT*8];
    alignas(16) glm::ivec3 position;

    GPUOctree();
    GPUOctree(Octree &octree, const int &nodeSlotsUsed);
};

#endif