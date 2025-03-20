#ifndef HLL_MATRIX_
#define HLL_MATRIX_
typedef struct{
    int M;
    int N;
    int MAXNZ;
    int **JA;
    double **AS;
}HLL_matrix;
#endif