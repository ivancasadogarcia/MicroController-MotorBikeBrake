/* Fichero:   Main.c
 * Autores: Ivan Casado & David Carpintero
 *
 * Creado on 15 de marzo del 2018       
 */

#include "Main.h"
#include "LCD.h"

/*---------------------------------------------*/
/*Codigo extraido de la página web de Microchip*/
/*---------------------------------------------*/
/*--Funciones para el uso de la pantalla LCD---*/

void LCDInit ()     //Inicilización de la pantalla LCD
{
	unsigned int Local_16;
	PMCONbits.PMPEN		= 1;	// PMP enabled
	PMCONbits.PSIDL		= 0;	// Continue module operation in Idle mode
	PMCONbits.ADRMUX	= 0;	// Address and data appear on separate pins
	PMCONbits.PTBEEN	= 0;	// Byte Enable Port disabled
	PMCONbits.PTWREN	= 1;	// Write Enable Strobe Port enabled
	PMCONbits.PTRDEN	= 1;	// Read/Write Strobe Port enabled
	PMCONbits.CSF		= 2;	// PMCS1 and PMCS2 function as chip select
	PMCONbits.ALP		= 1;	// Address Latch Polarity Active-high (PMALL and PMALH)
	PMCONbits.CS2P		= 1;	// Chip Select 2 Polarity Active-high
	PMCONbits.CS1P		= 1;	// Chip Select 1 Polarity Active-high
	PMCONbits.BEP		= 1;	// Byte Enable Active-high
	PMCONbits.WRSP		= 1;	// Master Mode, Write Strobe active-high
	PMCONbits.RDSP		= 1;	// Master Mode, Read/write strobe active-high
	
	// PMMODE
	PMMODEbits.IRQM		= 0;	// No interrupt generated
	PMMODEbits.INCM		= 0;	// No increment or decrement of address
	PMMODEbits.MODE16	= 0;	// 8-bit data mode
	PMMODEbits.MODE		= 3;	// Master mode 1(PMCSx, PMRD/PMWR, PMENB, PMBE, PMA<x:0> and PMD<7:0>)
	PMMODEbits.WAITB	= 3;	// Data wait of 4Tcy; multiplexed address phase of 4 Tcy
	PMMODEbits.WAITM	= 0xf;	// Read to Byte Enable Strobe: Wait of additional 15 Tcy
	PMMODEbits.WAITE	= 3;	// Data Hold After Strobe: Wait of 4 Tcy
	
	// PMADDR
	PMADDR 	= 0x0000;           // For LCD, there is no address, so zero is assigned.
	
	// PMAEN
	PMAEN 	= 0x0001;			// PMA15:2 function as port I/O, PMALH/PMALL enabled
	
	for (Local_16 = 0; Local_16 < 40000; Local_16++);
	// Set the default function, DL:8-bit
	LCDwrite(LCD_I, LCD_DataLength8);	
	
	for (Local_16 = 0; Local_16 < 40; Local_16++);
	// Set the display control, turn on LCD
	LCDwrite(LCD_I, LCD_DonCoffBoff);			
	
	for (Local_16 = 0; Local_16 < 20; Local_16++);
	// Set the entry mode, set cursor in increase mode
	LCDwrite(LCD_I, LCD_CursorIncNS);			
	
	for (Local_16 = 0; Local_16 < 20; Local_16++);
	// Set cursor shift, shift right
	LCDwrite(LCD_I, LCD_CursorNonShift);		
	
	for (Local_16 = 0; Local_16 < 200; Local_16++);
	// Clear the display, clear display
	LCDclear();									
}

void LCDclear ()    //Limpiar la escritura en la LCD
{
	LCDwrite(LCD_I, LCD_ClearDisplay);
}   

void LCDwrite (unsigned char inputType, unsigned char inputByte)
{
	PMADDR = inputType;
	PMDIN1 = inputByte;
}

void LCDwriteLine (unsigned char lineNum, unsigned char * inputDisplay) //Función de escritura de la LCD
{
	unsigned char Local_8, Local_82;
	
    for (Local_82 = 0; Local_82 < 250; Local_82++);
	if (lineNum==LCD_LINE1)
		LCDwrite(LCD_I, LCD_DDRAM1(0));	// go to 1st char of line 1
	else if (lineNum==LCD_LINE2)
		LCDwrite(LCD_I, LCD_DDRAM2(0));	// go to 1st char of line 2
	
	for (Local_8 = 0; Local_8 < LCD_DISPLAY_LEN; Local_8++) {
		for (Local_82 = 0; Local_82 < 30; Local_82++);
		LCDwrite(LCD_D, *inputDisplay);
		inputDisplay++;
	}
}