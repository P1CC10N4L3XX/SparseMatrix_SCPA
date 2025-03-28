/*
    file:logging.c
    Author: P1CC10N4L3XX

    https://github.com/P1CC10N4L3XX/SparseMatrix_SCPA
*/

#ifndef HLL_MATRIX_
#define HLL_MATRIX_
typedef struct{
    int M;
    int N;
    int MAXNZ;
    int **JA;
    double **AS;
}ELLPACK_block;

typedef struct{
    int hackSize;
    int numberOfBlocks;
    ELLPACK_block **blocks;
}HLL_matrix;

#endif