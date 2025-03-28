/*
    file:logging.c
    Author: P1CC10N4L3XX

    https://github.com/P1CC10N4L3XX/SparseMatrix_SCPA
*/

#ifndef HLL_UTILS_
#define HLL_UTILS_

#include "../../Matrix/HLL.h"
#include "../../Matrix/matrix_mrkt.h"

HLL_matrix *transformMatrixToHLL(matrix_mrkt *m, int hackSize);
HLL_matrix *init_HLL_matrix(int hackSize, int numberOfBlocks, ELLPACK_block **ellpackBlocks);
void freeHLLMatrix(HLL_matrix *hllMatrix);

#endif