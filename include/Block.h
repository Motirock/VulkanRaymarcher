#ifndef BLOCK_H
#define BLOCK_H

#include "Face.h"

struct Block {
    uint16_t ID = 0;

    Block(uint16_t blockID);
    Block();

    bool operator == (const Block &other);
    bool operator != (const Block &other); 
};

#endif