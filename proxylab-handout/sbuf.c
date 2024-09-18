#include "csapp.h"
#include "sbuf.h"

/**
 * Create an empty, bounded shared buffer with n slots.
 */
void sbuf_init(sbuf_t *sp, int n) {
    sp->buf = Calloc(n, sizeof(int));   
    sp->n = n;                          /* Buffer holds max of n items*/
    sp->front = sp->rear = 0;           /* Empty buffer iff front*/
    Sem_init(&sp->mutex, 0, 1);         /* Binary semaphore for locking */
    Sem_init(&sp->slots, 0, n);         /* Initially, buf has n empty slots */
    Sem_init(&sp->items, 0, 0);         /* Initially, buf has zero data items*/
}

/**
 * Clean up buffer sp
 */
void sbuf_deinit(sbuf_t *sp) {
    Free(sp->buf);
}

/**
 * Insert item onto the rear of shared buffer sp.
 */
void sbuf_insert(sbuf_t *sp, int item) {
    P(&sp->slots);
    P(&sp->mutex);
    sp->buf[(++sp->rear)%(sp->n)] = item;
    V(&sp->mutex);
    V(&sp->items);
}

/**
 * Remove and return the first item from buffer sp
 */
int sbuf_remove(sbuf_t *sp) {
    int item;
    P(&sp->items);
    P(&sp->mutex);
    item = sp->buf[(++sp->front)%(sp->n)];
    V(&sp->mutex);
    V(&sp->slots);
    return item;
}