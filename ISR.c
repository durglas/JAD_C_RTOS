//*************************************************************************************************
//* Long Jump RTOS Hail Mary Attempt Context Switcher
//* By James A. Douglas
//*
//* Set up return jump for current task, then jump into the next task.
//* Works as val is 0 on first call, but 1 when we return later.
//*
//* Honor Code: I have neither given or received, nor have I tolerated others' use
//* of unauthorized aid.
//* --James Douglas
//*************************************************************************************************
#include <msp430.h>
#include "task.h" //task structs n such

extern task_t task_arr[MAX_TASKS]; //array of tasks to use
extern task_t* curr_task; //pointer to current task in use
extern task_t* end_task; //end task in use

//**********************************************************************
//* Context Switch Interrupt Service Routine
//* Called on Timer Rollover every ~1/1000 of a second
//**********************************************************************
#pragma vector=TIMER0_A0_VECTOR
__interrupt void nextTask(void){
   
   int val = setjmp(curr_task->task_buf); //set up return jump (val==0)
   
   if(val == 0){
      curr_task++;
      if(curr_task > end_task) curr_task = task_arr; //wrap around if needed
      longjmp(curr_task->task_buf,1); //return into next task (val==1)
   }
}
