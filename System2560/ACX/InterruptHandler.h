#ifndef INTERRUPTHANDLER_H
#define INTERRUPTHANDLER_H
void serial_open(long speed, int config);
char serial_read();
void serial_write(char c);
void serial_print(char *str);
#endif