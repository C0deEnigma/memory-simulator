#include "allocator.h"
#include<iostream>
#include <climits>
#include <iterator>

using namespace std;

Allocator::Allocator(int size) 
{
    totalSize=size;
    nextId=1;                 // Starting with ID=1
    allocRequests=0;          // Initially the allocation requests are zero
    allocFailures=0;           // Initially the allocation failures are zero


    Block initial_block;          // Creat an initial_block block of entire memory
    initial_block.start=0;      // Memory starts at zero, we use integer adress since it is a simulation not real memory
    initial_block.size=size;    // initial_block block will have the size of entire memory
    initial_block.free=true;    // This block is not in use currently
    initial_block.id=-1;        // This block is free

    blocks.push_back(initial_block);  // Insert the initial_block block into blocks (linked list)
}



// Traverse blocks from beginning
// Find first free block with size >= requested size
// If exact size: mark allocated
// If larger: split into allocated + free block
// Assign unique ID to allocated block
// Return allocation ID
// If no block fits, return -1

int Allocator::allocateFirstFit(int req_size) 
{
    allocRequests++;
    if (req_size <= 0)
    {
        allocFailures++; 
        return -1;
    }
    for(auto it=blocks.begin() ; it!=blocks.end() ; ++it)
    {
        if(it->free)
        {
            if(it->size==req_size)
            {
                it->free=false;
                int newId=nextId++;
                it->id=newId;
                return newId;
            }
            else if(it->size > req_size)
            {
                Block remaining;
                remaining.start=it->start+req_size;
                remaining.size=it->size-req_size;
                remaining.free=true;
                remaining.id=-1;
                blocks.insert(next(it), remaining);

                it->free=false;
                it->size=req_size;
                int newId=nextId++;
                it->id=newId;
                return newId;
            }
        }
    }
    allocFailures++; 
    return -1; // placeholder
}



// Traverse ALL free blocks
// Choose the smallest free block that can fit requested size
// Allocation mechanics SAME as First Fit

int Allocator::allocateBestFit(int req_size) 
{
    allocRequests++;
    if (req_size <= 0)
    { 
        allocFailures++; 
        return -1;
    }

    auto best_it=blocks.end();
    int min_remaining=INT_MAX;
    for(auto it=blocks.begin() ; it!=blocks.end() ; ++it)
    {
        if(it->free)
        {
            int remaining=it->size-req_size;
            if(remaining>=0 && remaining<min_remaining)
            {
                min_remaining=remaining;
                best_it=it;
            }
        }
    }
    if(best_it!=blocks.end())
    {
        if(min_remaining==0)
        {
            best_it->free=false;
            int newId=nextId++;
            best_it->id=newId;
            return newId;
        }
        else
        {
            Block remaining;
            remaining.start=best_it->start+req_size;
            remaining.size=min_remaining;
            remaining.free=true;
            remaining.id=-1;
            blocks.insert(next(best_it), remaining);

            best_it->free=false;
            best_it->size=req_size;
            int newId=nextId++;
            best_it->id=newId;
            return newId;
        }
    }
    allocFailures++; 
    return -1; // placeholder
}



// - Traverse ALL free blocks
// - Choose the largest free block
// - Allocation mechanics SAME as First Fit

int Allocator::allocateWorstFit(int req_size) 
{
    allocRequests++;
    if (req_size <= 0) 
    {
        allocFailures++; 
        return -1;
    }

    auto worst_it = blocks.end();
    int max_remaining = INT_MIN;
    for(auto it=blocks.begin() ; it!=blocks.end() ; ++it)
    {
        if(it->free)
        {
            int remaining=it->size-req_size;
            if(remaining>=0 && remaining>max_remaining)
            {
                max_remaining=remaining;
                worst_it=it;
            }
        }
    }
    if(worst_it!=blocks.end())
    {
        if(max_remaining==0)
        {
            worst_it->free=false;
            int newId=nextId++;
            worst_it->id=newId;
            return newId;
        }
        else
        {
            Block remaining;
            remaining.start=worst_it->start+req_size;
            remaining.size=max_remaining;
            remaining.free=true;
            remaining.id=-1;
            blocks.insert(next(worst_it), remaining);

            worst_it->free=false;
            worst_it->size=req_size;
            int newId=nextId++;
            worst_it->id=newId;
            return newId;
        }
    }
    allocFailures++; 
    return -1; // placeholder
}



// Find block with given ID
// Mark it as free
// Reset ID
// Coalesce adjacent free blocks

void Allocator::freeBlock(int id) 
{
    for(auto it=blocks.begin() ; it!=blocks.end() ; ++it)
    {
        if(it->id==id) 
        {
            it->id=-1;
            it->free=true;
            coalesce(it);
            break;
        }
    }
    // ID not found → Ignore
}



// Merge free blocks
// Check neighboring blocks of given block and merge them if they are free 

void Allocator::coalesce(list<Block>::iterator it) 
{
    // Merge with previous block FIRST
    if(it!=blocks.begin())
    {
        auto prev_it=prev(it);
        if(prev_it->free)
        {
            prev_it->size += it->size;
            blocks.erase(it);           // Erase current it
            it=prev_it;                 // Restore it to prev it (leftmost)
        }
    }
    // Merge with next block
    auto next_it=next(it);
    if(next_it!=blocks.end() && next_it->free)
    {
        it->size += next_it->size;
        blocks.erase(next_it);
    }

}



// Print current memory layout
// Show address range and FREE / USED with ID

void Allocator::dumpMemory() 
{
    cout<<"Memory Dump"<<endl;

    for(auto &block:blocks)
    {
        cout<<"["<<block.start<<" - "<<block.start+block.size-1<<"] ";

        if(block.free) 
            cout<<"FREE"<<endl;
        else 
            cout<<"USED "<<"(ID = "<<block.id<<")"<<endl;
    }
}

//
void Allocator::printStats()
{
    cout<<"Internal Fragmentation  = 0"<<endl; // Internal Fragmentation is zero in variable partitioning
    int free_size=0;
    int largest_free_size=0;
    for(auto &block:blocks)
        if(block.free)
        {
            free_size+=block.size;
            largest_free_size=max(largest_free_size,block.size);
        }

    cout<<"Total Memory = "<<totalSize<<endl;
    cout<<"Total Memory Used = "<<totalSize-free_size<<endl;
    cout<<"Memory Utilization = "<<(double)(totalSize-free_size)/totalSize*100<<"%"<<endl;

    if(free_size>0)
        cout<<"External Fragmentation = "<<(double)(free_size-largest_free_size)/free_size*100<<"%"<<endl;
    else
        cout<<"External Fragmentation = 0%"<<endl;

    if(allocRequests>0)
        cout<<"Allocation Failure Rate = "<<(double)(allocFailures)/allocRequests*100<<"%"<<endl;  // To avoid ZeroDivisionEror
    else 
        cout<<"Allocation Failure Rate = 0%"<<endl;
}
