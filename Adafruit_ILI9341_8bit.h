/*
	This is a library for the Teensy 3.2 (but any Teensy 3.0 through Teensy 3.6 will
	work).

	How to connect:

	First find the pins for PORTC bits 0 through 7. These pins can be found for all
	Teensys at https://www.pjrc.com/teensy/schematic.html in the schematics.

	Connect the pins for PORTC bits 0 through 7 to the LCD data bits 0 through 7.

	For the Teensy 3.2 this means that:
	LCD_D0 = digital pin 15 (PORTC bit 0)
	LCD_D1 = digital pin 22 (PORTC bit 1)
	LCD_D2 = digital pin 23 (PORTC bit 2)
	LCD_D3 = digital pin 9  (PORTC bit 3)
	LCD_D4 = digital pin 10 (PORTC bit 4)
	LCD_D5 = digital pin 13 (PORTC bit 5)
	LCD_D6 = digital pin 11 (PORTC bit 6)
	LCD_D7 = digital pin 12 (PORTC bit 7)

	The LCD Control Pins are connected in the following way:
	TFT_RD         = digital pin 21
	TFT_WR         = digital pin 20
	TFT_RS         = digital pin 19
	TFT_CS         = digital pin 18
	TFT_RST        = digital pin 17

	Run sketch.ino to test the ILI9341 display.

	Here are the benchmark results:
	Benchmark Time (microseconds)
	Screen fill 88319
	Text 12309
	Lines 87538
	Horiz/Vert Lines 7062
	Rectangles (outline) 4670
	Rectangles (filled) 183556
	Circles (filled) 35540
	Circles (outline) 35868
	Triangles (outline) 27423
	Triangles (filled) 62862
	Rounded rects (outline) 14888
	Rounded rects (filled) 200625
*/

#ifndef _ADAFRUIT_ILI9341H_
#define _ADAFRUIT_ILI9341H_

#include "Print.h"
#include <Adafruit_GFX.h>

#define HX8357D 0xD ///< Our internal const for D type
#define HX8357B 0xB ///< Our internal const for B type

#define HX8357_TFTWIDTH 320  ///< 320 pixels wide
#define HX8357_TFTHEIGHT 480 ///< 480 pixels tall

#define HX8357_NOP 0x00     ///< No op
#define HX8357_SWRESET 0x01 ///< software reset
#define HX8357_RDDID 0x04   ///< Read ID
#define HX8357_RDDST 0x09   ///< (unknown)

#define HX8357_RDPOWMODE 0x0A ///< Read power mode Read power mode
#define HX8357_RDMADCTL 0x0B  ///< Read MADCTL
#define HX8357_RDCOLMOD 0x0C  ///< Column entry mode
#define HX8357_RDDIM 0x0D     ///< Read display image mode
#define HX8357_RDDSDR 0x0F    ///< Read dosplay signal mode

#define HX8357_SLPIN 0x10  ///< Enter sleep mode
#define HX8357_SLPOUT 0x11 ///< Exit sleep mode
#define HX8357B_PTLON 0x12 ///< Partial mode on
#define HX8357B_NORON 0x13 ///< Normal mode

#define HX8357_INVOFF 0x20  ///< Turn off invert
#define HX8357_INVON 0x21   ///< Turn on invert
#define HX8357_DISPOFF 0x28 ///< Display on
#define HX8357_DISPON 0x29  ///< Display off

#define HX8357_CASET 0x2A ///< Column addr set
#define HX8357_PASET 0x2B ///< Page addr set
#define HX8357_RAMWR 0x2C ///< Write VRAM
#define HX8357_RAMRD 0x2E ///< Read VRAm

#define HX8357B_PTLAR 0x30   ///< (unknown)
#define HX8357_TEON 0x35     ///< Tear enable on
#define HX8357_TEARLINE 0x44 ///< (unknown)
#define HX8357_MADCTL 0x36   ///< Memory access control
#define HX8357_COLMOD 0x3A   ///< Color mode

#define HX8357_SETOSC 0xB0      ///< Set oscillator
#define HX8357_SETPWR1 0xB1     ///< Set power control
#define HX8357B_SETDISPLAY 0xB2 ///< Set display mode
#define HX8357_SETRGB 0xB3      ///< Set RGB interface
#define HX8357D_SETCOM 0xB6     ///< Set VCOM voltage

