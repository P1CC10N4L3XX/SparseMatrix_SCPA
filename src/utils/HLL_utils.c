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
#include "../garbage_collector/headers/memory_alloc.h"


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
    matrix_mrkt **blocks = (matrix_mrkt **)memory_alloc(*total_blocks * sizeof(*blocks));
    if(!blocks){
        fprintf(stderr,"divideInBlocks: Error: can't allocate memory\n");
        freeAll();
        exit(EXIT_FAILURE);
    }
    int *I = NULL,*J = NULL;
    int M = size;
    int N = m->N;
    int NZ = 0;
    double *val = NULL;


    int count=0;
    for(int i=0;i<m->NZ;i++){
        if(m->I[i] % size == 0 && i!=0 && m->I[i]!=0 && m->I[i]!=m->I[i-1]){
            blocks[count] = init_matrix_mrkt(I,J,M,N,NZ,val);
            NZ=0;
            val = NULL;
            I = NULL;
            J = NULL;
            val = NULL;

            count++;
        }
        int *temp_I =(int *) memory_realloc(I,sizeof(int)*(NZ+1));
        int *temp_J =(int *) memory_realloc(J,sizeof(int)*(NZ+1));
        double *temp_val = (double *)memory_realloc(val, sizeof(double)*(NZ+1));

        if(!temp_I || !temp_J || !temp_val){
            fprintf(stderr, "divideInBlocks: Error: can't allocate memory\n");
            freeAll();
            exit(EXIT_FAILURE);
        }

        I = temp_I;
        J = temp_J;
        val = temp_val;
        I[NZ] = m->I[i]%size;
        J[NZ] = m->J[i];
        val[NZ] = m->val[i];
        NZ++;
    }

    blocks[count] = init_matrix_mrkt(I,J,M,N,NZ,val);
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
    int **JA = (int **)memory_alloc(sizeof(*JA)*m->M);
    if(!JA){
        fprintf(stderr,"computeJA: Error: can't allocate memory for JA\n");
        freeAll();
        exit(EXIT_FAILURE);
    }
    int count;
    for(int i=0; i<m->M; i++){
        int *raw = memory_alloc(sizeof(*raw)*MAXNZ);
        if(!raw){
            fprintf(stderr, "computeJA: Error: can't allocate memory\n");
            freeAll();
            exit(EXIT_FAILURE);
        }
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
    double **AS =(double **) memory_alloc(sizeof(*AS)*m->M);
    if(!AS){
        fprintf(stderr, "computeAS_HLL: Error: can't allocate memory for AS\n");
        freeAll();
        exit(EXIT_FAILURE);
    }
    int count;
    for(int i=0;i<m->M;i++){
        double *raw = (double *) memory_calloc(MAXNZ, sizeof(*raw));
        if(!raw){
            fprintf(stderr, "computeAS_HLL: Error: can't allocate memory\n");
            freeAll();
            exit(EXIT_FAILURE);
        }
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
    ELLPACK_block *ellpackMatrix = (ELLPACK_block *)memory_alloc(sizeof(*ellpackMatrix));
    if(!ellpackMatrix){
        fprintf(stderr, "init_ELLPACK_Matrix: Error: can't allocate memory\n");
        freeAll();
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
    HLL_matrix *hllMatrix = (HLL_matrix *)memory_alloc(sizeof(*hllMatrix));
    if(!hllMatrix){
        fprintf(stderr, "init_HLL_Matrix: Error: can't allocate memory\n");
        freeAll();
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


    ELLPACK_block **ellpackBlocks = (ELLPACK_block **) memory_alloc(sizeof(*ellpackBlocks)*numberOfBlocks);
    if(!ellpackBlocks){
        fprintf(stderr, "transformMatrixToHLL: Error: can't allocate memory\n");
        freeAll();
        exit(EXIT_FAILURE);
    }

    for(int i=0; i<numberOfBlocks; i++){
        ellpackBlocks[i] = transformMatrixToELLPACK(matrix_blocks[i]);
    }


    return init_HLL_Matrix(hackSize,numberOfBlocks,ellpackBlocks);
}