#include "Region.h"

#include <stdexcept>
#include <iostream>

Region::Region(glm::vec3 position) : position(position) {
    for (int i = 0; i < REGION_SIZE; i++) {
        for (int j = 0; j < REGION_SIZE; j++) {
            for (int k = 0; k < REGION_SIZE; k++) {
                chunks[i+j*REGION_SIZE+k*REGION_SIZE*REGION_SIZE] = new Chunk(glm::vec3(i*CHUNK_SIZE, j*CHUNK_SIZE, k*CHUNK_SIZE)+position);
            }
        }
    }
}

Region::~Region() {
    for (int i = 0; i < REGION_SIZE*REGION_SIZE*REGION_SIZE; i++) {
        delete chunks[i];
    }
}

void Region::generateValues(const siv::PerlinNoise &noise) {
    for (int i = 0; i < REGION_SIZE*REGION_SIZE*REGION_SIZE; i++) {
        //std::cout << i << ' ';
        chunks[i]->generateValues(noise);
    }
}