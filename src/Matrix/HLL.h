#ifdef HLL_MATRIX_
typedef struct{
    int M;
    int N;
    int MAXNZ;
    int **JA;
    int **AS;
}HLL_matrix;
#endif