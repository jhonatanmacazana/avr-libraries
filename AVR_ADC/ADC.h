#ifndef ADC_H_
#define ADC_H_

/*************************************************************************
 Title	:   C include file for the ADC library (ADC.c)
 Author:    Jhonatan Macazana <jhonatan.macazana@utec.edu.pe> 
 Software:  AVR-GCC 4.x
 Hardware:  Designed for ATmega328P, similar AVR devices

 DESCRIPTION
       Basic routines for the ADC configuration.

       This Library uses interruptions for the ADC and have a buffer to 
       store the data. It has 2 modes: 10 bits and 8 bits.

*****************************************************************************/



/**
*	ADC Mode Definitions
*	2 modes of operation.  
*
*/
#define TENBIT			0
#define EIGHTBIT		1

/**
*	UART Clock Definitions
*	Used to obtain the UBRR value for the desired baud rate.
*
*/
#ifndef F_CPU				/* Prevent compiler error by supplying a default */
#define F_CPU 8000000UL
#endif

/**
*	ADC Optimum Frequency: 125KHz
*	Definition of the pre escaler. It uses the previous definition of the
*	frequency of the microcontroller.
*
*/
#if F_CPU == 1000000		
	#define ADC_PRESC	3
#elif F_CPU == 8000000
	#define ADC_PRESC	6
#elif F_CPU == 16000000 
	#define ADC_PRESC	7
#endif


/**
*	ADC Buffer Definitions
*	Used to store the data. The size of the buffer has to be a power 
* 	of 2. Otherwise it won't work properly. 
*
*/
#define ADC_BUFFER_SIZE		8     /* 2,4,8,16,32,64,128 or 256 bytes */
#define ADC_BUFFER_MASK		(ADC_BUFFER_SIZE - 1)


/**
*	ADC States
*	There are two states in order to known the current state of the 
* 	ADC conversion.
*
*/
typedef enum
{
	ADC_RDY,				// Conversion ready.
	ADC_WAIT				// Conversion in progress
} states_ADC;



/**
*	ADC Mode
*	Chose the mode you want to work with the ADC.
*	The posible modes are 10BIT to obtain more precision in the conversion 
*	or 8BIT to set a standard mode.
*
*/
#define ADC_MODE 		TENBIT			/* 10BIT -- 8BIT */


/**
*	ADC Channel
*	Choose the bit of the port C you want to work with the ADC.
*	The posible modes are 0, 1, 2, 3, 4, 5, 6 and 7. Remember that the Reset
*	interrupt is triggered with the PC6 pin.
*
*/
#define ADC_CHANNEL		1	


/**
*	Functions 
*/

/**
 @brief		Configure the ADC to work at 125 KHz with the voltage reference on AVCC. 
 @param		none
 @return 	none
*/
void ADC_Init(void);


/**
 @brief		Start the ADC conversion. 
 @param		none
 @return 	none
*/
void ADC_Start(void);


/**
 @brief		Wait for the ADC to finish the conversion.
 @param		none
 @return 	Value of the conversion.
*/
#if ADC_MODE== TENBIT
uint16_t ADC_GetValue(void);
#elif ADC_MODE == EIGHTBIT
uint8_t ADC_GetValue(void);
#endif

#endif /* ADC_H_ */ 