/*
    file:array_utils.h
    Author: P1CC10N4L3XX

    https://github.com/P1CC10N4L3XX/SparseMatrix_SCPA
*/

#ifndef RANDOM_VECTOR_
#define RANDOM_VECTOR_

#include <stdlib.h>

#include "../../models/array.h"

array *generateRandomArray(size_t len);
array *initArray(double *v, size_t len);
void printRandomArray(array *randomArray);

#endif