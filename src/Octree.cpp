#include "Octree.h"

#include <iostream>

//Unoptimized, only on CPU

int Octree::createOctree(int *values, glm::ivec3 position) {
    nodes = new OctreeNode[MAX_NODE_COUNT];

    int nodeSlotsUsed = 0;
    baseNode.subdivide(values, CHUNK_SIZE, nodeSlotsUsed, nodes);

    this->position = position;

    return nodeSlotsUsed;
}

Octree::~Octree() {
    delete[] nodes;
}

//Optimized, for GPU

GPUOctree::GPUOctree() {
    //nodes = new GPUOctreeNode[MAX_NODE_COUNT];

    position = glm::ivec3(0, 0, 0);
    baseNode = GPUOctreeNode();
    for (int i = 0; i < MAX_NODE_COUNT; i++) {
        nodes[i] = GPUOctreeNode();
    }
}

GPUOctree::GPUOctree(Octree &octree, const int &nodeSlotsUsed) {
    this->position = octree.position;

    //Loading world node. Adapted from later standard algorithm for converting nodes to GPU nodes
    {
    OctreeNode &node = octree.baseNode;
    GPUOctreeNode &gpuNode = baseNode;
    gpuNode.data = 0;

    if (node.flags & FLAG_HOMOGENEOUS) {
        //This shouldn't ever happen anyway
        throw std::runtime_error("World node cannot be homogeneous");
    }
    else {
        if (node.childrenIndex+7 >= UINT16_MAX)
            throw std::runtime_error("Children index out of bounds");

        gpuNode.data |= node.childrenIndex/8;

        for (int childrenIndexOffset = 0; childrenIndexOffset < 8; childrenIndexOffset++) {
            if (octree.nodes[node.childrenIndex+childrenIndexOffset].value == 0)
                gpuNode.data |= NODE_STATUS_EMPTY << 16+childrenIndexOffset;
            else
                gpuNode.data |= NODE_STATUS_NON_EMPTY << 16+childrenIndexOffset;

            if (octree.nodes[node.childrenIndex+childrenIndexOffset].flags & FLAG_HOMOGENEOUS)
                gpuNode.data |= NODE_STATUS_HOMOGENEOUS << 24+childrenIndexOffset;
            else
                gpuNode.data |= NODE_STATUS_NON_HOMOGENEOUS << 24+childrenIndexOffset;
        }
    }
    }

    for (int i = 0; i < nodeSlotsUsed; i++) {
        OctreeNode &node = octree.nodes[i];
        GPUOctreeNode &gpuNode = nodes[i];
        gpuNode.data = 0;
        
        //Load color data if homogeneous, otherwise load children index and children status
        if (node.flags & FLAG_HOMOGENEOUS) {
            gpuNode.data |= (uint32_t) (node.color.r * 255) << 0;
            gpuNode.data |= (uint32_t) (node.color.g * 255) << 8;
            gpuNode.data |= (uint32_t) (node.color.b * 255) << 16;
            gpuNode.data |= (uint32_t) (node.color.a * 255) << 24;
        }
        else {
            if (node.childrenIndex+7 >= UINT16_MAX)
                throw std::runtime_error("Children index out of bounds");

            gpuNode.data |= node.childrenIndex/8;

            for (int childrenIndexOffset = 0; childrenIndexOffset < 8; childrenIndexOffset++) {
                if (octree.nodes[node.childrenIndex+childrenIndexOffset].value == 0)
                    gpuNode.data |= NODE_STATUS_EMPTY << 16+childrenIndexOffset;
                else
                    gpuNode.data |= NODE_STATUS_NON_EMPTY << 16+childrenIndexOffset;

                if (octree.nodes[node.childrenIndex+childrenIndexOffset].flags & FLAG_HOMOGENEOUS)
                    gpuNode.data |= NODE_STATUS_HOMOGENEOUS << 24+childrenIndexOffset;
                else
                    gpuNode.data |= NODE_STATUS_NON_HOMOGENEOUS << 24+childrenIndexOffset;
            }
        }
    }
}

