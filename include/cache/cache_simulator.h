#ifndef CACHE_SIMULATOR_H
#define CACHE_SIMULATOR_H

#include "cache_level.h"

class CacheSimulator
{
private:
    CacheLevel L1;
    CacheLevel L2;

    int memoryAccesses;

public:
    CacheSimulator(CacheLevel l1, CacheLevel l2);

    void access(int address);
    void printStats() const;
    void resetStats();
};

#endif