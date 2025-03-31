/*
    file:MRKT_utils.c
    Author: P1CC10N4L3XX

    https://github.com/P1CC10N4L3XX/SparseMatrix_SCPA
*/

#include <stdio.h>
#include <stdlib.h>

#include "headers/MRKT_utils.h"
#include "../mmio/headers/mmio.h"
#include "../Matrix/matrix_mrkt.h"

void freeMRKTMatrix(matrix_mrkt *m){
    free(m->I);
    free(m->J);
    free(m->val);
    free(m);
}

void printMRKTMatrix(matrix_mrkt *m){
    printf("STAMPA DELLA MATRICE IN FORMATO MARKET...\n\n");

    for(int i=0; i<m->NZ; i++){
        fprintf(stdout, "%d %d %3.3g\n", m->I[i], m->J[i], m->val[i]);
    }
}

matrix_mrkt *init_matrix_mrkt(int *I, int *J, int M, int N, int NZ, double *val){
    matrix_mrkt *mtx = (matrix_mrkt *) malloc(sizeof(*mtx));

    if(!mtx){
        fprintf(stderr, "init_matrix_mrkt: Error: can't allocate memory for matrix market\n");
        free(I);
        free(J);
        free(val);
        exit(EXIT_FAILURE);
    }

    mtx->I = I;
    mtx->J = J;
    mtx->M = M;
    mtx->N = N;
    mtx->NZ = NZ;
    mtx->val = val;

    return mtx;
}

matrix_mrkt *sortByRaw(matrix_mrkt *m){
    int *I = (int *)malloc((m->NZ)*sizeof(*I));
    int *J = (int *)malloc((m->NZ)*sizeof(*J));
    double *val = (double *)malloc((m->NZ)*sizeof(*val));

    if(!I || !J || !val){
        fprintf(stderr, "sortByRaw: Error: can't allocate memory for I,J,val\n");
        exit(EXIT_FAILURE);
    }

    int count=0;
    for(int i=0; i<(m->M); i++){
        for(int j=0; j<(m->NZ); j++){
            if(m->I[j]==i){
                I[count]=m->I[j];
                J[count]=m->J[j];
                val[count]=m->val[j];
                count++;
            }
        }
    }
    free(m->I);
    free(m->J);
    free(m->val);
    m->I = I;
    m->J = J;
    m->val = val;
    
    return m;
}

matrix_mrkt *read_matrix(char *filepath){
    MM_typecode matcode;
    FILE *f;
    int M, N, nz;
    int i, *I, *J;
    double *val;


    if((f = fopen(filepath,"r")) == NULL){
        fprintf(stderr,"read_matrix: Failure: can't open matrix file\n");
        exit(EXIT_FAILURE);
    }

    if(mm_read_banner(f,&matcode)){
        fprintf(stderr,"read_matrix: Failure: could not process Matrix market banner\n");
        exit(EXIT_FAILURE);
    }
    
    if(mm_is_complex(matcode) && mm_is_matrix(matcode) && mm_is_sparse(matcode)){
        fprintf(stderr,"Failure: sorry this application doesn't support this type of matrix: [%s]\n",mm_typecode_to_str(matcode));
        exit(EXIT_FAILURE);
    }

    if(mm_read_mtx_crd_size(f, &M, &N, &nz)){
        exit(EXIT_FAILURE);
    }
    
    I = (int *)malloc(nz * sizeof(int));
    J = (int *)malloc(nz * sizeof(int));
    val = (double *)malloc(nz * sizeof(double));

    if(!I || !J || !val){
        fprintf(stderr, "read_matrix: Error: can't allocate memory for I,J,val\n");
        exit(EXIT_FAILURE);
    }


    for(i=0; i<nz; i++){
        fscanf(f, "%d %d %lg\n", &I[i], &J[i], &val[i]);
        I[i]--;
        J[i]--;
    }

    matrix_mrkt *mtx = init_matrix_mrkt(I, J, M, N, nz, val);

    if (f != stdin) fclose(f);

    return sortByRaw(mtx);
}