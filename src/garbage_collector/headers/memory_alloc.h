#ifndef MEMORY_ALLOC_
#define MEMORY_ALLOC_

#include <stdlib.h>


typedef struct node{
    void *ptr;
    struct node *next;
}node;

void *memory_alloc(size_t size);
void *memory_realloc(void *type, size_t size);
void *memory_calloc(size_t count, size_t size);
void freeAll();

#endif