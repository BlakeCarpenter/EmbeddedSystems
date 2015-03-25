/*
 * Digital.c
 *
 * Created: 1/23/2015 10:11:41 AM
 *  Author: carpenterwb
 */ 

#define HIGH 0x1
#define LOW 0x0
#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2
#define NUM_LIGHTS 4
#define NUM_BUTTONS 2
#define STATE_OFF 0
#define STATE_FLASH 1
#define STATE_ROTATE 2

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
void flash(int lights[],int buttons[]);
void rotate(int lights[]);
void allOff();
int buttonCapture(int buttons[]);
int buttonDelay(int buttons[], int delay);

int state;

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	int lights[NUM_LIGHTS]={13,12,11,10};
	int buttons[NUM_BUTTONS]={9,8};
	state = 0;
	for(int i=0;i<NUM_LIGHTS;i++)pinMode(lights[i],OUTPUT);
	for(int i=0;i<NUM_BUTTONS;i++)pinMode(buttons[i],INPUT_PULLUP);
	while(1){
		while(!state){
			//if(buttonCapture(buttons)==9)state=STATE_FLASH;
			switch(buttonCapture(buttons))
			{
				case 9:
					state=STATE_FLASH;
					break;
				case 8:
					state=STATE_ROTATE;
					break;
				default:
					break;
			}
		}
		while(state==STATE_FLASH){
			flash(lights,buttons);
			switch(buttonCapture(buttons))
			{
				case 9:
					state=STATE_OFF;
					break;
				case 8:
					state=STATE_ROTATE;
					break;
				default:
					break;
			}
		}
		while(state==STATE_ROTATE){
			rotate(lights);
			switch(buttonCapture(buttons))
			{
				case 9:
					state=STATE_FLASH;
					break;
				case 8:
					state=STATE_OFF;
					break;
				default:
					break;
			}
			
		}
	}
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

void flash(int lights[],int buttons[]){
	int i;
	allOff(lights);
	for(i=0;i<NUM_LIGHTS;i++){
		digitalWrite(lights[i],HIGH);
	}
	_delay_ms(500);
	for(i=0;i<NUM_LIGHTS;i++){
		digitalWrite(lights[i],LOW);
	}
	_delay_ms(500);
}

void rotate(int lights[]){
	int i;
	allOff(lights);
	for(i=0;i<NUM_LIGHTS;i++){
		digitalWrite(lights[i],HIGH);
		_delay_ms(500);
		digitalWrite(lights[i],LOW);
	}
}

void allOff(int lights[]){
	for(int i=0;i<NUM_LIGHTS;i++){
		digitalWrite(lights[i],LOW);
	}
}

int buttonCapture(int buttons[]){
	for(int i=0;i<NUM_BUTTONS;i++)
		if(digitalRead(buttons[i])==LOW){
			while(digitalRead(buttons[i])==LOW){}
			return buttons[i];
		}
	return 0;
}

int buttonDelay(int buttons[], int delay){
	int x;
	for(int i=0;i<delay;i+=10){
		_delay_ms(10);
		x=buttonCapture(buttons);
		if(x)return x;
	}
	return 0;
}

void changeState(int newState){
	switch(state){
		
	}
}