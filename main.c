//*****************************************************************************************
//  Using ARC_lib <-- uses port 1
//
//
//*****************************************************************************************
#include <msp430.h> // prune these 
#include <msp430f6779a.h>
#include <ctl.h>
#include <i2c.h>              
#include <Error.h>
#include <terminal.h>      
#include <string.h>           // for memset function
#include <UCA2_uart.h>        // UART setup 

//mutex for crc
extern CTL_MUTEX_t crc_mutex;

CTL_TASK_t terminal_task,idle_task; // name your task (first thing to do when setting up a new task (1))

//********************************************* allocate mem for tasks (2)
//stack for terminal
unsigned terminal_stack[2000];
//unsigned idle_task[2000];

//******************************************** redefine putchar and getchar 
//make printf and friends use async
int __putchar(int c){
  return UCA2_TxChar(c);
}

//make printf and friends use async
int __getchar(void){
  return UCA2_Getc();
}

//******************************************* Example_Bare_bones main loop
void main(void){
// initialize tasking things  
  //create a main task with maximum priority so other tasks can be created without interruption
  //this should be called before other tasks are created
  ctl_task_init(&idle_task, 255, "idle");
  ctl_task_set_priority(&idle_task,0);  // drop to lowest priority to start created tasks running.



  //turn on LED's this will flash the LED's during startup
  P7DIR=0xFF;
  //init complete turn on LED0 and all others off
  P7OUT=0xFF;

  //initialize UART
  UCA2_init_UART(3,5,6);  //UCA2_init_UART(UART_PORT,UART_TX_PIN_NUM,UART_RX_PIN_NUM);
 
  //init I2C on P4.5 SDA and P4.4 SCL
  initI2C(4,5,4);

  // initialize stacks (3) 
  memset(terminal_stack,0xcd,sizeof(terminal_stack));                                           //write known values into the stack 
  terminal_stack[0]=terminal_stack[sizeof(terminal_stack)/sizeof(terminal_stack[0])-1]=0xfeed;  //put marker values at the words before/after the stack

  // creating the tasks
  ctl_task_run(&terminal_task,BUS_PRI_LOW,terminal,"EE444 IMU Project code","terminal",sizeof(terminal_stack)/sizeof(terminal_stack[0])-2,terminal_stack-1,0);

  _EINT();  // set global IR enable 
  LPM0;     // wait in lowpower mode 
}

//decode errors
char *err_decode(char buf[150], unsigned short source,int err, unsigned short argument){
  sprintf(buf,"source = %i, error = %i, argument = %i",source,err,argument);
  return buf;
}

//==============[task library error function]==============

//something went seriously wrong
//perhaps should try to recover/log error
void ctl_handle_error(CTL_ERROR_CODE_t e) __toplevel{
  //a really bad error occurred, reset and report the error 
  while(1){ //TODO replace this with a reset later
   __no_operation(); // for debug break here 
  }
}

