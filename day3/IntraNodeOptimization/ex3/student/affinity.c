#include <omp.h>
#include <stdio.h>

void c_affinity()
{
    int num_places;
    
    printf("*********************************\n");
    // get the number of places as defined by ICV or environment
    // variable 
    num_places = omp_get_num_places();
    printf("Number of places %d\n", num_places);

    // write the remaining of the code here ...

    printf("*********************************\n");
}
