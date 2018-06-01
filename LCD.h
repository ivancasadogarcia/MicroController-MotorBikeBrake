/* File:   Main.c
 * Authors: Ivan Casado & David Carpintero
 *
 * Created on 17 de marc del 2018       */

#define LCD_DISPLAY_LEN 	16      // Display line length. number of characters displayed in a line
#define LCD_DISPLAY_LINE	2		// number of lines displayed on the LCD

#define LCD_LINE1			0		// first line
#define LCD_LINE2			1		// second line

#define LCD_I				0		// instruction
#define LCD_D				1		// data

#define LCD_Busy			0x80	// LCD is busy
#define LCD_Free			0		// LCD is free

#define LCD_ClearDisplay	0x01	// clear display
#define LCD_ReturnHome		0x02	// move cursor back to 00h position
#define LCD_CursorIncNS		0x06	// assign cursor as increase mode but screen not shift
#define LCD_CursorDecNS		0x04	// assign cursor as decrease mode but screen not shift
#define LCD_CursorIncS		0x07	// assign cursor as increase mode and screen shift
#define LCD_CursorDecS		0x05	// assign cursor as decrease mode adn screen shift
#define LCD_DonConBoff		0x0e	// Display on, Cursor on, Blinking of Cursor off
#define LCD_DonConBon		0x0f	// Display on, Cursor on, Blinking of Cursor on
#define LCD_DonCoffBoff		0x0c	// Display on, Cursor off, Blinking of Cursor off
#define LCD_DoffCoffBoff	0x08	// Display off, Cursor off, Blinking of Cursor off
#define LCD_CursorShiftL	0x1c	// Cursor shift enabled, shift right
#define LCD_CursorShiftR	0x18	// Cursor shift enabled, shift left
#define LCD_CursorNonShift	0x10	// Cursor shift disabled
#define LCD_DataLength4		0x20	// 4-bit data length
#define LCD_DataLength8		0x3C	// 8-bit data length
#define LCD_CGRAM(_cgramAddr)	(((_cgramAddr)&0x0f)|0x40)		// set 4-bit CGRAM addres
#define LCD_DDRAM1(_ddramAddr)	(((_ddramAddr)&0x0f)|0x80)		// set 7-bit DDRAM address, line 1
#define LCD_DDRAM2(_ddramAddr)	(((_ddramAddr)&0x0f)|0xc0)		// set 7-bit DDRAM address, line 2


void LCDInit(void);

void LCDclear(void);

void LCDwrite(unsigned char , unsigned char );

void LCDwriteLine(unsigned char , unsigned char * );