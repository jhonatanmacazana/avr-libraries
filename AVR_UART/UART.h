#ifndef UART_H_
#define UART_H_

/*************************************************************************
 Title	:   C include file for the UART library (UART.c)
 Author:    Jhonatan Macazana <jhonatan.macazana@utec.edu.pe> 
 Software:  AVR-GCC 4.x
 Hardware:  Designed for ATmega328P, similar AVR devices

 DESCRIPTION
       Basic routines for the UART protocol.

       Originally based on the AppNote AVR306 from Microchip,
       changed the structure to use the 2X Mode of the AVR, 
       Designed for the ATmega328P microcontroller.

       This Library uses interruptions for the UART and have buffers to 
       store the data. The baud rate to initialize the UART is MYUBRR. 
       This value is obtain from a Macro definition.

*****************************************************************************/


/**
*	UART Clock Definitions
*	Used to obtain the UBRR value for the desired baud rate.
*
*/
#ifndef F_CPU				/* Prevent compiler error by supplying a default */
	#define F_CPU 8000000UL
#endif

#define UART_BAUD_RATE 		9600		
#define MYUBRR 				F_CPU/8/UART_BAUD_RATE-1


/**
*	UART Buffer Definitions
*	Used to store the incoming data. The size of the buffers has to be a power 
* 	of 2. Otherwise it won't work properly. 
*
*/
#define USART_RX_BUFFER_SIZE 8     /* 2,4,8,16,32,64,128 or 256 bytes */
#define USART_TX_BUFFER_SIZE 8     /* 2,4,8,16,32,64,128 or 256 bytes */
#define USART_RX_BUFFER_MASK (USART_RX_BUFFER_SIZE - 1)
#define USART_TX_BUFFER_MASK (USART_TX_BUFFER_SIZE - 1)


/**
*	Functions 
*/

/**
 @brief		Initialize the UART hardware in 2X Mode. 
 @param		ubrr_val uses the value from the macro definition of the main code
 @return 	none
*/
void USART_Init(unsigned int ubrr_val);

/**
 @brief		Receive data from the UART's buffer
 @param		none
 @return 	data received from UART
*/
uint8_t USART_Receive(void);

/**
 @brief		Send a byte with the UART 
 @param		data byte to be send through UART
 @return 	none
*/
void USART_Transmit(uint8_t data);


/**
 @brief		Send a string with the UART 
 @param		StringPtr String to be send through UART
 @return 	none
*/
void USART_putString(char* StringPtr);

/**
 @brief		Send the ASCII code of a number with the UART 
 @param		data Number to be send through UART in ASCII 
 @return 	none
*/
void USART_putNumber(uint16_t data);


#endif /* UART_H_ */