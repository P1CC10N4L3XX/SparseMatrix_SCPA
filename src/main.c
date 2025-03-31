/*
    file:main.c
    Author: P1CC10N4L3XX

    https://github.com/P1CC10N4L3XX/SparseMatrix_SCPA
*/

#include <stdio.h>
#include <stdlib.h>

#include "utils/headers/MRKT_utils.h"
#include "Matrix/matrix_mrkt.h"
#include "Matrix/CSR.h"
#include "Matrix/HLL.h"
#include "utils/headers/CSR_utils.h"
#include "utils/headers/HLL_utils.h"

#define HACK_SIZE 4



char *PATH = "../MatriciSparse/test/test.mtx";

int main(){
    matrix_mrkt *mtx = read_matrix(PATH);
    CSR_matrix *csrMatrix = transformMatrixToCSR(mtx);
    HLL_matrix *hllMatrix = transformMatrixToHLL(mtx,HACK_SIZE);

    printMRKTMatrix(mtx);
    puts("");
    printCSRMatrix(csrMatrix);
    puts("");
    printHLLMatrix(hllMatrix);
    puts("");
    
    freeMRKTMatrix(mtx);
    freeCSRMatrix(csrMatrix);
    freeHLLMatrix(hllMatrix);
}
