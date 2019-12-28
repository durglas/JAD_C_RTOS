//*************************************************************************************************
//* Long Jump RTOS Task Struct Setup File
//* By James A. Douglas
//*
//* Sets up task struct and four primary functions for RTOS use
//*   -RTOS_setup(void) gets the task structs ready for use
//*   -task_init(func_t f) initializes a task with the given function as its starting point
//*   -task_run(void) jumps into the task in curr_task and gets things going
//*   -task_remove(void) removes current task from use
//*
//* Also makes BEGIN_ATOMIC and END_ATOMIC macros
//*
//* Honor Code: I have neither given or received, nor have I tolerated others' use
//* of unauthorized aid.
//* --James Douglas
//*************************************************************************************************

#ifndef _dark_budgaw //you were supposed to bring balance to the msp430, not destroy it
#define _dark_budgaw

#include <setjmp.h> //the holy one

#define MAX_TASKS 6 //max number of tasks
#define TASK_SIZE 100 //number of unsigneds in each task stack


/*Typedef function and task*/
typedef void (*func_t)(void);

typedef struct _tmptask { //defines task type struct
   jmp_buf task_buf; 
   func_t funcy;
   unsigned* init_sp; 
}task_t;

//define current and end tasks
extern task_t* curr_task; //current task in use
extern task_t* end_task; //end task in use

/*Define the functions for running the RTOS*/

void RTOS_setup(void); //general setup before RTOS use

void task_init(func_t f); //task initializer

#define task_run() longjmp(curr_task->task_buf,1) //macro to run curr_task

void task_remove(void); //task remover

//atomic section macros
#define BEGIN_ATOMIC _disable_interrupts() // Deactivate interrupts for atomic operation
#define END_ATOMIC _enable_interrupts()  // Reactivate interrupts after atomic operation

#endif
