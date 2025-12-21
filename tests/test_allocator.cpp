#include <iostream>
#include "allocator.h"

using namespace std;

void test_first_fit()
{
    cout<<"========== TEST: First Fit =========="<<endl;

    Allocator a(1000);

    a.allocateFirstFit(200);
    a.allocateFirstFit(100);
    a.allocateFirstFit(200);
    a.allocateFirstFit(90);
    a.allocateFirstFit(200);
    a.freeBlock(2);
    a.freeBlock(4);
    a.dumpMemory();
    a.allocateFirstFit(80);
    a.dumpMemory();

    cout<<endl;
}

void test_best_fit()
{
    cout<<"========== TEST: Best Fit =========="<<endl;

    Allocator a(1000);

    a.allocateFirstFit(200);
    a.allocateFirstFit(100);
    a.allocateFirstFit(200);
    a.allocateFirstFit(90);
    a.allocateFirstFit(200);
    a.freeBlock(2);
    a.freeBlock(4);
    a.dumpMemory();
    a.allocateBestFit(80);
    a.dumpMemory();

    cout<<endl;
}

void test_worst_fit()
{
    cout<<"========== TEST: Worst Fit =========="<<endl;

    Allocator a(1000);

    a.allocateFirstFit(200);
    a.allocateFirstFit(100);
    a.allocateFirstFit(200);
    a.allocateFirstFit(90);
    a.allocateFirstFit(200);
    a.freeBlock(2);
    a.freeBlock(4);
    a.dumpMemory();
    a.allocateWorstFit(80);
    a.dumpMemory();

    cout<<endl;
}

void test_free_and_coalesce()
{
    cout<<"========== TEST: Free and Coalescing =========="<<endl;

    Allocator a(1000);

    a.allocateFirstFit(200);
    a.allocateFirstFit(100);
    a.allocateFirstFit(200);
    a.allocateFirstFit(90);
    a.allocateFirstFit(200);
    a.freeBlock(1);
    a.freeBlock(3);
    a.dumpMemory();
    a.freeBlock(2);
    a.dumpMemory();

    cout<<endl;
}

void test_fragmentation()
{
    cout<<"========== TEST: Fragmentation =========="<<endl;

    Allocator a(1000);

    a.allocateFirstFit(200);
    a.allocateFirstFit(100);
    a.allocateFirstFit(200);
    a.allocateFirstFit(90);
    a.allocateFirstFit(200);
    a.freeBlock(1);
    a.freeBlock(3);
    a.dumpMemory();
    a.printStats();

    cout<<endl;
}

void test_allocation_failure()
{
    cout<<"========== TEST: Allocation Failure =========="<<endl;

    Allocator a(1000);

    a.allocateFirstFit(200);
    a.allocateFirstFit(100);
    a.allocateFirstFit(200);
    a.allocateFirstFit(90);
    a.allocateFirstFit(200);
    a.freeBlock(1);
    a.freeBlock(3);
    a.dumpMemory();
    a.allocateFirstFit(250);
    a.printStats();
    a.allocateFirstFit(0);
    a.printStats();

    cout<<endl;
}

int main()
{
    cout<<"Running Allocator Tests"<<endl<<endl;

    test_first_fit();
    test_best_fit();
    test_worst_fit();
    test_free_and_coalesce();
    test_fragmentation();
    test_allocation_failure();

    cout<<"All tests executed"<<endl;
    return 0;
}
