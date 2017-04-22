#include "stm8s.h"
#include "oled1306/oled1306.h"
#include "max30100/hrm.h"
#include <stdlib.h>
#include <limits.h>

#define LED_PORT        GPIOD
#define LED_PIN         GPIO_PIN_3

#define I2C_PORT        GPIOB
#define I2C_PIN_SCL     GPIO_PIN_4
#define I2C_PIN_SDA     GPIO_PIN_5

#define I2C_SPEED               200000
#define I2C_SPEED2              100000
#define I2C_SLAVE_ADDRESS7      0xAA    // whatever

#define PLOT_TOP        0x02
#define PLOT_BOTTOM     0x06
#define PLOT_LEFT       0x00
#define PLOT_RIGHT      OLED1306_LCDWIDTH/3
#define PLOT_MAXY       PLOT_BOTTOM*8-1
#define PLOT_MINY       PLOT_TOP*8
#define PLOT_RANGE      32

#define CONV_DIGITS     5  // for unsigned short, 10 for unsigned long

#define SAMPLE_COUNT    4
#define PEAK_THRESHOLD  12

#define AVG_WIDTH       2

#define FILTER_ALPHA    0.9



void Delay(uint16_t nCount);
void I2C_Setup(void);
void convUI16toStr( uint16_t num, char *ptr );
void Screen_Plot(uint8_t x, int16_t, uint16_t range);
void Timer_Reset(void);
void Timer_Config(void);
void Timer_Start(void);
uint16_t Timer_Stop(void);
uint8_t FindPeaks(int16_t v, int16_t range);
void Screen_Peak(uint8_t x);
uint32_t DCFilter(int16_t v, int16_t pv);

const char val[] = "123";
const char hrmok[] = "HRM OK";
const char hrmfail[] = "HRM FAIL";
const char empts[] = "     ";

volatile uint16_t bpm = 0;
volatile int16_t range = 0;
volatile uint16_t scale_m = 0;
volatile int16_t max = SHRT_MIN;
volatile int16_t min = SHRT_MAX;
volatile int16_t moffset = SHRT_MAX;
volatile int16_t prevw = 0;
volatile uint16_t prevt = 0;
volatile uint16_t dt = 0;
volatile int16_t samples[SAMPLE_COUNT];

char u16val[5];

int main( void )
{
  CLK_DeInit();
  CLK_HSICmd(ENABLE);
  CLK_LSICmd(DISABLE);
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
  CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);
  
  GPIO_Init(LED_PORT, LED_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
  
  I2C_Setup();
  
  OLED1306_init();
  OLED1306_Clear();
  
 
  HRM_WriteReg(HRM_REG_MODE_CONF,(HRM_MODE_RESET));
  HRM_WriteReg(HRM_REG_MODE_CONF,(HRM_MODE_HRONLY));
  HRM_WriteReg(HRM_REG_SPO2_CONF,(HRM_SPOC_SRC_50 | HRM_SPOC_ADCRES_16B));
  HRM_WriteReg(HRM_REG_LED_CONF,(HRM_LED_IR_C_5));
  
  uint8_t x = 0;
  
  Timer_Config();
  Timer_Start();
  
  while (1)
  {
    /* Toggles LEDs */
    
    OLED1306_SetPos(0,0);
    
    
    if (HRM_IsOk()) {
      OLED1306_Puts((char*)hrmok);
      
      uint32_t fd = HRM_ReadFIFO();
      
      int16_t ir = (int16_t)((fd & 0xFFFF0000)>>16);
      //int16_t rd = (int16_t)(fd & 0x0000FFFF);
      
      uint32_t dc = DCFilter(ir,prevw);
      
      ir = (int16_t)(0x0000FFFF & dc);
      prevw = (int16_t)(0xFFFF0000 &(dc>>16));
      
      if (ir<min)
        min = ir;
      
      if (ir>max)
        max = ir;
    
      Screen_Plot(x,ir-moffset,range);
      
      
      if (FindPeaks(ir,range) == 1) {
        Screen_Peak(x);
        uint16_t ts = TIM1_GetCounter();
        dt = ts - prevt;
        prevt = ts; 
      }
      
      bpm = (uint16_t)((uint16_t)60000 / dt);
   
      
      x++;
      if (x>PLOT_RIGHT) {
        x = PLOT_LEFT;
        moffset = min;
        range = (max - min);  
        scale_m = range/PLOT_RANGE;
        max = SHRT_MIN;
        min = SHRT_MAX; 
        
        for (uint8_t sy = 0; sy<OLED1306_LCDHEIGHT; sy++) {
          OLED1306_SetPos(64,sy);
          for (uint8_t sx = 64; sx<OLED1306_LCDWIDTH; sx++) {
            OLED1306_Data(0x00);
          }
        }
        convUI16toStr(bpm,u16val);
        OLED1306_Putdigitval(u16val,64,2);
      }
    }
     else
      OLED1306_Puts((char*)hrmfail);
    
    GPIO_WriteReverse(LED_PORT,LED_PIN);
    while (!(TIM1_GetCounter()%30==0)) {} // refresh screen every 30ms~
  }
  return 0;
}


