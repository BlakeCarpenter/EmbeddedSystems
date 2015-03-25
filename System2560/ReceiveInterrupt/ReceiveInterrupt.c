/*
 * ReceiveInterrupt.c
 *
 * Created: 3/5/2015 10:58:31 AM
 *  Author: Spencer Allen, Blake Carpenter
 */ 

#define RECEIVE_LENGTH 32

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

char receive[RECEIVE_LENGTH];
static volatile char *receivePointer;
static volatile uint8_t numCharRec;
uint8_t receiverReady = 1;
volatile uint8_t transmit_ISR = 1;
static char *pData;

char * serial_gets(char * buffer, uint8_t length);
void serial_open(long speed, int config);
char serial_read();
void serial_write(char c);
void serial_print(char *str);

int main(void)
{
	sei();
	serial_open(19200, 0);
    while(1)
    {
		serial_gets(receive, 0);
		serial_print(receive);
		loop_until_bit_is_set(UCSR0B, UDRIE0);
		
		
    }
}

char * serial_gets(char * buffer, uint8_t length){
	receivePointer = buffer;
	numCharRec = 0;
	UCSR0B|=(1<<RXCIE0);
	while((UCSR0B&(1<<RXCIE0))!=0){}
	receivePointer = buffer;
	return buffer;
}

ISR(USART0_RX_vect){
	if(numCharRec >= RECEIVE_LENGTH-1){
		*receivePointer = 0;
		UCSR0B&=~(1<<RXCIE0);
	}
	else{
		*receivePointer = UDR0;
		if(*receivePointer=='\n'){
			receivePointer++;
			*receivePointer=0;
			UCSR0B&=~(1<<RXCIE0);
		}
		else{
			receivePointer++;
			numCharRec++;
		}
	}
	if(numCharRec >= RECEIVE_LENGTH-1){
		*receivePointer = 0;
		UCSR0B&=~(1<<RXCIE0);
	}
}

/******     OLD CODE     *****/

void serial_open(long speed, int config){
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