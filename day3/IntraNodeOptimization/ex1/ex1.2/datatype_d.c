#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <math.h>

typedef signed int MyType;

int main()
{

MyType time,time1;
int t,i,j,k,totalsize,size = 500,repetition=40;


totalsize = size * size * size;

MyType * a = (MyType *) _mm_malloc(totalsize * sizeof(MyType),64);
MyType * b = (MyType *) _mm_malloc(totalsize * sizeof(MyType),64);
MyType * c = (MyType *) _mm_malloc(totalsize * sizeof(MyType),64);

for(int i=0;i<totalsize;i++)
{
	a[i] = i+1;
	b[i] = i%2;
}

time = omp_get_wtime();

for(int t=0;t<repetition;t++)
	for(int i=0;i<totalsize;i++)
	{
			c[i] = a[i] * b[i];
	}

time = omp_get_wtime() - time;

printf("TIME double (with sizeof(MyType) = %d) is %e \n",sizeof(MyType), time/repetition);

_mm_free(a);
_mm_free(b);
_mm_free(c);

}