#include "Octree.h"

//Unoptimized, only on CPU

int Octree::createOctree(int *values) {
    int nodeSlotsUsed = 1;
    nodes[0] = OctreeNode(-1, -1, false, 0, WORLD_SIZE, 0, WORLD_SIZE, 0, WORLD_SIZE, nodeSlotsUsed);
    nodes[0].subdivide(values, WORLD_SIZE, nodeSlotsUsed, nodes);
    return nodeSlotsUsed;
}

//Optimized, for GPU

GPUOctree::GPUOctree(Octree &octree) {
    for (int i = 0; i < MAX_NODE_COUNT; i++) {
        OctreeNode &node = octree.nodes[i];
        GPUOctreeNode &gpuNode = nodes[i];
        gpuNode.data = 0;

        if (node.maxX-node.minX < 0)
            return;
        
        if (node.value == -1) {
            for (int childrenIndex = node.childrenIndex; childrenIndex < 8; childrenIndex++) {
                if (octree.nodes[childrenIndex].value == 0)
                    gpuNode.data |= NODE_STATUS_EMPTY << i;
                else
                    gpuNode.data |= NODE_STATUS_SOLID << i;
            }
            
            gpuNode.data |= node.childrenIndex << 8;
        }
        else {
            gpuNode.data |= (uint64_t) (node.color.r * 255) << 0;
            gpuNode.data |= (uint64_t) (node.color.g * 255) << 16;
            gpuNode.data |= (uint64_t) (node.color.b * 255) << 32;
            //gpuNode.data |= (uint16_t) (node.color.a * 255) << 0;
        }
    }
}