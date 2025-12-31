#include "cache_level.h"
#include<iostream>

using namespace std;

CacheLevel::CacheLevel(int cacheSize, int blockSize, int associativity, ReplacementPolicy policy)
{
    this->cacheSize=cacheSize;
    this->blockSize=blockSize;
    this->associativity=associativity;
    this->policy=policy;

    this->numSets=cacheSize/(blockSize*associativity); // Number of sets in cache level

    sets.resize(numSets); 

    hits=0;
    misses=0;
}


bool CacheLevel::access(int req_block_address)
{
    int set_number=req_block_address % numSets;

    deque<int> &set=sets[set_number];

    for(auto it=set.begin() ; it!=set.end() ; it++)
    {
        int block_address=*it;
        if(req_block_address==block_address)
        {
            if(policy==LRU)
            {
                set.erase(it);
                set.push_back(block_address);
            }
            hits++;
            return true;
        }
    }

    misses++;   // If tag is not present in set

    return false;
}

int CacheLevel::insert(int block_address)
{
    int set_number=block_address % numSets;

    deque<int> &set=sets[set_number];

    if(set.size()<associativity)
    {
        set.push_back(block_address);
        return -1;
    }

    int evicted_address=set.front();
    set.pop_front();
    set.push_back(block_address);
    return evicted_address;
}

void CacheLevel::remove(int req_block_address)
{
    int set_number=req_block_address % numSets;

    deque<int> &set=sets[set_number];

    for(auto it=set.begin() ; it!=set.end() ; it++)
    {
        int block_address=*it;
        if(block_address==req_block_address)
        {
            set.erase(it);
            return;
        }
    }
}

int CacheLevel::getHits() const
{
    return hits;
}

int CacheLevel::getMisses() const
{
    return misses;
}

int CacheLevel::getBlockSize() const
{
    return blockSize;
}


void CacheLevel::resetStats()
{
    hits=0;
    misses=0;
    return;
}
