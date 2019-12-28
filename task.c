//*************************************************************************************************
//* Long Jump RTOS Task Function Completion
//* By James A. Douglas
//*
//* Fills in the three remaining functions in task.h
//* And creates/manages task array and stack
//*
//* Honor Code: I have neither given or received, nor have I tolerated others' use
//* of unauthorized aid.
//* --James Douglas
//*************************************************************************************************
#include <msp430.h>
#include "task.h"

#pragma PERSISTENT(task_stack)
unsigned task_stack[MAX_TASKS*TASK_SIZE] = {0}; //initialize array for stacks

#pragma PERSISTENT(task_arr)
task_t task_arr[MAX_TASKS] = {0}; //array of tasks to use

task_t* curr_task = task_arr; //current task in use
task_t* end_task = task_arr - 1; //end task in use

//set up tasks to be usable
void RTOS_setup(void){
   unsigned i;
   for(i=0;i<MAX_TASKS;i++){
      task_arr[i].init_sp = task_stack + TASK_SIZE*(i+1); //load initial stack pointer in
   }
}

//load task and add it to array
void task_init(func_t f){
   BEGIN_ATOMIC; 
   
   if(end_task - task_arr < MAX_TASKS - 1){ //if there's room to add another task

      end_task++;
      end_task->funcy = f; //store function for later calling (setjmp doesn't preserve R11-R15)

      int val = setjmp(end_task->task_buf);
      
      //potential errata notice: since this function returns before longjmp comes back,
      //some stuff earlier on the stack may have changed (aka "stack unwinding")
      //since the following code literally changes the stack location as its first action (and val is R12)...
      //we good
      
      if(val!=0){ //when called, set up task and get going
         __set_SP_register(curr_task->init_sp);
         END_ATOMIC;
         (*(curr_task->funcy))(); //call task function
      }

   }
   
   END_ATOMIC; 
}

//remove current task, then run new current task to get things rolling again
void task_remove(void){
    BEGIN_ATOMIC;

    if(end_task == task_arr)for(;;); //infinite loop if last task removed

    if(curr_task!=end_task){
        /*swap end and current task*/
        task_t tmp = *end_task;
        *end_task = *curr_task;
        *curr_task = tmp;

        end_task--; //and move end to the left one
    }
    else{ //special case when removing the end
        curr_task = task_arr;
        end_task--;
    }
    task_run(); //jump into new current task
}
