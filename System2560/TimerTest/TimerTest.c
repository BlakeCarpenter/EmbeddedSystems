/*
 * TimerTest.c
 *
 * Created: 3/30/2015 10:37:04 PM
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
int ledState;

int main(void)
{
	
	OCR1A = 0; //50 ms
	TIMSK1 |= (1<<OCIE1A); //Enable Timer match interrupts
	//Set to NORMAL
	TCCR1A |= ((1<<WGM11)|(1<<WGM10));
	TCCR1B |= ((1<<WGM13)|(1<<WGM12));
	
	
	sei();//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	while(1){
		
	}

/*CTC idea
	CTC returns to zero at TOP. Set TOP to 200ms in ICR1, place 50ms in comp register
	(OCR1A). Setup interrupt for OCR1A match to turn off LED. Setup interrupt for TOVn
	to turn LED on.
*/

/*PWN idea
	PWN mode bounces from 0 to TOP to 0. Half increments? i.e., 25,75, so that you can
	use one value as the trigger.
*/

}

ISR(TIMER1_COMPA_vect){
	if(){ // countdown from 0xFFFF #TOVn flag is set 
		//negate 50ms and 150ms time interval int 
	} else{
		// 50ms time interval int
		//150ms time interval int
	}
	if(){ // 50ms time interval int
		//add 150ms 
	}
	else if(){ //150ms time interval int
		
		//add 150ms time interval int 
	}
	
}

//DIGITAL CODE
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