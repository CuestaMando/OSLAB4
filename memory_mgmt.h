#ifndef mem_Mgmt_H
#define mem_Mgmt_H

#include <stdbool.h>


// Memory Allocation Block


struct M_A_B{
    int offset;            //Location of memory block
    int size;              //Size of memory block
    bool allocated;        //If allocated to a current process or not

    //Struct Statements
    struct M_A_B * next;
    struct M_A_B * prev;   //Pointers back and forward
};

typedef struct M_A_B mab;
typedef mab * mabptr;


int getMemoryLeft();

bool memoryCheck(int size);

mabptr AllocMemory(int size);

void FreeMemory(mabptr memory);

mabptr splitMemory(mabptr memory, int size);

bool mergeMemory(mabptr top, mabptr bottom);


#endif