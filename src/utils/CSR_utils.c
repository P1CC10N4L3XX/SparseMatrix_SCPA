/*
    file:CSR_utils.c
    Author: P1CC10N4L3XX
    
    https://github.com/P1CC10N4L3XX/SparseMatrix_SCPA
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#include "../models/matrix_mrkt.h"
#include "../models/CSR.h"
#include "../models/array.h"
#include "headers/CSR_utils.h"
#include "headers/array_utils.h"


void printCSRMatrix(CSR_matrix *csrMatrix){
    printf("STAMPA DELLA MATRICE CSR...\n\n");
    int i=0;
    int j=0;
    char *IRP_string=NULL,*JA_string=NULL, *AS_string=NULL;

    printf("M: %d\n", csrMatrix->M);
    printf("N: %d\n",csrMatrix->N);
    printf("NZ: %d\n",csrMatrix->NZ);

    while(i<csrMatrix->M+1 || j<csrMatrix->NZ){
        if(i<csrMatrix->M+1){
            asprintf(&IRP_string, "%s%d, ", (IRP_string!=NULL)? IRP_string : "", csrMatrix->IRP[i]);
        }
        if(j<csrMatrix->NZ){
            asprintf(&JA_string, "%s%d, ", (JA_string!=NULL)? JA_string : "",csrMatrix->JA[j]);
            asprintf(&AS_string,"%s%2.3g, ", (AS_string!=NULL)? AS_string : "", csrMatrix->AS[j]);
        }
        i++;
        j++;
    }
    printf("IRP: [%s\b\b]\b\n",IRP_string);
    printf("JA: [%s\b\b]\n",JA_string);
    printf("AS: [%s\b\b]\b\n",AS_string);
    free(IRP_string);
    free(JA_string);
    free(AS_string);
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

array *sequential(CSR_matrix *csrMatrix, array *arr){
    double *v = (double *)calloc(sizeof(*v), csrMatrix->M);
    size_t len = (size_t) csrMatrix->M;
    clock_t start,end;
    double time;
    if(!v){
        fprintf(stderr, "sequential: can't allocate memory for vector\n");
        freeCSRMatrix(csrMatrix);
        freeArray(arr);
        exit(EXIT_FAILURE);
    }

    start = clock();

    for(int i=0; i<len; i++){
        for(int j=csrMatrix->IRP[i]; j<csrMatrix->IRP[i+1]; j++){
            v[i]+=csrMatrix->AS[j]*arr->v[csrMatrix->JA[j]];
        }
    }

    end = clock();

    time = (double)(end - start) / CLOCKS_PER_SEC;

    printf("FLOPS: %20.19g\n", (double)2*csrMatrix->NZ/time);

    return initArray(v,len);
}