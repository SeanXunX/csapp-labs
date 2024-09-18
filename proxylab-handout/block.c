#include "block.h"
#include "csapp.h"

void cache_init(deque *cache)
{
    cache->front = NULL;
    cache->rear = NULL;
    cache->total_size = 0;
}

void cache_free(deque *q)
{
    block *ptr = q->front, *_next;
    while (ptr) {
        _next = ptr->next;
        delete_block(ptr);
        ptr = _next;
    }
}

block *create_block(char *uri, char *content)
{
    block *new_block = (block*)Malloc(sizeof(block));
    strcpy(new_block->uri, uri);
    new_block->next = NULL;
    new_block->prev = NULL;
    new_block->content = content;
    new_block->size = strlen(content) + 1; 
    return new_block;
}

void delete_block(block *b)
{
    Free(b->content);
    Free(b);
}

int isEmpty(deque *q)
{
    return q->total_size == 0;
}

void push(deque *q, block *b)
{
    if (b == NULL) {
        return;
    }
    if (isEmpty(q)) {
        q->front = q->rear = b;
    } else {
        q->front->prev = b;
        b->next = q->front;
        q->front = b;
    }
    q->total_size += b->size;
}

void pop(deque *q)
{
    if (isEmpty(q)) {
        return;
    } else {
        q->total_size -= q->rear->size;
        q->rear = q->rear->prev;
        delete_block(q->rear->next);
        q->rear->next = NULL;
    }
}

block *get_cache(deque *q, char *uri)
{
    // todo: delete
    printf("*******enter get cache ************\n");
    printf("uri = %s\n", uri);

    block *ptr = q->front;
    while (ptr) {
        
        // todo: delete
        printf("ptr->uri = %s\n", ptr->uri);
    
        if (!strcmp(ptr->uri, uri)) {
            update(q, ptr);
            return ptr;
        }
        ptr = ptr->next;
    }
    return NULL;
}

void update(deque *q, block *b)
{
    //todo: delete
    printf("*************enter update()************\n");
    if (b->prev == NULL) {
        // b is front
        return;
    } else if (b->next == NULL) {
        b->prev->next = b->next;
        q->rear = b->prev;
        b->prev = NULL;
        q->total_size -= b->size;
        push(q, b);
    } else {
        b->prev->next = b->next;
        b->next->prev = b->prev;
        b->next = b->prev = NULL;
        q->total_size -= b->size;
        push(q, b);
    }
}
