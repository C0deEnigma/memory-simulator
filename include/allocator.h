// Defines allocator class

#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <list>
#include "block.h"

class Allocator {
private:
    int totalSize;              // total memory size
    std::list<Block> blocks;    // list of memory blocks
    int nextId;

public:
    // constructor
    Allocator(int size);

    // allocation algorithms
    int allocateFirstFit(int size);
    int allocateBestFit(int size);
    int allocateWorstFit(int size);

    // deallocation
    void freeBlock(int id);

    // debugging or visualization
    void dumpMemory();
};

#endif
