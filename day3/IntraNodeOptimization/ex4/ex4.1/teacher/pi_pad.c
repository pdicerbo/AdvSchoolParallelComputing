#include <omp.h>
#define NUM_THREADS 4
#define PAD 7     // assuming L1 line cache of 64 bytes
static long num_steps = 2000000000;

void main()
{
    int i, id, nthreads;
    double  sum[NUM_THREADS][PAD];
    double  pi, x;
    double  t1, t2;
    double step;

    // show affinity 
    c_affinity();

    step = 1.0/(double) num_steps;
    t1 = omp_get_wtime();

    #pragma omp parallel default(none) private(i, id, x, nthreads) shared(sum, num_steps, step) num_threads(NUM_THREADS)
    {
        id = omp_get_thread_num();
        nthreads = omp_get_num_threads();
        printf("id: %d, in place %d\n", id, omp_get_place_num());
        
        #pragma omp single
            printf("number of threads: %d\n", nthreads);
        
        sum[id][0]=0.0;
        for (i = id; i<num_steps; i=i+nthreads) {
            x = (i+0.5)*step;
            sum[id][0] += 4.0/(1.0+x*x);
        }
    }
    
    for (i=0, pi=0.0; i<NUM_THREADS; i++) 
        pi += sum[i][0]*step;
    printf("%13.10f\n", pi);
    
    t2 = omp_get_wtime();
    printf("Elapsed time in seconds: %f\n", t2-t1);
}

