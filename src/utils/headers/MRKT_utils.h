#ifndef MRKT_UTILS_
#define MRKT_UTILS_
#include "../../Matrix/matrix_mrkt.h"

matrix_mrkt *read_matrix(char *filepath);
matrix_mrkt *init_matrix_mrkt(int *I, int *J, int M, int N, int NZ, double *val);

#endif