#ifndef CHUNK_H
#define CHUNK_H

#include "Octree.h"
#include "PerlinNoise.hpp"

struct Chunk {
    int *values;
    glm::vec3 position;
    GPUOctree octree;

    Chunk(glm::ivec3 position);
    ~Chunk();

    int getValue(const int &x, const int &y, const int &z);
    void setValue(const int &x, const int &y, const int &z, const int value);

    void generateValues(const siv::PerlinNoise &noise);

    void createOctree();
};


#endif