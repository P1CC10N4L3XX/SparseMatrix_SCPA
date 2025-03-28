/*
    file:logging.c
    Author: P1CC10N4L3XX

    https://github.com/P1CC10N4L3XX/SparseMatrix_SCPA
*/

#ifndef MATRIX_MRKT_
#define MATRIX_MRKT_

typedef struct{
    int NZ;
    int M;
    int N;
    int *I;
    int *J;
    double *val;
}matrix_mrkt;

#endif