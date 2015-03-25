/*
 * Serial.c
 *
 * Created: 2/7/2015 6:34:45 PM
 *  Author: Benjamin Adams, Spencer Allen, Blake Carpenter
 */ 

#include <avr/io.h>

void serial_open(long speed, int config);
char serial_read();
void serial_write(char c);

int main(void)
{
	serial_open(9600, 0);
    while(1)
    {
        serial_write(serial_read());
   }
}

void serial_open(long speed, int config){
	int ubrr = (F_CPU/(16*speed))-1;
	UBRR0H = ubrr >> 8;
	UBRR0L = ubrr & 0xff;
	
	/*UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;*/
	
	/*Modify this with config for the various serial values*/
	UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);
	
	UCSR0B = _BV(RXEN0) | _BV(TXEN0); //Transmission Status
}

char serial_read(){
	loop_until_bit_is_set(UCSR0A, RXC0);
	return UDR0;
}

void serial_write(char c){
	loop_until_bit_is_set(UCSR0A, UDRE0);
	UDR0 = c;
}