/*
 * ADC.c
 *
 * Created: 4/27/2015 6:42:09 PM
 *  Author: Benjamin Adams, Spencer Allen, Blake Carpenter, Jonathan Tye
 */ 


#include <avr/io.h>
#include <util/delay.h>
#include <util/atomic.h>
#include <stdio.h>
#include <stdbool.h>
#include <avr/interrupt.h>
#include "InterruptHandler.h"

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