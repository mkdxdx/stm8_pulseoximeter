#include "stm8s.h"
#include "i2c_gen.h"
#include "oled1306.h"


#define OLED1306_Sleep		OLED1306_Write(OLED1306_CMD, OLED1306_DISPLAYOFF)
#define OLED1306_PWROn 		OLED1306_Write(OLED1306_CMD, OLED1306_DISPLAYON)

#define OLED1306_Normal		OLED1306_Write(OLED1306_CMD,  OLED1306_NORMALDISPLAY)
#define OLED1306_Invert		OLED1306_Write(OLED1306_CMD,  OLED1306_INVERTDISPLAY)

const uint8_t font6[] = {
	0x00,0x00,0x00,0x00,0x00,0x00,	// space
        0x00,0x00,0xDF,0xDF,0x00,0x00,	// !
        0x00,0x03,0x00,0x03,0x00,0x00,	// "
        0x24,0x7E,0x24,0x7E,0x24,0x00,	// #
        0x00,0x46,0x89,0xFF,0x91,0x62,	// $
        0x06,0x46,0x30,0x0C,0x62,0x60,	// %
        0x40,0xAC,0x92,0x6A,0x64,0x80,	// &
        0x00,0x00,0x00,0x03,0x00,0x00,	// '
        0x00,0x00,0x7E,0x81,0x00,0x00,	// (
        0x00,0x00,0x81,0x7E,0x00,0x00,	// )
        0x42,0x24,0xFF,0x24,0x42,0x00,	// *
        0x08,0x08,0x3E,0x08,0x08,0x00,	// +
        0x00,0xC0,0x40,0x00,0x00,0x00,	// ,
        0x08,0x08,0x08,0x08,0x08,0x00,	// -
        0x00,0xC0,0xC0,0x00,0x00,0x00,	// .
        0x00,0x60,0x18,0x06,0x00,0x00,	// /
        0x3E,0x41,0x41,0x41,0x3E,0x00,	// 0
        0x00,0x42,0x7F,0x40,0x00,0x00,	// 1
        0x46,0x61,0x51,0x49,0x46,0x00,	// 2
        0x22,0x41,0x49,0x49,0x36,0x00,	// 3
        0x0F,0x08,0x08,0x08,0x7F,0x00,	// 4
        0x27,0x45,0x45,0x45,0x39,0x00,	// 5
        0x3E,0x49,0x49,0x49,0x32,0x00,	// 6
        0x01,0x41,0x31,0x0D,0x03,0x00,	// 7
        0x36,0x49,0x49,0x49,0x36,0x00,	// 8
        0x06,0x49,0x29,0x19,0x0E,0x00,	// 9
        0x00,0x36,0x36,0x00,0x00,0x00,	// :
        0x00,0x36,0x16,0x00,0x00,0x00,	// ;
        0x00,0x08,0x14,0x22,0x00,0x00,	// <
        0x14,0x14,0x14,0x14,0x00,0x00,	// =
        0x00,0x22,0x14,0x08,0x00,0x00,	// >
        0x02,0x01,0x51,0x09,0x06,0x00,	// ?
        0x3E,0x59,0x65,0x59,0x2E,0x00,	// @
        0x7C,0x0A,0x09,0x09,0x7F,0x00,	// a
        0x7F,0x49,0x49,0x49,0x36,0x00,	// b
        0x3E,0x41,0x41,0x41,0x22,0x00,	// c
        0x7F,0x41,0x41,0x41,0x3E,0x00,	// d
        0x7F,0x49,0x49,0x49,0x49,0x00,	// e
        0x7F,0x09,0x09,0x09,0x09,0x00,	// f
        0x3E,0x41,0x49,0x49,0x3A,0x00,	// g
        0x7F,0x08,0x08,0x08,0x7F,0x00,	// h
        0x00,0x41,0x7F,0x41,0x00,0x00,	// i
        0x20,0x40,0x40,0x40,0x3F,0x00,	// j
        0x7F,0x08,0x14,0x22,0x41,0x00,	// k
        0x7F,0x40,0x40,0x40,0x40,0x00,	// l
        0x7F,0x02,0x04,0x02,0x7F,0x00,	// m
        0x7F,0x02,0x04,0x08,0x7F,0x00,	// n
        0x3E,0x41,0x41,0x41,0x3E,0x00,	// o
        0x7F,0x09,0x09,0x09,0x06,0x00,	// p
        0x3E,0x41,0x51,0x61,0x3E,0x00,	// q
        0x7F,0x19,0x29,0x49,0x06,0x00,	// r
        0x26,0x49,0x49,0x49,0x32,0x00,	// s
        0x01,0x01,0x7F,0x01,0x01,0x00,	// t
        0x3F,0x40,0x40,0x40,0x3F,0x00,	// u
        0x1F,0x20,0x40,0x20,0x1F,0x00,	// v
        0x3F,0x40,0x3F,0x40,0x3F,0x00,	// w
        0x71,0x0A,0x04,0x0A,0x71,0x00,	// x
        0x07,0x48,0x48,0x48,0x3F,0x00,	// y
        0x61,0x51,0x49,0x45,0x43,0x00,	// z
        0x00,0x7F,0x41,0x41,0x00,0x00,	// [
        0x00,0x06,0x18,0x60,0x00,0x00,	// backslash
        0x00,0x41,0x41,0x7F,0x00,0x00,	// ]
        0x04,0x02,0x01,0x02,0x04,0x00,	// ^ 
        0x80,0x80,0x80,0x80,0x80,0x80,	// _
        0x00,0x00,0x00,0x03,0x00,0x00	// '
};


