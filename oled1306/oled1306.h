// sd1306 oled display library
// by mkdxdx, use it as you like

// i2c addr
#define OLED1306_I2C_ADDRESS		0x78
// size
#define OLED1306_LCDWIDTH                      128
#define OLED1306_LCDHEIGHT                     8
#define OLED1306_SIZE						OLED1306_LCDWIDTH*OLED1306_LCDHEIGHT
// Commands
#define OLED1306_SETCONTRAST                  0x81
#define OLED1306_DISPLAYALLON_RESUME          0xA4
#define OLED1306_DISPLAYALLON                 0xA5
#define OLED1306_NORMALDISPLAY                0xA6
#define OLED1306_INVERTDISPLAY                0xA7
#define OLED1306_DISPLAYOFF                   0xAE
#define OLED1306_DISPLAYON                    0xAF
#define OLED1306_SETDISPLAYOFFSET             0xD3
#define OLED1306_SETCOMPINS                   0xDA
#define OLED1306_SETVCOMDETECT                0xDB
#define OLED1306_SETDISPLAYCLOCKDIV           0xD5
#define OLED1306_SETPRECHARGE                 0xD9
#define OLED1306_SETMULTIPLEX                 0xA8
#define OLED1306_SETLOWCOLUMN                 0x00
#define OLED1306_SETHIGHCOLUMN                0x10
#define OLED1306_SETSTARTLINE                 0x40
#define OLED1306_MEMORYMODE                   0x20
#define OLED1306_COLUMNADDR                   0x21
#define OLED1306_PAGEADDR                     0x22
#define OLED1306_COMSCANINC                   0xC0
#define OLED1306_COMSCANDEC                   0xC8
#define OLED1306_SEGREMAP                     0xA0
#define OLED1306_CHARGEPUMP                   0x8D
#define OLED1306_EXTERNALVCC                   0x1
#define OLED1306_SWITCHCAPVCC                  0x2
// Scrolling #defines
#define OLED1306_ACTIVATE_SCROLL               0x2F
#define OLED1306_DEACTIVATE_SCROLL             0x2E
#define OLED1306_SET_VERTICAL_SCROLL_AREA      0xA3
#define OLED1306_RIGHT_HORIZONTAL_SCROLL       0x26
#define OLED1306_LEFT_HORIZONTAL_SCROLL        0x27
#define OLED1306_VERT_AND_RIGHT_HORIZ_SCROLL   0x29
#define OLED1306_VERT_AND_LEFT_HORIZ_SCROLL    0x2A

#define OLED1306_CMD		                   0x00
#define OLED1306_DATA		                   0x40
#define OLED1306_CONT					       0x80

#define FONT_CHAR_WIDTH 6
# define DIGIT_BMP_WIDTH	16
# define DIGIT_BMP_HEIGHT       3
# define DIGIT_BMP_LENGTH       DIGIT_BMP_WIDTH*DIGIT_BMP_HEIGHT

#define OLED1306_Clear()  OLED1306_Fill(0x00)



// lcd init
void OLED1306_init(void); 
void OLED1306_Write(uint8_t c, uint8_t d);
// send cmd
void OLED1306_Cmd(uint8_t cmd);
// send data
void OLED1306_Data(uint8_t data);
// set cg pointer
void OLED1306_SetPos(uint8_t x, uint8_t y); 
// set contrast
void OLED1306_Contrast(uint8_t value);
// put character with small font
void OLED1306_PutChar(char c); 
// put string from spm
void OLED1306_PutSPM(char *s);
// put string from sram
void OLED1306_Puts(char *s);
// put big digit
void OLED1306_Putdigit(uint8_t n, uint8_t x, uint8_t y);
// put big digit value
void OLED1306_Putdigitval(char *s, uint8_t x, uint8_t y);
// set pixel
void OLED1306_SetPixel(uint8_t x, uint8_t y);
// put image

// fill
void OLED1306_Fill(uint8_t data);
void OLED1306_PutsF(char *s);
void Delay(uint16_t nCount);
