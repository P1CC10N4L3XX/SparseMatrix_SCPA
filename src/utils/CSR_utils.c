#include <stdio.h>
#include <stdlib.h>

#include "../Matrix/matrix_mrkt.h"
#include "../Matrix/CSR.h"
#include "headers/CSR_utils.h"

int countNzInRawK(int k,int NZ, int *raws){
    int count = 0;
    for(int i=0; i<NZ; i++){
        if(raws[i] == k){
            count++;
        }
    }

    return count;
}

int *computeIRP(matrix_mrkt *m){
    int *IRP = malloc(sizeof(int)*(m->M+1));
    IRP[0] = 1;
    for(int i=1; i<(m->M+1); i++){
        IRP[i] =  IRP[i-1] + countNzInRawK(i-1, m->NZ, m->I);
    }
    return IRP;
}

double *computeAS(matrix_mrkt *m){
    double *AS = malloc(sizeof(*AS)*(m->NZ));
    int count=0;
    for(int i=0; i<(m->M); i++){
        for(int j=0; j<(m->NZ); j++){
            if(m->I[j]==i){
                AS[count]=m->val[j];
                count++;
            }
        }
    }
    return AS;
}


CSR_matrix *init_CSR_matrix(int M, int N, int *IRP, double *AS, int *JA, int NZ){
    
    CSR_matrix *csrMatrix = (CSR_matrix *)malloc(sizeof(*csrMatrix));
    
    csrMatrix->IRP = IRP;
    csrMatrix->AS = AS;
    csrMatrix->JA = JA;
    csrMatrix->M = M;
    csrMatrix->N = N;

    return csrMatrix;
}


CSR_matrix *transformMatrixToCSR(matrix_mrkt *m){

    int *IRP = computeIRP(m);
    double *AS = computeAS(m);


    return init_CSR_matrix(m->M,m->N,IRP,AS,m->J,m->NZ);
}