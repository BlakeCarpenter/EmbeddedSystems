/*
 * Interrupt.c
 *
 * Created: 2/25/2015 10:33:09 PM
 *  Author: Blake
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define HIGH 0x1
#define LOW 0x0
#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2

struct pinMap {
	int addr;
	int bit;
};

struct pinMap map[54] = {
	{0x2c, 0},
	{0x2c, 1},
	{0x2c, 4},
	{0x2c, 5},
	{0x32, 5},
	{0x2c, 3},
	{0x100, 3},
	{0x100, 4},
	{0x100, 5},
	{0x100, 6},
	{0x23, 4},
	{0x23, 5},
	{0x23, 6},
	{0x23, 7},
	{0x103, 1},
	{0x103, 0},
	{0x100, 1},
	{0x100, 0},
	{0x29, 3},
	{0x29, 2},
	{0x29, 1},
	{0x29, 0},
	{0x20, 0},
	{0x20, 1},
	{0x20, 2},
	{0x20, 3},
	{0x20, 4},
	{0x20, 5},
	{0x20, 6},
	{0x20, 7},
	{0x26, 7},
	{0x26, 6},
	{0x26, 5},
	{0x26, 4},
	{0x26, 3},
	{0x26, 2},
	{0x26, 1},
	{0x26, 0},
	{0x29, 7},
	{0x32, 2},
	{0x32, 1},
	{0x32, 0},
	{0x109, 7},
	{0x109, 6},
	{0x109, 5},
	{0x109, 4},
	{0x109, 3},
	{0x109, 2},
	{0x109, 1},
	{0x109, 0},
	{0x23, 3},
	{0x23, 2},
	{0x23, 1},
	{0x23, 0}
};

int pinMode(int pNo, int type);
int digitalRead(int pNo);
void digitalWrite(int pNo, int value);
void serial_open(long speed, int config);
char serial_read();
void serial_write(char c);
void parseInterruptCount(int count);

volatile int interrupt_count = 0;

int main(void){
	pinMode(13,OUTPUT);
	pinMode(53,INPUT_PULLUP);
	serial_open(19200, 0);
	int count;
	
	PCICR |= (1<<PCIE0); //Enable PCI0 in PC Control Register
	PCMSK0 |= (1<<PCINT0); //Enable PC Mask 0
	sei();
	
while(1)
    {
        if(interrupt_count){
	        count = 50;
	        while(count) {
				_delay_ms(1);
		        count--;
	        }
	        serial_write(interrupt_count+48);
			serial_write('\n');
	        interrupt_count = 0;
        }
    }
}

void parseInterruptCount(int count){
	int digits[20];
	int i = 0;
	while(count>10 && i<20){
		digits[i] = count%10;
		count/=10;
		i++;
	}
	for(int j=0;j<20;j++){
		serial_write(digits[i]+0x30);
	}
	serial_write('\n');
}

void serial_open(long speed, int config){
	int ubrr = (F_CPU/(16*speed))-1;
	UBRR0H = ubrr >> 8;
	UBRR0L = ubrr & 0xff;
	
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

ISR (PCINT0_vect){
	interrupt_count++;
}

int pinMode(int pNo, int type){
	if(pNo < 0 || pNo > 53)
	return -1;
	struct pinMap n = map[pNo];
	volatile uint8_t * address = (volatile uint8_t *) n.addr;
	int mask = (1<<n.bit);

	switch(type){
		case 0x0:
		address[1] &= (~mask);
		address[2] &= (~mask);
		break;
		case 0x1:
		address[1] |= mask;
		break;
		case 0x2:
		address[1] &= (~mask);
		address[2] |= mask;
		break;
		default:
		break;
	}
	return 0;
}

int digitalRead(int pNo){
	struct pinMap n = map[pNo];
	volatile uint8_t * address = (volatile uint8_t *) n.addr;
	int mask = (1<<n.bit);
	return (!!(address[0] &= mask));
}

void digitalWrite(int pNo, int value){
	struct pinMap n = map[pNo];
	volatile uint8_t * address = (volatile uint8_t *) n.addr;
	int mask = (1<<n.bit);
	if(value == HIGH)
	address[2] |= mask;
	else if(value == LOW)
	address[2] &= ~mask;
}