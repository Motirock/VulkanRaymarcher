#include "Chunk.h"

#include <iostream>

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
    int valueIndex = 0;
    for (int z = 0; z < CHUNK_SIZE; z++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int x = 0; x < CHUNK_SIZE; x++) {
                //Reseting values
                values[valueIndex] = pow((z+position.z)/WORLD_DIMENSIONS.z, 1.5) < pow((x+position.x)/WORLD_DIMENSIONS.x, 2)+pow((y+position.y)/WORLD_DIMENSIONS.y, 2) ? 3 : 0;

                // float r = 0.0f;
                // r = noise.octave2D_01((x+position.x)/500.0f, (y+position.y)/500.0f, 6)*CHUNK_SIZE*WORLD_CHUNK_SIZE_Z;

                // bool isSolid = r >= (float) z+position.z;
                // if (isSolid && r < (float) z+position.z+2)
                //     values[valueIndex] = 2;
                // if (isSolid && r < (float) z+position.z+1)
                //     values[valueIndex] = 2;
                // else if (isSolid)
                //     values[valueIndex] = 1;
                // //values[valueIndex] = terrainNoise.noise3D_01((float) x/CHUNK_SIZE+i+noiseOffset, (float) y/CHUNK_SIZE+j, (float) z/CHUNK_SIZE+k) >= 0.5 ? 1 : 0;
                valueIndex++;
            }
        }
    }
}

void Chunk::createOctree() {
    Octree tempOctree = Octree();
    int nodeSlotsUsed = tempOctree.createOctree(values, glm::ivec3(0));
    for (int nodeIndex = 0; nodeIndex < nodeSlotsUsed; nodeIndex++) {
        if (tempOctree.nodes[nodeIndex].value != 0)
            tempOctree.nodes[nodeIndex].color = glm::vec4((float)tempOctree.nodes[nodeIndex].maxX/CHUNK_SIZE, (float)tempOctree.nodes[nodeIndex].maxY/CHUNK_SIZE, (float)tempOctree.nodes[nodeIndex].maxZ/CHUNK_SIZE, 1.0f);
        // else
        //     tempOctree.nodes[nodeIndex].color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
        // if (tempOctree.nodes[nodeIndex].value == 0)
        //     tempOctree.nodes[nodeIndex].color = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
        // else if (tempOctree.nodes[nodeIndex].value == 1)
        //     tempOctree.nodes[nodeIndex].color = glm::vec4(0.3f, 0.24f, 0.0f, 1.0f);
        // else if (tempOctree.nodes[nodeIndex].value == 2)
        //     tempOctree.nodes[nodeIndex].color = glm::vec4(0.0f, 0.5f, 0.0f, 1.0f);
        // else if (tempOctree.nodes[nodeIndex].value == 3)
        //     tempOctree.nodes[nodeIndex].color = glm::vec4(0.3f, 0.3f, 0.3f, 1.0f);

    }

    octree = GPUOctree(tempOctree, nodeSlotsUsed);
    octree.position = glm::ivec3(position);
}