//
// Created by 100652479 on 3/27/2020.
//

#ifndef PROCESS_MGMT_H
#define PROCESS_MGMT_H

#include "queue.h"
#include "util.h"

#define TOTAL_MEM     1024
#define RESERVED_MEM  64
#define NUM_PRINTERS 2
#define NUM_SCANNERS 1
#define NUM_MODEMS 1
#define NUM_CDS   2

enum indices{arrival_time, priority, cpu_time, memory_alloc, num_printers, num_scanners, num_modems, num_cds};

pcbptr create_pcb();										//Create Process
bool areEmptyQueues();                                       //Checks if Queues are empty
bool ReqRes_check(int printers, int scanners, int modems, int cds); //Returns true if the requested resources are available
bool ReqRes_alloc (pcbptr process, int printers,int scanners, int modems, int cds);
void start_process(pcbptr process);								//Starts process.
void restart_process(pcbptr process);							//Restarts suspended process.
void suspend_process(pcbptr process);							//Suspends process
void kill_process(pcbptr process); 								//Terminates process and frees its resoures.
void init_dispatcher(FILE *file);								//Initializes dispatchers
void start_dispatcher();											//Starts the process dispatcher
void init_process(pcbptr process, int * processInfo);		//Initializes process block
void placeInQueue(pcbptr process);								//Adds process to appropriate queue based on its priority level.


#endif //UNTITLED2_PROCESS_MGMT_H
