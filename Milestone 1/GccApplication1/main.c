/*
	Made by : Kokkinis Iraklis-Taxiarchis and Apostolos Stasinos
	
	This simple code simply turns on a LED ultilising external interrupts. Int0 toggles the LED.
	If our input is raised then a counter is activated counting 1 seconds, if it's 0 the counter counts 5 seconds.
	Int 1 resets the LED.
	Keep in mind this was Developed for the ATmega16l microcontroler using the STK500 development board. STK500 is 0 sensitive.
*/
#define F_CPU 1000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int main (void)
{
	//Inputs
	DDRD &= ~(1<<PD2);		//Int0
	DDRD &= ~(1<<PD3);		//Int1
	DDRB &= ~(1<<PB2);
	
	//outputs
	DDRB |= (1<<PB0);
	
	//Counter options
	TCCR1A = 0b00000000;	//this line is not really necessary 
	TIMSK = 0b00010000;		//Counter set to compare on Output A
	TCCR1B = 0b00001000;	//Starts the counter with a relative clock of F_CPU/1024 and with the CTC option enabled
	
	//External Interrupt Options	
	GICR |= 0b11000000;
	MCUCR |= 0b00000101;
	
	sei();
	
	PORTB |= (1<<PB0);		//LED output, begins closed
	
	
	
	while(1)
	{	}
		
	return 0;
	
}

ISR(INT0_vect)
{
	if(PINB & (1<<PB2))
	{
		OCR1AH = 0b00000011;
		OCR1AL = 0b11010001;	//These two serve as the compare value
		
		TCCR1B = 0b00001101;	//Starts the counter
	}
	else
	{
		OCR1AH = 0b00010011;
		OCR1AL = 0b00010011;	//These two serve as the compare value
		
		TCCR1B = 0b00001101;	//Starts the counter
	}
}

ISR(INT1_vect)
{
	PORTB = (1<<PB0);		//Sets the LED to inactive
}

ISR(TIMER1_COMPA_vect)
{
	PORTB ^= (1<<PB0);		//Toggles our LED
	TCCR1B = 0b00000000;	//Stops the counter
}