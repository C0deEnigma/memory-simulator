#include <iostream>
#include <string>
#include "allocator.h"

using namespace std;

int main()
{
    int memorySize;
    cout<<"Enter total memory size: ";
    cin>>memorySize;
    Allocator allocator(memorySize);

    cout<<endl<<"Commands:"<<endl;
    cout<<"  malloc firstFit <size>"<<endl;
    cout<<"  malloc bestFit <size>"<<endl;
    cout<<"  malloc worstFit <size>"<<endl;
    cout<<"  free <id>"<<endl;
    cout << "  dump"<<endl;
    cout << "  stats"<<endl;
    cout << "  exit"<<endl<<endl;

    string cmd;

    while (true)
    {
        cout<< "> ";
        cin>>cmd;

        if(cmd=="malloc")
        {
            string type;
            int size;

            cin>>type>>size;
            int id=-1;

            if(type=="firstFit")
                id=allocator.allocateFirstFit(size);

            else if(type=="bestFit")
                id=allocator.allocateBestFit(size);

            else if(type=="worstFit")
                id=allocator.allocateWorstFit(size);

            else
            {
                cout<<"Unknown allocation type"<<endl;
                continue;
            }


            if(id==-1)
                cout<<"Allocation failed"<<endl;

            else
                cout<<"Allocated block with ID "<<id<<endl;
        }

        else if (cmd=="free")
        {
            int id;
            cin>>id;
            allocator.freeBlock(id);
            cout<<"Freed block ID "<<id<<endl;
        }

        else if (cmd=="dump")
            allocator.dumpMemory();

        else if (cmd=="stats")  
            allocator.printStats();

        else if (cmd=="exit")
            break;

        else
            cout<<"Unknown command"<<endl;
    }

    return 0;
}
