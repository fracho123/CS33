#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#define N 20000
void transpose(int *dst, int *src, int dim) 
{ 
int i, j; 
    for (i = 0; i < dim; i++)
        for (j = 0; j < dim; j++) 
            dst[j*dim + i] = src[i*dim + j]; 
}

void transpose2dimensionalblocking(int *dst, int *src, int dim)
{
    int block = 16;
    int limit = dim - block;
	int i, j, i1, j1, j2;
    int irange;
    int jrange;
	
	for (i = 0; i < limit; i+=block) {
	    irange = i + block;
	    for (j = 0; j < limit; j+=block) {
	        jrange = j + block;
	       	
		//blocking by 16x16
		for (i1 = i; i1 < irange; i1++) {
	            for (j1 = j; j1 < jrange; j1++) {
	                dst[j1*dim + i1] = src[i1*dim + j1];
	            } 
        	}
	    } 
	    
	    //when the columns aren't a multiple of 16		
	    for (i1 = i; i1 < irange; i1++)
        	for (j2 = j1; j2 < dim; j2++) 
                    dst[j2*dim + i1] = src[i1*dim + j2];
	}

	//to get the rest of the matrix when dim is not an exact multiple of 16
	for (; i < dim; i++)
            for (j = 0; j < dim; j++) 
                dst[j*dim + i] = src[i*dim + j]; 
}

void transposeUnrolled(int *dst, int *src, int dim)
{
	int i, j, offset = dim - 8;
	int col0,col1,col2,col3,col4,col5,col6,col7;
	int rowj;
	for (i = 0; i < offset; i+=8) {
	    col0 = i * dim;
	    col1 = (i+1) * dim;
	    col2 = (i+2) * dim;
	    col3 = (i+3) * dim;
	    col4 = (i+4) * dim;
	    col5 = (i+5) * dim;
	    col6 = (i+6) * dim;
	    col7 = (i+7) * dim;
	    for (j = 0; j < dim; j++) {
	        rowj = j * dim;
	        dst[rowj + i] = src[col0 + j];
	        dst[rowj + i+1] = src[col1 + j];
	        dst[rowj + i+2] = src[col2 + j];
	        dst[rowj + i+3] = src[col3 + j];
	        dst[rowj + i+4] = src[col4 + j];
	        dst[rowj + i+5] = src[col5 + j];
	        dst[rowj + i+6] = src[col6 + j];
	        dst[rowj + i+7] = src[col7 + j];
	    } 
	}
	for (; i < dim; i++)
        for (j = 0; j < dim; j++) 
            dst[j*dim + i] = src[i*dim + j]; 
}


void transposeBlockingAndLoopUnrolling(int *dst, int *src, int dim)
{
    int block = 16;

    //variables for blocking
    int limit = dim - block;
    int i, j, i1, j1, j2;
    int irange;
    int jrange;
    
    //variables for unrolling
    int col0,col1,col2,col3,col4,col5,col6,col7;
    int rowj;

	for (i = 0; i < limit; i+=block) {
	    irange = i + block;
	    for (j = 0; j < limit; j+=block) {
	        jrange = j + block;
	       	
		//blocking by 16x16
		for (i1 = i; i1 < irange; i1+=8) {
		    col0 = i1 * dim;
	    	    col1 = (i1+1) * dim;
	    	    col2 = (i1+2) * dim;
	    	    col3 = (i1+3) * dim;
	    	    col4 = (i1+4) * dim;
	    	    col5 = (i1+5) * dim;
	    	    col6 = (i1+6) * dim;
	    	    col7 = (i1+7) * dim;

	            for (j1 = j; j1 < jrange; j1++) {
			rowj = j1 * dim;
	        	dst[rowj + i1] = src[col0 + j1];
	        	dst[rowj + i1+1] = src[col1 + j1];
	        	dst[rowj + i1+2] = src[col2 + j1];
	        	dst[rowj + i1+3] = src[col3 + j1];
	        	dst[rowj + i1+4] = src[col4 + j1];
	        	dst[rowj + i1+5] = src[col5 + j1];
	        	dst[rowj + i1+6] = src[col6 + j1];
	        	dst[rowj + i1+7] = src[col7 + j1];
	            } 
        	}
	    } 
	    
	    //when the columns aren't a multiple of 16		
	    for (i1 = i; i1 < irange; i1++)
        	for (j2 = j1; j2 < dim; j2++) 
                    dst[j2*dim + i1] = src[i1*dim + j2];
	}

	//to get the rest of the matrix when dim is not an exact multiple of 16
	for (; i < dim; i++)
            for (j = 0; j < dim; j++) 
                dst[j*dim + i] = src[i*dim + j]; 
}



void test_transpose(void)
{
	int i, j;
	int *dest1 = (int *) malloc(N * N * sizeof(int));
	int *dest2 = (int *) malloc(N * N * sizeof(int));
	int *source = (int *) malloc(N * N * sizeof(int));
	for (i = 0; i < N; i++) {
		int num = i % 2;
		for (j = 0; j < N; j++)
			source[i*N + j] = num;
	}
	
	clock_t start1;
	clock_t end1;
	start1 = clock();
	transpose(dest1, source, N);
	end1 = clock();
	printf("run time: %lf\n", difftime(end1, start1)/CLOCKS_PER_SEC);
	
	clock_t start;
	clock_t end;
	start = clock();
	transpose2dimensionalblocking(dest2, source, N);
	end = clock();
	printf("run time: %lf\n", difftime(end, start)/CLOCKS_PER_SEC);

	clock_t start2;
	clock_t end2;
	start2 = clock();
	transposeUnrolled(dest2, source, N);
	end2 = clock();
	printf("run time: %lf\n", difftime(end2, start2)/CLOCKS_PER_SEC);


	clock_t start3;
	clock_t end3;
	start3 = clock();
	transposeBlockingAndLoopUnrolling(dest2, source, N);
	end3 = clock();
	printf("run time: %lf\n", difftime(end3, start3)/CLOCKS_PER_SEC);

	
	int k, l;
	
	for (k = 0; k < N; k++)
		for (l = 0; l < N; l++)
			if (dest1[k*N + l] != dest2[k*N + l])
			    printf("bad program");
	
	free(dest1);
	free(dest2);
	free(source);
}

int main(void)
{
	test_transpose();
	
	return 0;
}