uint32_t DCFilter(int16_t v, int16_t pv) {
  int16_t w = v + FILTER_ALPHA * pv;
  int16_t wn = (w - pv);
  uint32_t result = (uint32_t)(((uint32_t)w<<16)| wn );
  return result;
}



#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
 
  
 
  
  /* Infinite loop */
  while (1)
  {
  }
}
#endif

void I2C_Setup(void) {
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_I2C, ENABLE);
  GPIO_Init(I2C_PORT, (I2C_PIN_SDA | I2C_PIN_SCL), GPIO_MODE_OUT_OD_HIZ_FAST);
#ifdef USE_FULL_ASSERT
  I2C_Init(I2C_SPEED,I2C_SLAVE_ADDRESS7,I2C_DUTYCYCLE_2,I2C_ACK_CURR,I2C_ADDMODE_7BIT,8);
#else
  I2C_Init(I2C_SPEED2,I2C_SLAVE_ADDRESS7,I2C_DUTYCYCLE_2,I2C_ACK_CURR,I2C_ADDMODE_7BIT,16);
#endif
  I2C_Cmd(ENABLE);
}

void Delay(uint16_t nCount)
{
  /* Decrement nCount value */
  while (nCount != 0)
  {
    nCount--;
  }
}



// some unsigned int>str conversion function
void convUI16toStr( uint16_t num, char *ptr )
{
    char digit;
    char buf[ CONV_DIGITS ];

    digit = 0;
    do {
        buf[ digit++ ] = (char)(num % 10) + '0';  // get each digit
    } while ( num /= 10 );     // if the quotient is zero, end the loop

    while( digit > 0 )         // reverse the digit order
        *ptr++ = buf[ --digit ];
    *ptr = '\0';
}


void Screen_Plot(uint8_t x, int16_t yv, uint16_t range) {
  uint8_t sv = (uint8_t)(yv/scale_m);
  uint8_t y = sv+PLOT_MINY;
  
  for (uint8_t py = PLOT_TOP; py<PLOT_BOTTOM; py++) {
    OLED1306_SetPos(x,py);
    OLED1306_Data(0x00);
  }
  
  if (y>PLOT_MAXY) 
    y = PLOT_MAXY;
  if (y<PLOT_MINY) 
    y = PLOT_MINY;
  
  OLED1306_SetPixel(x,y);
}

void Screen_Peak(uint8_t x) {
  for (uint8_t py = PLOT_TOP; py<PLOT_BOTTOM; py++) {
    OLED1306_SetPos(x,py);
    OLED1306_Data(0xFF);
  }
}

void Timer_Reset(void) {
  TIM1_SetCounter(0x0000);
}

void Timer_Config(void) {
  TIM1_DeInit();
  TIM1_TimeBaseInit(16000, TIM1_COUNTERMODE_UP, 0xFFFF, 0xFF);
  TIM1_PrescalerConfig(16000, TIM1_PSCRELOADMODE_IMMEDIATE);
}

void Timer_Start(void) {
  Timer_Reset();
  TIM1_Cmd(ENABLE);
}

uint16_t Timer_Stop(void) {
  TIM1_Cmd(DISABLE);
  return TIM1_GetCounter();
}

uint8_t FindPeaks(int16_t v, int16_t range) {
  for (uint8_t i = 1; i<SAMPLE_COUNT; i++)
    samples[i-1] = samples[i];
  
  samples[SAMPLE_COUNT-1] = v;
  
  int16_t slope = 0;
  for (uint8_t i = 1; i<SAMPLE_COUNT; i++) 
    slope += (samples[i-1] - samples[i]);
  
 
  
  if (slope >= (range/2)) {
    for (uint8_t i = 0; i<SAMPLE_COUNT; i++)
      samples[i] = 0;
    return 1;
  }
    
  else
    return 0;
}