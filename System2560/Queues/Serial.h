#ifndef SERIAL_H
#define SERIAL_H

#include "System.h"

#define MAX_BUFFER_SIZE 64

//Register access bits, static
#define RXCn 7
#define TXCn 6
#define URDEn 5
#define RXCIEn 7
#define TXCIEn 6
#define UDRIEn 5

int Serial_open(int port, long speed, int config, char *tx_buffer, int tx_buffer_size, char *rx_buffer, int rx_buffer_size);
void Serial_close(int port);
int Serial_available(int port);
int Serial_read(int port);
void Serial_write(int port, char databyte);
void usart_macro_init();
void init_serial_handlers();

typedef struct{
	byte active; //Port is active/inactive
	byte rx_qcb_id; //QCB Ids
	byte tx_qcb_id;
}serial_handler;

#endif