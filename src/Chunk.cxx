#include "Chunk.h"

#include <iostream>

Chunk::Chunk() {
    values = new int[CHUNK_SIZE*CHUNK_SIZE*CHUNK_SIZE];
}

Chunk::Chunk(glm::ivec3 position) {
    this->position = position;
    values = new int[CHUNK_SIZE*CHUNK_SIZE*CHUNK_SIZE];
}

Chunk::~Chunk() {
    delete[] values;
}

int Chunk::getValue(const int &x, const int &y, const int &z) {
    return values[x+y*CHUNK_SIZE+z*CHUNK_SIZE*CHUNK_SIZE];
}

void Chunk::setValue(const int &x, const int &y, const int &z, const int value) {
    values[x+y*CHUNK_SIZE+z*CHUNK_SIZE*CHUNK_SIZE] = value;
}

void Chunk::generateValues(const siv::PerlinNoise &noise) {
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            float r = noise.octave2D_01((x+position.x)/500.0f, (y+position.y)/500.0f, 1)*CHUNK_SIZE*WORLD_DIMENSIONS_CHUNKS.z;
            for (int z = 0; z < CHUNK_SIZE; z++) {
                //Reseting values
                int valueIndex = x+y*CHUNK_SIZE+z*CHUNK_SIZE*CHUNK_SIZE;
                //values[valueIndex] = pow((z+position.z)/WORLD_DIMENSIONS_VOXELS.z, 1.5) < pow((x+position.x)/WORLD_DIMENSIONS_VOXELS.x, 2)+pow((y+position.y)/WORLD_DIMENSIONS_VOXELS.y, 2) ? 1 : 0;

                bool isSolid = r >= (float) z+position.z;
                if (isSolid && r >= (float) z+position.z)//+3)
                    values[valueIndex] = 1;
                // if ((float) z+position.z > WORLD_DIMENSIONS_VOXELS.z/2) {
                //     if (isSolid && r >= (float) z+position.z+1)
                //         values[valueIndex] = 2;
                //     else if (isSolid)
                //         values[valueIndex] = 3;
                // }
                // else {
                //     if (isSolid && r >= (float) z+position.z+1)
                //         values[valueIndex] = 4;
                //     else
                //         values[valueIndex] = 5;
                // }
            }
        }
    }
}

int Chunk::createOctree() {
    Octree tempOctree = Octree();
    int nodeSlotsUsed = tempOctree.createOctree(values, glm::ivec3(0));
    for (int nodeIndex = 0; nodeIndex < nodeSlotsUsed; nodeIndex++) {
        if (tempOctree.nodes[nodeIndex].value != 0)
            tempOctree.nodes[nodeIndex].color = glm::vec4((float)tempOctree.nodes[nodeIndex].maxX/CHUNK_SIZE, (float)tempOctree.nodes[nodeIndex].maxY/CHUNK_SIZE, (float)tempOctree.nodes[nodeIndex].maxZ/CHUNK_SIZE, 1.0f);
        else
            tempOctree.nodes[nodeIndex].color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
        // if (tempOctree.nodes[nodeIndex].value == 0)
        //     tempOctree.nodes[nodeIndex].color = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
        // else if (tempOctree.nodes[nodeIndex].value == 1)
        //     tempOctree.nodes[nodeIndex].color = glm::vec4(0.2f+(rand()%10)/100.0f, 0.2f+(rand()%10)/100.0f, 0.2f+(rand()%10)/100.0f, 1.0f);
        // else if (tempOctree.nodes[nodeIndex].value == 2)
        //     tempOctree.nodes[nodeIndex].color = glm::vec4(0.3f, 0.24f, 0.0f, 1.0f);
        // else if (tempOctree.nodes[nodeIndex].value == 3)
        //     tempOctree.nodes[nodeIndex].color = glm::vec4(0.0f, 0.3f+(rand()%30)/100.0f, 0.0f, 1.0f);
        // else if (tempOctree.nodes[nodeIndex].value == 4)
        //     tempOctree.nodes[nodeIndex].color = glm::vec4(0.70f+(rand()%10)/100.0f, 0.65+(rand()%10)/100.0f, 0.45+(rand()%10)/100.0f, 1.0f);
        // else if (tempOctree.nodes[nodeIndex].value == 5)
        //     tempOctree.nodes[nodeIndex].color = glm::vec4(0.0f, 0.3f, 1.0f, 0.5f);
    }

    octree = GPUOctree(tempOctree, nodeSlotsUsed);
    octree.position = glm::ivec3(position);
    return nodeSlotsUsed;
}