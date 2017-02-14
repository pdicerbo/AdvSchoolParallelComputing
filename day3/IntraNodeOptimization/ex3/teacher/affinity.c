#include <omp.h>
#include <stdio.h>

void c_affinity()
{
    int num_places;
    int i, j;
    int *ids;
    int size_of_place;
    
    printf("*********************************\n");
    // get the number of places as defined by ICV or environment
    // variable 
    num_places = omp_get_num_places();
    printf("Number of places %d\n", num_places);

    // loop over places and print proc id belonging to each place 
    for (i=0;i<num_places;i++){
        size_of_place = omp_get_place_num_procs(i);
        printf("Number of processors per place %d: %d\n", i, size_of_place);
        
        ids = (int *)malloc(size_of_place*sizeof(int));
        omp_get_place_proc_ids(i, ids);

        for (j=0;j<size_of_place;j++)
            printf("proc_ids[%d]: %d\n", j, ids[j]); 
        free(ids);
    }
    printf("*********************************\n");
}
