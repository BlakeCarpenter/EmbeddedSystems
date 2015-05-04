/*
 * InterruptHandler.c
 *
 * Created: 3/5/2015 12:34:21 AM
 *  Author: Blake Carpenter, Spencer Allen, Benjamin Adams
 */ 


#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "InterruptHandler.h"

volatile uint8_t transmit_ISR = 1;
static char *pData;

void serial_open(long speed, int config){
	sei();
	int ubrr = (F_CPU/(16*speed))-1;
	UBRR0H = ubrr >> 8;
	UBRR0L = ubrr & 0xff;
	
	UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);
	UCSR0B = _BV(RXEN0) | _BV(TXEN0);
}

char serial_read(){
	loop_until_bit_is_set(UCSR0A, RXC0);
	return UDR0;
}

void serial_write(char c){
	loop_until_bit_is_set(UCSR0A, UDRE0);
	UDR0 = c;
}

void serial_print(char *str){
	if(transmit_ISR){
		pData = str;
		UCSR0B |= (1<<UDRIE0);
	}
}

ISR (USART0_UDRE_vect){
	if(*pData == 0){
		transmit_ISR = 1;
		UCSR0B &= ~(1<<UDRIE0);
	}
	else{
		transmit_ISR=0;
		UDR0=*pData;
		*pData++;
	}
	if(*pData==0){
		transmit_ISR = 1;
		UCSR0B &= ~(1<<UDRIE0);
	}
}