#include <stdio.h>
#include <stdlib.h>

#include "utils/headers/read.h"
#include "Matrix/matrix_mrkt.h"
#include "Matrix/CSR.h"
#include "utils/headers/CSR_utils.h"



char *PATH = "../MatriciSparse/cage4/cage4.mtx";

int main(){
    matrix_mrkt *mtx = read_matrix(PATH);
    int i;
    for(i=0; i<mtx->NZ; i++){
        fprintf(stdout, "%d %d %20.19g\n", mtx->I[i]+1, mtx->J[i]+1, mtx->val[i]);
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
    
}
