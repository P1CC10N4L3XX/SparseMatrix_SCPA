#ifdef CSR_MATRIX_

typedef struct{
    int M;
    int N;
    int *IRP;
    int *JA;
    int NZ;
    double *AS;
}CSR_matrix;

#endif