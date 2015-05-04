#ifndef DIGITAL_H
#define DIGITAL_H
int pinMode(int pNo, int type);
int digitalRead(int pNo);
void digitalWrite(int pNo, int value);
void flash(int lights[],int buttons[]);
void rotate(int lights[]);
void allOff();
int buttonCapture(int buttons[]);
int buttonDelay(int buttons[], int delay);
#endif