/*************************************************************************
 Title	:   UART library (UART.c)
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

 USAGE
       See the C include UART.h file for a description of each function
       
*****************************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "UART.h"


/* Static Variables */
static uint8_t USART_RxBuf[USART_RX_BUFFER_SIZE];
static volatile uint8_t USART_RxHead;
static volatile uint8_t USART_RxTail;
static uint8_t USART_TxBuf[USART_TX_BUFFER_SIZE];
static volatile uint8_t USART_TxHead;
static volatile uint8_t USART_TxTail;



/*
**	functions
*/

/*************************************************************************
Low-level function to initialize the UART
Input:    ubrr_val		MYUBRR from macro definition
Returns:  none
*************************************************************************/
void USART_Init(unsigned int ubrr_val)
{
	/* Set the baud rate */
	UBRR0H = (uint8_t) (ubrr_val>>8);                  
	UBRR0L = (uint8_t) ubrr_val;
	
	/* Enable 2X Mode */ 
	UCSR0A = (1<<U2X0);
	
	/* Enable USART receiver and transmitter */
	UCSR0B = ((1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0)); 

	/* Set frame format: 8 data 1stop */
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
	
	/* Flush buffers */    
	USART_RxTail = 0;
	USART_RxHead = 0;
	USART_TxTail = 0;
	USART_TxHead = 0;
}


/*************************************************************************
Interrupt Vector for the RX Mode.
If there are new unread data this ISR will execute. Saves the data and
change the index of the RX Buffer.
*************************************************************************/
ISR(USART_RX_vect)
{
	uint8_t data;
	uint8_t tmphead;

	/* Read the received data */
	data = UDR0;                 
	/* Calculate buffer index */
	tmphead = (USART_RxHead + 1) & USART_RX_BUFFER_MASK;
	/* Store new index */
	USART_RxHead = tmphead;
	/* Store received data in buffer */
	USART_RxBuf[tmphead] = data; 
}


/*************************************************************************
Interrupt Vector for the TX Mode.
If the buffer of the UART is empty this ISR will execute. Check for new 
data to be sent and puts that into the buffer . Change the index of the 
TX Buffer.
*************************************************************************/
ISR(USART_UDRE_vect)
{
	uint8_t tmptail;

	/* Check if all data is transmitted */
	if (USART_TxHead != USART_TxTail) 
	{
		/* Calculate buffer index */
		tmptail = (USART_TxTail + 1) & USART_TX_BUFFER_MASK;
		/* Store new index */
		USART_TxTail = tmptail;      
		/* Start transmission */
		UDR0 = USART_TxBuf[tmptail];  
	} else 
	{       
		/* Disable UDRE interrupt */
		UCSR0B &= ~(1<<UDRIE0);         
	}
}



/*************************************************************************
Waits until there are new data in the buffer. 
Input:    none
Returns:  Data received from the UART. 
*************************************************************************/
uint8_t USART_Receive(void)
{
	uint8_t tmptail;
	
	/* Wait for incoming data */
	while (USART_RxHead == USART_RxTail);
	/* Calculate buffer index */
	tmptail = (USART_RxTail + 1) & USART_RX_BUFFER_MASK;
	/* Store new index */
	USART_RxTail = tmptail;                
	/* Return data */
	return USART_RxBuf[tmptail];          
}


/*************************************************************************
Send Byte through UART. Enable the UDRE0 ISR. (buffer empty)
Input:    data 	byte to be send
Returns:  none
*************************************************************************/
void USART_Transmit(uint8_t data)
{
	uint8_t tmphead;
	
	/* Calculate buffer index */
	tmphead = (USART_TxHead + 1) & USART_TX_BUFFER_MASK;
	/* Wait for free space in buffer */
	while (tmphead == USART_TxTail);
	/* Store data in buffer */
	USART_TxBuf[tmphead] = data;
	/* Store new index */
	USART_TxHead = tmphead;               
	/* Enable UDRE interrupt */
	UCSR0B |= (1<<UDRIE0);                    
}


/*************************************************************************
Send String through UART. 
Input:    StringPtr	String to be send
Returns:  none
*************************************************************************/
void USART_putString(char* StringPtr)
{
	while(*StringPtr != 0x00) 	// Last char will be null. Check if there are more characters to send
	{				
		USART_Transmit(*StringPtr);	// Send 1 char at a time
		StringPtr++;					// Increment the index
	}
}

/*************************************************************************
Send Number through UART. 
Input:    data	Number to be send
Returns:  none
*************************************************************************/
void USART_putNumber(uint16_t data)
{
	char array[4];				// 4 digits of the number. Change if needed
	itoa(data, array, 10);		// Radix for the conversion: 10
	USART_putString(array);		// Send the ASCII codes obtained from data
}