const uint8_t digits[] = {
	0x80,0xF0,0xF8,0x7C,0x1E,0x0E,	
        0x0E,0x0E,0x0E,0x0E,0x1E,0x7C,	
        0xF8,0xF0,0x80,0x00,0xFF,0xFF,	
        0xFF,0x00,0x00,0x00,0x00,0x00,	
        0x00,0x00,0x00,0x00,0xFF,0xFF,	
        0xFF,0x00,0x01,0x0F,0x1F,0x3E,	
        0x78,0x70,0x70,0x70,0x70,0x70,	
        0x78,0x3E,0x1F,0x0F,0x01,0x00,	// 0
        0x00,0x00,0xC0,0xC0,0xE0,0x70,	
        0x38,0xFC,0xFE,0xFE,0x00,0x00,	
        0x00,0x00,0x00,0x00,0x00,0x00,	
        0x01,0x00,0x00,0x00,0x00,0xFF,	
        0xFF,0xFF,0x00,0x00,0x00,0x00,	
        0x00,0x00,0x00,0x00,0x00,0x00,	
        0x00,0x00,0x00,0x7F,0x7F,0x7F,	
        0x00,0x00,0x00,0x00,0x00,0x00,	// 1
        0x60,0x78,0x7C,0x1C,0x1E,0x0E,	
        0x0E,0x0E,0x0E,0x1E,0x3C,0xFC,	
        0xF8,0xE0,0x00,0x00,0x00,0x00,	
        0x00,0x00,0x00,0x80,0xC0,0xE0,	
        0xF0,0x78,0x3C,0x1F,0x07,0x03,	
        0x00,0x00,0x60,0x78,0x7C,0x7E,	
        0x77,0x73,0x71,0x70,0x70,0x70,	
        0x70,0x70,0x70,0x70,0x00,0x00,	// 2
        0x60,0x70,0x7C,0x3C,0x1E,0x0E,	
        0x0E,0x0E,0x0E,0x0E,0x1C,0xFC,	
        0xF8,0xF0,0x00,0x00,0x00,0x00,	
        0x00,0x00,0x00,0x00,0x0E,0x0E,	
        0x0E,0x0F,0x1F,0x3F,0xF9,0xF8,	
        0xE0,0x00,0x06,0x1E,0x3E,0x3C,	
        0x78,0x70,0x70,0x70,0x70,0x70,	
        0x38,0x3C,0x1F,0x0F,0x03,0x00,	// 3
        0x00,0x00,0x00,0x00,0x00,0x00,	
        0x80,0xE0,0xF0,0x78,0xFC,0xFE,	
        0xFE,0x00,0x00,0x00,0xC0,0xE0,	
        0xF0,0xB8,0x9E,0x8F,0x87,0x81,	
        0x80,0x80,0xFF,0xFF,0xFF,0x80,	
        0x80,0x00,0x03,0x03,0x03,0x03,	
        0x03,0x03,0x03,0x03,0x03,0x03,	
        0x7F,0x7F,0x7F,0x03,0x03,0x00,	// 4
        0x00,0xF0,0xFE,0xFE,0x0E,0x0E,	
        0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,	
        0x0E,0x0E,0x00,0x00,0x0E,0x0F,	
        0x0F,0x0D,0x06,0x07,0x07,0x07,	
        0x07,0x07,0x0F,0x1E,0xFC,0xF8,	
        0xF0,0x00,0x06,0x1E,0x3E,0x3C,	
        0x78,0x70,0x70,0x70,0x70,0x70,	
        0x38,0x3C,0x1F,0x0F,0x03,0x00,	// 5
        0x00,0xE0,0xF8,0xF8,0x3C,0x1E,	
        0x0E,0x0E,0x0E,0x0E,0x1E,0x3C,	
        0x7C,0x78,0x60,0x00,0xFF,0xFF,	
        0xFF,0x38,0x1C,0x0C,0x0E,0x0E,	
        0x0E,0x0E,0x1E,0x3C,0xFC,0xF8,	
        0xE0,0x00,0x01,0x0F,0x1F,0x3C,	
        0x38,0x70,0x70,0x70,0x70,0x70,	
        0x78,0x3C,0x1F,0x0F,0x07,0x00,	// 6
        0x00,0x0E,0x0E,0x0E,0x0E,0x0E,	
        0x0E,0x0E,0x0E,0x8E,0xCE,0xEE,	
        0x7E,0x1E,0x0E,0x00,0x00,0x00,	
        0x00,0x00,0x00,0x80,0xE0,0xF8,	
        0x7E,0x1F,0x03,0x01,0x00,0x00,	
        0x00,0x00,0x00,0x00,0x00,0x00,	
        0x78,0x7F,0x7F,0x07,0x00,0x00,	
        0x00,0x00,0x00,0x00,0x00,0x00,	// 7
        0x00,0xF0,0xF8,0xFC,0x1E,0x0E,	
        0x0E,0x0E,0x0E,0x0E,0x1C,0xFC,	
        0xF8,0xF0,0x00,0x00,0xC0,0xF0,	
        0xF9,0x3B,0x1F,0x0E,0x0E,0x0E,	
        0x0E,0x0E,0x1F,0x3B,0xF9,0xF0,	
        0xC0,0x00,0x07,0x0F,0x1F,0x3C,	
        0x78,0x70,0x70,0x70,0x70,0x70,	
        0x78,0x3C,0x1F,0x0F,0x07,0x00,	// 8
        0xE0,0xF0,0xF8,0x3C,0x1E,0x0E,	
        0x0E,0x0E,0x0E,0x0E,0x1C,0x3C,	
        0xF8,0xF0,0x80,0x00,0x07,0x1F,	
        0x3F,0x3C,0x78,0x70,0x70,0x70,	
        0x70,0x30,0x38,0x1C,0xFF,0xFF,	
        0xFF,0x00,0x06,0x1E,0x3E,0x3C,	
        0x78,0x70,0x70,0x70,0x70,0x78,	
        0x38,0x1F,0x1F,0x07,0x00,0x00	// 9
};



