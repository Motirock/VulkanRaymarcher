#ifndef REGION_H
#define REGION_H

#include "Chunk.h"

struct Region {
    Chunk *chunks[REGION_SIZE*REGION_SIZE*REGION_SIZE];
    glm::vec3 position;

    Region(glm::vec3 position);
    ~Region();

    void generateValues(const siv::PerlinNoise &noise);
};

#endif