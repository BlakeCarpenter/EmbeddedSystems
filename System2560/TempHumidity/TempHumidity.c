/*
 * TempHumidity.c
 *
 * Created: 4/8/2015 8:13:16 PM
 *  Author: Blake Carpenter, Spencer Allen, Benjamin Adams
 */ 


#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include <util/delay.h>

//Digital declarations and defines
struct pinMap {
	int addr;
	int bit;
};
int pinMode(int pNo, int type);
int digitalRead(int pNo);
void digitalWrite(int pNo, int value);
int state;
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
#define HIGH 0x1
#define LOW 0x0
#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2
void serial_open(long speed, int config);
char serial_read();
void serial_write(char c);
void serial_print(char *str);
volatile uint8_t transmit_ISR = 1;
static char *pData;

#define DHT_PIN 6
uint8_t receive_buffer[41];

int main(void){
	serial_open(19200, 0);
	sei();
	int humidity;
	int temperature;
	char humidityStringBuffer[5];
	char temperatureStringBuffer[5];
	char ilb[45];
	
	for(int i = 0; i < 45; i++) ilb[i] = 0;
	
	for(int i = 0; i < 5; i++){
		receive_buffer[i] = 0;
		humidityStringBuffer[i] = 0;
		temperatureStringBuffer[i] = 0;
	}
	while(1){
		_delay_ms(1000);
        pinMode(DHT_PIN,OUTPUT);
		digitalWrite(DHT_PIN, LOW);
		_delay_ms(5);
		pinMode(DHT_PIN,INPUT_PULLUP);
		while(digitalRead(DHT_PIN)==HIGH){}
		_delay_us(160);
		for(int i = 0; i < 40; i++){
			while(digitalRead(DHT_PIN)==LOW){}
			_delay_us(40);
			if(digitalRead(DHT_PIN)==HIGH) receive_buffer[i] = HIGH;
			else receive_buffer[i] = LOW;
			_delay_us(30);
		}
		/*serial_print("\n");
		for(int i = 0; i < 40; i++){
			if(receive_buffer[i]) serial_write('1');
			else serial_write('0');
		}*/
		humidity = 0;
		temperature = 0;
		for(int i = 0; i < 16; i++){
			humidity <<= 1;
			humidity |= receive_buffer[i];
		}
		for(int i = 16; i < 32; i++){
			temperature <<= 1;
			temperature |= receive_buffer[i];
		}
		
		humidity *= .1;
		temperature *= .1;
		
		itoa(humidity,humidityStringBuffer,10);
		itoa(temperature,temperatureStringBuffer,10);
		
		//strcpy(ilb, "Humidity: ");
		strcpy(ilb, humidityStringBuffer);
		//strcpy(ilb, ", Temperature: ");
		strcpy(ilb, temperatureStringBuffer);
		strcpy(ilb, "\n");
		
		for(int i = 0; i < 45; i++) serial_write(ilb[i]);
		
		/*serial_print("Humidity: ");
		//serial_print(humidityStringBuffer);
		serial_print(", Temperature: ");
		//serial_print(temperatureStringBuffer);
		serial_print("\n");*/
    }
	return 1;
}

//Digital Code
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