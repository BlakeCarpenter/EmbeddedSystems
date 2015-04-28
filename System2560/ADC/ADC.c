/*
 * ADC.c
 *
 * Created: 4/27/2015 6:42:09 PM
 *  Author: Spencer Allen, Blake Carpenter, Jonathan Tye
 */ 


#include <avr/io.h>
#include <util/delay.h>
#include <util/atomic.h>
#include <stdio.h>
#include <stdbool.h>
#include <avr/interrupt.h>
#include "InterruptHandler.h"

int ADC_read(uint8_t channel, uint8_t resolution, bool single_ended);
void ADC_print(uint16_t total);

volatile uint16_t total;
volatile uint8_t timesPolled;

int main(void){
	
	
	serial_open(19200, 0);
	
	//OCR0A = 12500; //50 ms
	OCR0A = 5000; //50 ms
	TIMSK0 |= (1<<OCIE0A); //Enable Timer match interrupts
	TCCR0A = 0;
	TCCR0B = 3;//start timer
	
	ADMUX &= ~0x1F;
	ADMUX |= 0; //Set channel
	ADMUX |= (1<<REFS0);
	ADCSRA |= (1<<ADEN);
	ADCSRA |= (1<<ADIE);
	ADCSRA |= (1<<ADATE);
	ADCSRB = 3;
	
	sei();
	
    while(1){
		
		//serial_write(serial_read());
    }
}

int ADC_read(uint8_t channel, uint8_t resolution, bool singleEnded){
	ADMUX &= ~0x1F;
	ADMUX |= channel;
	ADMUX |= (1<<REFS0);

	ADCSRA |= (1<<ADEN);
	ADCSRA |= (1<<ADSC);
	while(ADCSRA & (1<<ADSC)){
		
	}
	uint8_t low = ADCL;
	uint16_t hi = ADCH;
	hi = (hi << 8);
	return (hi | low);
}

void ADC_print(uint16_t t){
	int volts = t/5;
	char buff[25];
	sprintf(buff,"%d volts (0x%x)\n",volts,volts);
	serial_print(buff);
	
	total = 0;
	timesPolled = 0;
}

ISR(TIMER0_COMPA_vect){
	/*if(timesPolled >= 5){
		ADCSRA &= ~(1<<ADEN);
		ADC_print(total);
	}
	else{
		OCR0A += 60000;
		ADCSRA |= (1<<ADEN);
		ADCSRA |= (1<<ADSC);
	}
	if(timesPolled >= 5){
		ADCSRA &= ~(1<<ADEN);
		ADC_print(total);
	}*/
	//OCR0A = 12500; //50 ms
	OCR0A += 5000;
}

ISR(ADC_vect){
	if(timesPolled == 5){
		ADC_print(total);
		timesPolled = 0;
	}
	else{
		int temp = 0;
		uint8_t low = ADCL;
		uint16_t hi = ADCH;
		hi = (hi << 8);
		temp = (hi | low);
		total += temp;
		timesPolled++;
		
	}
}