void OLED1306_init(void)
{
  OLED1306_Cmd( OLED1306_DISPLAYOFF);
  OLED1306_Cmd( OLED1306_SETDISPLAYCLOCKDIV);
  OLED1306_Cmd( 0x80);
  OLED1306_Cmd( OLED1306_SETMULTIPLEX);
  OLED1306_Cmd( 0x3F);
  OLED1306_Cmd( OLED1306_SETDISPLAYOFFSET);
  OLED1306_Cmd( 0x00);
  OLED1306_Cmd( OLED1306_SETSTARTLINE | 0x00);
    // We use internal charge pump
  OLED1306_Cmd( OLED1306_CHARGEPUMP);
  OLED1306_Cmd( 0x14);
    // Horizontal memory mode
  OLED1306_Cmd( OLED1306_MEMORYMODE);
  OLED1306_Cmd( 0x00);
  OLED1306_Cmd( OLED1306_SEGREMAP | 0x1);
  OLED1306_Cmd( OLED1306_COMSCANDEC);
  OLED1306_Cmd( OLED1306_SETCOMPINS);
  OLED1306_Cmd( 0x12);
    // Max contrast
  OLED1306_Cmd( OLED1306_SETCONTRAST);
  OLED1306_Cmd( 0xCF);
  OLED1306_Cmd( OLED1306_SETPRECHARGE);
  OLED1306_Cmd( 0xF1);
  OLED1306_Cmd( OLED1306_SETVCOMDETECT);
  OLED1306_Cmd( 0x40);
  OLED1306_Cmd( OLED1306_DISPLAYALLON_RESUME);
    // Non-inverted display
  OLED1306_Cmd(  OLED1306_NORMALDISPLAY);
    // Turn display back on
  OLED1306_Cmd( OLED1306_DISPLAYON);

  OLED1306_Clear();
  OLED1306_SetPos(0,0);
}

void OLED1306_Contrast(uint8_t value)
{
  OLED1306_Cmd(OLED1306_DISPLAYOFF);
  OLED1306_Cmd(OLED1306_SETCONTRAST);
  OLED1306_Cmd(value);
  OLED1306_Cmd(OLED1306_DISPLAYON);
}

void OLED1306_Cmd(uint8_t cmd)
{
  I2C_Start();
  I2C_TXInit(OLED1306_I2C_ADDRESS);
  I2C_WriteByte(OLED1306_CMD);
  I2C_WriteByte(cmd);
  I2C_Stop();
}

void OLED1306_Data(uint8_t data)
{
  I2C_Start();
  I2C_TXInit(OLED1306_I2C_ADDRESS);
  I2C_WriteByte(OLED1306_DATA);
  I2C_WriteByte(data);
  I2C_Stop(); 
}

