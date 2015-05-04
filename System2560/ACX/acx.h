/*
 * acx.h
 *
 * Created: 4/20/2015 
 *  Author: efb
 */ 


#ifndef ACX_H_
#define ACX_H_

#define  ATMEGA_2560 //THIS IS DEFINED FOR ARDUINO MEGA2560 boards

//#ifndef     NULL
//    #define NULL    ((void *)0)
//#endif

#define		MAX_THREADS			8
#define		NUM_THREADS			8

#define		STACK_CANARY		0xAA

#define		T0_STACK_SIZE		128
#define		T1_STACK_SIZE		128
#define		T2_STACK_SIZE		128
#define		T3_STACK_SIZE		128
#define		T4_STACK_SIZE		128
#define		T5_STACK_SIZE		128
#define		T6_STACK_SIZE		128
#define		T7_STACK_SIZE		128

#define		STACK_MEM_SIZE		(T0_STACK_SIZE + T1_STACK_SIZE + \
								T2_STACK_SIZE + T3_STACK_SIZE + T4_STACK_SIZE +\
								T5_STACK_SIZE + T6_STACK_SIZE + T7_STACK_SIZE)

#define		T0_STACK_BASE_OFFS	(T0_STACK_SIZE-1)
#define		T1_STACK_BASE_OFFS	(T0_STACK_BASE_OFFS+T1_STACK_SIZE)
#define		T2_STACK_BASE_OFFS	(T1_STACK_BASE_OFFS+T2_STACK_SIZE)
#define		T3_STACK_BASE_OFFS	(T2_STACK_BASE_OFFS+T3_STACK_SIZE)
#define		T4_STACK_BASE_OFFS	(T3_STACK_BASE_OFFS+T4_STACK_SIZE)
#define		T5_STACK_BASE_OFFS	(T4_STACK_BASE_OFFS+T5_STACK_SIZE)
#define		T6_STACK_BASE_OFFS	(T5_STACK_BASE_OFFS+T6_STACK_SIZE)
#define		T7_STACK_BASE_OFFS	(T6_STACK_BASE_OFFS+T7_STACK_SIZE)

#define		T0_ID	0
#define		T1_ID	1
#define		T2_ID	2
#define		T3_ID	3
#define		T4_ID	4
#define		T5_ID	5
#define		T6_ID	6
#define		T7_ID	7

#define THREAD_CONTEXT_SIZE  18  // Number of bytes (registers) saved during context switch

#ifndef __ASSEMBLER__

typedef uint8_t byte;
typedef uint8_t bool;


#define x_getTID()	x_thread_id


//typedef void(* PTHREAD)(int, char * ); 
//---------------------------------------------------------------------------
// PTHREAD is a type that represents how threads are called--
// It is just a pointer to a function returning void
// that is a passed no parameters (for now).
//---------------------------------------------------------------------------
typedef void (*PTHREAD)(void);

//---------------------------------------------------------------------------
// This union is used to provide access to both bytes of a thread address
//---------------------------------------------------------------------------
typedef  union {
	PTHREAD pthread;
	byte addr[4];  // Only 2 bytes needed for UNO (atmega328, etc.), but 3 are needed for MEGA2560 (atmega2560).
} PTU;

//---------------------------------------------------------------------------
// This type is used for entries in the stack control table
//---------------------------------------------------------------------------
typedef	struct {
		byte *sp;
		byte *spBase;
}STACK_CONTROL;

// ACX Function prototypes
void	x_init(void);
void	x_delay(int);
void	x_schedule(void);
void x_new(byte, PTHREAD , byte);
void x_yield(void);
byte bit2mask8(int);


#endif


#endif /* ACX_H_ */