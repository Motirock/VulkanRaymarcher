#include "Octree.h"

#include <iostream>

//Unoptimized, only on CPU

int Octree::createOctree(int *values) {
    int nodeSlotsUsed = 1;
    nodes[0] = OctreeNode(-1, -1, false, 0, WORLD_SIZE, 0, WORLD_SIZE, 0, WORLD_SIZE, nodeSlotsUsed);
    nodes[0].subdivide(values, WORLD_SIZE, nodeSlotsUsed, nodes);
    return nodeSlotsUsed;
}

//Optimized, for GPU

GPUOctree::GPUOctree(Octree &octree, const int &nodeSlotsUsed) {
    for (int i = 0; i < nodeSlotsUsed; i++) {
        OctreeNode &node = octree.nodes[i];
        GPUOctreeNode &gpuNode = nodes[i];
        gpuNode.data = 0;
        
        if (node.flags & FLAG_HOMOGENEOUS) {
            gpuNode.data |= (uint32_t) (node.color.r * 255) << 0;
            gpuNode.data |= (uint32_t) (node.color.g * 255) << 8;
            gpuNode.data |= (uint32_t) (node.color.b * 255) << 16;
            gpuNode.data |= (uint32_t) (node.color.a * 255) << 24;
        }
        else {
            for (int childrenIndexOffset = 0; childrenIndexOffset < 8; childrenIndexOffset++) {
                if (octree.nodes[node.childrenIndex+childrenIndexOffset].value == 0)
                    gpuNode.data |= NODE_STATUS_EMPTY << 1;
                else
                    gpuNode.data |= NODE_STATUS_SOLID << 1;
            }
            
            gpuNode.data |= node.childrenIndex << 8;
        }
    }
}