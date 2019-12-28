# JAD_C_RTOS
An All-C, "simplified" RTOS to make the process of context switching and task usage easier than in the original. 

(See JAD_RTOS for the original and faster, if messier, CCS assembly RTOS)

Still designed for use with the MSP430FR6989 Board, though modification of the main and 
interrupt vector mean this could be used on any MSP430 family CPU.

Note that this project makes heavy use of the MSP430 C calling conventions to bypass the undefined behavior of setjmp and longjmp
to/from a function that has already returned from execution. (http://www.ti.com/lit/ug/slau132r/slau132r.pdf)

File Breakdown by purpose/room for modification:

  -main.c contains the starter tasks and clock initialization/running. Modify as necessary for desired tasks
  
  -ISR.c is the interrupt context switcher, this is the heart of the RTOS process
  
  -task.h contains RTOS type definitions and function prototypes
  
  -task.c initializes task memory and contains the core RTOS functions, modify the memory definitions as necessary

For any questions regarding the operation of this software or any of the hacky algorithmic choices,
email james.douglas3@valpo.edu and I would be glad to answer.
