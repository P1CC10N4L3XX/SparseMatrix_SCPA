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