#define HX8357B_SETDISPMODE 0xB4 ///< Set display mode
#define HX8357D_SETCYC 0xB4      ///< Set display cycle reg
#define HX8357B_SETOTP 0xB7      ///< Set OTP memory
#define HX8357D_SETC 0xB9        ///< Enable extension command

#define HX8357B_SET_PANEL_DRIVING 0xC0 ///< Set panel drive mode
#define HX8357D_SETSTBA 0xC0           ///< Set source option
#define HX8357B_SETDGC 0xC1            ///< Set DGC settings
#define HX8357B_SETID 0xC3             ///< Set ID
#define HX8357B_SETDDB 0xC4            ///< Set DDB
#define HX8357B_SETDISPLAYFRAME 0xC5   ///< Set display frame
#define HX8357B_GAMMASET 0xC8          ///< Set Gamma correction
#define HX8357B_SETCABC 0xC9           ///< Set CABC
#define HX8357_SETPANEL 0xCC           ///< Set Panel

#define HX8357B_SETPOWER 0xD0     ///< Set power control
#define HX8357B_SETVCOM 0xD1      ///< Set VCOM
#define HX8357B_SETPWRNORMAL 0xD2 ///< Set power normal

#define HX8357B_RDID1 0xDA ///< Read ID #1
#define HX8357B_RDID2 0xDB ///< Read ID #2
#define HX8357B_RDID3 0xDC ///< Read ID #3
#define HX8357B_RDID4 0xDD ///< Read ID #4

#define HX8357D_SETGAMMA 0xE0 ///< Set Gamma

#define HX8357B_SETGAMMA 0xC8        ///< Set Gamma
#define HX8357B_SETPANELRELATED 0xE9 ///< Set panel related

// Plan is to move this to GFX header (with different prefix), though
// defines will be kept here for existing code that might be referencing
// them. Some additional ones are in the ILI9341 lib -- add all in GFX!
// Color definitions
#define HX8357_BLACK 0x0000   ///< BLACK color for drawing graphics
#define HX8357_BLUE 0x001F    ///< BLUE color for drawing graphics
#define HX8357_RED 0xF800     ///< RED color for drawing graphics
#define HX8357_GREEN 0x07E0   ///< GREEN color for drawing graphics
#define HX8357_CYAN 0x07FF    ///< CYAN color for drawing graphics
#define HX8357_MAGENTA 0xF81F ///< MAGENTA color for drawing graphics
#define HX8357_YELLOW 0xFFE0  ///< YELLOW color for drawing graphics
#define HX8357_WHITE 0xFFFF   ///< WHITE color for drawing graphics


#define ILI9341_TFTWIDTH 320
#define ILI9341_TFTHEIGHT 480

#define ILI9341_NOP 0x00
#define ILI9341_SWRESET 0x01
#define ILI9341_RDDID 0xD3
#define ILI9341_RDDST 0x09

#define ILI9341_SLPIN 0x10
#define ILI9341_SLPOUT 0x11
#define ILI9341_PTLON 0x12
#define ILI9341_NORON 0x13

#define ILI9341_RDMODE 0x0A
#define ILI9341_RDMADCTL 0x0B
#define ILI9341_RDPIXFMT 0x0C
#define ILI9341_RDIMGFMT 0x0D
#define ILI9341_RDSELFDIAG 0x0F

#define ILI9341_INVOFF 0x20

#define ILI9341_INVON 0x21

#define ILI9341_GAMMASET 0x26

#define ILI9341_DISPOFF 0x28

#define ILI9341_DISPON 0x29

#define ILI9341_CASET 0x2A

#define ILI9341_PASET 0x2B

#define ILI9341_RAMWR 0x2C

#define ILI9341_RAMRD 0x2E

#define ILI9341_PTLAR 0x30

#define ILI9341_MADCTL 0x36

#define ILI9341_PIXFMT 0x3A

#define ILI9341_FRMCTR1 0xB1

#define ILI9341_FRMCTR2 0xB2

#define ILI9341_FRMCTR3 0xB3

#define ILI9341_INVCTR 0xB4

#define ILI9341_DFUNCTR 0xB6

#define ILI9341_PWCTR1 0xC0

#define ILI9341_PWCTR2 0xC1

#define ILI9341_PWCTR3 0xC2

#define ILI9341_PWCTR4 0xC3

#define ILI9341_PWCTR5 0xC4

#define ILI9341_VMCTR1 0xC5

