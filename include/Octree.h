#ifndef OCTREE_H
#define OCTREE_H

#include "OctreeNode.h"

const int32_t REGION_SIZE = 8;
const int32_t CHUNK_SIZE = 64;

const int32_t WORLD_DIMENSIONS_REGIONS_X = 1, WORLD_DIMENSIONS_REGIONS_Y = 1, WORLD_DIMENSIONS_REGIONS_Z = 1;

const glm::ivec3 WORLD_DIMENSIONS_REGIONS = glm::ivec3(WORLD_DIMENSIONS_REGIONS_X, WORLD_DIMENSIONS_REGIONS_Y, WORLD_DIMENSIONS_REGIONS_Z);
const glm::ivec3 WORLD_DIMENSIONS_CHUNKS = glm::ivec3(WORLD_DIMENSIONS_REGIONS_X, WORLD_DIMENSIONS_REGIONS_Y, WORLD_DIMENSIONS_REGIONS_Z)*REGION_SIZE;
const glm::ivec3 WORLD_DIMENSIONS_VOXELS = glm::ivec3(WORLD_DIMENSIONS_CHUNKS.x, WORLD_DIMENSIONS_CHUNKS.y, WORLD_DIMENSIONS_CHUNKS.z)*CHUNK_SIZE;

struct Octree {
    OctreeNode baseNode = OctreeNode(0, -1, false, 0, CHUNK_SIZE, 0, CHUNK_SIZE, 0, CHUNK_SIZE);
    OctreeNode *nodes;
    glm::ivec3 position;

    int createOctree(int *values, glm::ivec3 position);

    ~Octree();
};

struct GPUOctree {
    GPUOctreeNode baseNode;
    GPUOctreeNode nodes[MAX_NODE_COUNT];
    alignas(16) glm::ivec3 position;

    GPUOctree();
    GPUOctree(Octree &octree, const int &nodeSlotsUsed);
};

#endif