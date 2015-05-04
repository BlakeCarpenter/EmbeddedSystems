/*
 * main.c
 *
 * Created: 5/1/2015
 *  Author: Benjamin Adams, Spencer Allen, Blake Carpenter
 */


#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "InterruptHandler.h"
#include "ADC.h"
#include "acx.h"
#include "Digital.h"

uint16_t pot_read;

void blinky1(void);
void getADCRead(void);
void alterBlink(void);
void transmitPot(void);

int main(void)
{
	serial_open(19200, 0);
	
	//pinMode(13,OUTPUT);
	pinMode(52, OUTPUT);
	pinMode(53, OUTPUT);
	
    x_init();
	x_new(1,alterBlink,1);
	x_new(2,transmitPot,1);
    x_new(3,blinky1,1);
//  x_new(2,blinky2,1);
	//x_new(3,getADCRead,1);
    x_yield();
	

    //DDRB |= 0x80;
    while(1){
        getADCRead();
		x_yield();
    }
}

void getADCRead(){
	pot_read = ADC_read(0,10,1);
	//serial_write(pot_read);
}

void alterBlink(){
	x_delay(2000);
	DDRB |= 0x01;
	uint8_t ticks;
	while(1){
		//serial_write('k');
		ticks=(pot_read/35)+1;
		PORTB ^= 0x01;  // toggle pin
		x_delay(ticks);
		PORTB &= ~(0x01);
		x_delay(31-ticks);
	}
}

void transmitPot(){
	while(1){
		//x_suspend(0);
		int volts = pot_read;
		char buff[25];
		sprintf(buff,"%d volts (0x%x)\n",volts,volts);
		serial_print(buff);
		//x_resume(0);
		x_yield();
		x_delay(500);
	}
}

void blinky1(){
    DDRB |= 0x01;
    while(1){
	    PORTB ^= 0x02;  // toggle pin
	    x_delay(125);
    }
}