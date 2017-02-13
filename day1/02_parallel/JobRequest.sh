#!/bin/bash

#PBS -N FirstJob
#PBS -l walltime=00:00:20
#PBS -l select=2:ncpus=16:mpiprocs=16:mem=3GB
#PBS -q R457635
#PBS -A train_scA2017

module load autoload intelmpi

cd $PBS_O_WORKDIR

export I_MPI_DEBUG=5

mpirun -np 32 ./a.out
