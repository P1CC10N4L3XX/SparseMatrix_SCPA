/*
    file:array_utils.c
    Author: P1CC10N4L3XX

    https://github.com/P1CC10N4L3XX/SparseMatrix_SCPA
*/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "headers/array_utils.h"
#include "../models/array.h"
#include "../garbage_collector/headers/memory_alloc.h"

array *initArray(double *v, size_t len){
    array *randomArray = (array *)memory_alloc(sizeof(*randomArray));
    if(!randomArray){
        fprintf(stderr, "initArray: Error: can't allocate memory for array\n");
        freeAll();
        exit(EXIT_FAILURE);
    }
    randomArray->len = len;
    randomArray->v = v;
    return randomArray;
}


array *generateRandomArray(size_t len){
    double *v =(double *) memory_alloc(sizeof(*v) * len);
    if(!v){
        fprintf(stderr, "generateRandomVector: Error: can't allocate memory for random vector\n");
        freeAll();
        exit(EXIT_FAILURE);
    }
    double random_number=0;
    srand(time(NULL));
    for(int i=0;i<len;i++){
        random_number = (double)rand()/RAND_MAX;
        v[i] = random_number;
    }

    return initArray(v,len);
}

void printRandomArray(array *randomArray){
    double *v = randomArray->v;
    printf("vector: [");
    for(int i=0;i<randomArray->len;i++){
        printf("%2.3g, ",v[i]);
    }
    printf("\b\b]\n");
}