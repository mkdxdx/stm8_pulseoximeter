#include <stdlib.h>
#include <limits.h>


#include "stm8s.h"
#include "oled1306/oled1306.h"
#include "max30100/hrm.h"

#include "filter.h"

#define BATT_FULL       40700          // approx 2.07V on ADIN2
#define BATT_LOW        BATT_FULL*0.6796F // approx 1.4V on ADIN2
#define BATT_SCALE      BATT_FULL-BATT_LOW

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
#define PLOT_RIGHT      OLED1306_LCDWIDTH/2
#define PLOT_MAXY       PLOT_BOTTOM*8-1
#define PLOT_MINY       PLOT_TOP*8
#define PLOT_RANGE      32

#define CONV_DIGITS     5  // for unsigned short, 10 for unsigned long
#define SAMPLE_COUNT    5
#define AVG_WIDTH       2



enum E_PULSE_STATE {
  PS_RISING = 0,
  PS_FALLING
};



void Delay(uint16_t nCount);
void I2C_Setup(void);
void convUI16toStr( uint16_t num, char *ptr );
void Screen_Plot(uint8_t x, int16_t y);
void Timer_Reset(void);
void Timer_Config(void);
void Timer_Start(void);
uint16_t Timer_Stop(void);
uint8_t FindPeaks(int16_t v, int16_t range);
void Screen_Peak(uint8_t x);
void ADC_Config(void);
uint16_t ADC_GetValue(void);
uint8_t getBattery(void);


const char val[] = "123";
const char hrmok[] = "OK";
const char hrmfail[] = "FAIL";
const char empts[] = "     ";

volatile uint16_t prevbpm = 0;
volatile int16_t range = 0;
volatile int16_t rd2 = 0;
volatile int16_t rd3 = 0;
volatile int16_t rd4 = 0;
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
  HRM_WriteReg(HRM_REG_SPO2_CONF,(HRM_SPOC_SRC_1000 | HRM_SPOC_ADCRES_14B));
  HRM_WriteReg(HRM_REG_LED_CONF,(HRM_LED_IR_C_9));
  
  
  uint8_t x = 0;
  
  Timer_Config();
  Timer_Start();
  
  ADC_Config();
  

  
  while (1)
  {
    /* Toggles LEDs */
    
    OLED1306_SetPos(0,0);
    uint8_t status = HRM_IsOk();
    
    if (status) {
      
      
      uint32_t fd = HRM_ReadFIFO();
      
      int16_t ir = (int16_t)((fd & 0xFFFF0000)>>16); 
      
      // red led value omission - until i will figure out a reliable way to measure SPO2
      //int16_t rd = (int16_t)(fd & 0x0000FFFF);
      
      uint32_t dc = DCFilter(ir,prevw);
      
      ir = (int16_t)(0x0000FFFF & dc);
      prevw = (int16_t)(0xFFFF0000 &(dc>>16));
      
      ir = MeanMedian(ir);
      ir = BWorthFilter(ir);
    
      
      if (ir<min)
        min = ir;
      
      if (ir>max)
        max = ir;
    
      
      Screen_Plot(x,ir);
      
      if (FindPeaks(ir,range)) {
        uint16_t ts = TIM1_GetCounter();
        if (ts-prevt>50) {
          Screen_Peak(x);
          dt = ts - prevt;
        }
        

        prevt = ts; 
      }
      
      uint16_t tbpm = (uint16_t)((uint16_t)60000 / dt);
      uint16_t bpm = 0;
      if (tbpm<=300) {
        bpm = (tbpm + prevbpm)>>1;
        prevbpm = bpm;
      }
      
     
      x++;
      if (x>PLOT_RIGHT) {
        x = PLOT_LEFT;
        moffset = min;
        range = (max - min);  
        rd2 = range/2;
        rd3 = range/3;
        rd4 = rd2/2;
        
        max = SHRT_MIN;
        min = SHRT_MAX; 
      
        status = HRM_IsOk();
        if (status) { 
          OLED1306_SetPos(0,0);
          OLED1306_Puts((char*)hrmok); 
        }
        for (uint8_t sy = 0; sy<OLED1306_LCDHEIGHT; sy++) {
          OLED1306_SetPos(64,sy);
          for (uint8_t sx = 64; sx<OLED1306_LCDWIDTH; sx++) {
            OLED1306_Data(0x00);
          }
        }
        convUI16toStr(bpm,u16val);
        OLED1306_Putdigitval(u16val,64,2);
        
        // battery
        OLED1306_SetPos(0,7);
        convUI16toStr(getBattery(),u16val);
        OLED1306_Puts(u16val);
        OLED1306_PutChar('%');
      }
    }
    else {
      OLED1306_Puts((char*)hrmfail);
      status = HRM_IsOk();
    }
      
    
    GPIO_WriteReverse(LED_PORT,LED_PIN);
    while (!(TIM1_GetCounter()%10==0)) {} // every 10 ms 
  }
  return 0;
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


void Screen_Plot(uint8_t x, int16_t yv) {
  int8_t y = 32-(yv-moffset)/3;
 
 
  
  for (uint8_t py = PLOT_TOP; py<PLOT_BOTTOM; py++) {
    OLED1306_SetPos(x,py);
    OLED1306_Data(0x00);
  }
  
  if (y>PLOT_MAXY) 
    y = PLOT_MAXY;
  if (y<PLOT_MINY) 
    y = PLOT_MINY;
  
  if (x%2==0) {
    OLED1306_SetPixel(x,32);
  }
  
  OLED1306_SetPixel(x,y);
}

void Screen_Peak(uint8_t x) {
  OLED1306_SetPos(x,PLOT_TOP+1);
  OLED1306_Data(0xFF);
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

void ADC_Config(void) {
  ADC1_DeInit();
  // single conversion, channel 2, fclk/8, no external trigger, align left, no schmitt trigger,
  ADC1_Init(ADC1_CONVERSIONMODE_SINGLE,ADC1_CHANNEL_2,ADC1_PRESSEL_FCPU_D8, 0,DISABLE, ADC1_ALIGN_LEFT, 0,DISABLE);
  ADC1_Cmd(ENABLE);
}

uint16_t ADC_GetValue(void) {
  ADC1_StartConversion();
  while (!ADC1_GetFlagStatus(ADC1_FLAG_EOC)) {}
  return ADC1_GetConversionValue();
}

uint8_t FindPeaks(int16_t v, int16_t range) {
  for (uint8_t i = 1; i<SAMPLE_COUNT; i++)
    samples[i-1] = samples[i];
  samples[SAMPLE_COUNT-1] = v;
  
  int8_t slope = 0;
  for (uint8_t i = 1; i<SAMPLE_COUNT; i++) {
    if (samples[i-1]<samples[i]) {
      slope++;
    } else {
      slope--;
    }
  }
  
  if (v>=(int16_t)((moffset+rd2+rd4)) && (slope >= SAMPLE_COUNT-2)) {
    return 1;
  } else {
    return 0;
  }
}

// get battery status in %
uint8_t getBattery(void) {
  uint16_t adv = (ADC_GetValue() & 0xFF00);
  float scaled = adv-BATT_LOW;
  float bscal = BATT_SCALE;
  float perc = (scaled/bscal)*100;
  return (uint8_t)perc;
}

