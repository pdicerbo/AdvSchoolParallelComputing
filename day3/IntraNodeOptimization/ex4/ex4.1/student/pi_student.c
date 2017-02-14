#include <omp.h>
#define NUM_THREADS 4
static long num_steps = 2000000000;

void main()
{
    int i, id, nthreads;
    double  sum[NUM_THREADS];
    double  pi, x;
    double  t1, t2;
    double step;
    
    step = 1.0/(double) num_steps;
    
    //timing
    t1 = omp_get_wtime();
    
    // we did not make use of reduction clause or critical section on purpose
    // to highlight false sharing
    #pragma omp parallel default(none) num_threads(NUM_THREADS)
    {
        id = omp_get_thread_num();
        nthreads = omp_get_num_threads();

        #pragma omp single
            printf("number of threads: %d\n", nthreads);
        
        sum[id] = 0.0; 
        for (i = id; i<num_steps; i=i+nthreads) {
            x = (i+0.5)*step;
            sum[id] += 4.0/(1.0+x*x);
        }
    }
    for (i=0, pi=0.0; i<NUM_THREADS; i++) 
        pi += sum[i]*step;
    printf("%13.10f\n", pi);
    
    t2 = omp_get_wtime();
    printf("Elapsed time in seconds: %f\n", t2-t1);
}

