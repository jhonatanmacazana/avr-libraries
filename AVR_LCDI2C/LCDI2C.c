/*************************************************************************
 Title	:   LCD - I2C adapter library (LCDI2C.c)
 Author:    Jhonatan Macazana <jhonatan.macazana@utec.edu.pe> 
 Software:  AVR-GCC 4.x
 Hardware:  Designed for ATmega328P, similar AVR devices

 DESCRIPTION
       Basic routines for the I2C protocol and LCD commands.

       Originally based on the Github Code of eagl1 
       (https://github.com/eagl1/LCD1602_I2C_all_code). Changed
       the macros and new functions for the LCD. 
       Designed for the ATmega328P microcontroller.

       This Library only uses the I2C pins of the AVR. 
       The bit rate to initialize the I2C is MYTWBR. 
       This value is obtain from a Macro definition.

 USAGE
       See the C include LCDI2C.h file for a description of each function
       
*****************************************************************************/

#include <avr/io.h>
#include <util/twi.h>
#include <stdlib.h>
#include "LCDI2C.h"


/*
**	functions - LCD
*/


/*************************************************************************
Low-level function to initialize the LCD.
Input:    none
Returns:  none
*************************************************************************/
void LCD_Init(void)
{
	/* Initialize LCD */
	sendCMD(LCD_8BIT);
	sendCMD(LCD_4BIT);
	sendCMD(LCD_FUNCTION_4BIT_2LINES);
	sendCMD(LCD_DISP_ON);
	sendCMD(LCD_CLR);
}

/*************************************************************************
Allows to write new commands to the LCD 
Input:    CMD	Command to be send
Returns:  none
*************************************************************************/
void sendCMD(uint8_t CMD)
{
	uint8_t bitmask;
	/* Send Address - Write Condition */
	I2C_Start(LCD_Add_WR);
	
	/* Send commands. MS Nibble*/
	I2C_Transmit(bitmask = (CMD & 0xF0) | 0x08);
	I2C_Transmit(bitmask |= (1 << E));
	I2C_Transmit(bitmask &= ~(1 << E));
	
	/* Send commands. LS Nibble*/
	I2C_Transmit(bitmask = ((CMD << 4) & 0xF0) | 0x08);
	I2C_Transmit(bitmask |= (1 << E));
	I2C_Transmit(bitmask &= ~(1 << E));
	
	/* Stop Condition */
	I2C_Stop();
}

/*************************************************************************
Put a char on the LCD Display. 
Input:    data	char to be shown
Returns:  none
*************************************************************************/
void sendData(uint8_t data)
{
	uint8_t bitmask;
	/* Send Address - Write Condition */
	I2C_Start(LCD_Add_WR);
	
	/* Send Data. MS Nibble*/
	I2C_Transmit(bitmask = (data & 0xF0) | 0x08 | (1 << RS));
	I2C_Transmit(bitmask |= (1 << E));
	I2C_Transmit(bitmask &= ~(1 << E));
	
	/* Send Data. LS Nibble*/
	I2C_Transmit(bitmask = ((data << 4) & 0xF0) | 0x08 | (1 << RS));
	I2C_Transmit(bitmask |= (1 << E));
	I2C_Transmit(bitmask &= ~(1 << E));
	
	/* Stop Condition */
	I2C_Stop();
}

/*************************************************************************
Change the current position of the cursor 
Input:    row	Choose the new row
		  col 	Choose the new col
Returns:  none
*************************************************************************/
void LCD_GotoXY (uint8_t row, uint8_t col)
{
	/* Change the cursor*/
	if (row == 1)
	sendCMD(LCD_ROW1 + col);
	if (row == 2)
	sendCMD(LCD_ROW2 + col);
}

/*************************************************************************
Put a String on the LCD Display. 
Input:    arr1	String to be shown
Returns:  none
*************************************************************************/
void LCD_String(char* arr1)
{
	/* Last char will be null. Check for characters to send*/
	while(*arr1 != 0x00)
	{
		sendData(*arr1);		// Send 1 char at the time
		arr1++;					// Increment the index
	}
}

/*************************************************************************
Put a Number on the LCD Display. 
Input:    numb	number to be shown
Returns:  none
*************************************************************************/
void LCD_Number(uint16_t numb)
{
	char array[4];				// 4 digits of the number. Change if needed
	itoa(numb, array, 10);		// Radix for the conversion: 10
	LCD_String(array);			// Send the ASCII codes obtained from data
	
}


/*
**	functions - I2C
*/

/*************************************************************************
Low-level function to initialize the I2C
Input:    none
Returns:  none
*************************************************************************/
void I2C_Init(void)
{
	/* Set SCL to 50 kHz. Macro definition */
	TWSR = 0x00;			// Prescaler = 1
	TWBR = MYTWBR;			// Macro 72
	
	/* Enable TWI */
	TWCR = (1 << TWEN);
}


/*************************************************************************
Low-level function that send a Start condition and the address of a device
Input:    address 	Address of device with a W/R condition at the end.
Returns:  none
*************************************************************************/
void I2C_Start(uint8_t address)
{
	/* Send Start Condition*/
	TWCR |= (1 << TWINT) | (1 << TWSTA);
	
	/* Wait for the acknowledge bit */
	while (!(TWCR & (1 << TWINT)));
	
	/* Send the address and W/R condition */
	TWDR = address;
	TWCR |= (1 << TWINT) | (1 << TWEA);
	
	/* Wait for the acknowledge bit */
	while (!(TWCR & (1 << TWINT)));
}

/*************************************************************************
Low-level function that send a Stop condition.
Input:    none
Returns:  none
*************************************************************************/
void I2C_Stop(void)
{
	/* Send Stop Condition */
	TWCR |= (1 << TWINT) | (1 << TWSTO);
}


/*************************************************************************
Send Byte through I2C. Wait for the acknowledge bit.
Input:    data 	byte to be send
Returns:  none
*************************************************************************/
void I2C_Transmit(uint8_t data)
{
	/* Send the Data */
	TWDR = data;
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
	
	/* Wait for the acknowledge bit */
	while (!(TWCR & (1 << TWINT)));
}

/*************************************************************************
Waits until there are new data. 
Input:    none
Returns:  Data received from the UART. 
*************************************************************************/
uint8_t I2C_Receive(void)
{
	/*Clean the flag for the incoming data*/
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
	
	/* Wait for the acknowledge bit */
	while (!(TWCR & (1 << TWINT)));
	
	/* Return the incoming data */
	return TWDR;
}