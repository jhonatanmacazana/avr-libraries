/*************************************************************************
 Title	:   C include file for the ADC library (ADC.c)
 Author:    Jhonatan Macazana <jhonatan.macazana@utec.edu.pe> 
 Software:  AVR-GCC 4.x
 Hardware:  Designed for ATmega328P, similar AVR devices

 DESCRIPTION
       Basic routines for the ADC configuration.

       This Library uses interruptions for the ADC and have a buffer to 
       store the data. It has 2 modes: 10 bits and 8 bits.

 USAGE
       See the C include ADC.h file for a description of each function

*****************************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#include "ADC.h"

/* Static Variables */
static volatile states_ADC ADC_status;
static volatile uint16_t ADC_Value;
static volatile uint16_t ADC_P;
#if ADC_MODE == TENBIT
static uint16_t ADC_Buffer[ADC_BUFFER_SIZE];
#elif ADC_MODE == EIGHTBIT
static uint8_t ADC_Buffer[ADC_BUFFER_SIZE];
#endif
static volatile uint8_t ADC_Head;
static volatile uint8_t ADC_Tail;



/*
**	functions
*/

/*************************************************************************
Low-level function to initialize the ADC
Input:    none
Returns:  none
*************************************************************************/
void ADC_Init(void)
{
	/* Voltage Reference = AVCC */
	ADMUX |= (1<<REFS0)|(ADC_CHANNEL<<MUX0);
	#if ADC_MODE == EIGHTBIT
	ADMUX |= (1<<ADLAR); 	// Adjust the bits to the left
	#endif

	/* ADC Enable, ADC Interrupt Enable, Prescaler predefined to work at 125 KHz */
	ADCSRA |= (1<<ADEN)|(1<<ADIE)|(ADC_PRESC<<ADPS0);
}

void ADC_Start()
{
	/* Activa ADC */
	ADCSRA |= (1<<ADSC);
	/* Cambia de estadp*/
	ADC_status = ADC_WAIT;
}


/*************************************************************************
Interrupt Vector for the ADC.
If the conversion is ready this ISR will execute. Saves the data and
change the index of the ADC Buffer.
*************************************************************************/
ISR(ADC_vect)
{
	/* ADC Value */
	#if ADC_MODE == TENBIT
	uint16_t temp;
	temp = ADC;
	#elif ADC_MODE == EIGHTBIT
	uint8_t temp;
	temp = ADCH;
	#endif
	
	uint8_t tmphead;
	
	/* Calculate buffer index */
	tmphead = (ADC_Head + 1) & ADC_BUFFER_MASK;
	
	/* Store new index */
	ADC_Head = tmphead;
	
	/* Store the data in the buffer */
	ADC_Buffer[tmphead] = temp;

	/* Change the current state */
	ADC_status = ADC_RDY;
}




/*************************************************************************
Waits until there are new data in the buffer.
Input:    none
Returns:  Data converted from the ADC. 
*************************************************************************/
#if ADC_MODE == TENBIT
uint16_t ADC_GetValue(void)
{
	/*Espera a nueva data*/
	while(ADC_Head == ADC_Tail){}
	uint8_t tmptail;
	
	/* Calcula el indice */
	tmptail = (ADC_Tail + 1) & ADC_BUFFER_MASK;
	/* Almacena nuevo indice */
	ADC_Tail = tmptail;
	/* Devuelve el valor */
	return ADC_Buffer[ADC_Tail];
}

#elif ADC_MODE == EIGHTBIT
uint8_t ADC_GetValue(void)
{
	/*Espera a nueva data*/
	while(ADC_Head == ADC_Tail){}
	uint8_t tmptail;
	
	/* Calcula el indice */
	tmptail = (ADC_Tail + 1) & ADC_BUFFER_MASK;
	/* Almacena nuevo indice */
	ADC_Tail = tmptail;
	/* Devuelve el valor */
	return ADC_Buffer[ADC_Tail];
}
#endif
