//*************************************************************************************************
//* Long Jump RTOS Hail Mary Attempt
//* By James A. Douglas
//*
//* Yes
//*
//* Honor Code: I have neither given or received, nor have I tolerated others' use
//* of unauthorized aid.
//* --James Douglas
//*************************************************************************************************

#include <msp430.h> 
#include "task.h"

#define ENABLE_PINS 0xFFFE // Enables inputs and outputs
#define SMCLK       0x0200  //Value to set for SMCLK as timer source
#define UP          0x0010  //Value to set for UP counting
#define CLEAR       0x0004  //Value of Clock Clear

void task1(void);
void task2(void);

int main(void){
   WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
   PM5CTL0 = ENABLE_PINS; // Enable inputs and outputs

   P1DIR |= BIT0; //set up red LED for output
   P1OUT &= ~BIT0;
   
   P9DIR |= BIT7; //set up green LED for output
   P9OUT &= ~BIT7;
   
   RTOS_setup();
   
   task_init(task1);
   task_init(task2);
   
   TA0CCR0 = 1000;//1kHZ switches
   TA0CCTL0 |= CCIE;//enable interrupt
   TA0CTL = SMCLK|UP|CLEAR; //start the clock
   
   task_run(); //run starting with the first initialized task
   
}

void task1(void){ //flip red LED every now and then
   int i;
   for(;;){
      for(i=0;i<10000;i++);
      P1OUT^=BIT0;
      task_init(task2); //dynamically make a task, why not
   }
}

void task2(void){ //flip green LED every now and then
   int i;
   for(;;){
      for(i=0;i<20000;i++);
      P9OUT^=BIT7;
      task_remove(); //end task with each blink
   }
}