#define ILI9341_VMCTR2 0xC7

#define ILI9341_RDID1 0xDA

#define ILI9341_RDID2 0xDB

#define ILI9341_RDID3 0xDC

#define ILI9341_RDID4 0xDD

#define ILI9341_GMCTRP1 0xE0

#define ILI9341_GMCTRN1 0xE1

/*
#define ILI9341_PWCTR6  0xFC
*/

// Color definitions

#define ILI9341_BLACK 0x0000 /*   0,   0,   0 */

#define ILI9341_NAVY 0x000F /*   0,   0, 128 */

#define ILI9341_DARKGREEN 0x03E0 /*   0, 128,   0 */

#define ILI9341_DARKCYAN 0x03EF /*   0, 128, 128 */

#define ILI9341_MAROON 0x7800 /* 128,   0,   0 */

#define ILI9341_PURPLE 0x780F /* 128,   0, 128 */

#define ILI9341_OLIVE 0x7BE0 /* 128, 128,   0 */

#define ILI9341_LIGHTGREY 0xC618 /* 192, 192, 192 */

#define ILI9341_DARKGREY 0x7BEF /* 128, 128, 128 */

#define ILI9341_BLUE 0x001F /*   0,   0, 255 */

#define ILI9341_GREEN 0x07E0 /*   0, 255,   0 */

#define ILI9341_CYAN 0x07FF /*   0, 255, 255 */

#define ILI9341_RED 0xF800 /* 255,   0,   0 */

#define ILI9341_MAGENTA 0xF81F /* 255,   0, 255 */

#define ILI9341_YELLOW 0xFFE0 /* 255, 255,   0 */

#define ILI9341_WHITE 0xFFFF /* 255, 255, 255 */

#define ILI9341_ORANGE 0xFD20 /* 255, 165,   0 */

#define ILI9341_GREENYELLOW 0xAFE5 /* 173, 255,  47 */

#define ILI9341_PINK 0xF81F

/*
Define pins and Output Data Registers
*/

#define TFT_DATA GPIO6_DR

#define TFT_RD 1 //AD_B1_08

#define TFT_WR 2 //AD_B1_09

#define TFT_RS 3 //AD_B1_10

#define TFT_CS 4 //AD_B1_11

#define TFT_RST 0 //AD_B0_03

#define RD_IDLE digitalWriteFast(TFT_RD, HIGH)
#define RD_ACTIVE digitalWriteFast(TFT_RD, LOW)
#define WR_IDLE digitalWriteFast(TFT_WR, HIGH)
#define WR_ACTIVE digitalWriteFast(TFT_WR, LOW)
#define CD_COMMAND digitalWriteFast(TFT_RS, LOW)
#define CD_DATA digitalWriteFast(TFT_RS, HIGH)
#define CS_IDLE digitalWriteFast(TFT_CS, HIGH)
#define CS_ACTIVE digitalWriteFast(TFT_CS, LOW)

// If you are not using Teensy 3.2 change these pins!
#define D0 19 //AD_B1_00
#define D1 18 //AD_B1_01
#define D2 14 //AD_B1_02
#define D3 15 //AD_B1_03
#define D4 40 //AD_B1_04
#define D5 41 //AD_B1_05
#define D6 17 //AD_B1_06
#define D7 16 //AD_B1_07

class Adafruit_ILI9341_8bit_STM : public Adafruit_GFX {
 public:
  Adafruit_ILI9341_8bit_STM(void);

  void begin(void),

      setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1),

      pushColor(uint16_t color),

      fillScreen(uint16_t color),

      drawPixel(int16_t x, int16_t y, uint16_t color),

      drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color),

      drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color),

      fillRect(int16_t x, int16_t y, int16_t w, int16_t h,

               uint16_t color),

      setRotation(uint8_t r),

      invertDisplay(boolean i);

  uint16_t color565(uint8_t r, uint8_t g, uint8_t b);

  /* These are not for current use, 8-bit protocol only! */

  // uint8_t  readdata(void),

  uint8_t readcommand8(uint8_t reg);

  uint32_t readID(void);

  uint8_t tabcolor;

  uint8_t read8(void);

  void setReadDataBus(void),

      setWriteDataBus(void),

      write8(uint8_t),

      writecommand(uint8_t c),

      writedata(uint8_t d); 
};

#endif  // _ADAFRUIT_ILI9341H_
