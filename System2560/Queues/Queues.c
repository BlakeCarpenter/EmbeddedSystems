/*
 * Queues.C
 * Author: Blake Carpenter, Spencer Allen, Benjamin Adams
 * Version: 1.0
 *
 * Current Version Notes:
 *  For putc and getc, need to implement circular queue functionality, use smask.
 */

#include <stdlib.h>
#include "Queues.h"
#include "System.h"

QCB qcb[QCB_MAX_COUNT];

void main(){
	return 0;
}

byte Q_putc(byte qid, char data){
    if(qid >= QCB_MAX_COUNT || qid < 0)return -1;
    QCB *Q_pointer = &qcb[qid];
    if(Q_pointer->flags == Q_FULL) return 0;
    
    //Else the queue is at any other state, so put the char
    *(Q_pointer->pQ+Q_pointer->in) = data;
    Q_pointer->in++;

    //Check to see if the queue is now full
    if(Q_pointer->in == Q_pointer->out && Q_pointer->available)
        Q_pointer->flags = Q_FULL;
    return 1;
}

byte Q_getc(byte qid, char *pdata ){
    if(qid >= QCB_MAX_COUNT || qid < 0) return -1;
    QCB *Q_pointer = &qcb[qid];
    if(Q_pointer->available){
        *pdata = (Q_pointer->pQ+Q_pointer->out);
        *(Q_pointer->pQ+Q_pointer->out) = NULL;
        Q_pointer->out++;
        Q_pointer->available--;

        //Check if empty to set flag
        if(Q_pointer->available==0)
            Q_pointer->flags = Q_EMPTY;
        return 1;
    }
    else return 0;
}

int8_t Q_create(int qsize, char * pbuffer){
    int i;
    for(i=0;i<QCB_MAX_COUNT;i++) if(qcb[i].pQ==NULL){
        QCB temp = qcb[i];
        temp.smask=qsize-1;
        temp.available=qsize;
        temp.pQ=pbuffer;
        return i;
    }
    return -1;
}

void Q_delete(byte qid){
    qcb[qid].pQ=NULL;
}

int Q_used(byte qid){
    if(qid >= QCB_MAX_COUNT || qid < 0) return -1;
    return 64-qcb[qid].available;
}

int Q_unused(byte qid){
    if(qid >= QCB_MAX_COUNT || qid < 0) return -1;
    return qcb[qid].available;
}
