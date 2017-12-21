#define F_CPU 4000000UL
/*
 * AnalogTest1.c
 *
 * Created: 8/11/2017 4:02:14 μμ
 * Author : Kokkinis, Stasinos
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>

//The notes we use adjusted to a modulation of 8
#define C4   1911
#define G4   1276
#define F4   1432
#define A4   1136  

//Here we will store the ADC values
int R,P,P1,P2;


//Initialisation

void initialise1()
{
	DDRA &= ~(1<<PA0 | 1<<PA1 | 1<<PA2 | 1<<PA3);
	ADCSRA = 0b10001011;
	ADMUX = 0b01100000;
	SFIOR = 0b00000000;
	DDRB |= (1<<PB0 | 1<<PB1 | 1<<PB2 | 1<<PB3 | 1<<PB4 | 1<<PB5 | 1<<PB6 | 1<< PB7);
	
	
	DDRD |= (1<<PD5 | 1<<PD4);
	TCCR1A |=(1<<COM1B1);
	TCCR1B |= (1<<WGM13 | CS11);
	
}

//called after we get our values
void PWM2()
{
	int note;
	//Here we check which photodiode gives the highest value,then which one is the second highest and so on.Afterwards we calculate our output
	if((P1>P2) && (P1>P) && (P1>R))
	{
		if((P2>P) && (P2>R))
		{
			if(P>R)
			{
				note=0.4*G4+0.3*F4+0.2*C4+0.1*A4;
			}
			else
			{
				note=0.4*G4+0.3*F4+0.2*A4+0.1*C4;
			}
		}
		else if((P>P2)&&(P>R))
		{
			if(P2>R)
			{
				note=0.4*G4+0.3*C4+0.2*F4+0.1*A4;
			}
			else
			{
				note=0.4*G4+0.3*C4+0.2*A4+0.1*F4;
			}
		}
		else
		{
			if(P2>P)
			{
				note=0.4*G4+0.3*A4+0.2*F4+0.1*C4;
			}
			else
			{
				note=0.4*G4+0.3*A4+0.2*C4+0.1*F4;
			}
		}
	}
	else if ((P2>P1) && (P2>P) && (P2>R))
	{
		if((P1>P) && (P1>R))
		{
			if(P>R)
			{
				note=0.4*F4+0.3*G4+0.2*C4+0.1*A4;
			}
			else
			{
				note=0.4*F4+0.3*G4+0.2*A4+0.1*C4;
			}
		}
		else if((P>P1)&&(P>R))
		{
			if(P1>R)
			{
				note=0.4*F4+0.3*C4+0.2*G4+0.1*A4;
			}
			else
			{
				note=0.4*F4+0.3*C4+0.2*A4+0.1*G4;
			}
		}
		else
		{
			if(P1>P)
			{
				note=0.4*F4+0.3*A4+0.2*G4+0.1*C4;
			}
			else
			{
				note=0.4*F4+0.3*A4+0.2*C4+0.1*G4;
			}
		}
	}
	else if ((P>P2) && (P>P1) && (P>R))
	{
		if((P2>P1) && (P2>R))
		{
			if(P1>R)
			{
				note=0.4*C4+0.3*F4+0.2*G4+0.1*A4;
			}
			else
			{
				note=0.4*C4+0.3*F4+0.2*A4+0.1*G4;
			}
		}
		else if((P1>P2)&&(P1>R))
		{
			if(P2>R)
			{
				note=0.4*C4+0.3*G4+0.2*F4+0.1*A4;
			}
			else
			{
				note=0.4*C4+0.3*G4+0.2*A4+0.1*F4;
			}
		}
		else
		{
			if(P2>P1)
			{
				note=0.4*C4+0.3*A4+0.2*F4+0.1*G4;
			}
			else
			{
				note=0.4*C4+0.3*A4+0.2*G4+0.1*F4;
			}
		}
	}
	else
	{
		if((P2>P1) && (P2>P))
		{
			if(P1>P)
			{
				note=0.4*A4+0.3*F4+0.2*G4+0.1*C4;
			}
			else
			{
				note=0.4*A4+0.3*F4+0.2*C4+0.1*G4;
			}
		}
		else if((P1>P2)&&(P1>P))
		{
			if(P2>P)
			{
				note=0.4*A4+0.3*G4+0.2*F4+0.1*C4;
			}
			else
			{
				note=0.4*A4+0.3*G4+0.2*C4+0.1*F4;
			}
		}
		else
		{
			if(P2>P1)
			{
				note=0.4*A4+0.3*C4+0.2*F4+0.1*G4;
			}
			else
			{
				note=0.4*A4+0.3*C4+0.2*G4+0.1*F4;
			}
		}
	}
	///////////////////////////////////////////////////////////////////////////////////////////////
	ICR1H = (note >> 8); //first set the high byte
	ICR1L = note;        //now the low byte
	OCR1B = 500;		//duty cycle, affects the volume of the output
	_delay_ms(10);
}




int main(void)
{
	
	initialise1();
	
	sei();
	
	ADCSRA |= (1<<ADSC);
    while (1) 
    {
		
		
    }
	
	return 0;
}

//this interupt service routine is responsible for our sampling process
//each sensor is checked once per 4 cycles, because there are 4 of them

ISR(ADC_vect)
{
		
		if(ADMUX==0b01100000)
		{
			P=ADCH;
			ADMUX = 0b01100001;
		}
		else if (ADMUX==0b01100001)
		{
			R=ADCH;
			ADMUX = 0b01100010;
		}
		else if (ADMUX==0b01100010)
		{
			P1=ADCH;
			ADMUX = 0b01100011;
		}
		else if (ADMUX==0b01100011)
		{
			P2=ADCH;
			ADMUX = 0b01100000;
		}
		PWM2();		//after sampling is done, we call our PWM function, which is responsible for the output
		PORTB=0xFF;
		ADCSRA |= (1<<ADSC);
	
}
