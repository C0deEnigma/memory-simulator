#ifndef CACHE_LEVEL_H
#define CACHE_LEVEL_H

#include <vector>
#include <deque>

// Replacement policy type
enum ReplacementPolicy {
    FIFO,
    LRU
};

// Represents one cache block
struct CacheLine
{
    int tag;
};

// Represents one cache level (L1 or L2)
class CacheLevel
{
private:
    int cacheSize;
    int blockSize;
    int associativity;
    int numSets;

    ReplacementPolicy policy;

    // Each set is a list of cache lines
    std::vector<std::deque<CacheLine>> sets;

    int hits;
    int misses;

public:
    CacheLevel(int cacheSize,
               int blockSize,
               int associativity,
               ReplacementPolicy policy);

    // Returns true if hit, false if miss
    bool access(int address);

    void printStats() const;
    void resetStats();
};

#endif
