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


void printCSRMatrix(CSR_matrix *csrMatrix){
    printf("STAMPA DELLA MATRICE CSR...\n\n");
    int i=0;
    int j=0;
    char *IRP_string,*JA_string, *AS_string;

    printf("M: %d\n", csrMatrix->M);
    printf("N: %d\n",csrMatrix->N);
    printf("NZ: %d\n",csrMatrix->NZ);

    while(i<csrMatrix->M+1 || j<csrMatrix->NZ){
        if(i<csrMatrix->M+1){
            asprintf(&IRP_string, "%s%d ", IRP_string, csrMatrix->IRP[i]);
        }
        if(j<csrMatrix->NZ){
            asprintf(&JA_string, "%s%d ",JA_string ,csrMatrix->JA[j]);
            asprintf(&AS_string,"%s%2.3g ", AS_string, csrMatrix->AS[j]);
        }
        i++;
        j++;
    }
    printf("IRP: [ %s]\n",IRP_string);
    printf("JA: [ %s]\n",JA_string);
    printf("AS: [ %s]\n",AS_string);
}

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
    int *IRP = (int *)malloc(sizeof(int)*(m->M+1));
    if(!IRP){
        fprintf(stderr,"computeIRP: Error: can't allocate memory for IRP\n");
        exit(EXIT_FAILURE);
    }
    IRP[0] = 0;
    for(int i=1; i<(m->M+1); i++){
        IRP[i] =  IRP[i-1] + countNzInRawK(i-1, m->NZ, m->I);
    }
    return IRP;
}

CSR_matrix *init_CSR_matrix(int M, int N, int *IRP, double *AS, int *JA, int NZ){
    
    CSR_matrix *csrMatrix = (CSR_matrix *)malloc(sizeof(*csrMatrix));
    if(!csrMatrix){
        fprintf(stderr, "init_CSR_matrix: Error: can't allocate memory for CSR matrix\n");
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
    csrMatrix->NZ = NZ;

    return csrMatrix;
}


CSR_matrix *transformMatrixToCSR(matrix_mrkt *m){
    if(m==NULL){
        printf("transformMatrixToCSR: Error: matrix point to NULL\n");
    }
    assert(m!=NULL);

    int *IRP = computeIRP(m);


    return init_CSR_matrix(m->M,m->N,IRP,m->val,m->J,m->NZ);
}