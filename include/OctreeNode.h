#ifndef OCTREE_NODE_H
#define OCTREE_NODE_H

const int MAX_NODE_COUNT = 5;//__UINT16_MAX__;
const int MAX_VALUE = 2;

#include "VkUtils.h"

struct GPUOctreeNode {
    int32_t childrenIndex;
    int32_t value;
    bool homogenous;
    int32_t minX, minY, maxX, maxY;
    alignas(16) glm::vec3 END;

    GPUOctreeNode() {
        childrenIndex = -1;
        value = -1;
        homogenous = false;
        minX = -1;
        minY = -1;
        maxX = -1;
        maxY = -1;
    }

    GPUOctreeNode(int childrenIndex, int value, bool homogenous, int minX, int minY, int maxX, int maxY) {
        this->childrenIndex = childrenIndex;
        this->value = value;
        this->homogenous = homogenous;
        this->minX = minX;
        this->minY = minY;
        this->maxX = maxX;
        this->maxY = maxY;
    }
};

struct OctreeNode {
    OctreeNode *children[2][2];
    int value = -1; //Only if homogenous
    bool homogenous = false;
    int minX, minY, maxX, maxY;

    OctreeNode(int minX, int minY, int maxX, int maxY) {
        this->minX = minX;
        this->minY = minY;
        this->maxX = maxX;
        this->maxY = maxY;
    }

    void checkIfHomogenous(int **values) {
        int a = values[minY][minX];
        for (int y = minY; y < maxY; y++) {
            for (int x = minX; x < maxX; x++) {
                if (values[y][x] != a) {
                    homogenous = false;
                    return;
                }
            }
        }
        homogenous = true;
        value = a;
    }

    void subdivide(int **values) {
        checkIfHomogenous(values);
        if (homogenous) {
            return;
        }

        int size = maxX-minX;
        
        children[0][0] = new OctreeNode(minX, minY, minX+size/2, minY+size/2);
        children[0][1] = new OctreeNode(minX+size/2, minY, minX+size, minY+size/2);
        children[1][0] = new OctreeNode(minX, minY+size/2, minX+size/2, minY+size);
        children[1][1] = new OctreeNode(minX+size/2, minY+size/2, minX+size, minY+size);
        children[0][0]->subdivide(values);
        children[0][1]->subdivide(values);
        children[1][0]->subdivide(values);
        children[1][1]->subdivide(values);
    }    

    OctreeNode &getNodeAtPosition(int x, int y) {
        int size = maxX-minX;

        if (homogenous) {
            return *this;
        }
        if (x < minX+size/2) {
            if (y < minY+size/2) {
                return children[0][0]->getNodeAtPosition(x, y);
            } else {
                return children[1][0]->getNodeAtPosition(x, y);
            }
        } else {
            if (y < minY+(maxY-minY)/2) {
                return children[0][1]->getNodeAtPosition(x, y);
            } else {
                return children[1][1]->getNodeAtPosition(x, y);
            }
        }
    }

    int getSize() {
        return maxX-minX;
    }
};

#endif