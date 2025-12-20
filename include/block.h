// Defines memory blocks which are dynamic in size

#ifndef BLOCK_H
#define BLOCK_H

struct Block {
    int start;   // starting address of the block
    int size;    // size of the block
    bool free;   // true = free, false = allocated
    int id;      // allocation id, -1 if free
};

#endif
