/*
    file:logging.c
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

matrix_mrkt *init_matrix_mrkt(int *I, int *J, int M, int N, int NZ, double *val){
    matrix_mrkt *mtx = (matrix_mrkt *) malloc(sizeof(*mtx));

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
        printf("Failure: can't open matrix file\n");
        exit(1);
    }

    if(mm_read_banner(f,&matcode)){
        printf("Failure: could not process Matrix market banner\n");
        exit(1);
    }
    
    if(mm_is_complex(matcode) && mm_is_matrix(matcode) && mm_is_sparse(matcode)){
        printf("Failure: sorry this application doesn't support this type of matrix: [%s]\n",mm_typecode_to_str(matcode));
        exit(1);
    }

    if(mm_read_mtx_crd_size(f, &M, &N, &nz)){
        exit(1);
    }
    
    I = (int *)malloc(nz * sizeof(int));
    J = (int *)malloc(nz * sizeof(int));
    val = (double *)malloc(nz * sizeof(double));

    for(i=0; i<nz; i++){
        fscanf(f, "%d %d %lg\n", &I[i], &J[i], &val[i]);
        I[i]--;
        J[i]--;
    }

    matrix_mrkt *mtx = init_matrix_mrkt(I, J, M, N, nz, val);

    if (f != stdin) fclose(f);

    return sortByRaw(mtx);
}