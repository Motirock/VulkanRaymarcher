#include "Block.h"

Block::Block(uint16_t blockID) : ID(blockID) {}

Block::Block() {}

bool Block::operator==(const Block &other) {
    return this->ID == other.ID;
}

bool Block::operator!=(const Block &other) {
    return this->ID != other.ID;
}