#ifndef DIGITAL_H
#define DIGITAL_H

#define HIGH 0x1
#define LOW 0x0
#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2

int pinMode(int pNo, int type);
int digitalRead(int pNo);
void digitalWrite(int pNo, int value);
void flash(int lights[],int buttons[]);
void rotate(int lights[]);
void allOff();
int buttonCapture(int buttons[]);
int buttonDelay(int buttons[], int delay);
#endif