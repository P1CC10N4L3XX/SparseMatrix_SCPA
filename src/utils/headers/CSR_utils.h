/*
    file:CSR_utils.h
    Author: P1CC10N4L3XX

    https://github.com/P1CC10N4L3XX/SparseMatrix_SCPA
*/

#ifndef CSR_UTILS_
#define CSR_UTILS_

#include "../../models/CSR.h"
#include "../../models/array.h"
#include "../../models/matrix_mrkt.h"

CSR_matrix *transformMatrixToCSR(matrix_mrkt *m);
CSR_matrix *init_CSR_matrix(int M, int N, int *IRP, double *AS, int *JA, int NZ);
array *sequential(CSR_matrix *csrMatrix, array *arr);
void printCSRMatrix(CSR_matrix *csrMatrix);


#endif 