//  
//   Modified from Advanced MPI (Archer Training Material,EPCC)
//  
//   http://www.archer.ac.uk/training/course-material/2016/09/160929_AdvMPI_EPCC/index.php
//   
//
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "mpi.h"

// Boundary value at the top of the domain
#define TOP_VALUES 1.0
// Boundary value at the bottom of the domain
#define BOTTOM_VALUES 10.0
// The maximum number of iterations
#define MAX_ITERATIONS 5000000
// The convergence to terminate at
#define CONVERGENCE_ACCURACY 1e-4
// How often to report the norm
#define REPORT_NORM_PERIOD 1000

int nx, ny;

void initialise(double*, double*, int, int, int);

int main(int argc, char * argv[]) {	
	int size, myrank;
	int ProcTop, ProcBottom;
	int PrevLocalDim;

	MPI_Win win;
	MPI_Aint disp;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (argc != 3) {
		if (myrank==0) fprintf(stderr, "You must provide the size in X and size in Y as arguments to this code\n");
		return -1;
	}
	nx=atoi(argv[1]);
	ny=atoi(argv[2]);

	if (myrank==0) printf("Solving to accuracy of %.0e, global system size is x=%d y=%d\n", CONVERGENCE_ACCURACY, nx, ny);
	int local_nx=nx/size;
	PrevLocalDim = local_nx;
	if (local_nx * size < nx) {
		if (myrank < nx - local_nx * size) local_nx++;
		// if (myrank == nx - local_nx * size) PrevLocalDim++;
	}
	if(myrank == nx%size)
		PrevLocalDim++;

	double * grid = malloc(sizeof(double) * (local_nx + 2) * ny);
	double * grid_new = malloc(sizeof(double) * (local_nx + 2) * ny);
	double * temp = malloc(sizeof(double) * (local_nx + 2) * ny);
	double start_time;

	disp = ny*local_nx*sizeof(double);
	MPI_Win_create(&grid[ny], disp, sizeof(double), MPI_INFO_NULL, MPI_COMM_WORLD, &win);
	// disp = ny*(local_nx + 2)*sizeof(double);
	// MPI_Win_create(grid, disp, sizeof(double), MPI_INFO_NULL, MPI_COMM_WORLD, &win);
	ProcTop = myrank==0 ? MPI_PROC_NULL : myrank-1;
	ProcBottom = myrank==size-1 ? MPI_PROC_NULL : myrank+1;

	initialise(grid, grid_new, local_nx, myrank, size);

	double rnorm=0.0, bnorm=0.0, norm, tmpnorm=0.0;
	MPI_Request requests[]={MPI_REQUEST_NULL, MPI_REQUEST_NULL, MPI_REQUEST_NULL, MPI_REQUEST_NULL};

	int i,j,k;
	for (i=1;i<=local_nx;i++) {
		for (j=0;j<ny;j++) {		
			tmpnorm=tmpnorm+pow(grid[j+(i*ny)]*4-grid[(j-1) + (i*ny)]-grid[(j+1) + (i*ny)] - grid[j+((i-1)*ny)] - grid[j+((i+1)*ny)], 2);
		}
	}
	MPI_Allreduce(&tmpnorm, &bnorm, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
	bnorm=sqrt(bnorm);

	start_time=MPI_Wtime();
	for (k=0;k<MAX_ITERATIONS;k++) {
 
		/*
		if (myrank > 0) {
			MPI_Isend(&grid[ny], ny, MPI_DOUBLE, myrank-1, 0, MPI_COMM_WORLD, &requests[0]);
			MPI_Irecv(&grid[0], ny, MPI_DOUBLE, myrank-1, 0, MPI_COMM_WORLD, &requests[1]);
		}
		if (myrank < size-1) {
			MPI_Isend(&grid[local_nx * ny], ny, MPI_DOUBLE, myrank+1, 0, MPI_COMM_WORLD, &requests[2]);
			MPI_Irecv(&grid[(local_nx+1) * ny], ny, MPI_DOUBLE, myrank+1, 0, MPI_COMM_WORLD, &requests[3]);
		}
		MPI_Waitall(4, requests, MPI_STATUSES_IGNORE);
		*/

		MPI_Win_fence(MPI_MODE_NOPRECEDE, win);
		// MPI_Win_lock(MPI_LOCK_SHARED, ProcTop, MPI_MODE_NOCHECK, win);
		disp = (PrevLocalDim-1)*ny;
		MPI_Get(&grid[0],ny,MPI_DOUBLE,ProcTop,disp,ny,MPI_DOUBLE,win);
		// MPI_Win_unlock(ProcTop, win);

		// MPI_Win_lock(MPI_LOCK_SHARED, ProcBottom, MPI_MODE_NOCHECK, win);
		disp = 0;
		MPI_Get(&grid[(local_nx+1)*ny],ny,MPI_DOUBLE,ProcBottom,disp,ny,MPI_DOUBLE,win);
		// MPI_Win_unlock(ProcBottom, win);
		MPI_Win_fence(MPI_MODE_NOSUCCEED, win);

		tmpnorm=0.0;
		for (i=1;i<=local_nx;i++) {
			for (j=0;j<ny;j++) {
				tmpnorm=tmpnorm+pow(grid[j+(i*ny)]*4-grid[(j-1) + (i*ny)]-grid[(j+1) + (i*ny)] - grid[j+((i-1)*ny)] - grid[j+((i+1)*ny)], 2);
			}
		}
		MPI_Allreduce(&tmpnorm, &rnorm, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
		norm=sqrt(rnorm)/bnorm;
		if (norm < CONVERGENCE_ACCURACY) break;		
		for (i=1;i<=local_nx;i++) {
			for (j=0;j<ny;j++) {
				grid_new[j+(i*ny)]=0.25 * (grid[(j-1) + (i*ny)]+grid[(j+1) + (i*ny)] + grid[j+ ((i+1)*ny)] + grid[j+ ((i-1)*ny)]);
			}
		}
		memcpy(temp, grid_new, sizeof(double) * (local_nx + 2) * ny);
		memcpy(grid_new, grid, sizeof(double) * (local_nx + 2) * ny);
		memcpy(grid, temp, sizeof(double) * (local_nx + 2) * ny);

		if (k % REPORT_NORM_PERIOD == 0 && myrank==0) printf("Iteration= %d Relative Norm=%e\n", k, norm);
	}
	if (myrank==0) printf("\nTerminated on %d iterations, Relative Norm=%e, Total time=%e seconds\n", k, norm,
			MPI_Wtime() - start_time);
	free(grid);
	free(grid_new);
	free(temp);
	MPI_Finalize();
	return 0;
}

/**
 * Initialises the arrays, such that grid contains the boundary conditions at the start and end points and all other
 * points are zero. grid_new is set to equal grid
 */
void initialise(double * grid, double * grid_new, int local_nx, int myrank, int size) {
	int i, j;
	for (j=0;j<ny;j++) {
		grid_new[j]=grid[j]=myrank==0 ? TOP_VALUES: 0;
	}
	for (j=0;j<ny;j++) {
		grid_new[j+(ny*(local_nx+1))]=grid[j+(ny*(local_nx+1))]=myrank==size ? BOTTOM_VALUES: 0;		
	}
	for (i=1;i<=local_nx;i++) {
		for (j=0;j<ny;j++) {
			grid_new[j+(ny*i)]=grid[j+(ny*i)]=0;
		}
	}	
}
