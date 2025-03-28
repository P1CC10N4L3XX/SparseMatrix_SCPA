/*
    file:logging.c
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

#define HACK_SIZE 3



char *PATH = "../MatriciSparse/cage4/cage4.mtx";

int main(){
    matrix_mrkt *mtx = read_matrix(PATH);
    int i;
    for(i=0; i<mtx->NZ; i++){
        fprintf(stdout, "%d %d %20.19g\n", mtx->I[i], mtx->J[i], mtx->val[i]);
    }
    
    printf("%d\n\n",mtx->NZ);

    CSR_matrix *csrMatrix = transformMatrixToCSR(mtx);

    int *IRP = csrMatrix->IRP;
    double *AS = csrMatrix->AS;

    for(i=0; i<(mtx->M+1); i++){
        printf("%d\n",IRP[i]);
    }

    printf("\n\n");

    for(i=0; i<(mtx->NZ); i++){
        printf("%20.19g\n",AS[i]);
    }

    HLL_matrix *hllMatrix = transformMatrixToHLL(mtx,HACK_SIZE);
    
    freeMRKTMatrix(mtx);
    freeCSRMatrix(csrMatrix);
    freeHLLMatrix(hllMatrix);
}
