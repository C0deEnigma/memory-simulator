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
    int allocRequests;
    int allocFailures;

    // Coalesce neighbour blocks if they are free after deallocation
    void coalesce(std::list<Block>::iterator it);

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

    // Calculates and Prints internal fragmentation, external fragmenation, allocation failure rate and memory utilization
    void printStats();
};

#endif
