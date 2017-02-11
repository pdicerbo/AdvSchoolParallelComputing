/* program hello */
/* Adapted from mpihello.f by drs */

#include <mpi.h>
#include <stdio.h>
#include <time.h>

int main(int argc, char **argv)
{
  int *buf, i, rank, nints, len;
  char hostname[256];
  
  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Barrier(MPI_COMM_WORLD);

  gethostname(hostname,255);

  if (rank==0) printf("\n");
  printf("Hello world!  I am process number: %d on host %s\n", rank, hostname);
  /* try to sleep*/ 
  sleep(5);
  printf("enough sleeping \n");
  MPI_Finalize();
  return 0;
}


