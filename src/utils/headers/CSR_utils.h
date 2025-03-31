/*
    file:CSR_utils.h
    Author: P1CC10N4L3XX

    https://github.com/P1CC10N4L3XX/SparseMatrix_SCPA
*/

#ifndef CSR_UTILS_
#define CSR_UTILS_

#include "../../Matrix/CSR.h"
#include "../../Matrix/matrix_mrkt.h"

CSR_matrix *transformMatrixToCSR(matrix_mrkt *m);
CSR_matrix *init_CSR_matrix(int M, int N, int *IRP, double *AS, int *JA, int NZ);
void freeCSRMatrix(CSR_matrix *csrMatrix);
void printCSRMatrix(CSR_matrix *csrMatrix);


#endif 