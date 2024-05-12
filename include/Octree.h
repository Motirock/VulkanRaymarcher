#ifndef OCTREE_H
#define OCTREE_H

#include "OctreeNode.h"

const int32_t CHUNK_SIZE = 32;
const uint32_t WORLD_CHUNK_SIZE_X = 2, WORLD_CHUNK_SIZE_Y = 2, WORLD_CHUNK_SIZE_Z = 2;
const glm::ivec3 WORLD_DIMENSIONS = glm::ivec3(WORLD_CHUNK_SIZE_X, WORLD_CHUNK_SIZE_Y, WORLD_CHUNK_SIZE_Z)*32;

struct Octree {
    OctreeNode baseNode = OctreeNode(0, -1, false, 0, CHUNK_SIZE, 0, CHUNK_SIZE, 0, CHUNK_SIZE);
    OctreeNode nodes[MAX_NODE_COUNT];
    glm::ivec3 position;

    int createOctree(int *values, glm::ivec3 position);
};

struct GPUOctree {
    GPUOctreeNode baseNode;
    GPUOctreeNode nodes[MAX_NODE_COUNT];
    alignas(16) glm::ivec3 position;

    GPUOctree();
    GPUOctree(Octree &octree, const int &nodeSlotsUsed);
};

struct GPUOctreeGrid {
    GPUOctree octrees[WORLD_CHUNK_SIZE_X*WORLD_CHUNK_SIZE_Y*WORLD_CHUNK_SIZE_Z]; 

    GPUOctreeGrid();
};

#endif