void OLED1306_Write(uint8_t regaddr, uint8_t data)
{    
  I2C_Start();
  I2C_TXInit(OLED1306_I2C_ADDRESS);
  I2C_WriteByte(regaddr);
  I2C_WriteByte(data);
  I2C_Stop();
}


void OLED1306_SetPos(uint8_t x, uint8_t y)
{
	OLED1306_Cmd(0xB0 + y);
	OLED1306_Cmd(x & 0xf);
	OLED1306_Cmd(0x10 | (x >> 4));
}

void OLED1306_PutChar(char c)
{
	if (c>=0x61)
		c -= 32;
	uint16_t char_index = c - (char)' ';
        char_index *= FONT_CHAR_WIDTH;
	for (uint8_t i = 0; i<FONT_CHAR_WIDTH; i++)
		OLED1306_Data(font6[char_index+i]);
}

void OLED1306_PutsF(char *s)
{
	while(*s!='\0')
	{	
		OLED1306_PutChar(*s);
		s++;
	}
}

void OLED1306_Puts(char *s) {
  I2C_Start();
  I2C_TXInit(OLED1306_I2C_ADDRESS);
  I2C_WriteByte(OLED1306_DATA);
  
  while(*s!='\0') {	
    char c = *s;
    if (c>=0x61)
      c-=32;
    uint16_t char_index = c - (char)' ';
    char_index *= FONT_CHAR_WIDTH;
    for (uint8_t i = 0; i<FONT_CHAR_WIDTH; i++) {
      I2C_WriteByte(font6[char_index+i]); 
    }
    s++;
  }
  
  I2C_Stop();
}

        

        
void OLED1306_Putdigit(uint8_t n, uint8_t x, uint8_t y)
{
	if (n>9)
		n -= '0';
	
	uint16_t seg = n*DIGIT_BMP_LENGTH;
	for (uint8_t i = 0; i<DIGIT_BMP_HEIGHT; i++) {
		OLED1306_SetPos(x,y+i);
                
                I2C_Start();
                I2C_TXInit(OLED1306_I2C_ADDRESS);
                I2C_WriteByte(OLED1306_DATA);
		for (uint8_t c = 0; c<DIGIT_BMP_WIDTH; c++) {
                        I2C_WriteByte(digits[seg]);
			seg++;
		}
                I2C_Stop(); 
	}
}


void OLED1306_Putdigitval(char *s, uint8_t x, uint8_t y)
{
	while(*s!='\0')	{
		OLED1306_Putdigit(*s,x,y);
		x += DIGIT_BMP_WIDTH;
		s++;
	}
}

void OLED1306_PutdigitvalF(char *s, uint8_t x, uint8_t y)
{
  // idea is to minimize SetXY and to draw characters as strips
  // and in the end there only should be 3 SetXY draws regardless of digit count

  
  /*
  while(*s!='\0')
  {
          OLED1306_Putdigit(*s,x,y);
          x += DIGIT_BMP_WIDTH;
          s++;
  } 
  */
}

void OLED1306_SetPixel(uint8_t x, uint8_t y)
{
	uint8_t value = 1;
	uint8_t ny = y/8; // find row index
	uint8_t px = y%8;
	value = 1<<px;
	OLED1306_SetPos(x,ny);
	OLED1306_Data(value);
}

void OLED1306_ImgP(char *img, uint8_t w, uint8_t h, uint8_t xpos, uint8_t ypos)
{
	uint8_t x;
	uint8_t y;
	for (y = ypos; y<h; y++)
	{
		OLED1306_SetPos(xpos,ypos+y);
		for (x = xpos; x<w; x++)
		{
			OLED1306_Data(*img);
			img++;
		}
	}
}

void OLED1306_Fill(uint8_t data)
{
  
	OLED1306_SetPos(0,0);
	uint16_t x = 0;
        
        I2C_Start();
        I2C_TXInit(OLED1306_I2C_ADDRESS);
        I2C_WriteByte(OLED1306_DATA);
   
	for (x = 0; x<OLED1306_SIZE; x++) {
          I2C_WriteByte(data);
	}
	I2C_Stop();
  
}

void OLED1306_RectBuffer(uint8_t *buf, uint8_t px, uint8_t py, uint8_t w, uint8_t h)
{
	uint8_t x;
	uint8_t y;
	uint16_t i = 0;
	for (y = py; y<py+h; y++)
	{
		OLED1306_SetPos(px,y);
		//i2c_start(OLED1306_I2C_ADDRESS);
		for (x = px; x<px+w; x++)
		{
			//i2c_write(OLED1306_DATA|OLED1306_CONT);
			//i2c_write(buf[i]);
			i++;
		}
		//i2c_stop();
	}
}
