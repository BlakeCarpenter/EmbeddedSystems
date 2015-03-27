/*
 * Queues.C
 * Author: Blake Carpenter, Spencer Allen, Benjamin Adams
 */

#include <stdlib.h>
#include <stdio.h>
#include "Queues.h"
#include "System.h"

byte Q_putc(byte qid, char data){
    if(!qcb[qid].pQ) return -1; //Queue doesn't exist
    QCB *point = &qcb[qid];
    if(point->in==point->out && (point->flags&(1<<Q_FULL))) return -1; //Queue is full

    *(point->pQ+point->in) = data;
    point->available++;
    point->in++;
    point->in &= point->smask;

    if(point->flags&(1<<Q_EMPTY)) point->flags &= ~(1<<Q_EMPTY);
    if(point->in==point->out) point->flags|=(1<<Q_FULL);
	return 1;
}

byte Q_getc(byte qid, char *pdata ){
    if(!qcb[qid].pQ) return -1; //Queue doesn't exist
    QCB *point = &qcb[qid];
    if(point->in==point->out && (point->flags&(1<<Q_EMPTY))) return -1; //Queue is empty

    *pdata = *(point->pQ+point->out);
    *(point->pQ+point->out) = 0;
    point->available--;
    point->out++;
    point->out &= point->smask;

    if(point->flags&(1<<Q_FULL)) point->flags &= ~(1<<Q_FULL);
    if(point->in==point->out) point->flags|=(1<<Q_EMPTY);
	return 1;
}

int8_t Q_create(int qsize, char * pbuffer){
    int i,j;
	if(qsize&(qsize-1)) return -1;
    for(i=0;i<QCB_MAX_COUNT;i++) if(qcb[i].pQ==NULL){
        qcb[i].pQ = pbuffer;
        qcb[i].in = 0;
        qcb[i].out = 0;
        qcb[i].smask = 31;
        qcb[i].flags |= (1<<Q_EMPTY);
        qcb[i].available = 32;
        for(j=0; j<qsize; j++) *(qcb[i].pQ+j) = 0;
        return i;
    }
    return -1;
}

void Q_delete(byte qid){
    qcb[qid].pQ=NULL;
}

int Q_used(byte qid){
	if(qid >= QCB_MAX_COUNT || qid < 0) return -1;
	return qcb[qid].available;
}

int Q_unused(byte qid){
    if(qid >= QCB_MAX_COUNT || qid < 0) return -1;
    return 32-qcb[qid].available;
}