/*
    file:timer.c
    Author: P1CC10N4L3XX

    https://github.com/P1CC10N4L3XX/SparseMatrix_SCPA
*/

#include <time.h>
#include <stdio.h>

#include "headers/timer.h"
#include "../garbage_collector/headers/memory_alloc.h"
#include "../models/array.h"

timer_result *measure_flops(array *(*function)(void *, void *),void *mtx, void *arr,int NZ){
    clock_t end, start;
    double time;

    start = clock();

    array *function_result = function(mtx, arr);

    end = clock();

    time = (double) (end - start) / CLOCKS_PER_SEC;

    double flops = 2 * NZ/time;
    
    timer_result *result = memory_alloc(sizeof(*result));
    result->res = function_result;
    result->time = flops;

    return result;
}