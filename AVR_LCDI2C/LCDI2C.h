#ifndef LCDI2C_H_
#define LCDI2C_H_

/*************************************************************************
 Title	:   C include file for the LCD - I2C adapter library (LCDI2C.c)
 Author	:   Jhonatan Macazana <jhonatan.macazana@utec.edu.pe> 
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

*****************************************************************************/


/**
*	I2C Clock Definitions
*	Used to obtain the UBRR value for the desired baud rate.
*
*/
#ifndef F_CPU		/* Prevent compiler error by supplying a default */
#define F_CPU 1000000UL
#endif
#define I2C_VEL			10000			// 10 kHz for the I2C
#define MYTWBR			((F_CPU/I2C_VEL - 16)/2)	


/**
*	I2C-Adapter Address
*	Choose the address of the I2C adapter. The default values 
*	are 0x27 and 0x3F. Change if needed.
*
*/
#define LCD_Add			0x27			// Address of the LCD


/**
*	LCD Pins definition
*	Choose the enable, register select and read/write pins of the LCD. The default 
* 	values are 2,1,0 respectively. 
*
*/
#define E				2   			// E  bit
#define RW				1   			// RW bit
#define RS				0   			// RS bit


/**
*	LCD Command Definitions
*	Used to configure the device. This commands are used on the 
*	LCD_Init() function. 
*
*/
#define LCD_8BIT					0x33
#define LCD_4BIT					0x32
#define LCD_CLR						0x01
#define LCD_ROW1					0x80
#define LCD_ROW2					0xC0
#define LCD_DISP_ON					0x0C
#define LCD_DISP_ON_CURSOR			0x0E
#define LCD_DISP_ON_CURSOR_BLINK	0x0F
#define LCD_FUNCTION_4BIT_2LINES	0x28


/**
*	LCD Macro Definitions
*	A brief set of macros to make the code easier to read.
*
*/
#define LCD_Add_WR		((LCD_Add<<1)|TW_WRITE)
#define LCD_Add_RD		((LCD_Add<<1)|TW_READ)
#define LCD_Clear()		(sendCMD(LCD_CLR))
#define LCD_Up()		(sendCMD(LCD_ROW1))
#define LCD_Down()		(sendCMD(LCD_ROW2))



/**
*	Functions - LCD
*/

/**
 @brief		Low-level function to initialize the LCD. 
 @param		none
 @return 	none
*/
void LCD_Init(void);

/**
 @brief		Allows to write new commands to the LCD.
 @param		CMD 	Command to be send
 @return 	none
*/
void sendCMD(uint8_t CMD);

/**
 @brief		Change the current position of the cursor 
 @param		row 	Choose the new row
 			col 	Choose the new col
 @return 	none
*/
void LCD_GotoXY (uint8_t row, uint8_t col);

/**
 @brief		Put a String on the LCD Display. 
 @param		arr1	String to be shown
 @return 	none
*/
void LCD_String(char* arr1);

/**
 @brief		Put a Number on the LCD Display.
 @param		numb	number to be shown
 @return 	none
*/
void LCD_Number(uint16_t numb);




/**
*	Functions - I2C
*/

/**
 @brief		Low-level function to initialize the I2C
 @param		none
 @return 	none
*/
void I2C_Init(void);

/**
 @brief		Low-level function that send a Start condition and the address of a device.
 @param		address 	Address of device with a W/R condition at the end.
 @return 	none
*/
void I2C_Start(uint8_t address);

/**
 @brief		Low-level function that send a Stop condition.
 @param		none
 @return 	none
*/
void I2C_Stop(void);

/**
 @brief		Send Byte through I2C. Wait for the acknowledge bit.
 @param		data 	byte to be send
 @return 	none
*/
void I2C_Transmit(uint8_t data);

/**
 @brief		Waits until there are new data. 
 @param		none
 @return 	Data received from the UART. 
*/
uint8_t I2C_Receive(void);

/**
 @brief		Low-level function to initialize the LCD. 
 @param		none
 @return 	none
*/



#endif /* LCDI2C_H_ */