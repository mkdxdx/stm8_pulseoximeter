#include "stm8s.h"
#include "i2c_gen.h"
#include "hrm.h"

uint8_t HRM_IsOk(void) {
  return (~(HRM_ReadReg(HRM_REG_PARTID) & HRM_PARTID));
}

void HRM_Init(void) {
  
}

void HRM_WriteReg(uint8_t addr, uint8_t data) {
  I2C_Start();
  I2C_TXInit(HRM_ADDRW);
  I2C_WriteByte(addr);
  I2C_WriteByte(data);
  I2C_Stop();
}

uint8_t HRM_ReadReg(uint8_t addr) {
  uint8_t tempcr = I2C->CR2; // if ack setting has been modified somewhere, we store it here
  
  I2C_Start();
  I2C_TXInit(HRM_ADDRW);
  I2C_WriteByte(addr);
  
  I2C_Start();
  I2C_RXInit(HRM_ADDRD);
  I2C->CR2 &= ~I2C_CR2_ACK;
  
  uint8_t data = I2C_ReadByte();
  
  I2C_Stop();
  I2C->CR2 = tempcr;  
  return data;
}


uint32_t HRM_ReadFIFO(void) {
  uint8_t tempcr = I2C->CR2; // if ack setting has been modified somewhere, we store it here
  uint32_t data;
  
  
  I2C_Start();
  I2C_TXInit(HRM_ADDRW);
  I2C_WriteByte(HRM_REG_FIFO_DR);
  I2C_Start();
  I2C_RXInit(HRM_ADDRD);
  
  for (uint8_t i=0; i<4; i++) {
    if (i>=3) I2C->CR2 &= ~I2C_CR2_ACK; // and because NACK is required after byte read we generate it
    data = (data<<8) | I2C_ReadByte();
  }
  I2C_Stop();
  
  I2C->CR2 = tempcr; 
  
 

  return data;
}