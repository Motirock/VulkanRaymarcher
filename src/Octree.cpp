#include "Octree.h"

#include <iostream>

//Unoptimized, only on CPU

int Octree::createOctree(int *values) {
    int nodeSlotsUsed = 0;
    worldNode.subdivide(values, WORLD_SIZE, nodeSlotsUsed, nodes);
    return nodeSlotsUsed;
}

//Optimized, for GPU

GPUOctree::GPUOctree(Octree &octree, const int &nodeSlotsUsed) {
    //Loading world node. Adapted from later standard algorithm for converting nodes to GPU nodes
    {
    OctreeNode &node = octree.worldNode;
    GPUOctreeNode &gpuNode = worldNode;
    gpuNode.data = 0;

    if (node.flags & FLAG_HOMOGENEOUS) {
        //This shouldn't ever happen anyway
        throw std::runtime_error("World node cannot be homogeneous");
    }
    else {
        if (node.childrenIndex+7 >= UINT16_MAX)
            throw std::runtime_error("Children index out of bounds");

        gpuNode.data |= node.childrenIndex;

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

            gpuNode.data |= node.childrenIndex;

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