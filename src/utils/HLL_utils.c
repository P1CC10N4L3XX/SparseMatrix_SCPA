/*
    file:HLL_utils.c
    Author: P1CC10N4L3XX

    https://github.com/P1CC10N4L3XX/SparseMatrix_SCPA
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "../models/HLL.h"
#include "../models/matrix_mrkt.h"
#include "headers/MRKT_utils.h"
#include "headers/HLL_utils.h"


void freeHLLMatrix(HLL_matrix *hllMatrix){
    for(int i=0; i<hllMatrix->numberOfBlocks; i++){
        free(hllMatrix->blocks[i]->AS);
        free(hllMatrix->blocks[i]->JA);
        free(hllMatrix->blocks[i]);
    }
    free(hllMatrix->blocks);
    free(hllMatrix);
}

void printEllpackBlock(ELLPACK_block *ellpackBlock){
    printf("M: %d\n",ellpackBlock->M);
    printf("N: %d\n",ellpackBlock->N);
    printf("MAXNZ: %d\n",ellpackBlock->MAXNZ);
    char *JA_string = NULL, *AS_string = NULL;
    for(int i=0;i<ellpackBlock->M;i++){
        asprintf(&JA_string,"%s[",(JA_string!=NULL) ? JA_string : "");
        asprintf(&AS_string,"%s[",(AS_string!=NULL) ? AS_string : "");
        for(int j=0;j<ellpackBlock->MAXNZ;j++){
            asprintf(&JA_string,"%s%d, ",JA_string,ellpackBlock->JA[i][j]);
            asprintf(&AS_string,"%s%2.3g, ",AS_string,ellpackBlock->AS[i][j]);
        }
        asprintf(&JA_string,"%s\b\b],",JA_string);
        asprintf(&AS_string,"%s\b\b],",AS_string);
    }
    printf("JA: [%s\b]\n",JA_string);
    printf("AS: [%s\b]\n",AS_string);
    free(JA_string);
    free(AS_string);
}

void printHLLMatrix(HLL_matrix *hllMatrix){
    printf("Stampa della matrice HLL...\n\n");
    for(int i=0;i<hllMatrix->numberOfBlocks; i++){
        printf("Blocco %d\n",i);
        printEllpackBlock(hllMatrix->blocks[i]);
        puts("");
    }
}


matrix_mrkt **divideInBlocks(matrix_mrkt *m, unsigned int size, int *total_blocks){
    *total_blocks = (m->M + size - 1)/size;
    matrix_mrkt **blocks = (matrix_mrkt **)malloc(*total_blocks * sizeof(*blocks));
    if(!blocks){
        fprintf(stderr,"divideInBlocks: Error: can't allocate memory\n");
        exit(EXIT_FAILURE);
    }
    int *I,*J;
    int M = size;
    int N = m->N;
    double *val;
    int *NZ = (int *)calloc(*total_blocks,sizeof(*NZ));
    if(!NZ){
        fprintf(stderr, "divideInBlocks: Error: can't allocate memory\n");
        free(blocks);
        exit(EXIT_FAILURE);
    }

    int j=1;
    for(int i=0;i<m->NZ;i++){
        if(m->I[i]<size*j){
            NZ[j-1]++;
        }else{
            NZ[j]++;
            j++;
        }
    }
    int count=0;
    int total=0;
    for(int i=0; i<*total_blocks; i++){
        count = 0;
        I = (int *)malloc(sizeof(*I)*NZ[i]);
        J = (int *)malloc(sizeof(*J)*NZ[i]);
        val = malloc(sizeof(*val)*NZ[i]);
        if(!I || !J || !val){
            fprintf(stderr, "divideInBlocks: Error: can't allocate memory for I, J, val\n");
            freeMRKTMatrix(m);
            exit(EXIT_FAILURE);
        }
        total+=NZ[i];
        for(int j = (i == 0) ? 0 : NZ[i-1]; j<total; j++){
            I[count]= (i == 0) ? m->I[j] : m->I[j]-size;
            J[count]=m->J[j];
            val[count]=m->val[j];
            count++;
        }
        blocks[i]=init_matrix_mrkt(I,J,M,N,NZ[i],val);
    }

    free(NZ);
    
    return  blocks;

}

int computeMAXNZ(matrix_mrkt *m){
    int count,MAXNZ = 0;
    for(int i=0;i<m->M;i++){
        count=0;
        for(int j=0; j<m->NZ; j++){
            if(m->I[j]==i){
                count++;
            }
        }
        MAXNZ = (count > MAXNZ) ? count : MAXNZ;
    }
    return MAXNZ;
}

int **computeJA(matrix_mrkt *m, int MAXNZ){
    int **JA = (int **)malloc(sizeof(*JA)*m->M);
    if(!JA){
        fprintf(stderr,"computeJA: Error: can't allocate memory for JA\n");
        freeMRKTMatrix(m);
        exit(EXIT_FAILURE);
    }
    int count;
    for(int i=0; i<m->M; i++){
        int *raw = malloc(sizeof(*raw)*MAXNZ);
        count = 0;
        for(int j=0; j<m->NZ;j++){
            if(m->I[j]==i){
                raw[count] = m->J[j];
                count++;
            }
        }
        if(count-1<MAXNZ){
            for(int i=count-1;i<MAXNZ;i++){
                raw[i]=raw[count-1];
            }
        }
        JA[i]=raw;
    }
    return JA;

}

double **computeAS_HLL(matrix_mrkt *m, int MAXNZ){
    double **AS = malloc(sizeof(*AS)*m->M);
    if(!AS){
        fprintf(stderr, "computeAS_HLL: Error: can't allocate memory for AS\n");
        freeMRKTMatrix(m);
        exit(EXIT_FAILURE);
    }
    int count;
    for(int i=0;i<m->M;i++){
        double *raw = calloc(MAXNZ, sizeof(*raw));
        count = 0;
        for(int j=0; j<m->NZ; j++){
            if(m->I[j]==i){
                raw[count] = m->val[j];
                count++;
            }
        }
        AS[i]=raw;
    }
    return AS;
}

ELLPACK_block *init_ELLPACK_Matrix(int M, int N,int MAXNZ,int **JA,double **AS){
    ELLPACK_block *ellpackMatrix = (ELLPACK_block *)malloc(sizeof(*ellpackMatrix));

    if(!ellpackMatrix){
        fprintf(stderr, "initELLPACKMatrix: Error: can't allocate memory for ELLPACK matrix\n");
        for(int i=0;i<MAXNZ;i++){
            free(JA[i]);
            free(AS[i]);
        }
        free(JA);
        free(AS);
        exit(EXIT_FAILURE);
    }

    ellpackMatrix->M = M;
    ellpackMatrix->N = N;
    ellpackMatrix->AS = AS;
    ellpackMatrix->JA = JA;
    ellpackMatrix->MAXNZ = MAXNZ;

    return ellpackMatrix;

}

ELLPACK_block *transformMatrixToELLPACK (matrix_mrkt *m){
    int M = m->M;
    int N = m->N;
    int MAXNZ = computeMAXNZ(m);
    int **JA = computeJA(m,MAXNZ);
    double **AS = computeAS_HLL(m, MAXNZ);

    return init_ELLPACK_Matrix(M,N,MAXNZ,JA,AS);
}

HLL_matrix *init_HLL_Matrix(int hackSize, int numberOfBlocks, ELLPACK_block **ellpackBlocks){
    HLL_matrix *hllMatrix = (HLL_matrix *)malloc(sizeof(*hllMatrix));

    if(!hllMatrix){
        fprintf(stderr, "initHLLMatrix: Error: can't allocate memory for HLL matrix\n");
        for(int i=0; i<numberOfBlocks; i++){
            free(ellpackBlocks[i]->AS);
            free(ellpackBlocks[i]->JA);
            free(ellpackBlocks[i]);
        }
        free(ellpackBlocks);
        exit(EXIT_FAILURE);
    }

    hllMatrix->hackSize = hackSize;
    hllMatrix->numberOfBlocks = numberOfBlocks;
    hllMatrix->blocks = ellpackBlocks;

    return hllMatrix;
}


HLL_matrix *transformMatrixToHLL(matrix_mrkt *m,int hackSize){
    if(hackSize <= 0){
        printf("transformMatrixToHLL: Error: hackSize have to be a positive != 0 number\n");
    }
    assert(hackSize > 0);

    int numberOfBlocks;

    matrix_mrkt **matrix_blocks= divideInBlocks(m,hackSize,&numberOfBlocks);
    ELLPACK_block **ellpackBlocks = (ELLPACK_block **) malloc(sizeof(*ellpackBlocks)*numberOfBlocks);

    for(int i=0; i<numberOfBlocks; i++){
        ellpackBlocks[i] = transformMatrixToELLPACK(matrix_blocks[i]);
    }


    return init_HLL_Matrix(hackSize,numberOfBlocks,ellpackBlocks);
}