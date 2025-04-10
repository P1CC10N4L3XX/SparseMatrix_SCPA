/*
    file: memory_alloc.c
    Author: P1CC10N4L3XX

    https://github.com/P1CC10N4L3XX/SparseMatrix_SCPA
*/
#include <stdio.h>
#include <stdlib.h>

#include "headers/memory_alloc.h"

node *garbage_collector = NULL;

void freeAll(){
    node *current = garbage_collector;
    while(current!=NULL){
        free(current->ptr);
        node *temp = current;
        current = current->next;
        free(temp);
    }
}

unsigned int controlDouble(void *ptr){
    node *current = garbage_collector;
    while(current!=NULL){
        if(current->ptr == ptr){
            return 1;
        }
        current=current->next;
    }
    return 0;
}

void addToGarbage(void *ptr){
    if(!controlDouble(ptr)){
        node *newNode = (node *)malloc(sizeof(*newNode));
        if(!newNode){
            fprintf(stderr, "addToGarbage: Error: can't allcate memory\n");
            freeAll();
            exit(EXIT_FAILURE);
        }
        newNode->ptr = ptr;
        newNode->next = garbage_collector;
        garbage_collector = newNode;
    }
}

void *memory_alloc(size_t size){
    void *ptr = malloc(size);
    addToGarbage(ptr);
    return ptr;
}

void *memory_realloc(void *_ptr, size_t size){
    void *ptr = realloc(_ptr, size);
    if (!ptr) {
        return NULL;
    }

    if (_ptr != ptr) {
        node *prev = NULL, *curr = garbage_collector;
        while (curr) {
            if (curr->ptr == _ptr) {
                curr->ptr = ptr; 
                return ptr;
            }
            prev = curr;
            curr = curr->next;
        }
        addToGarbage(ptr);
    }

    return ptr;
}


void *memory_calloc(size_t count, size_t size){
    void *ptr = calloc(count,size);
    addToGarbage(ptr);
    return ptr;
}