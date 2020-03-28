//
// Created by 100652479 on 3/27/2020.
//

#include <stdio.h>
#include <stdlib.h>
#include "memory_mgmt.h"
#include "process_mgmt.h"
#include "queue.h"

#define DEBUG_MEMORY false	//Debug flag specific to this file
#define TOTAL_MEMORY 1024	//Total amount of memory in the system

int mRemainingMem = TOTAL_MEMORY;	//Amount of free memory in the system
mabptr mFirstBlock = NULL;			//Pointer to the first block in memory

//Initializes empty memory block and returns a pointer to it. Internal to this module.
mabptr getNewMemBlock() {
    mabptr memBlock = malloc(sizeof(mab));
    memBlock->next = NULL;
    memBlock->prev = NULL;
    memBlock->allocated = false;
    memBlock->size = 0;
    memBlock->offset = 0;

    return memBlock;
}

bool memoryCheck(int size) {
    bool canBeAllocated = false;

    //If this is the first time allocating memory, initialize the memory.
    if (mRemainingMem == TOTAL_MEMORY && mFirstBlock == NULL) {
        mFirstBlock = getNewMemBlock();//Initially, the whole memory consists of a single block.
        mFirstBlock->size = TOTAL_MEMORY;
        mFirstBlock->offset = 0;
    }
    mabptr head = mFirstBlock;	//Pointer used to traverse the list of mem. blocks

    /*Traverse the list of mem blocks until an available large enough memory block is found
       or the end of memory is reached.*/
    while(head != NULL && ( (!head->allocated && head->size < size) ||
                            head->allocated )){
        /*If the end of memory is reached, return NULL to indicate allocation failure.*/
        if( (head->offset + head->size) > TOTAL_MEMORY) {
            canBeAllocated = false;
            break;
        }
        head = head->next;
    }

    if(head != NULL)
        canBeAllocated = true;
    else
        canBeAllocated = false;

    return canBeAllocated;
}

//Allocate memory block
mabptr AllocMemory(int size){
    //If this is the first time allocating memory, initialize the memory.
    if (mRemainingMem == TOTAL_MEMORY && mFirstBlock == NULL) {
        mFirstBlock = getNewMemBlock();//Initially, the whole memory consists of a single block.
        mFirstBlock->size = TOTAL_MEMORY;
        mFirstBlock->offset = 0;
    }

    /*Make sure there is enough memory left in the system*/
    if (mRemainingMem < size) {
        if(DEBUG)
            printf("\tSystem out of memory!!\n");
        return NULL;
    }

    mabptr head = mFirstBlock;	//Pointer used to traverse the list of mem. blocks
    mabptr memory = NULL;		//Pointer to memory being allocated

    /*Traverse the list of mem blocks until an available large enough memory block is found
       or the end of memory is reached.*/
    while(head != NULL && ( (!head->allocated && head->size < size) ||
                            head->allocated )){
        /*If the end of memory is reached, return NULL to indicate allocation failure.*/
        if( (head->offset + head->size) > TOTAL_MEMORY)
            return NULL;
        head = head->next;
    }

    /*If head == NULL, then the end of memory has been reached.*/
    if (head != NULL) {
        if(!head->allocated && head->size > size) {
            splitMemory(head, size); //Reallocate memory block to new process and resize it
            memory = head;
            memory->allocated = true;
            mRemainingMem -= size;
        } else { //head->size == size
            memory = head;
            memory->allocated = true;
            mRemainingMem -= memory->size;
        }
    }

    return memory;
}

//Free memory block
void FreeMemory(mabptr memory){
    if(memory == NULL)
        return;

    if(DEBUG_MEMORY)
        printf("\tFreed memory: offset = %d  mem = %dMB\n", memory->offset, memory->size);

    memory->allocated = false;
    mRemainingMem += memory->size;

    /*If previous block is free, merge them and set the pointer to point to
      the merged block*/
    if(memory->prev != NULL && !memory->prev->allocated) {
        mergeMemory(memory->prev, memory);
        memory = memory->prev;
    }

    /*If next block is free, merge them*/
    if(memory->next != NULL && !memory->next->allocated) {
        mergeMemory(memory, memory->next);
    }
}

//Splits block into two. Returns pointer to leftover block
mabptr splitMemory(mabptr memory, int size) {
    if(memory == NULL) {
        if(DEBUG)
            printf("\tERROR - Passed NULL pointer to mem_split()\n");
        return NULL;
    }

    int originalSize = memory->size;
    memory->size = size;

    mabptr leftover = NULL;	//Pointer to block of memory left over after the allocation

    if (memory->size < originalSize) {
        /*Create a new block for the leftover memory and redirect the pointers*/
        leftover = getNewMemBlock();
        leftover->allocated = false;
        leftover->size = originalSize - memory->size;
        leftover->offset = memory->offset + memory->size;
        leftover->prev = memory;
        leftover->next = memory->next;
        memory->next = leftover;
    }
    return leftover;
}

//Returns true if merge was successful. False otherwise.
bool mergeMemory(mabptr top, mabptr bottom) {
    if(top == NULL || bottom == NULL) {
        if(DEBUG)
            printf("\tERROR - Passed NULL pointer to mem_merge()\n");
        return false;
    }

    /*Redirect pointers*/
    top->next = bottom->next;
    if(bottom->next != NULL)
        bottom->next->prev = top;

    /*Free memory*/
    top->size += bottom->size;
    free(bottom);
    bottom = NULL;
    return true;
}