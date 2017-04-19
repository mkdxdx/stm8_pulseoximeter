#include "stm8s.h"
#include "i2c_gen.h"

void I2C_Start() {
  I2C_GenerateSTART(ENABLE);
  while(!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT)) {}
}

void I2C_TXInit(uint8_t address) {
   I2C_Send7bitAddress(address, I2C_DIRECTION_TX);
   while(!I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {}
}
void I2C_RXInit(uint8_t address) {
  I2C_Send7bitAddress(address, I2C_DIRECTION_RX);
  while(!I2C_CheckEvent(I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) {}
}

void I2C_WriteByte(uint8_t byte) {
  I2C_SendData(byte);
  while(!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {}
}

uint8_t I2C_ReadByte(void) {
  while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_RECEIVED)) {}
  uint8_t data = I2C_ReceiveData();
  return data;
}