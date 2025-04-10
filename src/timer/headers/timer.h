#ifndef TIMER_
#define TIMER_

#include <time.h>
#include "../../models/array.h"

typedef struct{
    double time;
    void *res;
}timer_result;

timer_result *measure_flops(array *(*function)(void *, void *),void *mtx, void *array,int NZ);

#endif