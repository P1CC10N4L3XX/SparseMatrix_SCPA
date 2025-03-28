#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "../Matrix/HLL.h"
#include "../Matrix/matrix_mrkt.h"
#include "headers/MRKT_utils.h"

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
        I = malloc(sizeof(*I)*NZ[i]);
        J = malloc(sizeof(*J)*NZ[i]);
        val = malloc(sizeof(*val)*NZ[i]);
        total+=NZ[i];
        for(int j = (i == 0) ? 0 : NZ[i-1]; j<total; j++){
            I[count]=m->I[j];
            J[count]=m->J[j];
            val[count]=m->val[j];
            count++;
        }
        blocks[i]=init_matrix_mrkt(I,J,M,N,NZ[i],val);
    }
    
    return  blocks;

}

int computeMAXNZ(matrix_mrkt *m){
    int MAXNZ=0;
    int count=0;
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
    int **JA = malloc(sizeof(*JA)*m->M);
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
        JA[i]=raw;
    }
    return JA;

}

double **computeAS_HLL(matrix_mrkt *m, int MAXNZ){
    double **AS = malloc(sizeof(*AS)*m->M);
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

ELLPACK_block *transformMatrixToELLPACK (matrix_mrkt *m){
    int M = m->M;
    int N = m->N;
    int MAXNZ = computeMAXNZ(m);
    int **JA = computeJA(m,MAXNZ);
    double **AS = computeAS_HLL(m, MAXNZ);
    /*for(int i=0;i<m->M;i++){      //stampa di JA per test
        printf("[ ");
        for(int j=0; j<MAXNZ; j++){         
            printf("%d ",JA[i][j]);
        }
        printf("]\n");
    }*/

    /*for(int i=0;i<m->M;i++){    //stampa di AS per test
        printf("[ ");           
        for(int j=0; j<MAXNZ; j++){
            printf("%2.3g ",AS[i][j]);
        }
        printf("]\n");
    }*/


    return initELLPACKMatrix(M,N,MAXNZ,JA,AS);
}

HLL_matrix *initHLLMatrix(int hackSize, int numberOfBlocks, ELLPACK_block **ellpackBlocks){
    HLL_matrix *hllMatrix = malloc(sizeof(*hllMatrix));

    if(!hllMatrix){
        fprintf(stderr, "initHLLMatrix: Error: can't allocate memory for HLL matrix\n");
        exit(EXIT_FAILURE);
    }

    hllMatrix->hackSize = hackSize;
    hllMatrix->numberOfBlocks = numberOfBlocks;
    hllMatrix->blocks = ellpackBlocks;

    return hllMatrix;
}

ELLPACK_block *initELLPACKMatrix(int M, int N,int MAXNZ,int **JA,double **AS){
    ELLPACK_block *ellpackMatrix = malloc(sizeof(*ellpackMatrix));

    if(!ellpackMatrix){
        fprintf(stderr, "initELLPACKMatrix: Error: can't allocate memory for ELLPACK matrix");
        exit(EXIT_FAILURE);
    }

    ellpackMatrix->M = M;
    ellpackMatrix->N = N;
    ellpackMatrix->AS = AS;
    ellpackMatrix->JA = JA;
    ellpackMatrix->MAXNZ = MAXNZ;

    return ellpackMatrix;

}


HLL_matrix *transformMatrixToHLL(matrix_mrkt *m,int hackSize){
    if(hackSize <= 0){
        printf("transformMatrixToHLL: Error: hackSize have to be a positive != 0 number\n");
    }
    assert(hackSize > 0);

    int numberOfBlocks;

    matrix_mrkt **matrix_blocks= divideInBlocks(m,hackSize,&numberOfBlocks);
    ELLPACK_block **ellpackBlocks = malloc(sizeof(*ellpackBlocks)*numberOfBlocks);

    for(int i=0; i<numberOfBlocks; i++){
        ellpackBlocks[i] = transformMatrixToELLPACK(matrix_blocks[i]);
    }


    return initHLLMatrix(hackSize,numberOfBlocks,ellpackBlocks);
}