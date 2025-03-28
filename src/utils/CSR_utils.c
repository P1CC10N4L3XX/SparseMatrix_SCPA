/*
    file:logging.c
    Author: P1CC10N4L3XX
    
    https://github.com/P1CC10N4L3XX/SparseMatrix_SCPA
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "../Matrix/matrix_mrkt.h"
#include "../Matrix/CSR.h"
#include "headers/CSR_utils.h"

void freeCSRMatrix(CSR_matrix *csrMatrix){
    free(csrMatrix->AS);
    free(csrMatrix->IRP);
    free(csrMatrix->JA);
    free(csrMatrix);
}

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
    if(!IRP){
        fprintf(stderr,"computeIRP: Error: can't allocate memory for IRP\n");
        exit(EXIT_FAILURE);
    }
    IRP[0] = 1;
    for(int i=1; i<(m->M+1); i++){
        IRP[i] =  IRP[i-1] + countNzInRawK(i-1, m->NZ, m->I);
    }
    return IRP;
}

double *computeAS_CSR(matrix_mrkt *m){
    double *AS = malloc(sizeof(*AS)*(m->NZ));
    if(!AS){
        fprintf(stderr, "computeAS_CSR: Error: can't allocate memory for AS\n");
        exit(EXIT_FAILURE);
    }
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
    if(!csrMatrix){
        fprintf(stderr, "init_CSR_matrix: Error: can't allocate memory for CSR matrix");
        free(IRP);
        free(AS);
        free(JA);
        exit(EXIT_FAILURE);
    }
    
    csrMatrix->IRP = IRP;
    csrMatrix->AS = AS;
    csrMatrix->JA = JA;
    csrMatrix->M = M;
    csrMatrix->N = N;

    return csrMatrix;
}


CSR_matrix *transformMatrixToCSR(matrix_mrkt *m){
    if(m==NULL){
        printf("transformMatrixToCSR: Error: matrix point to NULL\n");
    }
    assert(m!=NULL);

    int *IRP = computeIRP(m);
    double *AS = computeAS_CSR(m);


    return init_CSR_matrix(m->M,m->N,IRP,AS,m->J,m->NZ);
}