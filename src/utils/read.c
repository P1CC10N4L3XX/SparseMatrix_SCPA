#include <stdio.h>
#include <stdlib.h>

#include "../mmio/mmio.h"

int read_matrix(char *filepath){
    MM_typecode matcode;
    FILE *f;
    int M, N, nz;
    int i, *I, *J;
    double *val;
    if((f = fopen(filepath,"r")) == NULL)
        printf("Failure: can't open matrix file\n");
        exit(1);
    if(mm_read_banner(f,&matcode)){
        printf("Failure: could not process Matrix market banner\n");
        exit(1);
    }
    
    if(mm_is_complex(matcode) && mm_is_matrix(matcode) && mm_is_sparse(matcode)){
        printf("Failure: sorry this application doesn't support this type of matrix: [%s]\n",mm_typecode_to_str(matcode));
        exit(1);
    }

    if(mm_read_mtx_crd_size(f, &M, &N, &nz))
        exit(1);
    
    I = (int *)malloc(nz * sizeof(int));
    J = (int *)malloc(nz * sizeof(int));
    val = (double *)malloc(nz * sizeof(double));

    for(i=0; i<nz; i++){
        fscanf(f, "%d %d %lg\n", &I[i], &J[i], &val[i]);
        I[i]--;
        J[i]--;
    }
}