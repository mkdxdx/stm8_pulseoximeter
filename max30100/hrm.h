// max30100 library to initialize and read data from the chip
// by mkdxdx, use it as you like

#define HRM_ADDRW               0xAE
#define HRM_ADDRD               (HRM_ADDRW)

#define HRM_PARTID              0x11

#define HRM_REG_INTS            0x00
#define HRM_REG_INTEN           0x01
#define HRM_REG_FIFO_WRPT       0x02
#define HRM_REG_OVFC            0x03
#define HRM_REG_FIFO_RDPT       0x04
#define HRM_REG_FIFO_DR         0x05

#define HRM_REG_MODE_CONF       0x06
#define HRM_REG_SPO2_CONF       0x07
#define HRM_REG_LED_CONF        0x09

#define HRM_REG_TEMP_INT        0x16
#define HRM_REG_TEMP_FR         0x17

#define HRM_REG_REVID           0xFE
#define HRM_REG_PARTID          0xFF

// mode config register bits
#define HRM_MODE_SHDN           (1<<7)
#define HRM_MODE_RESET          (1<<6)
#define HRM_MODE_TEMPEN         (1<<3)
#define HRM_MODE_HRONLY         (1<<1)
#define HRM_MODE_SPOEN          (1<<1) | (1<<0)

// SPO2 register bits
#define HRM_SPOC_HIRES          (1<<6)

#define HRM_SPOC_SRC_50         0x00
#define HRM_SPOC_SRC_100        0x01
#define HRM_SPOC_SRC_167        0x02
#define HRM_SPOC_SRC_200        0x03
#define HRM_SPOC_SRC_400        0x04
#define HRM_SPOC_SRC_600        0x05
#define HRM_SPOC_SRC_800        0x06
#define HRM_SPOC_SRC_1000       0x07

// also controls led pulse width
#define HRM_SPOC_ADCRES_13B     0x00            // 200us
#define HRM_SPOC_ADCRES_14B     (1<<0)          // 400us
#define HRM_SPOC_ADCRES_15B     (1<<1)          // 800us
#define HRM_SPOC_ADCRES_16B     (1<<1) | (1<<0) // 1600us

// LED config register bits
#define HRM_LED_IR_C_0        0x00 // 0.0 mA
#define HRM_LED_IR_C_1        0x01 // 4.4 mA
#define HRM_LED_IR_C_2        0x02 // 7.6 mA
#define HRM_LED_IR_C_3        0x03 // 11.0 mA
#define HRM_LED_IR_C_4        0x04 // 14.2 mA
#define HRM_LED_IR_C_5        0x05 // 17.4 mA
#define HRM_LED_IR_C_6        0x06 // 20.8 mA
#define HRM_LED_IR_C_7        0x07 // 24.0 mA
#define HRM_LED_IR_C_8        0x08 // 27.1 mA
#define HRM_LED_IR_C_9        0x09 // 30.6 mA
#define HRM_LED_IR_C_10       0x0A // 33.8 mA
#define HRM_LED_IR_C_11       0x0B // 37.0 mA
#define HRM_LED_IR_C_12       0x0C // 40.2 mA
#define HRM_LED_IR_C_13       0x0D // 43.6 mA
#define HRM_LED_IR_C_14       0x0E // 46.8 mA
#define HRM_LED_IR_C_15       0x0F // 50.0 mA

#define HRM_LED_RD_C_0        (HRM_LED_IR_C_0 << 4)
#define HRM_LED_RD_C_1        (HRM_LED_IR_C_1 << 4)
#define HRM_LED_RD_C_2        (HRM_LED_IR_C_2 << 4)
#define HRM_LED_RD_C_3        (HRM_LED_IR_C_3 << 4)
#define HRM_LED_RD_C_4        (HRM_LED_IR_C_4 << 4)
#define HRM_LED_RD_C_5        (HRM_LED_IR_C_5 << 4)
#define HRM_LED_RD_C_6        (HRM_LED_IR_C_6 << 4)
#define HRM_LED_RD_C_7        (HRM_LED_IR_C_7 << 4)
#define HRM_LED_RD_C_8        (HRM_LED_IR_C_8 << 4)
#define HRM_LED_RD_C_9        (HRM_LED_IR_C_9 << 4)
#define HRM_LED_RD_C_10       (HRM_LED_IR_C_10 << 4)
#define HRM_LED_RD_C_11       (HRM_LED_IR_C_11 << 4)
#define HRM_LED_RD_C_12       (HRM_LED_IR_C_12 << 4)
#define HRM_LED_RD_C_13       (HRM_LED_IR_C_13 << 4)
#define HRM_LED_RD_C_14       (HRM_LED_IR_C_14 << 4)
#define HRM_LED_RD_C_15       (HRM_LED_IR_C_15 << 4)

uint8_t HRM_IsOk(void);
void HRM_Init(void);
void HRM_WriteReg(uint8_t addr, uint8_t data);
uint8_t HRM_ReadReg(uint8_t addr);
uint32_t HRM_ReadFIFO(void);




