#include <iostream>
#include "cache_simulator.h"
#include "cache_level.h"

using namespace std;

// Helper to build a standard L1 + L2 cache
CacheSimulator buildCache()
{
    CacheLevel L1(
        64,     // cache size
        4,      // block size
        2,      // associativity
        FIFO    // replacement policy
    );

    CacheLevel L2(
        128,
        4,
        4,
        FIFO
    );

    return CacheSimulator(L1, L2);
}

CacheSimulator buildCache_LRU()
{
    CacheLevel L1(
        64,     // cache size
        4,      // block size
        2,      // associativity
        LRU    // replacement policy
    );

    CacheLevel L2(
        128,
        4,
        4,
        LRU
    );

    return CacheSimulator(L1, L2);
}

void test_basic_hits()
{
    cout<<"========== TEST: Basic Hits =========="<<endl;

    CacheSimulator cache=buildCache();

    cache.access(0);    // miss both → insert into L1
    cache.access(64);   // miss both → insert into L1
    cache.access(0);    // L1 Hit

    cache.printStats();
    cout<<endl;
}


void test_eviction_and_demotion()
{
    cout<<"========== TEST: Eviction & Demotion =========="<<endl;

    CacheSimulator cache=buildCache();

    cache.access(0);
    cache.access(32);
    cache.access(64);   // L1 miss, L2 miss -> Demotion of 0, Insertion of 16 in L1
    cache.access(64);   // L1 Hit
    cache.access(0);    // L2 Hit -> Demotion of 16, Promotion of 0

    cache.printStats();
    cout<<endl;
}

void test_FIFO()
{
    cout<<"========== TEST: FIFO =========="<<endl;

    CacheSimulator cache=buildCache();

    cache.access(1);
    cache.access(33);
    cache.access(1);
    cache.access(65);    // Demotion of 1 to L2
    cache.access(33);    // L1 hit   

    cache.printStats();
    cout<<endl;
}

void test_LRU()
{
    cout<<"========== TEST: LRU =========="<<endl;

    CacheSimulator cache=buildCache_LRU();

    cache.access(1);
    cache.access(33);
    cache.access(1);
    cache.access(65);    // Demotion of 33 to L2 since 1 was accessed later 
    cache.access(33);    // L2 hit   

    cache.printStats();
    cout<<endl;
}

void test_stats_reset()
{
    cout<<"========== TEST: Stats Reset =========="<<endl;

    CacheSimulator cache=buildCache();

    cache.access(0);
    cache.access(4);
    cache.printStats();

    cache.resetStats();
    cout<<"After reset:"<<endl;
    cache.printStats();

    cout<<endl;
}

int main()
{
    cout<<"Running Cache Simulator Tests"<<endl<<endl;

    test_basic_hits();
    test_eviction_and_demotion();
    test_FIFO();
    test_LRU();
    test_stats_reset();

    cout<<"All cache tests executed"<<endl;
    return 0;
}
