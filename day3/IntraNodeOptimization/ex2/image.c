#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <math.h>
#include "image.h"
#define _RESTRICT restrict


//**************************************

void negative(int num_row,int num_col, float * red_mm, float * green_mm, float * blu_mm, float * neg_red, float * neg_green,float *  neg_blu)
{

	int mul;

	// (max(R, G, B) + min(R, G, B)) / 2.
	for(int i=0;i<num_row;i++)
	{
		mul = i*num_col;
		for(int j=0;j<num_col;j++)
		{
			__assume_aligned(red_mm, 64);
			__assume_aligned(neg_red, 64);
			neg_red[j+mul] = 255 - red_mm[j+mul];
		}
	}

	for(int i=0;i<num_row;i++)
	{
		mul = i*num_col;
		for(int j=0;j<num_col;j++)
		{
			__assume_aligned(blu_mm, 64);
			__assume_aligned(neg_blu, 64);
			neg_blu[j+mul] = 255 - blu_mm[j+mul];
		}
	}

	for(int i=0;i<num_row;i++)
	{
		mul = i*num_col;
		for(int j=0;j<num_col;j++)
		{
			__assume_aligned(green_mm, 64);
			__assume_aligned(neg_green, 64);			
			neg_green[j+mul] = 255 - green_mm[j+mul];
		}
	}


}

//**************************************

void bw_img(int num_row,int num_col, float * red_mm, float * green_mm, float * blu_mm, float * out_bw)
{
	// (max(R, G, B) + min(R, G, B)) / 2.
	for(int i=0;i<num_row;i++)
		for(int j=0;j<num_col;j++)
		{
			__assume_aligned(red_mm, 64);
			__assume_aligned(green_mm, 64);
			__assume_aligned(blu_mm, 64);
			__assume_aligned(out_bw, 64);
			out_bw[j+i*num_col] = (fmaxf( fmaxf(red_mm[j+i*num_col],green_mm[j+i*num_col]), blu_mm[j+i*num_col] ) + 
			                       fminf( fminf(red_mm[j+i*num_col],green_mm[j+i*num_col]), blu_mm[j+i*num_col] ) ) /2 ;
		}

}

//**************************************

void image_py(int num_rows,int num_cols,int operation,
unsigned char * red,unsigned char * green,unsigned char * blu,
int* data, unsigned char * out_red,unsigned char * out_green,unsigned char * out_blu)
{
	
	
	double time;
	float * red_mm   = (float *) _mm_malloc(num_rows*num_cols*sizeof(float),64);
	float * green_mm = (float *) _mm_malloc(num_rows*num_cols*sizeof(float),64);
    float * blu_mm  = (float *) _mm_malloc(num_rows*num_cols*sizeof(float),64);

	red_mm[0:num_rows*num_cols]   = (float)  red[0:num_rows*num_cols];
	green_mm[0:num_rows*num_cols] = (float)  green[0:num_rows*num_cols];
	blu_mm[0:num_rows*num_cols]   = (float)  blu[0:num_rows*num_cols];
		
	if(operation == 0)
	{
		float * neg_red   = (float *) _mm_malloc(num_rows*num_cols*sizeof(float),64);
		float * neg_green = (float *) _mm_malloc(num_rows*num_cols*sizeof(float),64);
   		float * neg_blu  = (float *) _mm_malloc(num_rows*num_cols*sizeof(float),64);
		
		time = omp_get_wtime();
		negative(num_rows,num_cols,red_mm,green_mm,blu_mm,neg_red,neg_green,neg_blu);
		time = omp_get_wtime()-time;

		data[0]  = 3;
		out_red[0:num_rows*num_cols]   = (unsigned char) neg_red[0:num_rows*num_cols];
		out_green[0:num_rows*num_cols] = (unsigned char) neg_green[0:num_rows*num_cols];
		out_blu[0:num_rows*num_cols]   = (unsigned char) neg_blu[0:num_rows*num_cols];
		printf("NEGATIVE TIME TO SOLUTION : %e \n",time);
	}
	
	if(operation == 1)
	{	
		float * out_bw   = (float *) _mm_malloc(num_rows*num_cols*sizeof(float),64);
		
		time = omp_get_wtime();
		bw_img(num_rows,num_cols,red_mm,green_mm,blu_mm,out_bw);
		time = omp_get_wtime()-time;
		
		out_red[0:num_rows*num_cols] = (unsigned char) out_bw[0:num_rows*num_cols];
		_mm_free(out_bw);
		data[0]  = 1;
		printf("BW TIME TO SOLUTION : %e \n",time);
	}
	
	

	_mm_free(red_mm);
	_mm_free(green_mm);
	_mm_free(blu_mm);

}