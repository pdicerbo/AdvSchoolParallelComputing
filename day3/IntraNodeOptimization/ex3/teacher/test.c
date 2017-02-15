#include <stdio.h>
#include <omp.h>

void main()
{
    int id, nthreads, place_id;
    
    //affinity attributes
    c_affinity();

    #pragma omp parallel default(none) private(id, nthreads, place_id) 
    {
        nthreads = omp_get_num_threads();
        #pragma omp single
        {
            printf("number of threads: %d\n", nthreads);
        }

        id = omp_get_thread_num();
        place_id = omp_get_place_num();
        printf("id: %2d, in place %2d\n", id, place_id);
    }
}

