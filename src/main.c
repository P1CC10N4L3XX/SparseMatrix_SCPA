/*
    file:main.c
    Author: P1CC10N4L3XX

    https://github.com/P1CC10N4L3XX/SparseMatrix_SCPA
*/

#include <stdio.h>
#include <stdlib.h>

#include "utils/headers/MRKT_utils.h"
#include "models/matrix_mrkt.h"
#include "models/CSR.h"
#include "models/HLL.h"
#include "models/array.h"
#include "utils/headers/CSR_utils.h"
#include "utils/headers/HLL_utils.h"
#include "utils/headers/array_utils.h"
#include "garbage_collector/headers/memory_alloc.h"


#define HACK_SIZE 2



char *PATH = "../MatriciSparse/test/test.mtx";

int main(){
    matrix_mrkt *mtx = read_matrix(PATH);
    CSR_matrix *csrMatrix = transformMatrixToCSR(mtx);
    HLL_matrix *hllMatrix = transformMatrixToHLL(mtx,HACK_SIZE);
    array *arr = generateRandomArray(mtx->N);
    array *res = sequential(csrMatrix, arr);

    printMRKTMatrix(mtx);
    puts("");
    printCSRMatrix(csrMatrix);
    puts("");
    printHLLMatrix(hllMatrix);
    puts("");
    printRandomArray(arr);
    puts("");
    printRandomArray(res);
    puts("");
    
    freeAll();
}
