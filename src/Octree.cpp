#include "Octree.h"

#include <iostream>

//Unoptimized, only on CPU

int Octree::createOctree(int *values, glm::ivec3 position) {
    int nodeSlotsUsed = 0;
    baseNode.subdivide(values, CHUNK_SIZE, nodeSlotsUsed, nodes);

    this->position = position;

    return nodeSlotsUsed;
}

//Optimized, for GPU

GPUOctree::GPUOctree() {
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


GPUOctreeGrid::GPUOctreeGrid() {
    for (int i = 0; i < 8; i++) {
        octrees[i] = GPUOctree();
    }
}

// for (int k = 0; k < WORLD_DIMENSIONS.z/CHUNK_SIZE; k++) {
            //     for (int j = 0; j < WORLD_DIMENSIONS.y/CHUNK_SIZE; j++) {
            //         for (int i = 0; i < WORLD_DIMENSIONS.x/CHUNK_SIZE; i++) {
            //             Octree octree{};
            //             std::cout << 'a';

            //             int values[CHUNK_SIZE*CHUNK_SIZE*CHUNK_SIZE];
            //             int nodeIndex = 0;
            //             for (int z = 0; z < CHUNK_SIZE; z++) {
            //                 for (int y = 0; y < CHUNK_SIZE; y++) {
            //                     for (int x = 0; x < CHUNK_SIZE; x++) {
            //                         values[nodeIndex] = terrainNoise.noise2D_01((float) x/CHUNK_SIZE, (float) y/CHUNK_SIZE) >= (float) z/CHUNK_SIZE ? 1 : 0;
            //                         nodeIndex++;
            //                     }
            //                 }
            //             }

            //             int nodeSlotsUsed = octree.createOctree(values, glm::ivec3(i*CHUNK_SIZE, j*CHUNK_SIZE, k*CHUNK_SIZE));

            //             for (int nodeIndex = 0; nodeIndex < nodeSlotsUsed; i++) {
            //                 if (octree.nodes[nodeIndex].value != 0)
            //                     octree.nodes[nodeIndex].color = glm::vec4((float)octree.nodes[i].maxX/CHUNK_SIZE, (float)octree.nodes[i].maxY/CHUNK_SIZE, (float)octree.nodes[i].maxZ/CHUNK_SIZE, 1.0f);
            //                 else
            //                     octree.nodes[nodeIndex].color = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
            //             }

            //             GPUOctree gpuOctree(octree, nodeSlotsUsed);
            //             octreeGrid.octrees[i+j*2+k*4] = gpuOctree;
            //         }
            //     }
            // }