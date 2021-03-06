#include "stm8s.h"
#include "i2c_gen.h"

void I2C_Start() {
  uint16_t w = (uint16_t)-1;
  I2C_GenerateSTART(ENABLE);
  while(!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT) && (w)) {w--;}
}

void I2C_TXInit(uint8_t address) {
   uint16_t w = (uint16_t)-1;
   I2C_Send7bitAddress(address, I2C_DIRECTION_TX);
   while(!I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) && (w)) {w--;}
}
void I2C_RXInit(uint8_t address) {
  uint16_t w = (uint16_t)-1;
  I2C_Send7bitAddress(address, I2C_DIRECTION_RX);
  while(!I2C_CheckEvent(I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) && (w)) {w--;}
}

void I2C_WriteByte(uint8_t byte) {
  uint16_t w = (uint16_t)-1;
  I2C_SendData(byte);
  while(!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED) && (w)) {w--;}
}

uint8_t I2C_ReadByte(void) {
  uint16_t w = (uint16_t)-1;
  while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_RECEIVED) && (w)) {w--;}
  uint8_t data = I2C_ReceiveData();
  return data;
}