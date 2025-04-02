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

array *initArray(double *v, size_t len){
    array *randomArray = (array *)malloc(sizeof(*randomArray));
    if(!randomArray){
        free(v);
        fprintf(stderr, "initArray: Error: can't allocate memory for array\n");
        exit(EXIT_FAILURE);
    }
    randomArray->len = len;
    randomArray->v = v;
    return randomArray;
}


array *generateRandomArray(size_t len){
    double *v =(double *) malloc(sizeof(*v) * len);
    if(!v){
        fprintf(stderr, "generateRandomVector: Error: can't allocate memory for random vector\n");
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

void freeArray(array *arr){
    free(arr->v);
    free(arr);
}