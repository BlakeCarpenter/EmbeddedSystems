/*
 * Serial.c
 *
 * Created: 2/7/2015 6:34:45 PM
 *  Author: Benjamin Adams, Spencer Allen, Blake Carpenter
 *
 * Currently only works for port 0.
 */ 

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "Serial.h"
#include "Queues.h"

serial_handler sp[4];
/*
char rx_buffer[4][MAX_BUFFER_SIZE];
char tx_buffer[4][MAX_BUFFER_SIZE];
*/

char rx_buffer[MAX_BUFFER_SIZE];
char tx_buffer[MAX_BUFFER_SIZE];

int main(void){
	init_serial_handlers();
	sei();
	UCSR0B |= (1<<RXCIE0);
	Serial_open(0, 9600, 0, tx_buffer, 32, rx_buffer, 32);
	while(1){
		Serial_write(0, 'F');
		Serial_write(0, Serial_read(0));
	}
}


int Serial_open(int port, long speed, int config, char *tx_buffer, int tx_buffer_size, char *rx_buffer, int rx_buffer_size){
	//Check if it's already active
	if(sp[port].active) return -1;
	
	//Baud
	int ubrr = (F_CPU/(16*speed))-1;
	UBRR0H = ubrr >> 8;
	UBRR0L = ubrr & 0xff;
	UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);
	UCSR0B = _BV(RXEN0) | _BV(TXEN0); //Transmission Status
	
	//Set up QCBs
	sp[port].rx_qcb_id = Q_create(rx_buffer_size, rx_buffer);
	sp[port].tx_qcb_id = Q_create(tx_buffer_size, tx_buffer);
	
	return 0;
}

void Serial_close(int port){
	Q_delete(sp[port].rx_qcb_id);
	Q_delete(sp[port].tx_qcb_id);
	sp[port].active = 0;
}

int Serial_available(int port){
	return qcb[sp[port].tx_qcb_id].available;
}

int Serial_read(int port){
	char x;
	if(Q_getc(sp[0].rx_qcb_id,&x)>-1)
		return x;
	return -1;
}

void Serial_write(int port, char databyte){
	Q_putc(sp[0].tx_qcb_id, databyte);
	UCSR0B |= (1<<UDRIE0);
}

void init_serial_handlers(){
	for(int i = 0; i < 4; i++){
		sp[i].active = 0;
	}
}

ISR(USART0_RX_vect){
	Q_putc(sp[0].rx_qcb_id, UDR0);
}

ISR(USART0_UDRE_vect){
	if(!qcb[sp[0].tx_qcb_id].available) UCSR0B &= ~(1<<UDRIE0);
	else{
		char x;
		Q_getc(sp[0].tx_qcb_id, &x);
		UDR0 = x;
	}
	if(!qcb[sp[0].tx_qcb_id].available) UCSR0B &= ~(1<<UDRIE0);
}