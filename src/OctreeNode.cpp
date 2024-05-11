#include "Octree.h"

#include <iostream>

//Unoptimized, only on CPU

OctreeNode::OctreeNode() {
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

OctreeNode::OctreeNode(int childrenIndex, int value, bool homogeneous, int minX, int maxX, int minY, int maxY, int minZ, int maxZ) {
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

bool OctreeNode::isHomogeneous(int *values, const int &WORLD_SIZE) {
    for (int z = minZ; z < maxZ; z++) {
        for (int y = minY; y < maxY; y++) {
            for (int x = minX; x < maxX; x++) {
                if (values[x+y*WORLD_SIZE+z*WORLD_SIZE*WORLD_SIZE] != values[minX+minY*WORLD_SIZE+minZ*WORLD_SIZE*WORLD_SIZE]) {
                    //TODO this is wrong
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

void OctreeNode::subdivide(int *values, const int &WORLD_SIZE, int &nodeSlotsUsed, OctreeNode *nodes) {
    if (flags & FLAG_HOMOGENEOUS)
        return;

    int size = maxX-minX;
    childrenIndex = nodeSlotsUsed;

    int i = 0;
    for (int z = 0; z < 2; z++) {
        for (int y = 0; y < 2; y++) {
            for (int x = 0; x < 2; x++) {
                nodes[childrenIndex+i] = OctreeNode(0, -1, false, minX+x*size/2, minX+(x+1)*size/2, minY+y*size/2, minY+(y+1)*size/2, minZ+z*size/2, minZ+(z+1)*size/2);
                i++;
            }
        }
    }
    nodeSlotsUsed += 8;
    for (int i = 0; i < 8; i++) {
        nodes[childrenIndex+i].isHomogeneous(values, WORLD_SIZE);
        nodes[childrenIndex+i].subdivide(values, WORLD_SIZE, nodeSlotsUsed, nodes);
    }
}

//Optimized, for GPU

GPUOctreeNode::GPUOctreeNode() {
    data = 0;
}

GPUOctreeNode::GPUOctreeNode(uint32_t data) {
    this->data = data;
}

void GPUOctreeNode::print() {
    for (int i = 0; i < 32; i++) {
        std::cout << (data & (1 << i) ? 1 : 0);
        if ((i+1) % 8 == 0)
            std::cout << ' ';
    }
    std::cout << '\n';
}