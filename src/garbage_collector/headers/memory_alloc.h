/*
    file: memory_alloc.h
    Author: P1CC10N4L3XX

    https://github.com/P1CC10N4L3XX/SparseMatrix_SCPA
*/
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