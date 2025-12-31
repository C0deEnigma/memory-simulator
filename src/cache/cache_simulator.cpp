#include "cache_simulator.h"
#include <iostream>

using namespace std;

CacheSimulator::CacheSimulator(CacheLevel l1, CacheLevel l2)
    : L1(l1), L2(l2)
{
    memoryAccesses=0;
}

void CacheSimulator::access(int address)
{
    memoryAccesses++;
    int block_address=address/L1.getBlockSize();
    // Hit in L1
    if(L1.access(block_address)) return;

    // Hit in L2
    if(L2.access(block_address))
    {
        int evicted_address=L1.insert(block_address);
        L2.remove(block_address);
        if(evicted_address!=-1) L2.insert(evicted_address);
        return;
    }

    // Miss in L1 and L2
    int evicted_address=L1.insert(block_address);
    if(evicted_address!=-1) L2.insert(evicted_address);
}

void CacheSimulator::printStats() const
{
    int l1Hits=L1.getHits();
    int l1Misses=L1.getMisses();

    int l2Hits=L2.getHits();
    int l2Misses=L2.getMisses();

    cout<<"L1 Hits : "<<l1Hits<<endl;
    cout<<"L1 Misses : "<<l1Misses<<endl;

    if(l1Hits+l1Misses>0)
        cout<<"L1 Hit Ratio : "<<(double)l1Hits/(l1Hits+l1Misses)<<endl;
    else
        cout<<"L1 Hit Ratio : 0"<<endl;

    cout<<endl;

    cout<<"L2 Hits : "<<l2Hits<<endl;
    cout<<"L2 Misses : "<<l2Misses<<endl;

    if(l2Hits+l2Misses>0)
        cout<<"L2 Hit Ratio : "<<(double)l2Hits/(l2Hits+l2Misses)<<endl;
    else
        cout<<"L2 Hit Ratio : 0"<<endl;

    cout<<endl;

    cout<<"Miss Propagation:"<<endl;
    cout<<"  L1 -> L2 accesses : "<<l1Misses<<endl;
    cout<<"  L2 -> Memory accesses : "<<l2Misses<<endl;
}

void CacheSimulator::resetStats()
{
    L1.resetStats();
    L2.resetStats();
    memoryAccesses=0;
}