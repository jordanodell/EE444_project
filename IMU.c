#include <bno055.h>
#include "i2c.h"
#include "IMU.h"

//unsigned short addr = BNO055_I2C_ADDR1; // global IMU addr
unsigned char glb_buff[10];  // global buffer 

//TODO list
/*
 create a set addr function ? only needed for multi IMU's
 create a read sys_err function 
*\

//TODO test this 
/* Changes accessible IMU register page */
short bno055_pageid(unsigned short addr,unsigned char *page){
  unsigned char tx_buf[1];
  short resp;

  resp = i2c_tx(addr, page, 1);//set pageid
  return resp;
}

/* This command will check the status of the IMU board reads 0x35 --> 0x39*/
short bno055_status(void){
  unsigned char tx_buf[1] = {BNO055_CALIB_STAT_ADDR};
  unsigned short addr = 0x28;
  short resp;
    
  resp = i2c_txrx(addr ,tx_buf ,1 ,glb_buff ,5);// read status data
  return resp;
}

short bno55_syserr(void){

return 0;
}


