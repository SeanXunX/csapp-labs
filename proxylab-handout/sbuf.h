#ifndef _SBUF_H
#define _SBUF_H

typedef struct {
    int *buf;   /* Buffer array */
    int n;      /* Maximum number of slots */
    int front;  /* buf[(front + 1) % n] is first item */
    int rear;   /* buf[rear % n] is lat item */
    sem_t mutex;    /* ptotects accesses slots */
    sem_t slots;    /* counts available slots */
    sem_t items;    /* Counts available items */
} sbuf_t;


#endif

void sbuf_init(sbuf_t *sp, int n);

void sbuf_deinit(sbuf_t *sp);

void sbuf_insert(sbuf_t *sp, int item);

int sbuf_remove(sbuf_t *sp);
