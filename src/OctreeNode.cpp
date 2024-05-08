#include "Octree.h"

#include <iostream>

GPUOctreeNode::GPUOctreeNode() {
    childrenIndex = -1;
    value = -1;
    flags = 0;
    minX = -1;
    minY = -1;
    maxX = -1;
    maxY = -1;
    minZ = -1;
    maxZ = -1;
}

GPUOctreeNode::GPUOctreeNode(int childrenIndex, int value, bool homogeneous, int minX, int maxX, int minY, int maxY, int minZ, int maxZ, int &nodeSlotsUsed) {
    this->childrenIndex = childrenIndex;
    this->value = value;
    this->flags = 0 | (homogeneous ? FLAG_HOMOGENEOUS : 0);
    this->minX = minX;
    this->maxX = maxX;
    this->minY = minY;
    this->maxY = maxY;
    this->minZ = minZ;
    this->maxZ = maxZ;
}

bool GPUOctreeNode::isHomogeneous(int *values, const int &WORLD_SIZE) {
    for (int z = minZ; z < maxZ; z++) {
        for (int y = minY; y < maxY; y++) {
            for (int x = minX; x < maxX; x++) {
                if (values[x+y*WORLD_SIZE+z*WORLD_SIZE*WORLD_SIZE] != values[minX+minY*WORLD_SIZE+minZ*WORLD_SIZE*WORLD_SIZE]) {
                    flags &= FLAG_NO_FLAG ^ FLAG_HOMOGENEOUS;
                    return false;
                }
            }
        }
    }
    value = values[minX+minY*WORLD_SIZE+minZ*WORLD_SIZE*WORLD_SIZE];
    flags |= FLAG_HOMOGENEOUS;
    return true;
}

void GPUOctreeNode::subdivide(int *values, const int &WORLD_SIZE, int &nodeSlotsUsed, GPUOctreeNode *nodes) {
    isHomogeneous(values, WORLD_SIZE);
    if (flags & FLAG_HOMOGENEOUS)
        return;

    int size = maxX-minX;
    childrenIndex = nodeSlotsUsed;

    int i = 0;
    for (int z = 0; z < 2; z++) {
        for (int y = 0; y < 2; y++) {
            for (int x = 0; x < 2; x++) {
                nodes[childrenIndex+i] = GPUOctreeNode(-1, -1, false, minX+x*size/2, minX+(x+1)*size/2, minY+y*size/2, minY+(y+1)*size/2, minZ+z*size/2, minZ+(z+1)*size/2, nodeSlotsUsed);
                
                i++;
            }
        }
    }
    nodeSlotsUsed += 8;
    for (int i = 0; i < 8; i++) {
        nodes[childrenIndex+i].subdivide(values, WORLD_SIZE, nodeSlotsUsed, nodes);
    }
}