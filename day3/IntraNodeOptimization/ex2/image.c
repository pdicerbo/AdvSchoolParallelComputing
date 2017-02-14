#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <math.h>
#include "image.h"



//**************************************

void negative(int num_row,int num_col, float * red_mm, float * green_mm, float * blu_mm, float * neg_red, float * neg_green,float *  neg_blu)
{

  // Implements negative operation
  //
  //    pixel[i] = 255 -  pixel[i]
  //


}

//**************************************

void bw_img(int num_row,int num_col, float * red_mm, float * green_mm, float * blu_mm, float * out_bw)
{

  // Implements Black and withe trasformation
  //
  //    pixel_BW[i] = (max(R, G, B) + min(R, G, B)) / 2.
  //
}

//**************************************

void image_py(int num_rows,int num_cols,int operation,
unsigned char * red,unsigned char * green,unsigned char * blu,
int* data, unsigned char * out_red,unsigned char * out_green,unsigned char * out_blu)
{
	
	
	double time;
	// allocate 3 aligned array : red_mm,green_mm and blu_mm
	
	// copy data from input array to memory aligned array

		
	if(operation == 0)
	{	
		// allocate 3 aligned array to store the negative image: neg_red,neg_green and neg_blu

		
		time = omp_get_wtime();
		negative(num_rows,num_cols,red_mm,green_mm,blu_mm,neg_red,neg_green,neg_blu);
		time = omp_get_wtime()-time;

		data[0]  = 3;

		// copy data from negative aligned array to output array

		printf("NEGATIVE TIME TO SOLUTION : %e \n",time);
	}
	
	if(operation == 1)
	{	
		// allocate bw image array : out_bw
		
		
		time = omp_get_wtime();
		bw_img(num_rows,num_cols,red_mm,green_mm,blu_mm,out_bw);
		time = omp_get_wtime()-time;
		
		// copy data from out_bw aligned array to red channel output array
		
		data[0]  = 1;
		printf("BW TIME TO SOLUTION : %e \n",time);
	}
	
	
	//free memory


}