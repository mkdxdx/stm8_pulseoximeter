// generic i2c header file
// this header file gives me free ~500b on multi-deviced i2c bus

#define I2C_Stop()      I2C_GenerateSTOP(ENABLE)

void I2C_Start();
void I2C_TXInit(uint8_t address);
void I2C_RXInit(uint8_t address);
void I2C_WriteByte(uint8_t byte);
uint8_t I2C_ReadByte(void);