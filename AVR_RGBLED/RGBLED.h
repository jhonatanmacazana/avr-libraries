#ifndef RGBLED_H_
#define RGBLED_H_

/*************************************************************************
 Title	:   C include file for the RGB-Led library (RGBLED.c)
 Author:    Jhonatan Macazana <jhonatan.macazana@utec.edu.pe> 
 Software:  AVR-GCC 4.x
 Hardware:  Designed for ATmega328P, similar AVR devices

 DESCRIPTION
       Basic library for use a RGB Led.

       This Library uses macro definitions for the port, 
       the pin and the type of the Led. The values are only dominant colors.
       This Library does not include a function to change the brightness of
       the Led.
       
*****************************************************************************/

/**
*	RGB Type Definitions
*	Declare the 2 types of RGB Led.
*
*/
#define COMMON_ANODE		0
#define COMMON_CATHODE		1

/**
*	RGB Usage Definitions
*	Choose the type of led and the port definitions.
*
*/
#define TYPE_RGB		COMMON_CATHODE
#define PORT_RGB		PORTD
#define DDR_RGB			DDRD
#define PIN_R			3
#define PIN_G			4
#define PIN_B			5

/**
*	RGB Macros
*	Macros for both types of RGBs. 
*
*/
#if TYPE_RGB == COMMON_CATHODE
	#define RGB_CLEAR()		(PORT_RGB &= ~((1<<PIN_R)|(1<<PIN_G)|(1<<PIN_B)))
	#define RGB_ALL()		(PORT_RGB |= (1<<PIN_R)|(1<<PIN_G)|(1<<PIN_B))
	#define RGB_RED()		(PORT_RGB |= (1<<PIN_R))
	#define RGB_GREEN()		(PORT_RGB |= (1<<PIN_G))
	#define RGB_BLUE()		(PORT_RGB |= (1<<PIN_B))
#else
	#define RGB_CLEAR()		(PORT_RGB |= ((1<<PIN_R)|(1<<PIN_G)|(1<<PIN_B)))
	#define RGB_ALL()		(PORT_RGB &= ~((1<<PIN_R)|(1<<PIN_G)|(1<<PIN_B)))
	#define RGB_RED()		(PORT_RGB &= ~(1<<PIN_R))
	#define RGB_GREEN()		(PORT_RGB &= ~(1<<PIN_G))
	#define RGB_BLUE()		(PORT_RGB &= ~(1<<PIN_B))
#endif

/**
*	RGB Colors
*	Macros for the colors that the LED can shown. 
*
*/
#define RED				0
#define GREEN			1
#define BLUE			2
#define YELLOW			3
#define CYAN			4
#define MAGENTA			5



/**
*	Functions 
*/

/**
 @brief		Configure the port of the LED as output.
 @param		none
 @return 	none
*/
void RGBLed_Init(void);

/**
 @brief		Change the color of the RGB Led. 
 @param		color 	color the led will shown
 @return 	none
*/
void RGBLed_Color(uint8_t color);

/**
 @brief		Blink the led every second. The value can be modified if needed. 
 @param		none
 @return 	none
*/
void RGBLed_Blink(void);


#endif /* RGBLED_H_ */