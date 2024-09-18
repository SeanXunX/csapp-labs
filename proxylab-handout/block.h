#pragma once

/*
 *  block-deque: a FILO deque
 */

#include "csapp.h"

#define MAX_OBJECT_SIZE 102400

typedef struct block {
    char uri[MAXLINE];
    struct block* next;
    struct block* prev;
    char *content;
    int size;
} block;

typedef struct deque {
    block *front;
    block *rear;
    int total_size;
} deque;

/**
 * cache_init - initialize the cache deque
 */
void cache_init(deque *cache);

/**
 * cache_free
 */
void cache_free(deque *q);

/** 
 * create_block - content must be allocated dynamically
 */
block* create_block(char *uri, char *content);

/**
 * delete_block - Be careful to free content.
 */
void delete_block(block *b);

/**
 * isEmpty(deque *q)
 */
int isEmpty(deque *q);

/**
 * push - add new block b to the front of the deque 
 */
void push(deque *q, block *b);

/**
 * pop - remove the elem at the rear of the deque
 */
void pop(deque *q);

/**
 * find - return block * in the deque with given uri. If not found, return NULL.
 */
block *get_cache(deque *q, char *uri);

/**
 * update - move the block b to the front of deque
 */
void update(deque *q, block *b);
