// Defines allocator class

#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <list>
#include "block.h"

using namespace std;

class Allocator {
private:
    int totalSize;              // total memory size
    list<Block> blocks;    // list of memory blocks

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
